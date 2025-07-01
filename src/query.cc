#include "query.hh"
#include "config.hh"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>

static int ApplicationType;
static char LoginData[30];

void SetQueryManagerLoginData(int Type, const char *Data){
	ApplicationType = Type;
	if(Data != NULL){
		strncpy(LoginData, Data, sizeof(LoginData));
		LoginData[sizeof(LoginData) - 1] = 0;
	}else{
		LoginData[0] = 0;
	}
}

// TODO(fusion): An initializer list executes in the same order fields are
// declared. This initialization should work as long as `BufferSize` is the
// first field declared in `TQueryManagerConnection`, which it is.
TQueryManagerConnection::TQueryManagerConnection(int QueryBufferSize) :
		BufferSize(std::max<int>(QueryBufferSize, KB(16))),
		Buffer(new uint8[this->BufferSize]),
		ReadBuffer(this->Buffer, this->BufferSize),
		WriteBuffer(this->Buffer, this->BufferSize),
		Socket(-1),
		QueryOk(false)
{
	this->connect();
}

TQueryManagerConnection::~TQueryManagerConnection(void){
	this->disconnect();
	delete[] this->Buffer;
}

bool ResolveHostNameAddress(const char *HostName, in_addr_t *OutAddr){
	// TODO(fusion): Just use `getaddrinfo`?
	int ErrorCode;
	hostent HostEnt;
	hostent *HostEntResult;
	char Buffer[2048];
	int Ret = gethostbyname_r(HostName, &HostEnt,
			Buffer, sizeof(Buffer), &HostEntResult, &ErrorCode);
	if(Ret != 0 || HostEntResult == NULL){
		return false;
	}

	if(OutAddr){
		*OutAddr = ((in_addr_t)HostEntResult->h_addr_list[0][0] << 24)
				| ((in_addr_t)HostEntResult->h_addr_list[0][1] << 16)
				| ((in_addr_t)HostEntResult->h_addr_list[0][2] <<  8)
				| ((in_addr_t)HostEntResult->h_addr_list[0][3] <<  0);
	}

	return true;
}

void TQueryManagerConnection::connect(void){
	for(int i = 0; i < NumberOfQueryManagers; i += 1){
		in_addr_t Addr = inet_addr(QUERY_MANAGER[i].Host);
		if(Addr == 0xFFFFFFFF && !ResolveHostNameAddress(QUERY_MANAGER[i].Host, &Addr)){
			print(2, "TQueryManagerConnection::connect: Kann Rechnernamen nicht auflösen.\n");
			continue;
		}

		this->Socket = socket(AF_INET, SOCK_STREAM, 0);
		if(this->Socket == -1){
			print(2, "TQueryManagerConnection::connect: Kann Socket nicht öffnen.\n");
			continue;
		}

		struct sockaddr_in QueryManagerAddress = {};
		QueryManagerAddress.sin_family = AF_INET;
		QueryManagerAddress.sin_port = htons(QUERY_MANAGER[i].Port);
		QueryManagerAddress.sin_addr.s_addr = Addr;
		if(::connect(this->Socket, (struct sockaddr*)&QueryManagerAddress, sizeof(QueryManagerAddress)) == -1){
			this->disconnect();
			continue;
		}

		this->prepareQuery(0);
		this->sendByte((uint8)ApplicationType);
		this->sendString(QUERY_MANAGER[i].Password);
		if(ApplicationType == 1){
			this->sendString(LoginData);
		}

		int Ret = this->executeQuery(30, false);
		if(Ret != 0){
			print(2, "TQueryManagerConnection::connect: Anmeldung fehlgeschlagen (%d).\n", Ret);
			this->disconnect();
			continue;
		}

		break;
	}

	if(!this->isConnected()){
		print(2, "TQueryManagerConnection::connect: Kein Query-Manager verfügbar.\n");
	}
}

void TQueryManagerConnection::disconnect(void){
	if(this->Socket != -1){
		if(close(this->Socket) == -1){
			error("TQueryManagerConnection::disconnect: Fehler %d beim Schließen der Socket.\n", errno);
		}
		this->Socket = -1;
	}
}

int TQueryManagerConnection::write(const uint8 *Buffer, int Size){
	int Attempts = 50;
	int BytesToWrite = Size;
	const uint8 *WritePtr = Buffer;
	while(BytesToWrite > 0){
		int BytesWritten = (int)::write(this->Socket, WritePtr, BytesToWrite);
		if(BytesWritten > 0){
			BytesToWrite -= BytesWritten;
			WritePtr += BytesWritten;
		}else if(BytesWritten == 0){
			break;
		}else{
			// TODO(fusion): We don't set the socket as non blocking so I don't
			// think we can even get `EAGAIN` here.
			if(errno != EAGAIN || Attempts <= 0){
				return -1;
			}

			DelayThread(0, 100000);
			Attempts -= 1;
		}
	}
	return Size - BytesToWrite;
}

int TQueryManagerConnection::read(uint8 *Buffer, int Size, int Timeout){
	int Attempts = 50;
	int BytesToRead = Size;
	uint8 *ReadPtr = Buffer;
	while(BytesToRead > 0){
		struct pollfd pollfd = {};
		pollfd.fd = this->Socket;
		pollfd.events = POLLIN;
		if(poll(&pollfd, 1, Timeout * 1000) != 1){
			return -2;
		}

		int BytesRead = ::read(this->Socket, ReadPtr, BytesToRead);
		if(BytesRead > 0){
			BytesToRead -= BytesRead;
			ReadPtr += BytesRead;
		}else if(BytesRead == 0){
			break;
		}else if(errno != EINTR){
			if(errno != EAGAIN || BytesToRead == Size || Attempts <= 0){
				return -1;
			}

			DelayThread(0, 100000);
			Attempts -= 1;
		}
	}
	return Size - BytesToRead;
}

void TQueryManagerConnection::prepareQuery(int QueryType){
	this->QueryOk = true;
	this->WriteBuffer.Position = 0;

	try{
		this->WriteBuffer.writeWord(0);
		this->WriteBuffer.writeByte((uint8)QueryType);
	}catch(const char *str){
		error("TQueryManagerConnection::prepareQuery: %s.\n", str);
		this->QueryOk = false;
	}
}

void TQueryManagerConnection::sendFlag(bool Flag){
	this->sendByte(Flag ? 0x01 : 0x00);
}

void TQueryManagerConnection::sendByte(uint8 Byte){
	try{
		this->WriteBuffer.writeByte(Byte);
	}catch(const char *str){
		error("TQueryManagerConnection::sendByte: %s.\n", str);
		this->QueryOk = false;
	}
}

void TQueryManagerConnection::sendWord(uint16 Word){
	try{
		this->WriteBuffer.writeWord(Word);
	}catch(const char *str){
		error("TQueryManagerConnection::sendWord: %s.\n", str);
		this->QueryOk = false;
	}
}

void TQueryManagerConnection::sendQuad(uint32 Quad){
	try{
		this->WriteBuffer.writeQuad(Quad);
	}catch(const char *str){
		error("TQueryManagerConnection::sendQuad: %s.\n", str);
		this->QueryOk = false;
	}
}

void TQueryManagerConnection::sendString(const char *String){
	try{
		this->WriteBuffer.writeString(String);
	}catch(const char *str){
		error("TQueryManagerConnection::sendString: %s.\n", str);
		this->QueryOk = false;
	}
}

void TQueryManagerConnection::sendBytes(const uint8 *Buffer, int Count){
	try{
		this->WriteBuffer.writeBytes(Buffer, Count);
	}catch(const char *str){
		error("TQueryManagerConnection::sendBytes: %s.\n", str);
		this->QueryOk = false;
	}
}

bool TQueryManagerConnection::getFlag(void){
	return this->getByte() != 0;
}

uint8 TQueryManagerConnection::getByte(void){
	uint8 Result = 0;
	try{
		Result = this->ReadBuffer.readByte();
	}catch(const char *str){
		error("TQueryManagerConnection::getByte: %s.\n", str);
	}
	return Result;
}

uint16 TQueryManagerConnection::getWord(void){
	uint16 Result = 0;
	try{
		Result = this->ReadBuffer.readWord();
	}catch(const char *str){
		error("TQueryManagerConnection::getWord: %s.\n", str);
	}
	return Result;
}

uint32 TQueryManagerConnection::getQuad(void){
	uint32 Result = 0;
	try{
		Result = this->ReadBuffer.readQuad();
	}catch(const char *str){
		error("TQueryManagerConnection::getQuad: %s.\n", str);
	}
	return Result;
}

void TQueryManagerConnection::getString(char *Buffer, int MaxLength){
	try{
		this->ReadBuffer.readString(Buffer, MaxLength);
	}catch(const char *str){
		error("TQueryManagerConnection::getString: %s.\n", str);
		if(MaxLength > 0){
			Buffer[0] = 0;
		}
	}
}

void TQueryManagerConnection::getBytes(uint8 *Buffer, int Count){
	try{
		this->ReadBuffer.readBytes(Buffer, Count);
	}catch(const char *str){
		error("TQueryManagerConnection::getBytes: %s.\n", str);
	}
}

int TQueryManagerConnection::executeQuery(int Timeout, bool AutoReconnect){
	// NOTE(fusion): `prepareQuery` will already leave two extra bytes at the
	// beginning of the buffer for writing the request's length.
	int PacketSize = this->WriteBuffer.Position;
	int PayloadSize = PacketSize - 2;
	this->WriteBuffer.Position = 0;
	if(PayloadSize < 0xFFFF){
		this->sendWord((uint16)PayloadSize);
	}else{
		PacketSize += 4;
		if(PacketSize > this->BufferSize){
			error("TQueryManagerConnection::executeQuery: Puffer zu klein.\n");
			return QUERY_FAILED;
		}

		memmove(&this->Buffer[6], &this->Buffer[2], PayloadSize);
		this->sendWord(0xFFFF);
		this->sendQuad((uint32)PayloadSize);
	}

	if(!this->QueryOk){
		error("TQueryManagerConnection::executeQuery: Fehler beim Zusammenbauen der Anfrage.\n");
		return QUERY_FAILED;
	}

	for(int Attempt = 0; true; Attempt += 1){
		if(!this->isConnected()){
			if(!AutoReconnect){
				return QUERY_FAILED;
			}

			// TODO(fusion): There was also a "fast" path that allocated on the
			// stack with variable length arrays. It would be a problem to mimic
			// that with `alloca` since its memory is only released at the end
			// of the function and we're in a loop.
			uint8 *TempBuffer = new uint8[PacketSize];
			memcpy(TempBuffer, this->Buffer, PacketSize);
			this->connect();
			memcpy(this->Buffer, TempBuffer, PacketSize);
			delete[] TempBuffer;

			if(!this->isConnected()){
				return QUERY_FAILED;
			}
		}

		if(this->write(this->Buffer, PacketSize) != PacketSize){
			this->disconnect();
			if(Attempt > 0){
				error("TQueryManagerConnection::executeQuery: Fehler beim Abschicken der Anfrage.\n");
				return QUERY_FAILED;
			}
			continue;
		}

		uint8 Help[4];
		int BytesRead = this->read(Help, 2, Timeout);
		if(BytesRead != 2){
			this->disconnect();
			if(BytesRead == -2 || Attempt > 0){
				return QUERY_FAILED;
			}
			continue;
		}

		int ResponseSize = ((int)Help[0]) | ((int)Help[1] << 8);
		if(ResponseSize == 0xFFFF){
			if(this->read(Help, 4, Timeout) != 4){
				this->disconnect();
				return QUERY_FAILED;
			}

			ResponseSize = ((int)Help[0]) | ((int)Help[1] << 8)
					| ((int)Help[2] << 16) | ((int)Help[3] << 24);
		}

		if(ResponseSize <= 0 || ResponseSize > this->BufferSize){
			this->disconnect();
			error("TQueryManagerConnection::executeQuery: Ungültige Datengröße %d.\n", ResponseSize);
			return QUERY_FAILED;
		}

		if(this->read(this->Buffer, ResponseSize, Timeout) != ResponseSize){
			this->disconnect();
			error("TQueryManagerConnection::executeQuery: Fehler beim Auslesen der Daten.\n");
			return QUERY_FAILED;
		}

		int Status = this->getByte();
		if(Status == QUERY_FAILED){
			error("TQueryManagerConnection::executeQuery: Anfrage fehlgeschlagen.\n");
		}

		return Status;
	}
}

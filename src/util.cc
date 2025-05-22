#include "common.hh"

static void (*ErrorFunction)(const char *Text) = NULL;
static void (*PrintFunction)(int Level, const char *Text) = NULL;

void SetErrorFunction(TErrorFunction *Function){
	ErrorFunction = Function;
}

void SetPrintFunction(TPrintFunction *Function){
	PrintFunction = Function;
}

void error(char *Text, ...){
	char s[1024];

	va_list ap;
	va_start(ap, Text);
	vsnprintf(s, sizeof(s), Text, ap);
	va_end(ap);

	if(ErrorFunction){
		ErrorFunction(s);
	}else{
		printf("%s", s);
	}
}

void print(int Level, char *Text, ...){
	char s[1024];

	va_list ap;
	va_start(ap, Text);
	vsnprintf(s, sizeof(s), Text, ap);
	va_end(ap);

	if(PrintFunction){
		PrintFunction(Level, s);
	}else{
		printf("%s", s);
	}
}

// TReadStream
// =============================================================================
bool TReadStream::readFlag(void){
	return this->readByte() != 0;
}

uint16 TReadStream::readWord(void){
	// NOTE(fusion): Data is encoded in little endian.
	uint8 Byte0 = this->readByte();
	uint8 Byte1 = this->readByte();
	return ((uint16)Byte1 << 8) | (uint16)Byte0;
}

uint32 TReadStream::readQuad(void){
	// NOTE(fusion): Data is encoded in little endian.
	uint8 Byte0 = this->readByte();
	uint8 Byte1 = this->readByte();
	uint8 Byte2 = this->readByte();
	uint8 Byte3 = this->readByte();
	return ((uint32)Byte3 << 24) | ((uint32)Byte2 << 16)
			| ((uint32)Byte1 << 8) | (uint32)Byte0;
}

void TReadStream::readString(char *Buffer, int MaxLength){
	if(Buffer == NULL || MaxLength == 0){
		error("TReadStream::readString: Übergebener Puffer existiert nicht.\n");
		throw "internal error";
	}

	int Length = (int)this->readWord();
	if(Length == 0xFFFF){
		Length = (int)this->readQuad();
	}

	if(Length > 0){
		if(MaxLength < 0 || MaxLength > Length){
			this->readBytes(Buffer, Length);
			Buffer[Length] = 0;
		}else{
			this->readBytes(Buffer, MaxLength - 1);
			this->skip(Length - MaxLength + 1);
			Buffer[MaxLength - 1] = 0;
		}
	}else{
		Buffer[0] = 0;
	}
}

void TReadStream::readBytes(uint8 *Buffer, int Count){
	if(Buffer == NULL){
		error("TReadStream::readBytes: Übergebener Puffer existiert nicht.\n");
		throw "internal error";
	}

	for(int i = 0; i < Count; i += 1){
		Buffer[i] = this->readByte();
	}
}

// TReadBuffer
// =============================================================================
TReadBuffer::TReadBuffer(uint8 *Data, int Size){
	if(Data == NULL){
		error("TReadBuffer::TReadBuffer: data ist NULL.\n");
		Size = 0;
	}else if(Size < 0){
		error("TReadBuffer::TReadBuffer: Ungültige Datengröße %d.\n", Size);
		Size = 0;
	}

	this->Data = Data;
	this->Size = Size;
	this->Position = 0;
}

uint8 TReadBuffer::readByte(void){
	if((this->Size - this->Position) < 1){
		throw "buffer empty";
	}

	uint8 Byte = this->Data[this->Position];
	this->Position += 1;
	return Byte;
}

uint16 TReadBuffer::readWord(void){
	if((this->Size - this->Position) < 2){
		throw "buffer empty";
	}

	uint8 Byte0 = this->Data[this->Position];
	uint8 Byte1 = this->Data[this->Position + 1];
	this->Position += 2;
	return ((uint16)Byte1 << 8) | (uint16)Byte0;
}

uint32 TReadBuffer::readQuad(void){
	if((this->Size - this->Position) < 4){
		throw "buffer empty";
	}

	uint8 Byte0 = this->Data[this->Position];
	uint8 Byte1 = this->Data[this->Position + 1];
	uint8 Byte2 = this->Data[this->Position + 2];
	uint8 Byte3 = this->Data[this->Position + 3];
	this->Position += 4;
	return ((uint32)Byte3 << 24) | ((uint32)Byte2 << 16)
			| ((uint32)Byte1 << 8) | (uint32)Byte0;
}

void TReadBuffer::readBytes(uint8 *Buffer, int Count){
	if(Buffer == NULL || Count <= 0){
		error("TReadBuffer::readBytes: Übergebener Puffer existiert nicht.\n");
		throw "buffer not existing";
	}

	if((this->Size - this->Position) < Count){
		throw "buffer empty";
	}

	memcpy(Buffer, &this->Data[this->Position], Count);
	this->Position += Count;
}

bool TReadBuffer::eof(void){
	return this->Size <= this->Position;
}

void TReadBuffer::skip(int Count){
	if((this->Size - this->Position) < Count){
		throw "buffer empty";
	}

	this->Position += Count;
}

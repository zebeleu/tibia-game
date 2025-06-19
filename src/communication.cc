#include "communication.hh"
#include "config.hh"
#include "connections.hh"
#include "containers.hh"
#include "crypto.hh"
#include "query.hh"
#include "threads.hh"

#include "stubs.hh"

#include <signal.h>

#define MAX_COMMUNICATION_THREADS 1100
#define THREAD_OWN_STACK_SIZE ((int)KB(64))

static int TERMINAL_VERSION[3] = {770, 770, 770};
static int TCPSocket;
static ThreadHandle AcceptorThread;
static pid_t AcceptorThreadPID;
static int ActiveConnections;

static Semaphore RSAMutex(1);
static TRSAPrivateKey PrivateKey;

static TQueryManagerConnectionPool QueryManagerConnectionPool(10);
static int LoadHistory[360];
static int LoadHistoryPointer;
static int TotalLoad;
static int TotalSend;
static int TotalRecv;
static uint32 LagEnd;
static uint32 EarliestFreeAccountAdmissionRound;
static store<TWaitinglistEntry, 100> Waitinglist;
static TWaitinglistEntry *WaitinglistHead;

static Semaphore CommunicationThreadMutex(1);
static bool UseOwnStacks;
static uint8 CommunicationThreadStacks[MAX_COMMUNICATION_THREADS][THREAD_OWN_STACK_SIZE];
static pid_t LastUsingCommunicationThread[MAX_COMMUNICATION_THREADS];
static int FreeCommunicationThreadStacks[MAX_COMMUNICATION_THREADS];
static int NumberOfFreeCommunicationThreadStacks;

// Communication Thread Stacks
// =============================================================================
void GetCommunicationThreadStack(int *StackNumber, void **Stack){
	*StackNumber = -1;
	*Stack = NULL;

	if(!UseOwnStacks){
		error("GetCommunicationThreadStack: Bibliothek unterstützt keine eigenen Stacks.\n");
		return;
	}

	CommunicationThreadMutex.down();
	for(int i = 0; i < NumberOfFreeCommunicationThreadStacks; i += 1){
		int FreeStack = FreeCommunicationThreadStacks[i];
		if(LastUsingCommunicationThread[FreeStack] == 0
				|| kill(LastUsingCommunicationThread[FreeStack], 0) == -1){
			// NOTE(fusion): A little swap and pop action.
			NumberOfFreeCommunicationThreadStacks -= 1;
			FreeCommunicationThreadStacks[i] = FreeCommunicationThreadStacks[NumberOfFreeCommunicationThreadStacks];

			*StackNumber = FreeStack;
			*Stack = CommunicationThreadStacks[FreeStack];
			break;
		}
	}
	CommunicationThreadMutex.up();
}

void AttachCommunicationThreadStack(int StackNumber){
	LastUsingCommunicationThread[StackNumber] = getpid();
}

void ReleaseCommunicationThreadStack(int StackNumber){
	CommunicationThreadMutex.down();
	FreeCommunicationThreadStacks[NumberOfFreeCommunicationThreadStacks] = StackNumber;
	NumberOfFreeCommunicationThreadStacks += 1;
	CommunicationThreadMutex.up();
}

void InitCommunicationThreadStacks(void){
	if(UseOwnStacks){
		memset(CommunicationThreadStacks, 0xAA, sizeof(CommunicationThreadStacks));
		for(int i = 0; i < MAX_COMMUNICATION_THREADS; i += 1){
			LastUsingCommunicationThread[i] = 0;
			FreeCommunicationThreadStacks[i] = i;
		}
		NumberOfFreeCommunicationThreadStacks = MAX_COMMUNICATION_THREADS;
	}
}

void ExitCommunicationThreadStacks(void){
	if(UseOwnStacks){
		if(NumberOfFreeCommunicationThreadStacks != MAX_COMMUNICATION_THREADS){
			error("FreeCommunicationThreadStacks: Nicht alle Stacks freigegeben.\n");
		}

		int HighestStackAddress = -1;
		int LowestStackAddress = THREAD_OWN_STACK_SIZE;
		for(int i = 0; i < MAX_COMMUNICATION_THREADS; i += 1){
			for(int Addr = 0; Addr < THREAD_OWN_STACK_SIZE; Addr += 1){
				if(CommunicationThreadStacks[i][Addr] != 0xAA){
					if(Addr < LowestStackAddress){
						LowestStackAddress = Addr;
					}

					if(Addr > HighestStackAddress){
						HighestStackAddress = Addr;
					}
				}
			}
		}

		// NOTE(fusion): It seems we want to track whether the stack size is
		// too small but I'd argue the method is not very robust.
		if((HighestStackAddress - LowestStackAddress) > (THREAD_OWN_STACK_SIZE / 2)){
			error("Maximale Stack-Ausdehnung: %d..%d\n", LowestStackAddress, HighestStackAddress);
		}
	}
}

// Load History
// =============================================================================
void InitLoadHistory(void){
	for(int i = 0; i < NARRAY(LoadHistory); i += 1){
		LoadHistory[i] = 0;
	}
	LoadHistoryPointer = 0;
	TotalLoad = 0;
	TotalSend = 0;
	TotalRecv = 0;
	LagEnd = 0;
	EarliestFreeAccountAdmissionRound = 0;
	InitLog("netload");
}

bool LagDetected(void){
	return RoundNr <= LagEnd;
}

void NetLoad(int Amount, bool Send){
	CommunicationThreadMutex.down();
	if(Send){
		TotalSend += Amount;
	}else{
		TotalRecv += Amount;
	}
	CommunicationThreadMutex.up();
}

void NetLoadSummary(void){
	CommunicationThreadMutex.down();
	Log("netload", "gesendet:  %d Bytes.\n", TotalSend);
	Log("netload", "empfangen: %d Bytes.\n", TotalRecv);
	TotalSend = 0;
	TotalRecv = 0;
	CommunicationThreadMutex.up();
}

void NetLoadCheck(void){
	static int LastRecv;

	int DeltaRecv = TotalRecv - LastRecv;
	LastRecv = TotalRecv;
	if(DeltaRecv < 0){
		return;
	}

	int DeltaRecvPerPlayer = 0;
	int PlayersOnline = GetPlayersOnline();
	if(PlayersOnline > 0){
		DeltaRecvPerPlayer = DeltaRecv / PlayersOnline;
	}

	TotalLoad -= LoadHistory[LoadHistoryPointer];
	TotalLoad += DeltaRecvPerPlayer;
	LoadHistory[LoadHistoryPointer] = DeltaRecvPerPlayer;
	LoadHistoryPointer += 1;
	if(LoadHistoryPointer >= NARRAY(LoadHistory)){
		LoadHistoryPointer = 0;
	}

	// NOTE(fusion): Running this lag check only makes sense if `LoadHistory`
	// is filled up which won't be the case until this function executes at
	// least as many times as there are entries in `LoadHistory`.
	//	Looking at `AdvanceGame`, we see that this function is called every
	// 10 rounds, giving us the value of `EarliestLagCheckRound`.
	constexpr uint32 EarliestLagCheckRound = 10 * NARRAY(LoadHistory);
	if(RoundNr >= EarliestLagCheckRound && PlayersOnline >= 50){
		int AvgDeltaRecvPerPlayer = (TotalLoad / NARRAY(LoadHistory));
		if(DeltaRecvPerPlayer < (AvgDeltaRecvPerPlayer / 2)){
			Log("game", "Lag erkannt!\n");
			LagEnd = RoundNr + 30;

			// NOTE(fusion): This formula looks weird but when we take `MaxPlayers` 
			// and `PremiumPlayerBuffer` to be 950 and 150 (taken from the config
			// file, although their values are now loaded from the database), we
			// get a line with negative values when the number of players online
			// is less than 650, and exactly 60 when it is 950. Since the delay
			// is 60 when `PremiumPlayerBuffer` is zero, I don't think this is a
			// coincidence.
			int FreeAccountAdmissionDelay = 60;
			if(PremiumPlayerBuffer != 0){
				FreeAccountAdmissionDelay = (PlayersOnline + PremiumPlayerBuffer * 2 - MaxPlayers);
				FreeAccountAdmissionDelay = (FreeAccountAdmissionDelay * 30) / PremiumPlayerBuffer;
				if(FreeAccountAdmissionDelay < 0){
					FreeAccountAdmissionDelay = 0;
				}
			}

			uint32 FreeAccountAdmissionRound = RoundNr + (uint32)FreeAccountAdmissionDelay;
			if(EarliestFreeAccountAdmissionRound < FreeAccountAdmissionRound){
				EarliestFreeAccountAdmissionRound = FreeAccountAdmissionRound;
			}

			TConnection *Connection = GetFirstConnection();
			while(Connection != NULL){
				if(Connection->Live()){
					Connection->EmergencyPing();
				}
				Connection = GetNextConnection();
			}
		}
	}
}

// Communication Handling
// =============================================================================
bool WriteToSocket(TConnection *Connection, uint8 *Buffer, int Size){
	// TODO(fusion): I think `Size` refers to the payload but `Buffer` also has
	// room for writing the packet's length at the beginning and enough room for
	// padding (supposedly).

	while((Size % 8) != 0){
		Buffer[Size + 2] = rand_r(&Connection->RandomSeed);
		Size += 1;
	}

	for(int i = 0; i < Size; i += 8){
		Connection->SymmetricKey.encrypt(&Buffer[i + 2]);
	}

	TWriteBuffer WriteBuffer(Buffer, 2);
	WriteBuffer.writeWord((uint16)Size);

	int Attempts = 50;
	int BytesToWrite = Size + 2;
	uint8 *WritePtr = Buffer;
	while(BytesToWrite > 0){
		int ret = (int)write(Connection->GetSocket(), WritePtr, BytesToWrite);
		if(ret > 0){
			BytesToWrite -= ret;
			WritePtr += ret;
		}else if(ret == 0){
			// TODO(fusion): Can this even happen?
			error("WriteToSocket: Fehler %d beim Senden an Socket %d.\n",
					errno, Connection->GetSocket());
			return false;
		}else{
			if(errno == EINTR){
				continue;
			}

			if(errno != EAGAIN || Attempts <= 0){
				if(errno == ECONNRESET || errno == EPIPE || errno == EAGAIN){
					Log("game", "Verbindung an Socket %d zusammengebrochen.\n",
							Connection->GetSocket());
				}else{
					error("WriteToSocket: Fehler %d beim Senden an Socket %d.\n",
							errno, Connection->GetSocket());
				}
				return false;
			}

			DelayThread(0, 100000);
			Attempts -= 1;
		}
	}

	// TODO(fusion): Do we add 50 extra bytes to account for TCP segment headers?
	// This does make sense If we assume packets are split into ~2.5 segments on
	// average, with each segment header being 20 bytes.
	NetLoad(Size + 50, true);
	return true;
}

bool SendLoginMessage(TConnection *Connection, int Type, char *Message, int WaitingTime){
	// TODO(fusion):
	//	LOGIN_MESSAGE_ERROR = 20
	//	LOGIN_MESSAGE_? = 21
	//	LOGIN_MESSAGE_WAITING_LIST = 22
	if(Type != 20 && Type != 21 && Type != 22){
		error("SendLoginMessage: Ungültiger Meldungstyp %d.\n", Type);
		return true;
	}

	if(Message == NULL){
		error("SendLoginMessage: Message ist NULL.\n");
		return true;
	}

	if(Type == 22 && (WaitingTime < 0 || WaitingTime > UINT8_MAX)){
		error("SendLoginMessage: Ungültige Wartezeit %d.\n", WaitingTime);
		return true;
	}

	if(strlen(Message) > 290){
		error("SendLoginMessage: Botschaft zu lang (%s).\n", Message);
		return true;
	}

	// TODO(fusion): Writing output messages should have more robust helpers
	// to avoid all sorts of memory bugs but since we're only doing it in two
	// places from what I've seen, I'm not actually gonna bother (for now at
	// least).

	// NOTE(fusion): We make sure we leave two extra bytes at the beginning so
	// `WriteToSocket` can write the packet size. We also make sure that the
	// remainder of the buffer has a size that is multiple of 8 so `WriteToSocket`
	// can add any necessary padding for XTEA encryption without overflowing it.
	uint8 Data[302]; // 2 + 300
	TWriteBuffer WriteBuffer(Data + 2, sizeof(Data) - 2);
	WriteBuffer.writeWord(0);
	WriteBuffer.writeByte((uint8)Type);
	WriteBuffer.writeString(Message);
	if(Type == 22){
		WriteBuffer.writeByte(WaitingTime);
	}

	int Size = WriteBuffer.Position;
	WriteBuffer.Position = 0;
	WriteBuffer.writeWord((uint16)(Size - 2));
	return WriteToSocket(Connection, Data, Size);
}

#ifndef TIBIA_CONNECTIONS_HH_
#define TIBIA_CONNECTIONS_HH_ 1

#include "common.hh"
#include "crypto.hh"
#include "enums.hh"

struct TConnection;
struct TPlayer;

// TODO(fusion): The maximum number of connections should probably be kept in
// sync with the maximum number of communication threads, or maybe it is the
// same constant.
#define MAX_CONNECTIONS 1100

struct TKnownCreature {
	KNOWNCREATURESTATE State;
	uint32 CreatureID;
	TKnownCreature *Next;
	TConnection *Connection;
};

struct TConnection {
	TConnection(void);
	void Process(void);
	void ResetTimer(int Command);
	void EmergencyPing(void);
	pid_t GetPID(void);
	int GetSocket(void);
	const char *GetIPAddress(void);
	void Free(void);
	void Assign(void);
	void Connect(int Socket);
	void Login(void);
	bool JoinGame(TReadBuffer *Buffer);
	void EnterGame(void);
	void Die(void);
	void Logout(int Delay, bool StopFight);
	void Close(bool Delay);
	void Disconnect(void);
	TPlayer *GetPlayer(void);
	const char *GetName(void);
	void GetPosition(int *x, int *y, int *z);
	bool IsVisible(int x, int y, int z);
	KNOWNCREATURESTATE KnownCreature(uint32 ID, bool UpdateFollows);
	uint32 NewKnownCreature(uint32 NewID);
	void ClearKnownCreatureTable(bool Unchain);
	void UnchainKnownCreature(uint32 ID);

	bool InGame(void) const {
		return this->State == CONNECTION_GAME
			|| this->State == CONNECTION_DEAD;
	}

	bool Live(void) const {
		return this->State == CONNECTION_LOGIN
			|| this->State == CONNECTION_GAME
			|| this->State == CONNECTION_DEAD
			|| this->State == CONNECTION_LOGOUT;
	}

	// DATA
	// =================
	uint8 InData[2048];
	int InDataSize;
	bool SigIOPending;
	bool WaitingForACK;
	uint8 OutData[16384];
	int NextToSend;
	int NextToCommit;
	int NextToWrite;
	bool Overflow;
	bool WillingToSend;
	TConnection *NextSendingConnection;
	uint32 RandomSeed;
	CONNECTIONSTATE State;
	pid_t PID;
	int Socket;
	char IPAddress[16];
	TXTEASymmetricKey SymmetricKey;
	bool ConnectionIsOk;
	bool ClosingIsDelayed;
	uint32 TimeStamp;
	uint32 TimeStampAction;
	int TerminalType;
	int TerminalVersion;
	int TerminalOffsetX;
	int TerminalOffsetY;
	int TerminalWidth;
	int TerminalHeight;
	uint32 CharacterID;
	char Name[31];
	TKnownCreature KnownCreatureTable[150];
};

TConnection *AssignFreeConnection(void);
TConnection *GetFirstConnection(void);
TConnection *GetNextConnection(void);
void ProcessConnections(void);
void InitConnections(void);
void ExitConnections(void);

#endif // TIBIA_CONNECTIONS_HH_

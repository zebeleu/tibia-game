#ifndef TIBIA_CONNECTION_HH_
#define TIBIA_CONNECTION_HH_ 1

#include "common.hh"
#include "crypto.hh"
#include "enums.hh"

struct TConnection;
struct TPlayer;

struct TKnownCreature {
	KNOWNCREATURESTATE State;
	uint32 CreatureID;
	TKnownCreature *Next;
	TConnection *Connection;
};

struct TConnection {
	TPlayer *GetPlayer(void);
	void Logout(int Delay, bool StopFight);
	bool IsVisible(int x, int y, int z);
	void EnterGame(void);
	const char *GetIPAddress(void);
	void Die(void);
	KNOWNCREATURESTATE KnownCreature(uint32 CreatureID, bool UpdateFollows);
	int GetSocket(void);
	void EmergencyPing(void);
	void Login(void);
	const char *GetName(void);
	void Connect(int Socket);
	void Close(bool Delay);
	void Free(void);
	pid_t GetPID(void);

	// TODO(fusion): Maybe rename this later?
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
	uint8 field5_0x4806;
	uint8 field6_0x4807;
	int NextToSend;
	int NextToCommit;
	int NextToWrite;
	bool Overflow;
	bool WillingToSend;
	uint8 field12_0x4816;
	uint8 field13_0x4817;
	TConnection *NextSendingConnection;
	uint32 RandomSeed;
	CONNECTIONSTATE State;
	pid_t PID;
	int Socket;
	char IPAddress[16];
	TXTEASymmetricKey SymmetricKey;
	bool ConnectionIsOk;
	bool ClosingIsDelayed;
	uint8 field23_0x4852;
	uint8 field24_0x4853;
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
	uint8 field35_0x4897;
	TKnownCreature KnownCreatureTable[150];
};

#endif // TIBIA_CONNECTION_HH_

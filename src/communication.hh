#ifndef TIBIA_COMMUNICATION_HH_
#define TIBIA_COMMUNICATION_HH_ 1

#include "common.hh"
#include "connections.hh"
#include "cr.hh"

struct TWaitinglistEntry {
    TWaitinglistEntry *Next;
    char Name[30];
    uint32 NextTry;
    bool FreeAccount;
    bool Newbie;
    bool Sleeping;
};

void GetCommunicationThreadStack(int *StackNumber, void **Stack);
void AttachCommunicationThreadStack(int StackNumber);
void ReleaseCommunicationThreadStack(int StackNumber);
void InitCommunicationThreadStacks(void);
void ExitCommunicationThreadStacks(void);

void InitLoadHistory(void);
bool LagDetected(void);
void NetLoad(int Amount, bool Send);
void NetLoadSummary(void);
void NetLoadCheck(void);

bool WriteToSocket(TConnection *Connection, uint8 *Buffer, int Size);
bool SendLoginMessage(TConnection *Connection, int Type, const char *Message, int WaitingTime);
bool SendData(TConnection *Connection);
bool SendData(TConnection *Connection, const uint8 *Data, int Size);

bool GetWaitinglistEntry(const char *Name, uint32 *NextTry, bool *FreeAccount, bool *Newbie);
void InsertWaitinglistEntry(const char *Name, uint32 NextTry, bool FreeAccount, bool Newbie);
void DeleteWaitinglistEntry(const char *Name);
int GetWaitinglistPosition(const char *Name, bool FreeAccount, bool Newbie);
int CheckWaitingTime(const char *Name, TConnection *Connection, bool FreeAccount, bool Newbie);

int ReadFromSocket(TConnection *Connection, uint8 *Buffer, int Size);
bool CallGameThread(TConnection *Connection);
bool CheckConnection(TConnection *Connection);
TPlayerData *PerformRegistration(TConnection *Connection, char *PlayerName,
		uint32 AccountID, const char *PlayerPassword, bool GamemasterClient);
bool HandleLogin(TConnection *Connection);
bool ReceiveCommand(TConnection *Connection);

#endif //TIBIA_COMMUNICATION_HH_

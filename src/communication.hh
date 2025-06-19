#ifndef TIBIA_COMMUNICATION_HH_
#define TIBIA_COMMUNICATION_HH_ 1

#include "common.hh"
#include "connections.hh"

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
bool SendLoginMessage(TConnection *Connection, int Type, char *Message, int WaitingTime);

#endif //TIBIA_COMMUNICATION_HH_

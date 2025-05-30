#ifndef TIBIA_STUBS_HH_
#define TIBIA_STUBS_HH_ 1

#include "common.hh"
#include "enums.hh"
#include "connection.hh"
#include "creature.hh"
#include "map.hh"
#include "player.hh"

// IMPORTANT(fusion): These function definitions exist to test compilation. They're
// not yet implemented and will cause the linker to fail with unresolved symbols.

typedef void TRefreshSectorFunction(int SectorX, int SectorY, int SectorZ, const uint8 *Data, int Size);
typedef void TSendMailsFunction(TPlayerData *PlayerData);

extern void AbortWriter(void);
extern void AnnounceChangedCreature(uint32 CreatureID, int Type);
extern void BroadcastMessage(int Mode, const char *Text, ...) ATTR_PRINTF(2, 3);
extern void Change(Object Obj, ObjectType NewType, uint32 Value);
extern bool CheckRight(uint32 CreatureID, RIGHT Right);
extern void CleanHouseField(int x, int y, int z);
extern void CreatePlayerList(bool Online);
extern Object GetBodyObject(uint32 CreatureID, int Position);
extern TCreature *GetCreature(uint32 CreatureID);
extern TConnection *GetFirstConnection(void);
extern TConnection *GetNextConnection(void);
extern void Log(const char *ProtocolName, const char *Text, ...) ATTR_PRINTF(2, 3);
extern void LogoutAllPlayers(void);
extern void Move(uint32 CreatureID, Object Obj, Object Con, int Count, bool NoMerge, Object Ignore);
extern void MoveCreatures(int Delay);
extern void NetLoadCheck(void);
extern void NetLoadSummary(void);
extern void NotifyAllCreatures(Object Obj, int Type, Object OldCon);
extern int ObjectDistance(Object Obj1, Object Obj2);
extern void ProcessCommunicationControl(void);
extern void ProcessConnections(void);
extern void ProcessCreatures(void);
extern void ProcessCronSystem(void);
extern void ProcessMonsterhomes(void);
extern void ProcessMonsterRaids(void);
extern void ProcessReaderThreadReplies(TRefreshSectorFunction *RefreshSector, TSendMailsFunction *SendMails);
extern void ProcessSkills(void);
extern void ProcessWriterThreadReplies(void);
extern void ReceiveData(void);
extern void RefreshCylinders(void);
extern void RefreshMap(void);
extern void RefreshSector(int SectorX, int SectorY, int SectorZ, const uint8 *Data, int Size);
extern void SavePlayerDataOrder(void);
extern void SendAll(void);
extern void SendAmbiente(TConnection *Connection);
extern void SendClearTarget(TConnection *Connection);
extern void SendMails(TPlayerData *PlayerData);
extern void SendMessage(TConnection *Connection, int Mode, const char *Text, ...) ATTR_PRINTF(3, 4);
extern void SendPlayerData(TConnection *Connection);
extern void SendPlayerSkills(TConnection *Connection);
extern void SendPlayerState(TConnection *Connection, uint8 State);
extern void WriteKillStatistics(void);

#endif //TIBIA_STUBS_HH_

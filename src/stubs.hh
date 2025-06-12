#ifndef TIBIA_STUBS_HH_
#define TIBIA_STUBS_HH_ 1

#include "common.hh"
#include "enums.hh"
#include "connection.hh"
#include "cr.hh"
#include "magic.hh"
#include "map.hh"

// IMPORTANT(fusion): These function definitions exist to test compilation. They're
// not yet implemented and will cause the linker to fail with unresolved symbols.

typedef void TRefreshSectorFunction(int SectorX, int SectorY, int SectorZ, const uint8 *Data, int Size);
typedef void TSendMailsFunction(TPlayerData *PlayerData);

extern void AbortWriter(void);
extern void AnnounceChangedCreature(uint32 CreatureID, int Type);
extern void AnnounceChangedObject(Object Obj, int Type);
extern void BroadcastMessage(int Mode, const char *Text, ...) ATTR_PRINTF(2, 3);
extern void Change(Object Obj, ObjectType NewType, uint32 Value);
extern void ChangeNPCState(TCreature *Npc, int NewState, bool Stimulus);
extern void CharacterDeathOrder(TCreature *Creature, int OldLevel,
			uint32 Offender, const char *Remark, bool Unjustified);
extern bool CheckRight(uint32 CreatureID, RIGHT Right);
extern void CleanHouseField(int x, int y, int z);
extern void ConvinceMonster(TCreature *Master, TCreature *Slave);
extern void ChallengeMonster(TCreature *Challenger, TCreature *Monster);
extern Object Create(Object Con, ObjectType Type, uint32 Value);
extern Object CreateAtCreature(uint32 CreatureID, ObjectType Type, uint32 Value);
extern TCreature *CreateMonster(int Race, int x, int y, int z, int Home, uint32 Master, bool ShowEffect);
extern void CreatePlayerList(bool Online);
extern void CreatePool(Object Con, ObjectType Type, uint32 Value);
extern void Delete(Object Obj, int Count);
extern void GetExitPosition(uint16 HouseID, int *x, int *y, int *z);
extern TConnection *GetFirstConnection(void);
extern TConnection *GetNextConnection(void);
extern TPlayer *GetPlayer(uint32 CreatureID);
extern void GraphicalEffect(int x, int y, int z, int Type);
extern void GraphicalEffect(Object Obj, int Type);
extern int IdentifyPlayer(const char *Name, bool ExactMatch, bool IgnoreGamemasters, TPlayer **Player);
extern void InitLog(const char *ProtocolName);
extern void KickGuest(uint16 HouseID, TPlayer *Host, TPlayer *Guest);
extern void KillStatisticsOrder(int NumberOfRaces, const char *RaceNames, int *KilledPlayers, int *KilledCreatures);
extern bool LagDetected(void);
extern void LoadMonsterRaid(const char *FileName, int Start,
		bool *Type, int *Date, int *Interval, int *Duration);
extern void Log(const char *ProtocolName, const char *Text, ...) ATTR_PRINTF(2, 3);
extern void LogoutAllPlayers(void);
extern void Missile(Object Start, Object Dest, int Type);
extern void Merge(uint32 CreatureID, Object Obj, Object Dest, int Count, Object Ignore);
extern void Move(uint32 CreatureID, Object Obj, Object Con, int Count, bool NoMerge, Object Ignore);
extern void NetLoadCheck(void);
extern void NetLoadSummary(void);
extern void NotifyAllCreatures(Object Obj, int Type, Object OldCon);
extern void ProcessCommunicationControl(void);
extern void ProcessConnections(void);
extern void ProcessCronSystem(void);
extern void ProcessMonsterhomes(void);
extern void ProcessReaderThreadReplies(TRefreshSectorFunction *RefreshSector, TSendMailsFunction *SendMails);
extern void ProcessWriterThreadReplies(void);
extern void ReceiveData(void);
extern void RefreshCylinders(void);
extern void RefreshMap(void);
extern void RefreshSector(int SectorX, int SectorY, int SectorZ, const uint8 *Data, int Size);
extern void SavePlayerDataOrder(void);
extern void SendAll(void);
extern void SendAmbiente(TConnection *Connection);
extern void SendClearTarget(TConnection *Connection);
extern void SendCloseContainer(TConnection *Connection, int ContainerNr);
extern void SendCloseTrade(TConnection *Connection);
extern void SendCreatureHealth(TConnection *Connection, uint32 CreatureID);
extern void SendCreatureLight(TConnection *Connection, uint32 CreatureID);
extern void SendCreatureOutfit(TConnection *Connection, uint32 CreatureID);
extern void SendCreatureSpeed(TConnection *Connection, uint32 CreatureID);
extern void SendCreatureSkull(TConnection *Connection, uint32 CreatureID);
extern void SendCreatureParty(TConnection *Connection, uint32 CreatureID);
extern void SendDeleteField(TConnection *Connection, int x, int y, int z, Object Obj);
extern void SendAddField(TConnection *Connection, int x, int y, int z, Object Obj);
extern void SendChangeField(TConnection *Connection, int x, int y, int z, Object Obj);
extern void SendDeleteInContainer(TConnection *Connection, int ContainerNr, Object Obj);
extern void SendCreateInContainer(TConnection *Connection, int ContainerNr, Object Obj);
extern void SendChangeInContainer(TConnection *Connection, int ContainerNr, Object Obj);
extern void SendDeleteInventory(TConnection *Connection, int Position);
extern void SendSetInventory(TConnection *Connection, int Position, Object Obj);
extern void SendEditList(TConnection *Connection, uint8 ListType, uint32 ID, const char *Text);
extern void SendFullScreen(TConnection *Connection);
extern void SendFloors(TConnection *Connection, bool Up);
extern void SendGraphicalEffect(TConnection *Connection, int x, int y, int z, int Type);
extern void SendTextualEffect(TConnection *Connection, int x, int y, int z, int Color, const char *Text);
extern void SendMissileEffect(TConnection *Connection, int OrigX, int OrigY, int OrigZ, int DestX, int DestY, int DestZ, int Type);
extern void SendRow(TConnection *Connection, int Direction);
extern void SendMails(TPlayerData *PlayerData);
extern void SendMarkCreature(TConnection *Connection, uint32 CreatureID, int Color);
extern void SendMessage(TConnection *Connection, int Mode, const char *Text, ...) ATTR_PRINTF(3, 4);
extern void SendMoveCreature(TConnection *Connection, uint32 CreatureID, int x, int y, int z);
extern void SendPlayerData(TConnection *Connection);
extern void SendPlayerSkills(TConnection *Connection);
extern void SendPlayerState(TConnection *Connection, uint8 State);
extern void SendResult(TConnection *Connection, RESULT r);
extern void SendSnapback(TConnection *Connection);
extern void SendTradeOffer(TConnection *Connection, const char *Name, bool OwnOffer, Object Obj);
extern void ShowGuestList(uint16 HouseID, TPlayer *Player, char *Buffer);
extern void ShowSubownerList(uint16 HouseID, TPlayer *Player, char *Buffer);
extern void ShowNameDoor(Object Door, TPlayer *Player, char *Buffer);
extern void Talk(uint32 CreatureID, int Mode, const char *Addressee, const char *Text, bool CheckSpamming);
extern void TextualEffect(Object Obj, int Color, const char *Text, ...) ATTR_PRINTF(3, 4);
extern void Turn(uint32 CreatureID, Object Obj);
extern void Use(uint32 CreatureID, Object Obj1, Object Object2, uint8 Info);

#endif //TIBIA_STUBS_HH_

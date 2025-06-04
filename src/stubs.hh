#ifndef TIBIA_STUBS_HH_
#define TIBIA_STUBS_HH_ 1

#include "common.hh"
#include "enums.hh"
#include "connection.hh"
#include "creature.hh"
#include "magic.hh"
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
extern void CharacterDeathOrder(TCreature *Creature, int OldLevel,
			uint32 Offender, const char *Remark, bool Unjustified);
extern bool CheckRight(uint32 CreatureID, RIGHT Right);
extern void CircleShapeSpell(TCreature *Actor, int DestX, int DestY, int DestZ,
				int Range, int Animation, int Radius, TImpact *Impact, int Effect);
extern void CleanHouseField(int x, int y, int z);
extern int ComputeDamage(TCreature *Creature, int SpellNr, int Damage, int Variation);
extern void ConvinceMonster(TCreature *Master, TCreature *Slave);
extern void ChallengeMonster(TCreature *Challenger, TCreature *Monster);
extern int CountInventoryObjects(uint32 CreatureID, ObjectType Type, uint32 Value);
extern Object CreateAtCreature(uint32 CreatureID, ObjectType Type, uint32 Value);
extern void CreateField(int x, int y, int z, int FieldType, uint32 Owner, bool Peaceful);
extern void CreateMonster(int Race, int x, int y, int z, int Home, uint32 Master, bool ShowEffect);
extern void CreatePlayerList(bool Online);
extern void CreatePool(Object Con, ObjectType Type, uint32 Value);
extern void Delete(Object Obj, int Count);
extern bool FieldPossible(int x, int y, int z, int FieldType);
extern Object GetBodyObject(uint32 CreatureID, int Position);
extern TCreature *GetCreature(uint32 CreatureID);
extern TCreature *GetCreature(Object Obj);
extern TConnection *GetFirstConnection(void);
extern TConnection *GetNextConnection(void);
extern const char *GetName(Object Obj);
extern TPlayer *GetPlayer(uint32 CreatureID);
extern bool GetRaceNoConvince(int Race);
extern bool GetRaceNoIllusion(int Race);
extern bool GetRaceNoParalyze(int Race);
extern bool GetRaceNoSummon(int Race);
extern TOutfit GetRaceOutfit(int Race);
extern int GetRacePoison(int Race);
extern int GetRaceSummonCost(int Race);
extern void GraphicalEffect(int x, int y, int z, int Type);
extern void GraphicalEffect(Object Obj, int Type);
extern int IdentifyPlayer(const char *Name, bool ExactMatch, bool IgnoreGamemasters, TPlayer **Player);
extern void InitLog(const char *ProtocolName);
extern void KickGuest(uint16 HouseID, TPlayer *Host, TPlayer *Guest);
extern void LoadMonsterRaid(const char *FileName, int Start,
		bool *Type, int *Date, int *Interval, int *Duration);
extern void Log(const char *ProtocolName, const char *Text, ...) ATTR_PRINTF(2, 3);
extern void LogoutAllPlayers(void);
extern void Missile(Object Start, Object Dest, int Type);
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
extern bool SearchFlightField(uint32 FugitiveID, uint32 PursuerID, int *x, int *y, int *z);
extern bool SearchSummonField(int *x, int *y, int *z, int Distance);
extern void SendAll(void);
extern void SendAmbiente(TConnection *Connection);
extern void SendClearTarget(TConnection *Connection);
extern void SendEditList(TConnection *Connection, uint8 ListType, uint32 ID, const char *Text);
extern void SendMails(TPlayerData *PlayerData);
extern void SendMarkCreature(TConnection *Connection, uint32 CreatureID, int Color);
extern void SendMessage(TConnection *Connection, int Mode, const char *Text, ...) ATTR_PRINTF(3, 4);
extern void SendPlayerData(TConnection *Connection);
extern void SendPlayerSkills(TConnection *Connection);
extern void SendPlayerState(TConnection *Connection, uint8 State);
extern void ShowGuestList(uint16 HouseID, TPlayer *Player, char *Buffer);
extern void ShowSubownerList(uint16 HouseID, TPlayer *Player, char *Buffer);
extern void ShowNameDoor(Object Door, TPlayer *Player, char *Buffer);
extern void TextualEffect(Object Obj, int Color, const char *Text, ...) ATTR_PRINTF(3, 4);
extern bool ThrowPossible(int FromX, int FromY, int FromZ,
						int ToX, int ToY, int ToZ, int Power);
extern void WriteKillStatistics(void);

#endif //TIBIA_STUBS_HH_

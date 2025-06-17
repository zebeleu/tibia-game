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
extern void BroadcastMessage(int Mode, const char *Text, ...) ATTR_PRINTF(2, 3);
extern void ChangeNPCState(TCreature *Npc, int NewState, bool Stimulus);
extern void CharacterDeathOrder(TCreature *Creature, int OldLevel,
			uint32 Offender, const char *Remark, bool Unjustified);
extern void CleanHouseField(int x, int y, int z);
extern void ConvinceMonster(TCreature *Master, TCreature *Slave);
extern void ChallengeMonster(TCreature *Challenger, TCreature *Monster);
extern TCreature *CreateMonster(int Race, int x, int y, int z, int Home, uint32 Master, bool ShowEffect);
extern void CreatePlayerList(bool Online);
extern uint32 GetCharacterID(const char *Name);
extern const char *GetCharacterName(const char *Name);
extern void GetExitPosition(uint16 HouseID, int *x, int *y, int *z);
extern TConnection *GetFirstConnection(void);
extern TConnection *GetNextConnection(void);
extern const char *GetHouseName(uint16 HouseID);
extern const char *GetHouseOwner(uint16 HouseID);
extern TPlayer *GetPlayer(uint32 CreatureID);
extern void GetProfessionName(char *Buffer, int Profession, bool Article, bool Capitals);
extern int IdentifyPlayer(const char *Name, bool ExactMatch, bool IgnoreGamemasters, TPlayer **Player);
extern void InitLog(const char *ProtocolName);
extern void KickGuest(uint16 HouseID, TPlayer *Host, TPlayer *Guest);
extern void KillStatisticsOrder(int NumberOfRaces, const char *RaceNames, int *KilledPlayers, int *KilledCreatures);
extern bool LagDetected(void);
extern void LoadSectorOrder(int SectorX, int SectorY, int SectorZ);
extern void Log(const char *ProtocolName, const char *Text, ...) ATTR_PRINTF(2, 3);
extern void LogoutAllPlayers(void);
extern void NetLoadCheck(void);
extern void NetLoadSummary(void);
extern void PrepareHouseCleanup(void);
extern void FinishHouseCleanup(void);
extern void PlayerlistOrder(int NumberOfPlayers, char *PlayerNames, int *PlayerLevels, int *PlayerProfessions);
extern void ProcessConnections(void);
extern void ProcessMonsterhomes(void);
extern void ProcessReaderThreadReplies(TRefreshSectorFunction *RefreshSector, TSendMailsFunction *SendMails);
extern void ProcessWriterThreadReplies(void);
extern void ReceiveData(void);
extern void SavePlayerData(TPlayerData *Slot);
extern bool LoadPlayerData(TPlayerData *Slot);
extern bool PlayerDataExists(uint32 CharacterID);
extern void SavePlayerDataOrder(void);
extern void SendAll(void);
extern void SendAmbiente(TConnection *Connection);
extern void SendBuddyData(TConnection *Connection, uint32 CharacterID, const char *Name, bool Online);
extern void SendBuddyStatus(TConnection *Connection, uint32 CharacterID, bool Online);
extern void SendClearTarget(TConnection *Connection);
extern void SendContainer(TConnection *Connection, int ContainerNr);
extern void SendCloseChannel(TConnection *Connection, int ChannelID);
extern void SendCloseContainer(TConnection *Connection, int ContainerNr);
extern void SendCloseRequest(TConnection *Connection);
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
extern void SendOpenOwnChannel(TConnection *Connection, int Channel);
extern void SendPlayerData(TConnection *Connection);
extern void SendPlayerSkills(TConnection *Connection);
extern void SendPlayerState(TConnection *Connection, uint8 State);
extern void SendResult(TConnection *Connection, RESULT r);
extern void SendSnapback(TConnection *Connection);
extern void SendTalk(TConnection *Connection, uint32 StatementID,
		const char *Sender, int Mode, const char *Text, int Data);
extern void SendTalk(TConnection *Connection, uint32 StatementID,
		const char *Sender, int Mode, int Channel, const char *Text);
extern void SendTalk(TConnection *Connection, uint32 StatementID,
		const char *Sender, int Mode, int x,int y,int z, const char *Text);
extern void SendTradeOffer(TConnection *Connection, const char *Name, bool OwnOffer, Object Obj);
extern void ShowGuestList(uint16 HouseID, TPlayer *Player, char *Buffer);
extern void ShowSubownerList(uint16 HouseID, TPlayer *Player, char *Buffer);
extern void ShowNameDoor(Object Door, TPlayer *Player, char *Buffer);

// moveuse.cc
extern void UseContainer(uint32 CreatureID, Object Con, uint8 ContainerNr);
extern void UseChest(uint32 CreatureID, Object Chest);
extern void UseLiquidContainer(uint32 CreatureID, Object Obj, Object Dest);
extern void UseFood(uint32 CreatureID, Object Obj);
extern void UseTextObject(uint32 CreatureID, Object Obj);
extern void UseAnnouncer(uint32 CreatureID, Object Obj);
extern void UseKeyDoor(uint32 CreatureID, Object Key, Object Door);
extern void UseNameDoor(uint32 CreatureID, Object Door);
extern void UseLevelDoor(uint32 CreatureID, Object Door);
extern void UseQuestDoor(uint32 CreatureID, Object Door);
extern void UseWeapon(uint32 CreatureID, Object Weapon, Object Target);
extern void UseChangeObject(uint32 CreatureID, Object Obj);
extern void UseObject(uint32 CreatureID, Object Obj);
extern void UseObjects(uint32 CreatureID, Object Obj1, Object Obj2);
extern void MovementEvent(Object Obj, Object Start, Object Dest);
extern void CollisionEvent(Object Obj, Object Dest);
extern void SeparationEvent(Object Obj, Object Start);

#endif //TIBIA_STUBS_HH_

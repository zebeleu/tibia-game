#ifndef TIBIA_STUBS_HH_
#define TIBIA_STUBS_HH_ 1

#include "common.hh"
#include "enums.hh"
#include "connections.hh"
#include "cr.hh"
#include "magic.hh"
#include "map.hh"
#include "operate.hh"

// IMPORTANT(fusion): These function definitions exist to test compilation. They're
// not yet implemented and will cause the linker to fail with unresolved symbols.

typedef void TRefreshSectorFunction(int SectorX, int SectorY, int SectorZ, const uint8 *Data, int Size);
typedef void TSendMailsFunction(TPlayerData *PlayerData);

extern void AbortWriter(void);
extern void AddBuddyOrder(TCreature *Creature, uint32 CharacterID);
extern void RemoveBuddyOrder(TCreature *Creature, uint32 BuddyID);
extern void ChangeGuests(uint16 HouseID, TPlayer *Player, const char *Text);
extern void ChangeSubowners(uint16 HouseID, TPlayer *Player, const char *Text);
extern void ChangeNameDoor(Object Door, TPlayer *Player, const char *Text);
extern void CharacterDeathOrder(TCreature *Creature, int OldLevel,
			uint32 Offender, const char *Remark, bool Unjustified);
extern void CleanHouseField(int x, int y, int z);
extern void DecrementIsOnlineOrder(uint32 CharacterID);
extern void GetExitPosition(uint16 HouseID, int *x, int *y, int *z);
extern int GetOrderBufferSpace(void);
extern const char *GetHouseName(uint16 HouseID);
extern const char *GetHouseOwner(uint16 HouseID);
extern void InitLog(const char *ProtocolName);
extern bool IsInvited(uint16 HouseID, TPlayer *Player, int TimeStamp);
extern bool IsOwner(uint16 HouseID, TPlayer *Player);
extern void KickGuest(uint16 HouseID, TPlayer *Host, TPlayer *Guest);
extern void KillStatisticsOrder(int NumberOfRaces, const char *RaceNames, int *KilledPlayers, int *KilledCreatures);
extern void LoadCharacterOrder(uint32 CharacterID);
extern void LoadSectorOrder(int SectorX, int SectorY, int SectorZ);
extern void Log(const char *ProtocolName, const char *Text, ...) ATTR_PRINTF(2, 3);
extern void LogoutOrder(TPlayer *Player);
extern bool MayOpenDoor(Object Door, TPlayer *Player);
extern void PrepareHouseCleanup(void);
extern void FinishHouseCleanup(void);
extern void PlayerlistOrder(int NumberOfPlayers, char *PlayerNames, int *PlayerLevels, int *PlayerProfessions);
extern void ProcessReaderThreadReplies(TRefreshSectorFunction *RefreshSector, TSendMailsFunction *SendMails);
extern void ProcessWriterThreadReplies(void);
extern void PunishmentOrder(TCreature *Creature, const char *Name, const char *IPAddress,
					int Reason, int Action, const char *Comment, int NumberOfStatements,
					vector<TReportedStatement> *ReportedStatements, uint32 StatementID,
					bool IPBanishment);
extern void SavePlayerData(TPlayerData *Slot);
extern bool LoadPlayerData(TPlayerData *Slot);
extern bool PlayerDataExists(uint32 CharacterID);
extern void SavePlayerDataOrder(void);
extern void ShowGuestList(uint16 HouseID, TPlayer *Player, char *Buffer);
extern void ShowSubownerList(uint16 HouseID, TPlayer *Player, char *Buffer);
extern void ShowNameDoor(Object Door, TPlayer *Player, char *Buffer);

#endif //TIBIA_STUBS_HH_

#ifndef TIBIA_STUBS_HH_
#define TIBIA_STUBS_HH_ 1

#include "common.hh"
#include "enums.hh"
#include "cr.hh"
#include "map.hh"

// IMPORTANT(fusion): These function definitions exist to test compilation. They're
// not yet implemented and will cause the linker to fail with unresolved symbols.

typedef void TRefreshSectorFunction(int SectorX, int SectorY, int SectorZ, const uint8 *Data, int Size);
typedef void TSendMailsFunction(TPlayerData *PlayerData);

extern void LoadCharacterOrder(uint32 CharacterID);
extern void LoadSectorOrder(int SectorX, int SectorY, int SectorZ);
extern void ProcessReaderThreadReplies(TRefreshSectorFunction *RefreshSector, TSendMailsFunction *SendMails);
extern void SavePlayerData(TPlayerData *Slot);
extern bool LoadPlayerData(TPlayerData *Slot);
extern bool PlayerDataExists(uint32 CharacterID);
extern void SavePlayerDataOrder(void);

#endif //TIBIA_STUBS_HH_

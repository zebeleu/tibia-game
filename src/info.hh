#ifndef TIBIA_INFO_HH_
#define TIBIA_INFO_HH_ 1

#include "common.hh"
#include "map.hh"

// TODO(fusion): Probably move to `houses.hh` when we implement it?
constexpr uint16 HOUSEID_ANY = 0xFFFF;

bool JumpPossible(int x, int y, int z, bool AvoidPlayers);
bool SearchFreeField(int *x, int *y, int *z, int Distance, uint16 HouseID, bool Jump);

#endif //TIBIA_INFO_HH_

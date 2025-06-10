#ifndef TIBIA_INFO_HH_
#define TIBIA_INFO_HH_ 1

#include "common.hh"
#include "map.hh"

// TODO(fusion): Probably move to `houses.hh` when we implement it?
constexpr uint16 HOUSEID_ANY = 0xFFFF;
enum HouseList: uint8 {
	GUESTLIST = 1,
	SUBOWNERLIST = 2,
	DOORLIST = 3,
};

const char *GetLiquidName(int LiquidType);
uint8 GetLiquidColor(int LiquidType);
const char *GetName(Object Obj);
int GetWeight(Object Obj, int Count);
int GetCompleteWeight(Object Obj);
int GetRowWeight(Object Obj);
uint32 GetObjectCreatureID(Object Obj);
int GetObjectBodyPosition(Object Obj);
int GetObjectRNum(Object Obj);
bool ObjectInRange(uint32 CreatureID, Object Obj, int Range);
bool ObjectAccessible(uint32 CreatureID, Object Obj, int Range);
int ObjectDistance(Object Obj1, Object Obj2);
Object GetBodyContainer(uint32 CreatureID, int Position);
Object GetBodyObject(uint32 CreatureID, int Position);
Object GetTopObject(int x, int y, int z, bool Move);
Object GetContainer(uint32 CreatureID, int x, int y, int z);
Object GetObject(uint32 CreatureID, int x, int y, int z, int RNum, ObjectType Type);
Object GetRowObject(Object Obj, ObjectType Type, uint32 Value, bool Recurse);
Object GetInventoryObject(uint32 CreatureID, ObjectType Type, uint32 Value);
int CountObjectsInContainer(Object Con);
int CountObjects(Object Obj);
int CountObjects(Object Obj, ObjectType Type, uint32 Value);
int CountInventoryObjects(uint32 CreatureID, ObjectType Type, uint32 Value);
int CountMoney(Object Obj);
int CountInventoryMoney(uint32 CreatureID);
void CalculateChange(int Amount, int *Gold, int *Platinum, int *Crystal);
int GetHeight(int x, int y, int z);
bool JumpPossible(int x, int y, int z, bool AvoidPlayers);
bool FieldPossible(int x, int y, int z, int FieldType);
bool SearchFreeField(int *x, int *y, int *z, int Distance, uint16 HouseID, bool Jump);
bool SearchLoginField(int *x, int *y, int *z, int Distance, bool Player);
bool SearchSpawnField(int *x, int *y, int *z, int Distance, bool Player);
void InitInfo(void);
void ExitInfo(void);

#endif //TIBIA_INFO_HH_

#ifndef TIBIA_OPERATE_HH_
#define TIBIA_OPERATE_HH_ 1

#include "common.hh"
#include "map.hh"

enum : int {
	CREATURE_HEALTH_CHANGED		= 1,
	CREATURE_LIGHT_CHANGED		= 2,
	CREATURE_OUTFIT_CHANGED		= 3,
	CREATURE_SPEED_CHANGED		= 4,
	CREATURE_SKULL_CHANGED		= 5,
	CREATURE_PARTY_CHANGED		= 6,
};

enum : int {
	OBJECT_DELETED				= 0,
	OBJECT_CREATED				= 1,
	OBJECT_CHANGED				= 2,
	OBJECT_MOVED				= 3,
};

void AnnounceMovingCreature(uint32 CreatureID, Object Con);
void AnnounceChangedCreature(uint32 CreatureID, int Type);
void AnnounceChangedField(Object Obj, int Type);
void AnnounceChangedContainer(Object Obj, int Type);
void AnnounceChangedInventory(Object Obj, int Type);
void AnnounceChangedObject(Object Obj, int Type);
void AnnounceGraphicalEffect(int x, int y, int z, int Type);
void AnnounceTextualEffect(int x, int y, int z, int Color, const char *Text);
void AnnounceMissile(int OrigX, int OrigY, int OrigZ,
		int DestX, int DestY, int DestZ, int Type);
void CheckTopMoveObject(uint32 CreatureID, Object Obj, Object Ignore);
void CheckTopUseObject(uint32 CreatureID, Object Obj);
void CheckTopMultiuseObject(uint32 CreatureID, Object Obj);
void CheckMoveObject(uint32 CreatureID, Object Obj, bool Take);
void CheckMapDestination(uint32 CreatureID, Object Obj, Object MapCon);
void CheckMapPlace(uint32 CreatureID, ObjectType Type, Object MapCon);
void CheckContainerDestination(Object Obj, Object Con);
void CheckContainerPlace(ObjectType Type, Object Con, Object OldObj);
void CheckDepotSpace(uint32 CreatureID, Object Source, Object Destination, int Count);
void CheckInventoryDestination(Object Obj, Object Con, bool Split);
void CheckInventoryPlace(ObjectType Type, Object Con, Object OldObj);
void CheckWeight(uint32 CreatureID, Object Obj, int Count);
void CheckWeight(uint32 CreatureID, ObjectType Type, uint32 Value, int OldWeight);
void NotifyCreature(uint32 CreatureID, Object Obj, bool Inventory);
void NotifyCreature(uint32 CreatureID, ObjectType Type, bool Inventory);
void NotifyAllCreatures(Object Obj, int Type, Object OldCon);
void NotifyTrades(Object Obj);
void NotifyDepot(uint32 CreatureID, Object Obj, int Count);
void CloseContainer(Object Con, bool Force);
Object Create(Object Con, ObjectType Type, uint32 Value);
Object Copy(Object Con, Object Source);
void Move(uint32 CreatureID, Object Obj, Object Con, int Count, bool NoMerge, Object Ignore);
void Merge(uint32 CreatureID, Object Obj, Object Dest, int Count, Object Ignore);
void Change(Object Obj, ObjectType NewType, uint32 Value);
void Change(Object Obj, INSTANCEATTRIBUTE Attribute, uint32 Value);
void Delete(Object Obj, int Count);
void Empty(Object Con, int Remainder);
void GraphicalEffect(int x, int y, int z, int Type);
void GraphicalEffect(Object Obj, int Type);
void TextualEffect(Object Obj, int Color, const char *Format, ...) ATTR_PRINTF(3, 4);
void Missile(Object Start, Object Dest, int Type);

#endif //TIBIA_OPERATE_HH_

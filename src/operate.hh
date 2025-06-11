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
};

void CheckContainerDestination(Object Obj, Object Con);
void CheckInventoryDestination(Object Obj, Object Con, bool Split);

#endif //TIBIA_OPERATE_HH_

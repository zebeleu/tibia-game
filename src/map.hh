#ifndef TIBIA_MAP_HH_
#define TIBIA_MAP_HH_ 1

#include "common.hh"

// TODO(fusion): I'm not sure whether to put these.

struct Object {
	uint32 ObjectID;
};

constexpr Object NONE = {};

struct ObjectType {
	int TypeID;
};

struct TObjectType {
	char *Name;
	char *Description;
	uint8 Flags[9];
	uint32 Attributes[62];
	int AttributeOffsets[18];
};

#endif //TIBIA_MAP_HH_
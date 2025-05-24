#ifndef TIBIA_MAP_HH_
#define TIBIA_MAP_HH_ 1

#include "common.hh"
#include "objects.hh"

struct Object {
	// REGULAR FUNCTIONS
	// =========================================================================
	Object(void) { this->ObjectID = 0; }
	Object(uint32 ObjectID) { this->ObjectID = ObjectID; }
	bool exists(void);

	ObjectType getObjectType(void);
	void setObjectType(ObjectType Type);

	Object getNextObject(void);
	void setNextObject(Object NextObject);

	Object getContainer(void);
	void setContainer(Object Con);

	uint32 getCreatureID(void);
	//void setCreatureID(uint32 CreatureID); //??

	uint32 getAttribute(INSTANCEATTRIBUTE Attribute);
	void setAttribute(INSTANCEATTRIBUTE Attribute, uint32 Value);

	// DATA
	// =========================================================================
	uint32 ObjectID;
};

constexpr Object NONE;

struct TObject {
	uint32 ObjectID;
	Object NextObject;
	Object Container;
	ObjectType Type;
	uint32 Attributes[4];
};

struct TObjectBlock {
	TObject Object[32768];
};

struct TSector {
	Object MapCon[32][32];
	uint32 TimeStamp;
	uint8 Status;
	uint8 MapFlags;
};

struct TDepotInfo {
	char Town[20];
	int Size;
};

struct TMark {
	char Name[20];
	int x;
	int y;
	int z;
};

struct TCronEntry {
	Object Obj;
	uint32 RoundNr;
	int Previous;
	int Next;
};

void InitMap(void);
void ExitMap(bool Save);

#endif //TIBIA_MAP_HH_

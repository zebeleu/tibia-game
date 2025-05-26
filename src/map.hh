#ifndef TIBIA_MAP_HH_
#define TIBIA_MAP_HH_ 1

#include "common.hh"
#include "objects.hh"
#include "script.hh"

struct Object {
	// REGULAR FUNCTIONS
	// =========================================================================
	constexpr Object(void) : ObjectID(0) {}
	constexpr Object(uint32 ObjectID): ObjectID(ObjectID) {}

	constexpr bool operator==(const Object &other) const {
		return this->ObjectID == other.ObjectID;
	}

	constexpr bool operator!=(const Object &other) const {
		return this->ObjectID != other.ObjectID;
	}

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

// NOTE(fusion): Map management functions. Most for internal use.
void SwapObject(TWriteBinaryFile *File, Object Obj, uint32 FileNumber);
void SwapSector(void);
void UnswapSector(uint32 FileNumber);
void DeleteSwappedSectors(void);
void LoadObjects(TReadScriptFile *Script, TWriteStream *Stream, bool Skip);
void LoadObjects(TReadStream *Stream, Object Con);
void InitSector(int SectorX, int SectorY, int SectorZ);
void LoadSector(const char *FileName, int SectorX, int SectorY, int SectorZ);
void LoadMap(void);
void SaveObjects(Object Obj, TWriteStream *Stream, bool Stop);
void SaveObjects(TReadStream *Stream, TWriteScriptFile *Script);
void SaveSector(char *FileName, int SectorX, int SectorY, int SectorZ);
void SaveMap(void);
void InitMap(void);
void ExitMap(bool Save);

// NOTE(fusion): Object related functions.
Object CreateObject(void);
TObject *AccessObject(Object Obj);
void ChangeObject(Object Obj, ObjectType NewType);
int GetObjectPriority(Object Obj);
void PlaceObject(Object Obj, Object Con, bool Append);
Object AppendObject(Object Con, ObjectType Type);
Object GetFirstContainerObject(Object Con);
Object GetContainerObject(Object Con, int Index);
void GetObjectCoordinates(Object Obj, int *x, int *y, int *z);
uint8 GetMapContainerFlags(Object Obj);
Object GetMapContainer(int x, int y, int z);
Object GetMapContainer(Object Obj);
Object GetFirstObject(int x, int y, int z);

#endif //TIBIA_MAP_HH_

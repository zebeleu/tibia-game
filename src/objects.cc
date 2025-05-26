#include "objects.hh"
#include "containers.hh"

static vector<TObjectType> ObjectTypes(0, 5000, 1000);
static ObjectType SpecialObjects[49];

static FLAG TypeAttributeFlags[62] = {
	BANK, 					// WAYPOINTS
	CONTAINER, 				// CAPACITY
	CHANGEUSE, 				// CHANGETARGET
	KEYDOOR, 				// KEYDOORTARGET
	NAMEDOOR, 				// NAMEDOORTARGET
	LEVELDOOR, 				// LEVELDOORTARGET
	QUESTDOOR, 				// QUESTDOORTARGET
	FOOD, 					// NUTRITION
	INFORMATION, 			// INFORMATIONTYPE
	TEXT, 					// FONTSIZE
	WRITE, 					// MAXLENGTH
	WRITEONCE, 				// MAXLENGTHONCE
	LIQUIDSOURCE, 			// SOURCELIQUIDTYPE
	TELEPORTABSOLUTE, 		// ABSTELEPORTEFFECT
	TELEPORTRELATIVE, 		// RELTELEPORTDISPLACEMENT
	TELEPORTRELATIVE, 		// RELTELEPORTEFFECT
	AVOID, 					// AVOIDDAMAGETYPES
	RESTRICTLEVEL, 			// MINIMUMLEVEL
	RESTRICTPROFESSION, 	// PROFESSIONS
	TAKE, 					// WEIGHT
	ROTATE, 				// ROTATETARGET
	DESTROY, 				// DESTROYTARGET
	CLOTHES, 				// BODYPOSITION
	SKILLBOOST, 			// SKILLNUMBER
	SKILLBOOST, 			// SKILLMODIFICATION
	PROTECTION, 			// PROTECTIONDAMAGETYPES
	PROTECTION, 			// DAMAGEREDUCTION
	LIGHT, 					// BRIGHTNESS
	LIGHT, 					// LIGHTCOLOR
	CORPSE, 				// CORPSETYPE
	EXPIRE, 				// TOTALEXPIRETIME
	EXPIRE, 				// EXPIRETARGET
	WEAROUT, 				// TOTALUSES
	WEAROUT, 				// WEAROUTTARGET
	WEAPON, 				// WEAPONTYPE
	WEAPON, 				// WEAPONATTACKVALUE
	WEAPON, 				// WEAPONDEFENDVALUE
	SHIELD, 				// SHIELDDEFENDVALUE
	BOW, 					// BOWRANGE
	BOW, 					// BOWAMMOTYPE
	THROW, 					// THROWRANGE
	THROW, 					// THROWATTACKVALUE
	THROW, 					// THROWDEFENDVALUE
	THROW, 					// THROWMISSILE
	THROW, 					// THROWSPECIALEFFECT
	THROW, 					// THROWEFFECTSTRENGTH
	THROW, 					// THROWFRAGILITY
	WAND, 					// WANDRANGE
	WAND, 					// WANDMANACONSUMPTION
	WAND, 					// WANDATTACKSTRENGTH
	WAND, 					// WANDATTACKVARIATION
	WAND, 					// WANDDAMAGETYPE
	WAND, 					// WANDMISSILE
	AMMO, 					// AMMOTYPE
	AMMO, 					// AMMOATTACKVALUE
	AMMO, 					// AMMOMISSILE
	AMMO, 					// AMMOSPECIALEFFECT
	AMMO, 					// AMMOEFFECTSTRENGTH
	ARMOR, 					// ARMORVALUE
	HEIGHT, 				// ELEVATION
	DISGUISE, 				// DISGUISETARGET
	BANK, 					// MEANING
};

static FLAG InstanceAttributeFlags[18] = {
	CONTAINER,				// CONTENT
	CHEST,					// CHESTQUESTNUMBER
	CUMULATIVE,				// AMOUNT
	KEY,					// KEYNUMBER
	KEYDOOR,				// KEYHOLENUMBER
	LEVELDOOR,				// DOORLEVEL
	QUESTDOOR,				// DOORQUESTNUMBER
	QUESTDOOR,				// DOORQUESTVALUE
	RUNE,					// CHARGES
	TEXT,					// TEXTSTRING
	TEXT,					// EDITOR
	LIQUIDCONTAINER,		// CONTAINERLIQUIDTYPE
	LIQUIDPOOL,				// POOLLIQUIDTYPE
	TELEPORTABSOLUTE,		// ABSTELEPORTDESTINATION
	MAGICFIELD,				// RESPONSIBLE
	EXPIRE,					// REMAININGEXPIRETIME
	EXPIRESTOP,				// SAVEDEXPIRETIME
	WEAROUT,				// REMAININGUSES
};

static const char InstanceAttributeNames[18][30] = {
	"Content",						// CONTENT
	"ChestQuestNumber",				// CHESTQUESTNUMBER
	"Amount",						// AMOUNT
	"KeyNumber",					// KEYNUMBER
	"KeyholeNumber",				// KEYHOLENUMBER
	"Level",						// DOORLEVEL
	"DoorQuestNumber",				// DOORQUESTNUMBER
	"DoorQuestValue",				// DOORQUESTVALUE
	"Charges",						// CHARGES
	"String",						// TEXTSTRING
	"Editor",						// EDITOR
	"ContainerLiquidType",			// CONTAINERLIQUIDTYPE
	"PoolLiquidType",				// POOLLIQUIDTYPE
	"AbsTeleportDestination",		// ABSTELEPORTDESTINATION
	"Responsible",					// RESPONSIBLE
	"RemainingExpireTime",			// REMAININGEXPIRETIME
	"SavedExpireTime",				// SAVEDEXPIRETIME
	"RemainingUses",				// REMAININGUSES
};

// ObjectType
// =============================================================================
void ObjectType::setTypeID(int TypeID){
	if(TypeID < ObjectTypes.min || ObjectTypes.max < TypeID){
		error("ObjectType::setTypeID: Ungültiger Typ %d.\n", TypeID);
		TypeID = 0;
	}

	this->TypeID = TypeID;
}

bool ObjectType::getFlag(FLAG Flag){
	TObjectType *Type = ObjectTypes.at(this->TypeID);
	int FlagIndex = (int)(Flag / 8);
	uint8 FlagMask = (uint8)(1 << (Flag % 8));
	return (Type->Flags[FlagIndex] & FlagMask) != 0;
}

uint32 ObjectType::getAttribute(TYPEATTRIBUTE Attribute){
	if(!this->getFlag(TypeAttributeFlags[Attribute])){
		error("ObjectType::getAttribute: Typ %d hat kein Flag %d für Attribut %d.\n",
				this->TypeID, TypeAttributeFlags[Attribute], Attribute);
		return 0;
	}

	TObjectType *Type = ObjectTypes.at(this->TypeID);
	return Type->Attributes[Attribute];
}

int ObjectType::getAttributeOffset(INSTANCEATTRIBUTE Attribute){
	if(!this->getFlag(InstanceAttributeFlags[Attribute])){
		if(Attribute != CONTENT || !this->getFlag(CHEST)){
			return -1;
		}
	}

	TObjectType *Type = ObjectTypes.at(this->TypeID);
	return Type->AttributeOffsets[Attribute];
}

const char *ObjectType::getName(int Count){
	if(this->TypeID == 99){
		error("ObjectType::getName: Der Kreaturtyp hat keinen Namen.\n");
		return NULL;
	}

	// TODO(fusion): This is yet another rabbit hole. I'm not sure why we have
	// the object name copied to the stack when `Plural` returns its own static
	// buffer.
#if 0
	char ObjectName[50];
	TObjectType *Type = ObjectTypes.at(this->TypeID);
	if(Type->Name != NULL){
		strcpy(ObjectName, Type->Name);
	}else{
		ObjectName[0] = 0;
	}
	return Plural(ObjectName, Count);
#endif

	return "Unnamed";
}

const char *ObjectType::getDescription(void){
	TObjectType *Type = ObjectTypes.at(this->TypeID);
	return Type->Description;
}

// Object Type Related Functions
// =============================================================================
const char *GetInstanceAttributeName(int Attribute){
	ASSERT(Attribute >= 0 && Attribute <= 17);
	return InstanceAttributeNames[Attribute];
}

int GetInstanceAttributeByName(const char *Name){
	int Result = -1;
	for(int i = 0; i < NARRAY(InstanceAttributeNames); i += 1){
		if(stricmp(InstanceAttributeNames[i], Name) == 0){
			Result = i;
			break;
		}
	}
	return Result;
}

bool ObjectTypeExists(int TypeID){
	return ObjectTypes.min <= TypeID && TypeID <= ObjectTypes.max;
}

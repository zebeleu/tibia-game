#ifndef TIBIA_CREATURE_HH_
#define TIBIA_CREATURE_HH_ 1

#include "main.hh"
#include "connection.hh"
#include "containers.hh"
#include "skill.hh"

struct TCreature;

struct TCombatEntry{
	uint32 ID;
	uint32 Damage;
	int TimeStamp;
};

struct TCombat{
	// REGULAR FUNCTIONS
	// =========================================================================
	// TODO

	// DATA
	// =========================================================================
	TCreature *Master;
	uint32 EarliestAttackTime;
	uint32 EarliestDefendTime;
	uint32 LastDefendTime;
	uint32 LatestAttackTime;
	uint32 AttackMode;
	uint32 ChaseMode;
	uint32 SecureMode;
	uint32 AttackDest;
	bool Following;
	Object Shield;
	Object Close;
	Object Missile;
	Object Throw;
	Object Wand;
	Object Ammo;
	bool Fist;
	uint32 CombatDamage;
	int ActCombatEntry;
	TCombatEntry CombatList[20];
	int LearningPoints;
};

struct TToDoEntry {
    ToDoType Code;
    union{
        struct{
            uint32 Time;
        } Wait;

        struct{
            int x;
            int y;
            int z;
        } Go;

        struct{
            int Direction;
        } Rotate;

        struct{
            uint32 Obj;
            int x;
            int y;
            int z;
            int Count;
        } Move;

        struct{
            uint32 Obj;
            uint32 Partner;
        } Trade;

        struct{
            uint32 Obj1;
            uint32 Obj2;
            int Dummy;
        } Use;

        struct{
            uint32 Obj;
        } Turn;

        struct{
            uint32 Text; // POINTER? Probably a reference from `AddDynamicString`?
            int Mode;
            uint32 Addressee;
            bool CheckSpamming;
        } Talk;

        struct{
            int NewState;
        } ChangeState;
    };
};

struct TOutfit{
	int OutfitID;
	union{
		uint16 ObjectType;
		uint8 Colors[4];
	};
};

struct TCreature: TSkillBase {
	// REGULAR FUNCTIONS
	// =========================================================================
	// TODO

	// VIRTUAL FUNCTIONS
	// =========================================================================
	virtual ~TCreature(void);															// VTABLE[ 0]
	// Duplicate destructor that also calls operator delete.							// VTABLE[ 1]
	virtual void Death(void);															// VTABLE[ 2]
	virtual void MovePossible(void);													// VTABLE[ 3]
	virtual void IsPeaceful(void);														// VTABLE[ 4]
	virtual void GetMaster(void);														// VTABLE[ 5]
	virtual void TalkStimulus(void);													// VTABLE[ 6]
	virtual void DamageStimulus(void);													// VTABLE[ 7]
	virtual void IdleStimulus(void);													// VTABLE[ 8]
	virtual void CreatureMoveStimulus(void);											// VTABLE[ 9]
	virtual void AttackStimulus(void);													// VTABLE[10]

	// DATA
	// =========================================================================
	//void *VTABLE;					// IMPLICIT
	//TSkillBase super_TSkillBase;	// IMPLICIT
	TCombat Combat;
	uint32 ID;
	TCreature *NextHashEntry;
	uint32 NextChainCreature;
	char Name[31];
	char Murderer[31];
	TOutfit OrgOutfit;
	TOutfit Outfit;
	int startx;
	int starty;
	int startz;
	int posx;
	int posy;
	int posz;
	int Sex;
	int Race;
	int Direction;
	int Radius;
	CreatureType Type;
	bool IsDead;
	int LoseInventory;
	bool LoggingOut;
	bool LogoutAllowed;
	uint32 EarliestLogoutRound;
	uint32 EarliestProtectionZoneRound;
	uint32 EarliestYellRound;
	uint32 EarliestTradeChannelRound;
	uint32 EarliestSpellTime;
	uint32 EarliestMultiuseTime;
	uint32 EarliestWalkTime;
	uint32 LifeEndRound;
	TKnownCreature *FirstKnowingConnection;
	int SummonedCreatures;
	uint32 FireDamageOrigin;
	uint32 PoisonDamageOrigin;
	uint32 EnergyDamageOrigin;
	Object CrObject;
	vector<TToDoEntry> ToDoList;
	int ActToDo;
	int NrToDo;
	uint32 NextWakeup;
	bool Stop;
	bool LockToDo;
	uint8 Profession;
	TConnection *Connection;
};

#endif //TIBIA_CREATURE_HH_

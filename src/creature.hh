#ifndef TIBIA_CREATURE_HH_
#define TIBIA_CREATURE_HH_ 1

#include "common.hh"
#include "connection.hh"
#include "containers.hh"
#include "map.hh"

#include "crcombat.hh"
#include "crskill.hh"

struct TOutfit{
	int OutfitID;
	union{
		uint16 ObjectType;
		uint8 Colors[4];
	};
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

struct TCreature: TSkillBase {
	// REGULAR FUNCTIONS
	// =========================================================================
	TCreature(void);
	void Attack(void);
	int Damage(TCreature *Attacker, int Damage, int DamageType);
	void BlockLogout(int Delay, bool BlockProtectionZone);
	void ToDoGo(int DestX, int DestY, int DestZ, bool Dest, int MaxSteps);

	// VIRTUAL FUNCTIONS
	// =========================================================================
	virtual ~TCreature(void);															// VTABLE[ 0]
	// Duplicate destructor that also calls operator delete.							// VTABLE[ 1]
	virtual void Death(void);															// VTABLE[ 2]
	virtual bool MovePossible(int x, int y, int z, bool Execute, bool Jump);			// VTABLE[ 3]
	virtual bool IsPeaceful(void);														// VTABLE[ 4]
	virtual uint32 GetMaster(void);														// VTABLE[ 5]
	virtual void TalkStimulus(uint32 SpeakerID, const char *Text);						// VTABLE[ 6]
	virtual void DamageStimulus(uint32 AttackerID, int Damage, int DamageType);			// VTABLE[ 7]
	virtual void IdleStimulus(void);													// VTABLE[ 8]
	virtual void CreatureMoveStimulus(uint32 CreatureID, int Type);						// VTABLE[ 9]
	virtual void AttackStimulus(uint32 AttackerID);										// VTABLE[10]

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

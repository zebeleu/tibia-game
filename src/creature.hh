#ifndef TIBIA_CREATURE_HH_
#define TIBIA_CREATURE_HH_ 1

#include "main.hh"

struct TCreature;

struct TSkill{
	// REGULAR FUNCTIONS
	// =========================================================================
	TSkill(int SkNr, TCreature *Master);
	int Get(void);
	int GetProgress(void);
	void Check(void);
	void Change(int Amount);
	void SetMDAct(int MDAct);
	void Load(int Act, int Max, int Min, int DAct, int MDAct,
			int Cycle, int MaxCycle, int Count, int MaxCount, int AddLevel,
			int Exp, int FactorPercent, int NextLevel, int Delta);
	void Save(int *Act, int *Max, int *Min, int *DAct, int *MDAct,
			int *Cycle, int *MaxCycle, int *Count, int *MaxCount, int *AddLevel,
			int *Exp, int *FactorPercent, int *NextLevel, int *Delta);

	// VIRTUAL FUNCTIONS
	// =========================================================================
	virtual ~TSkill(void);																// VTABLE[ 0]
	//virtual ~TSkill(void); // Duplicate destructor that also calls operator delete.	// VTABLE[ 1]
	virtual void Set(int Value);														// VTABLE[ 2]
	virtual void Increase(int Amount);													// VTABLE[ 3]
	virtual void Decrease(int Amount);													// VTABLE[ 4]
	virtual int GetExpForLevel(int Level);												// VTABLE[ 5]
	virtual void Advance(int Range);													// VTABLE[ 6]
	virtual void ChangeSkill(int FactorPercent, int Delta);								// VTABLE[ 7]
	virtual int ProbeValue(int Max, bool Increase);										// VTABLE[ 8]
	virtual bool Probe(int Diff, int Prob, bool Increase);								// VTABLE[ 9]
	virtual bool Process(void);															// VTABLE[10]
	virtual bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue);		// VTABLE[11]
	virtual bool DelTimer(void);														// VTABLE[12]
	virtual int TimerValue(void);														// VTABLE[13]
	virtual bool Jump(int Range);														// VTABLE[14]
	virtual void Event(int Range);														// VTABLE[15]
	virtual void Reset(void);															// VTABLE[16]

	// DATA
	// =========================================================================
	//void *VTABLE;		// IMPLICIT
	int DAct;			// Delta Value - Probably from equipment.
	int MDAct;			// Delta Magic Value - Probably from spells.
	uint16 SkNr;
	TCreature *Master;
	int Act;			// Actual Value (?)
	int Max;
	int Min;
	int FactorPercent;
	int LastLevel;
	int NextLevel;
	int Delta;
	int Exp;
	int Cycle;
	int MaxCycle;
	int Count;
	int MaxCount;
	int AddLevel;
};

struct TSkillLevel: TSkill {
	~TSkillLevel(void) override;
	void Increase(int Amount) override;
	void Decrease(int Amount) override;
	int GetExpForLevel(int Level) override;
	bool Jump(int Range) override;
};

struct TSkillProbe: TSkill {
	~TSkillProbe(void) override;
	void Increase(int Amount) override;
	void Decrease(int Amount) override;
	int GetExpForLevel(int Level) override;
	void ChangeSkill(int FactorPercent, int Delta) override;
	int ProbeValue(int Max, bool Increase) override;
	bool Probe(int Diff, int Prob, bool Increase) override;
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	bool Jump(int Range) override;
	void Event(int Range) override;
};

struct TSkillBase{
	// REGULAR FUNCTIONS
	// =========================================================================
	// TODO

	// DATA
	// =========================================================================
	TSkill *Skills[25];
	TSkill *TimerList[25];
	uint16 FirstFreeTimer;
};

// TODO(fusion): Maybe split this file?
//==============================================================================
struct TCombatEntry{
	uint32 ID;
	uint32 Damage;
	int TimeStamp;
};

struct TCombat{
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
	// TODO

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

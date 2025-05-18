#ifndef TIBIA_SKILL_HH_
#define TIBIA_SKILL_HH_ 1

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
	// Duplicate destructor that also calls operator delete.							// VTABLE[ 1]
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
	void Increase(int Amount) override;
	void Decrease(int Amount) override;
	int GetExpForLevel(int Level) override;
	bool Jump(int Range) override;
};

struct TSkillProbe: TSkill {
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

struct TSkillAdd: TSkill {
	void Advance(int Range) override;
};

struct TSkillHitpoints: TSkillAdd {
	void Set(int Value) override;
};

struct TSkillMana: TSkillAdd {
	void Set(int Value) override;
};

struct TSkillGoStrength: TSkillAdd {
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	void Event(int Range) override;
};

struct TSkillCarryStrength: TSkillAdd {
	void Set(int Value) override;
};

struct TSkillSoulpoints: TSkillAdd {
	void Set(int Value) override;
	int TimerValue(void) override;
	void Event(int Range) override;
};

struct TSkillFed: TSkill {
	void Event(int Range) override;
};

struct TSkillLight: TSkill {
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	void Event(int Range) override;
};

struct TSkillIllusion: TSkill {
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	void Event(int Range) override;
};

struct TSkillPoison: TSkill {
	bool Process(void) override;
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	void Event(int Range) override;
	void Reset(void) override;
};

struct TSkillBurning: TSkill {
	void Event(int Range) override;
};

struct TSkillEnergy: TSkill {
	void Event(int Range) override;
};

struct TSkillBase{
	// REGULAR FUNCTIONS
	// =========================================================================
	TSkillBase(void);
	~TSkillBase(void);
	bool NewSkill(uint16 SkillNo, TCreature *Creature);
	bool SetSkills(int Race);
	void ProcessSkills(void);
	bool SetTimer(uint16 SkNr, int Cycle, int Count, int MaxCount, int AdditionalValue);
	void DelTimer(uint16 SkNr);

	// DATA
	// =========================================================================
	TSkill *Skills[25];
	TSkill *TimerList[25];
	uint16 FirstFreeTimer;
};

#endif //TIBIA_SKILL_HH_

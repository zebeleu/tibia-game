#ifndef TIBIA_CRCOMBAT_HH_
#define TIBIA_CRCOMBAT_HH_ 1

struct TCreature;

struct TCombatEntry{
	uint32 ID;
	uint32 Damage;
	int TimeStamp;
};

struct TCombat{
	// REGULAR FUNCTIONS
	// =========================================================================
	TCombat(void);
	void CheckCombatValues(void);

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

#endif //TIBIA_CRCOMBAT_HH_

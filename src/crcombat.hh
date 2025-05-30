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
	void GetWeapon(void);
	void GetAmmo(void);
	void CheckCombatValues(void);
	int GetDistance(void);
	void GetAttackValue(int *Attack, uint16 *SkillNr);
	int GetDefendDamage(void);
	void SetAttackDest(uint32 TargetID, bool Follow);
	void CanToDoAttack(void);
	void Attack(void);
	void StopAttack(int Delay);
	void DelayAttack(int Milliseconds);
	void CloseAttack(TCreature *Target);
	void DistanceAttack(TCreature *Target);
	void WandAttack(TCreature *Target);

	// DATA
	// =========================================================================
	TCreature *Master;
	uint32 EarliestAttackTime;
	uint32 EarliestDefendTime;
	uint32 LastDefendTime;
	uint32 LatestAttackTime;
	uint8 AttackMode;
	uint8 ChaseMode;
	uint8 SecureMode;
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

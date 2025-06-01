#ifndef TIBIA_MAGIC_HH_
#define TIBIA_MAGIC_HH_ 1

#include "common.hh"
#include "creature.hh"

struct TImpact{
	// VIRTUAL FUNCTIONS
	// =========================================================================
	virtual void handleField(int x, int y, int z);										// VTABLE[0]
	virtual void handleCreature(TCreature *Victim);										// VTABLE[1]
	virtual bool isAggressive(void);													// VTABLE[2]

	// DATA
	// =========================================================================
	//void *VTABLE;	// IMPLICIT
};

struct TDamageImpact: TImpact {
	TDamageImpact(TCreature *Actor, int DamageType, int Power, bool AllowDefense);
	void handleCreature(TCreature *Victim) override;

	TCreature *Actor;
	int DamageType;
	int Power;
	bool AllowDefense;
};

struct TFieldImpact: TImpact {
	TFieldImpact(TCreature *Actor, int FieldType);
	void handleField(int x, int y, int z) override;

	TCreature *Actor;
	int FieldType;
};

struct THealingImpact: TImpact {
	THealingImpact(TCreature *Actor, int Power);
	void handleCreature(TCreature *Victim) override;
	bool isAggressive(void) override;

	TCreature *Actor;
	int Power;
};

struct TSpeedImpact: TImpact {
	TSpeedImpact(TCreature *Actor, int Percent, int Duration);
	void handleCreature(TCreature *Victim) override;

	TCreature *Actor;
	int Percent;
	int Duration;
};

struct TDrunkenImpact: TImpact {
	TDrunkenImpact(TCreature *Actor, int Power, int Duration);
	void handleCreature(TCreature *Victim) override;

	TCreature *Actor;
	int Power;
	int Duration;
};

struct TStrengthImpact: TImpact {
	TStrengthImpact(TCreature *Actor, int Skills, int Percent, int Duration);
	void handleCreature(TCreature *Victim) override;

	TCreature *Actor;
	int Skills;
	int Percent;
	int Duration;
};

struct TOutfitImpact: TImpact {
	TOutfitImpact(TCreature *Actor, TOutfit Outfit, int Duration);
	void handleCreature(TCreature *Victim) override;

	TCreature *Actor;
	TOutfit Outfit;
	int Duration;
};

struct TSummonImpact: TImpact {
	TSummonImpact(TCreature *Actor, int Race, int Maximum);
	void handleField(int x, int y, int z) override;

	TCreature *Actor;
	int Race;
	int Maximum;
};

struct TCircle {
	int x[32];
	int y[32];
	int Count;
};

struct TSpellList {
	uint8 Syllable[10];
	uint8 RuneGr;
	uint8 RuneNr;
	const char *Comment;
	uint16 Level;
	uint16 RuneLevel;
	uint16 Flags;
	int Mana;
	int SoulPoints;
	int Amount;
};

void CheckMana(TCreature *Creature, int ManaPoints, int SoulPoints, int Delay);

#endif //TIBIA_MAGIC_HH_

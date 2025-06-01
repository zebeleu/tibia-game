#ifndef TIBIA_MAGIC_HH_
#define TIBIA_MAGIC_HH_ 1

#include "common.hh"
#include "creature.hh"

struct TImpact{
	// VIRTUAL FUNCTIONS
	// =========================================================================
	virtual void handleField(int a, int b, int c);										// VTABLE[0]
	virtual void handleCreature(TCreature *Victim);										// VTABLE[1]
	virtual bool isAggressive(void);													// VTABLE[2]

	// DATA
	// =========================================================================
	//void *VTABLE;	// IMPLICIT
};

#if 0
struct TSummonImpact {
	TImpact super_TImpact; // INHERITANCE?
	TCreature *Actor;
	int Race;
	int Maximum;
};

struct TSpeedImpact {
	TImpact super_TImpact; // INHERITANCE?
	TCreature *Actor;
	int Percent;
	int Duration;
};

struct THealingImpact {
	TImpact super_TImpact; // INHERITANCE?
	TCreature *Actor;
	int Power;
};

struct TOutfitImpact {
	TImpact super_TImpact; // INHERITANCE?
	TCreature *Actor;
	TOutfit Outfit;
	int Duration;
};

struct TFieldImpact {
	TImpact super_TImpact; // INHERITANCE?
	TCreature *Actor;
	int FieldType;
};

struct TDrunkenImpact {
	TImpact super_TImpact; // INHERITANCE?
	TCreature *Actor;
	int Power;
	int Duration;
};

struct TStrengthImpact {
	TImpact super_TImpact; // INHERITANCE?
	TCreature *Actor;
	int Skills;
	int Percent;
	int Duration;
};
#endif

struct TDamageImpact: TImpact{
	// REGULAR FUNCTIONS
	// =========================================================================
	TDamageImpact(TCreature *Actor, int DamageType, int Power, bool AllowDefense);

	// VIRTUAL FUNCTIONS
	// =========================================================================
	void handleCreature(TCreature *Victim) override;

	// DATA
	// =========================================================================
	// TImpact super_TImpact;	// IMPLICIT
	TCreature *Actor;
	int DamageType;
	int Power;
	bool AllowDefense;
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

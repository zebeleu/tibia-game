#include "magic.hh"
#include "creature.hh"

// TImpact
// =============================================================================
void TImpact::handleField(int a, int b, int c){
	// no-op
}

void TImpact::handleCreature(TCreature *Victim){
	// no-op
}

bool TImpact::isAggressive(void){
	return true;
}

// TDamageImpact
// =============================================================================
void TDamageImpact::TDamageImpact(TCreature *Actor, int DamageType, int Power, bool AllowDefense){
	if(Actor == NULL){
		error("TDamageImpact::TDamageImpact: Actor ist NULL.\n");
	}

	this->Actor = Actor;
	this->DamageType = DamageType;
	this->Power = Power;
	this->AllowDefense = AllowDefense;
}

// Magic Related Functions
// =============================================================================
void CheckMana(TCreature *Creature, int ManaPoints, int SoulPoints, int Delay){
	if(Creature == NULL){
		error("CheckMana: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(Creature->Type != PLAYER || ManaPoints < 0)
		return;

	TSkill *Mana = Creature->Skills[SKILL_MANA];
	if(Mana == NULL){
		error("CheckMana: Kein Skill MANA!\n");
		throw ERROR;
	}

	TSkill *Soul = Creature->Skills[SKILL_SOUL];
	if(Soul == NULL){
		error("CheckMana: Kein Skill SOULPOINTS!\n");
		throw ERROR;
	}

	if(!CheckRight(Creature->ID, UNLIMITED_MANA)){
		if(Mana->Get() < ManaPoints)
			throw NOTENOUGHMANA;

		if(Soul->Get() < SoulPoints)
			throw NOTENOUGHSOULPOINTS;

		Mana->Change(-ManaPoints);
		Soul->Change(-SoulPoints);
	}

	if(ManaPoints > 0){
		Creature->Skills[SKILL_MAGIC_LEVEL]->Increase(ManaPoints);
	}

	// NOTE(fusion): Maintain largest exhaust?
	uint32 EarliestSpellTime = ServerMilliseconds + Delay;
	if(Creature->EarliestSpellTime < EarliestSpellTime){
		Creature->EarliestSpellTime = EarliestSpellTime;
	}
}

#include "creature.hh"

#include "enums.hh"

// Creature Functions
//==============================================================================
// TODO(fusion): This was the first function I attempted to cleanup but soon
// realized we should start with the building blocks of the codebase, namely
// TSkill, TSkillBase, etc...
//	We should probably come back to this once we start doing creature functions
// again.
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
	int EarliestSpellTime = Delay + ServerMilliseconds;
	if(Creature->EarliestSpellTime < EarliestSpellTime){
		Creature->EarliestSpellTime = EarliestSpellTime;
	}
}

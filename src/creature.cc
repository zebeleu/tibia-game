#include "creature.hh"

#include "enums.hh"

TCreature::TCreature(void) :
		TSkillBase(),
		Combat(),
		ToDoList(0, 20, 10)
{
	this->Master = this;
	this->ID = 0;
	this->NextHashEntry = NULL;
	this->NextChainCreature =  0;
	this->Murderer[0] = 0;
	this->startx = 0;
	this->starty = 0;
	this->startz = 0;
	this->posx = 0;
	this->posy = 0;
	this->posz = 0;
	this->Direction = 0;
	this->Radius = INT_MAX;
	this->IsDead = false;
	this->LoseInventory = 2;
	this->LoggingOut = false;
	this->LogoutAllowed = false;
	this->EarliestLogoutRound = 0;
	this->EarliestProtectionZoneRound = 0;
	this->EarliestYellRound = 0;
	this->EarliestTradeChannelRound = 0;
	this->EarliestSpellTime = 0;
	this->EarliestMultiuseTime = 0;
	this->EarliestWalkTime = 0;
	this->LifeEndRound = 0;
	this->FirstKnowingConnection = NULL;
	this->SummonedCreatures = 0;
	this->FireDamageOrigin = 0;
	this->PoisonDamageOrigin = 0;
	this->EnergyDamageOrigin = 0;
	this->CrObject = NONE;
	this->ActToDo = 0;
	this->NrToDo = 0;
	this->NextWakeup = 0;
	this->Stop = false;
	this->LockToDo = false;
	this->Connection = NULL;

	for(int i = 0; i < NARRAY(this->Skills); i += 1){
		this->NewSkill((uint16)i, this);
	}
}

// TODO(fusion): Probably better to figure out how TCombat and Object work.

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
	uint32 EarliestSpellTime = Delay + ServerMilliseconds;
	if(Creature->EarliestSpellTime < EarliestSpellTime){
		Creature->EarliestSpellTime = EarliestSpellTime;
	}
}

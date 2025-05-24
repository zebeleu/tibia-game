#include "creature.hh"

TCombat::TCombat(void){
	this->Master = NULL;
	this->EarliestAttackTime = 0;
	this->EarliestDefendTime = 0;
	this->LastDefendTime = 0;
	this->LatestAttackTime = 0;
	this->AttackMode = 2;
	this->ChaseMode = 0;
	this->SecureMode = 1;
	this->AttackDest = 0;
	this->Following;
	this->Shield = NONE;
	this->Close = NONE;
	this->Missile = NONE;
	this->Throw = NONE;
	this->Wand = NONE;
	this->Ammo = NONE;
	this->Fist = false;
	this->CombatDamage = 0;
	this->ActCombatEntry = 0;
	for(int i = 0; i < NARRAY(this->CombatList); i += 1){
		this->CombatList[i].ID = 0;
		this->CombatList[i].Damage = 0;
		this->CombatList[i].TimeStamp = 0;
	}
	this->LearningPoints = 0;
}

// TODO(fusion): Probably better to figure out how Object work.

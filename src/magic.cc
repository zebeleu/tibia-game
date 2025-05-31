#include "magic.hh"
#include "config.hh"
#include "creature.hh"

#include "stubs.hh"

static const char SpellSyllable[51][6] = {
	"",
	"al",
	"ad",
	"ex",
	"ut",
	"om",
	"para",
	"ana",
	"evo",
	"ori",
	"mort",
	"lux",
	"liber",
	"vita",
	"flam",
	"pox",
	"hur",
	"moe",
	"ani",
	"ina",
	"eta",
	"amo",
	"hora",
	"gran",
	"cogni",
	"res",
	"mas",
	"vis",
	"som",
	"aqua",
	"frigo",
	"tera",
	"ura",
	"sio",
	"grav",
	"ito",
	"pan",
	"vid",
	"isa",
	"iva",
	"con",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
};

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
TDamageImpact::TDamageImpact(TCreature *Actor, int DamageType, int Power, bool AllowDefense){
	if(Actor == NULL){
		error("TDamageImpact::TDamageImpact: Actor ist NULL.\n");
	}

	this->Actor = Actor;
	this->DamageType = DamageType;
	this->Power = Power;
	this->AllowDefense = AllowDefense;
}

void TDamageImpact::handleCreature(TCreature *Victim){
	if(Victim == NULL){
		error("TDamageImpact::handleCreature: Opfer existiert nicht.\n");
		return;
	}

	TCreature *Actor = this->Actor;
	if(Actor != NULL && Actor != Victim){
		if(WorldType != NON_PVP || !Actor->IsPeaceful() || !Victim->IsPeaceful()){
			int DamageType = this->DamageType;
			int Damage = this->Power;
			if(DamageType == DAMAGE_PHYSICAL && this->AllowDefense){
				// TODO(fusion): Shouldn't we clamp `Damage` to zero?
				Damage -= Victim->Combat.GetDefendDamage();
			}
			Victim->Damage(Actor, Damage, DamageType);
		}
	}
}

// Magic Related Functions
// =============================================================================
void CheckMana(TCreature *Creature, int ManaPoints, int SoulPoints, int Delay){
	if(Creature == NULL){
		error("CheckMana: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(Creature->Type != PLAYER || ManaPoints < 0){
		return;
	}

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
		if(Mana->Get() < ManaPoints){
			throw NOTENOUGHMANA;
		}

		if(Soul->Get() < SoulPoints){
			throw NOTENOUGHSOULPOINTS;
		}

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

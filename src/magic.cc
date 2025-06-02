#include "magic.hh"
#include "config.hh"
#include "creature.hh"
#include "info.hh"
#include "monster.hh"

#include "stubs.hh"

#include <fstream>

static TSpellList SpellList[256];
static TCircle Circle[10];

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

static bool IsAggressionValid(TCreature *Actor, TCreature *Victim){
	ASSERT(Actor != NULL && Victim != NULL);

	if(Actor == Victim){
		return false;
	}

	if(WorldType == NON_PVP && Actor->IsPeaceful() && Victim->IsPeaceful()){
		return false;
	}

	if(GetRaceNoParalyze(Victim->Race)){
		return false;
	}

	if(Victim->Type == PLAYER && CheckRight(Victim->ID, INVULNERABLE)){
		return false;
	}

	return true;
}

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

// TFieldImpact
// =============================================================================
TFieldImpact::TFieldImpact(TCreature *Actor, int FieldType){
	if(Actor == NULL){
		error("TFieldImpact::TFieldImpact: Actor ist NULL.\n");
	}

	this->Actor = Actor;
	this->FieldType = FieldType;
}

void TFieldImpact::handleField(int x, int y, int z){
	TCreature *Actor = this->Actor;
	if(Actor != NULL){
		bool Peaceful = (WorldType == NON_PVP && Actor->IsPeaceful());
		CreateField(x, y, z, this->FieldType, Actor->ID, Peaceful);
	}
}

// THealingImpact
// =============================================================================
THealingImpact::THealingImpact(TCreature *Actor, int Power){
	if(Actor == NULL){
		error("THealingImpact::THealingImpact: Actor ist NULL.\n");
	}

	if(Power < 0){
		error("THealingImpact::THealingImpact: Power ist negativ (Actor: %s).\n",
				(Actor != NULL ? Actor->Name : "(unknown)"));
	}

	this->Actor = Actor;
	this->Power = Power;
}

void THealingImpact::handleCreature(TCreature *Victim){
	if(Victim == NULL){
		error("THealingImpact::handleCreature: Opfer existiert nicht.\n");
		return;
	}

	if(this->Actor != NULL && this->Power >= 0){
		int HitPoints = Victim->Skills[SKILL_HITPOINTS]->Get();
		if(HitPoints > 0){
			Victim->Skills[SKILL_HITPOINTS]->Change(this->Power);

			// NOTE(fusion): Remove paralyze.
			if(Victim->Skills[SKILL_GO_STRENGTH]->MDAct < 0){
				Victim->SetTimer(SKILL_GO_STRENGTH, 0, 0, 0, -1);
			}
		}
	}
}

bool THealingImpact::isAggressive(void){
	return false;
}

// TSpeedImpact
// =============================================================================
TSpeedImpact::TSpeedImpact(TCreature *Actor, int Percent, int Duration){
	if(Actor == NULL){
		error("TSpeedImpact::TSpeedImpact: Actor ist NULL.\n");
	}

	this->Actor = Actor;
	this->Percent = Percent;
	this->Duration = Duration;
}

void TSpeedImpact::handleCreature(TCreature *Victim){
	if(Victim == NULL){
		error("TSpeedImpact::handleCreature: Opfer existiert nicht.\n");
		return;
	}

	TCreature *Actor = this->Actor;
	if(Actor == NULL){
		return;
	}

	int Percent = this->Percent;
	if(Percent < 0 && !IsAggressionValid(Actor, Victim)){
		return;
	}

	TSkill *GoStrength = Victim->Skills[SKILL_GO_STRENGTH];
	if(Percent < -100){
		// TODO(fusion): Not sure what's this about.
		GoStrength->SetMDAct(-GoStrength->Act - 20);
	}else{
		GoStrength->SetMDAct((GoStrength->Act * Percent) / 100);
	}

	Victim->SetTimer(SKILL_GO_STRENGTH, this->Duration, 1, 1, -1);
}

// TDrunkenImpact
// =============================================================================
TDrunkenImpact::TDrunkenImpact(TCreature *Actor, int Power, int Duration){
	if(Actor == NULL){
		error("TDrunkenImpact::TDrunkenImpact: Actor ist NULL.\n");
	}

	if(Power > 6){
		error("TDrunkenImpact::TDrunkenImpact: Power ist zu groß (%d).\n", Power);
		Power = 6;
	}

	this->Actor = Actor;
	this->Power = Power;
	this->Duration = Duration;
}

void TDrunkenImpact::handleCreature(TCreature *Victim){
	if(Victim == NULL){
		error("TDrunkenImpact::handleCreature: Opfer existiert nicht.\n");
		return;
	}

	TCreature *Actor = this->Actor;
	if(Actor == NULL || !IsAggressionValid(Actor, Victim)){
		return;
	}

	int Power = this->Power;
	int Duration = this->Duration;
	TSkill *Drunk = Victim->Skills[SKILL_DRUNK];
	if(Drunk->TimerValue() <= Power){
		Victim->SetTimer(SKILL_DRUNK, Power, Duration, Duration, -1);
	}
}

// TStrengthImpact
// =============================================================================
TStrengthImpact::TStrengthImpact(TCreature *Actor, int Skills, int Percent, int Duration){
	if(Actor == NULL){
		error("TStrengthImpact::TStrengthImpact: Actor ist NULL.\n");
	}

	this->Actor = Actor;
	this->Skills = Skills;
	this->Percent = Percent;
	this->Duration = Duration;
}

void TStrengthImpact::handleCreature(TCreature *Victim){
	if(Victim  == NULL){
		error("TStrengthImpact::handleCreature: Opfer existiert nicht.\n");
		return;
	}

	TCreature *Actor = this->Actor;
	if(Actor == NULL){
		return;
	}

	int Percent = this->Percent;
	if(Percent < 0 && !IsAggressionValid(Actor, Victim)){
		return;
	}

	int Skills = this->Skills;
	int Duration = this->Duration;
	for(int SkillNr = 6; SkillNr <= 11; SkillNr += 1){
		if((Skills & 1) == 0
				&& (SkillNr == SKILL_SWORD
					|| SkillNr == SKILL_CLUB
					|| SkillNr == SKILL_AXE
					|| SkillNr == SKILL_SWORD)){
			continue;
		}

		if((Skills & 2) == 0 && SkillNr == SKILL_DISTANCE){
			continue;
		}

		if((Skills & 4) == 0 && SkillNr == SKILL_SHIELDING){
			continue;
		}

		TSkill *Skill = Victim->Skills[SkillNr];
		if(Percent < -100){
			Skill->SetMDAct(-Skill->Act - 20);
		}else{
			Skill->SetMDAct((Skill->Act * Percent) / 100);
		}
		Victim->SetTimer(SkillNr, Duration, 1, 1, -1);
	}
}

// TOutfitImpact
// =============================================================================
TOutfitImpact::TOutfitImpact(TCreature *Actor, TOutfit Outfit, int Duration){
	if(Actor == NULL){
		error("TOutfitImpact::TOutfitImpact: Actor ist NULL.\n");
	}

	this->Actor = Actor;
	this->Outfit = Outfit;
	this->Duration = Duration;
}

void TOutfitImpact::handleCreature(TCreature *Victim){
	if(Victim == NULL){
		error("TOutfitImpact::handleCreature: Opfer existiert nicht.\n");
		return;
	}

	Victim->Outfit = this->Outfit;
	Victim->SetTimer(SKILL_ILLUSION, 1, this->Duration, this->Duration, -1);
}

// TSummonImpact
// =============================================================================
TSummonImpact::TSummonImpact(TCreature *Actor, int Race, int Maximum){
	if(Actor == NULL){
		error("TSummonImpact::TSummonImpact: Actor ist NULL.\n");
	}

	// TODO(fusion): I think there might be a `IsRaceValid` function that was inlined.
	if(Race < 0 || Race >= NARRAY(RaceData)){
		error("TSummonImpact::TSummonImpact: Ungültige Rassennummer %d.\n", Race);
	}

	this->Actor = Actor;
	this->Race = Race;
	this->Maximum = Maximum;
}

void TSummonImpact::handleField(int x, int y, int z){
	TCreature *Actor = this->Actor;
	int Race = this->Race;
	int Maximum = this->Maximum;
	if(Actor != NULL
			&& Race >= 0 && Race < NARRAY(RaceData)
			&& Actor->SummonedCreatures < Maximum){
		int x, y, z;
		if(SearchSummonField(&x, &y, &z, 2)){
			CreateMonster(Race, x, y, z, 0, Actor->ID, true);
		}
	}
}

// Spell Primitives
// =============================================================================
void ActorShapeSpell(TCreature *Actor, TImpact *Impact, int Effect){
	if(Actor == NULL){
		error("ActorShapeSpell: Sprecher existiert nicht.\n");
		return;
	}

	if(Impact->isAggressive() && IsProtectionZone(Actor->posx, Actor->posy, Actor->posz)){
		return;
	}

	Impact->handleCreature(Actor);
	if(Effect != EFFECT_NONE){
		GraphicalEffect(Actor->posx, Actor->posy, Actor->posz, Effect);
	}
}

void VictimShapeSpell(TCreature *Actor, TCreature *Victim,
		int Range, int Animation, TImpact *Impact, int Effect){
	if(Actor == NULL){
		error("VictimShapeSpell: Sprecher existiert nicht.\n");
		return;
	}

	if(Victim == NULL || Actor->posz != Victim->posz){
		return;
	}

	int Distance = std::max<int>(
			std::abs(Actor->posx - Victim->posx),
			std::abs(Actor->posy - Victim->posy));
	if(Distance > Range){
		return;
	}

	// TODO(fusion): We don't check whether `Actor` is inside a protection zone.
	// It might be checked elsewhere.
	if(Impact->isAggressive() && IsProtectionZone(Victim->posx, Victim->posy, Victim->posz)){
		return;
	}

	if(!ThrowPossible(Actor->posx, Actor->posy, Actor->posz,
			Victim->posx, Victim->posy, Victim->posz, 0)){
		return;
	}

	if(Animation != ANIMATION_NONE && Distance > 0){
		Missile(Actor->CrObject, Victim->CrObject, Animation);
	}

	Impact->handleCreature(Victim);

	if(Effect != EFFECT_NONE){
		GraphicalEffect(Victim->posx, Victim->posy, Victim->posz, Effect);
	}
}

// TODO(fusion): This function wasn't in the debug symbols but it was repeated
// in both `OriginShapeSpell` and `CircleShapeSpell` so I'm almost sure it was
// inlined there.
static void ExecuteCircleSpell(int DestX, int DestY, int DestZ,
						int Radius, TImpact *Impact, int Effect){
	// TODO(fusion): This clamping wasn't present in the original function but
	// it is probably a good idea.
	if(Radius >= NARRAY(Circle)){
		Radius = NARRAY(Circle) - 1;
	}

	bool Aggressive = Impact->isAggressive();
	for(int R = 0; R <= Radius; R += 1){
		int CirclePoints = Circle[R].Count;
		for(int Point = 0; Point < CirclePoints; Point += 1){
			int FieldX = DestX + Circle[R].x[Point];
			int FieldY = DestY + Circle[R].y[Point];
			int FieldZ = DestZ;

			if(Aggressive && IsProtectionZone(FieldX, FieldY, FieldZ)){
				continue;
			}

			if(!ThrowPossible(DestX, DestY, DestZ, FieldX, FieldY, FieldZ, 0)){
				continue;
			}

			Impact->handleField(FieldX, FieldY, FieldZ);

			Object Obj = GetFirstObject(FieldX, FieldY, FieldZ);
			while(Obj != NONE){
				if(Obj.getObjectType().isCreatureContainer()){
					TCreature *Victim = GetCreature(Obj);
					if(Victim != NULL){
						Impact->handleCreature(Victim);
					}
				}
				Obj = Obj.getNextObject();
			}

			if(Effect != EFFECT_NONE){
				GraphicalEffect(FieldX, FieldY, FieldZ, Effect);
			}
		}
	}
}

void OriginShapeSpell(TCreature *Actor, int Radius, TImpact *Impact, int Effect){
	if(Actor == NULL){
		error("OriginShapeSpell: Übergebene Kreatur existiert nicht.\n");
		return;
	}

	ExecuteCircleSpell(Actor->posx, Actor->posy, Actor->posz, Radius, Impact, Effect);
}

void CircleShapeSpell(TCreature *Actor, int DestX, int DestY, int DestZ,
		int Range, int Animation, int Radius, TImpact *Impact, int Effect){
	if(Actor == NULL){
		error("CircleShapeSpell: Sprecher existiert nicht.\n");
		return;
	}

	int Distance = std::max<int>(
			std::abs(Actor->posx - DestX),
			std::abs(Actor->posy - DestY));
	if(Distance > Range || Actor->posz != DestZ){
		return;
	}

	if(!ThrowPossible(Actor->posx, Actor->posy, Actor->posz, DestX, DestY, DestZ, 0)){
		return;
	}

	if(Animation != ANIMATION_NONE && Distance > 0){
		Missile(Actor->CrObject, GetMapContainer(DestX, DestY, DestZ), Animation);
	}

	ExecuteCircleSpell(DestX, DestY, DestZ, Radius, Impact, Effect);
}

void DestinationShapeSpell(TCreature *Actor, TCreature *Victim,
		int Range, int Animation, int Radius, TImpact *Impact, int Effect){
	if(Actor == NULL){
		error("DestinationShapeSpell: Sprecher existiert nicht.\n");
		return;
	}

	if(Victim != NULL){
		CircleShapeSpell(Actor, Victim->posx, Victim->posy, Victim->posz,
				Range, Animation, Radius, Impact, Effect);
	}
}

void AngleShapeSpell(TCreature *Actor, int Angle, int Range, TImpact *Impact, int Effect){
	if(Actor == NULL){
		error("AngleShapeSpell: Übergebene Kreatur existiert nicht.\n");
		return;
	}

	int ActorX = Actor->posx;
	int ActorY = Actor->posy;
	int ActorZ = Actor->posz;
	int Direction = Actor->Direction;
	bool Aggressive = Impact->isAggressive();
	for(int Forward = 1; Forward <= Range; Forward += 1){
		int Left = -(Forward * Angle) / 90;
		int Right = +(Forward * Angle) / 90;
		for(int Across = Left; Across <= Right; Across += 1){
			int FieldX = ActorX;
			int FieldY = ActorY;
			int FieldZ = ActorZ;
			if(Direction == DIRECTION_NORTH){
				FieldX += Across;
				FieldY -= Forward;
			}else if(Direction == DIRECTION_EAST){
				FieldX += Forward;
				FieldY += Across;
			}else if(Direction == DIRECTION_SOUTH){
				FieldX -= Across;
				FieldY += Forward;
			}else if(Direction == DIRECTION_WEST){
				FieldX -= Forward;
				FieldY -= Across;
			}else{
				error("AngleShapeSpell: Ungültige Blickrichtung %d.\n", Direction);
				return;
			}

			if(Aggressive && IsProtectionZone(FieldX, FieldY, FieldZ)){
				continue;
			}

			if(!ThrowPossible(ActorX, ActorZ, ActorZ, FieldX, FieldY, FieldZ, 0)){
				continue;
			}

			Impact->handleField(FieldX, FieldY, FieldZ);

			Object Obj = GetFirstObject(FieldX, FieldY, FieldZ);
			while(Obj != NONE){
				if(Obj.getObjectType().isCreatureContainer()){
					TCreature *Victim = GetCreature(Obj);
					if(Victim != NULL){
						Impact->handleCreature(Victim);
					}
				}
				Obj = Obj.getNextObject();
			}

			if(Effect != EFFECT_NONE){
				GraphicalEffect(FieldX, FieldY, FieldZ, Effect);
			}
		}
	}
}

// Spell Casting
// =============================================================================
int GetDirection(int dx, int dy){
	// TODO(fusion): This function originally returned directions different from
	// the ones used by creatures. I've converted it to use the same values which
	// are also defined in `enums.hh` for simplicity.
	int Result;
	if(dx == 0){
		if(dy < 0){
			Result = DIRECTION_NORTH;
		}else if(dy > 0){
			Result = DIRECTION_SOUTH;
		}else{
			Result = DIRECTION_INVALID;
		}
	}else{
		// NOTE(fusion): This function uses the approximate tangent value, avoiding
		// floating point calculations, for whatever reason. The tangent is unique
		// and odd in the interval (-PI/2, +PI/2). We also need to recall that the
		// Y-axis is inverted in Tibia, so we need to negate `dy`.
		constexpr int Tangent_67_5 = 618;	// => 618 / 256 ~ 2.41 ~ tan(67.5 deg)
		constexpr int Tangent_22_5 = 106;	// => 106 / 256 ~ 0.41 ~ tan(22.5 deg)
		int Tangent = (-dy * 256) / dx;		// => (dy * 256) / dx ~ (dy / dx) * 256
		if(Tangent >= Tangent_67_5){
			Result = DIRECTION_NORTH;
		}else if(Tangent >= Tangent_22_5){
			Result = (dx < 0) ? DIRECTION_NORTHWEST : DIRECTION_NORTHEAST;
		}else if(Tangent >= -Tangent_22_5){
			Result = (dx < 0) ? DIRECTION_WEST : DIRECTION_EAST;
		}else if(Tangent >= -Tangent_67_5){
			Result = (dx < 0) ? DIRECTION_SOUTHWEST : DIRECTION_SOUTHEAST;
		}else{
			Result = DIRECTION_SOUTH;
		}
	}
	return Result;
}

void CheckSpellbook(TCreature *Actor, int SpellNr){
	if(Actor == NULL){
		error("CheckSpellbook: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(Actor->Type == PLAYER && !CheckRight(Actor->ID, ALL_SPELLS)
			&& !((TPlayer*)Actor)->SpellKnown(SpellNr)){
		throw SPELLUNKNOWN;
	}
}

void CheckAccount(TCreature *Actor, int SpellNr){
	if(Actor == NULL){
		error("CheckAccount: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	// TODO(fusion): Why is this the only function that checks the spell number?
	if(SpellNr < 1 || SpellNr >= NARRAY(SpellList)){
		error("CheckAccount: Ungültige Spruchnummer %d.\n", SpellNr);
		throw ERROR;
	}

	if(Actor->Type == PLAYER && (SpellList[SpellNr].Flags & 2) != 0
			&& !CheckRight(Actor->ID, PREMIUM_ACCOUNT)){
		throw NOPREMIUMACCOUNT;
	}
}

void CheckLevel(TCreature *Actor, int SpellNr){
	if(Actor == NULL){
		error("CheckLevel: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(Actor->Type == PLAYER && !CheckRight(Actor->ID, ALL_SPELLS)){
		TSkill *Level = Actor->Skills[SKILL_LEVEL];
		if(Level == NULL){
			error("CheckLevel: Kein Skill LEVEL.\n");
			throw ERROR;
		}

		if(Level->Get() < SpellList[SpellNr].Level){
			throw LOWLEVEL;
		}
	}
}

void CheckRuneLevel(TCreature *Actor, int SpellNr){
	if(Actor == NULL){
		error("CheckRuneLevel: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(Actor->Type == PLAYER && !CheckRight(Actor->ID, ALL_SPELLS)){
		TSkill *MagicLevel = Actor->Skills[SKILL_MAGIC_LEVEL];
		if(MagicLevel == NULL){
			error("CheckLevel: Kein Skill MAGLEVEL.\n");
			throw ERROR;
		}

		if(MagicLevel->Get() < SpellList[SpellNr].RuneLevel){
			throw LOWMAGICLEVEL;
		}
	}
}

void CheckMagicItem(TCreature *Actor, ObjectType Type){
	if(Actor == NULL){
		error("CheckMagicObject: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(Actor->Type == PLAYER && !CheckRight(Actor->ID, ALL_SPELLS)
			&& CountInventoryObjects(Actor->ID, Type, 0) == 0){
		throw MAGICITEM;
	}
}

void CheckRing(TCreature *Actor, int SpellNr){
	if(Actor == NULL){
		error("CheckRing: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	// TODO(fusion): Not sure why this was present in the original function or
	// what is the purpose of this function.
#if 0
	if(Actor->Type == PLAYER){
		CheckRight(Actor->ID, ALL_SPELLS);
	}
#endif
}

void CheckAffectedPlayers(TCreature *Actor, int x, int y, int z){
	if(Actor == NULL){
		error("CheckAffectedPlayers: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(WorldType == NORMAL
			&& Actor->Type == PLAYER
			&& Actor->Combat.SecureMode == SECURE_MODE_ENABLED){
		Object Obj = GetFirstObject(x, y, z);
		while(Obj != NONE){
			if(Obj.getObjectType().isCreatureContainer()){
				uint32 TargetID = Obj.getCreatureID();
				if(IsCreaturePlayer(TargetID) && Actor->ID != TargetID
						&& !((TPlayer*)Actor)->IsAttackJustified(TargetID)){
					throw SECUREMODE;
				}
			}
			Obj = Obj.getNextObject();
		}
	}
}

void CheckMana(TCreature *Actor, int ManaPoints, int SoulPoints, int Delay){
	if(Actor == NULL){
		error("CheckMana: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(Actor->Type != PLAYER || ManaPoints < 0){
		return;
	}

	TSkill *Mana = Actor->Skills[SKILL_MANA];
	if(Mana == NULL){
		error("CheckMana: Kein Skill MANA!\n");
		throw ERROR;
	}

	TSkill *Soul = Actor->Skills[SKILL_SOUL];
	if(Soul == NULL){
		error("CheckMana: Kein Skill SOULPOINTS!\n");
		throw ERROR;
	}

	if(!CheckRight(Actor->ID, UNLIMITED_MANA)){
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
		Actor->Skills[SKILL_MAGIC_LEVEL]->Increase(ManaPoints);
	}

	uint32 EarliestSpellTime = ServerMilliseconds + Delay;
	if(Actor->EarliestSpellTime < EarliestSpellTime){
		Actor->EarliestSpellTime = EarliestSpellTime;
	}
}

int ComputeDamage(TCreature *Actor, int SpellNr, int Damage, int Variation){
	if(Variation != 0){
		Damage += random(-Variation, Variation);
	}

	if(Actor != NULL && Actor->Type == PLAYER){
		int Level = Actor->Skills[SKILL_LEVEL]->Get();
		int MagicLevel = Actor->Skills[SKILL_MAGIC_LEVEL]->Get();
		int Multiplier = Level * 2 + MagicLevel * 3;
		if(SpellNr != 0){
			if((SpellList[SpellNr].Flags & 4) != 0 && Multiplier > 100){
				Multiplier = 100;
			}

			if((SpellList[SpellNr].Flags & 8) != 0 && Multiplier < 100){
				Multiplier = 100;
			}
		}
		Damage = (Damage * Multiplier) / 100;
	}

	return Damage;
}

bool IsAggressiveSpell(int SpellNr){
	if(SpellNr < 1 || SpellNr >= NARRAY(SpellList)){
		error("IsAggressiveSpell: Ungültige Spruchnummer %d.\n", SpellNr);
		return false;
	}

	return (SpellList[SpellNr].Flags & 1) != 0;
}

void MassCombat(TCreature *Actor, Object Target, int ManaPoints, int SoulPoints,
		int Damage, int Effect, int Radius, int DamageType, int Animation){
	if(!Target.exists()){
		error("MassCombat: Übergebenes Ziel existiert nicht.\n");
		throw ERROR;
	}

	if(Actor != NULL){
		error("MassCombat: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	int TargetX, TargetY, TargetZ;
	GetObjectCoordinates(Target, &TargetX, &TargetY, &TargetZ);
	CheckAffectedPlayers(Actor, TargetX, TargetY, TargetZ);
	if(!ThrowPossible(Actor->posx, Actor->posy, Actor->posz,
				TargetX, TargetY, TargetZ, 0)){
		throw CANNOTTHROW;
	}

	int Delay = 2000;
	if(WorldType == PVP_ENFORCED){
		Delay = 1000;
	}
	CheckMana(Actor, ManaPoints, SoulPoints, Delay);

	TDamageImpact Impact(Actor, DamageType, Damage, false);
	CircleShapeSpell(Actor, TargetX, TargetY, TargetZ,
			INT_MAX, Animation, Radius, &Impact, Effect);
}

void AngleCombat(TCreature *Actor, int ManaPoints, int SoulPoints,
		int Damage, int Effect, int Range, int Angle, int DamageType){
	if(Actor == NULL){
		error("AngleCombat: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	int Delay = 2000;
	if(WorldType == PVP_ENFORCED || (Range == 1 && Angle == 0)){
		Delay = 1000;
	}
	CheckMana(Actor, ManaPoints, SoulPoints, Delay);

	TDamageImpact Impact(Actor, DamageType, Damage, false);
	AngleShapeSpell(Actor, Angle, Range, &Impact, Effect);
}

void Combat(TCreature *Actor, Object Target, int ManaPoints, int SoulPoints,
		int Damage, int Effect, int Animation, int DamageType){
	if(!Target.exists()){
		error("Combat: Übergebenes Ziel existiert nicht.\n");
		throw ERROR;
	}

	if(Actor == NULL){
		error("Combat: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	int TargetX, TargetY, TargetZ;
	GetObjectCoordinates(Target, &TargetX, &TargetY, &TargetZ);
	if(!Target.getObjectType().isCreatureContainer()){
		Target = GetFirstSpecObject(TargetX, TargetY, TargetZ, TYPEID_CREATURE_CONTAINER);
	}

	if(Target == NONE){
		throw NOCREATURE;
	}

	CheckAffectedPlayers(Actor, TargetX, TargetY, TargetZ);
	if(!ThrowPossible(Actor->posx, Actor->posy, Actor->posz,
			TargetX, TargetY, TargetZ, 0)){
		throw CANNOTTHROW;
	}

	int Delay = 2000;
	if(WorldType == PVP_ENFORCED){
		Delay = 1000;
	}
	CheckMana(Actor, ManaPoints, SoulPoints, Delay);

	TDamageImpact Impact(Actor, DamageType, Damage, false);
	CircleShapeSpell(Actor, TargetX, TargetY, TargetZ,
			INT_MAX, Animation, 0, &Impact, Effect);
}

// Spell Functions
// =============================================================================
void KillAllMonsters(TCreature *Actor, int Effect, int Radius){
	if(Actor == NULL){
		error("KillAllMonsters: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(Actor->Type == PLAYER && !CheckRight(Actor->ID, CREATE_MONSTERS)){
		return;
	}

	// TODO(fusion): This is similar to `ExecuteCircleSpell` which makes me think
	// there is perhaps a common denominator?
	if(Radius >= NARRAY(Circle)){
		Radius = NARRAY(Circle) - 1;
	}

	int CenterX = Actor->posx;
	int CenterY = Actor->posy;
	int CenterZ = Actor->posz;
	for(int R = 0; R <= Radius; R += 1){
		int CirclePoints = Circle[R].Count;
		for(int Point = 0; Point < CirclePoints; Point += 1){
			int FieldX = CenterX + Circle[R].x[Point];
			int FieldY = CenterY + Circle[R].y[Point];
			int FieldZ = CenterZ;

			if(IsProtectionZone(FieldX, FieldY, FieldZ)){
				continue;
			}

			if(!ThrowPossible(CenterX, CenterY, CenterZ, FieldX, FieldY, FieldZ, 0)){
				continue;
			}

			Object Obj = GetFirstObject(FieldX, FieldY, FieldZ);
			while(Obj != NONE){
				if(Obj.getObjectType().isCreatureContainer()){
					TCreature *Victim = GetCreature(Obj);
					if(Victim == NULL){
						error("KillAllMonsters: Ungültige Kreatur.\n");
					}else if(Actor != Victim && Victim->Type == MONSTER){
						print(3, "Töte %s...\n", Victim->Name);
						Victim->Skills[SKILL_HITPOINTS]->Set(0);
						Victim->Death();
					}
				}
				Obj = Obj.getNextObject();
			}

			if(Effect != EFFECT_NONE){
				GraphicalEffect(FieldX, FieldY, FieldZ, Effect);
			}
		}
	}
}

void CreateField(TCreature *Actor, Object Target, int ManaPoints, int SoulPoints, int FieldType){
	if(Actor == NULL){
		error("CreateField: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	if(!Target.exists()){
		error("CreateField: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	int TargetX, TargetY, TargetZ;
	GetObjectCoordinates(Target, &TargetX, &TargetY, &TargetZ);
	if(!FieldPossible(TargetX, TargetY, TargetZ, FieldType)){
		throw NOROOM;
	}

	CheckAffectedPlayers(Actor, TargetX, TargetY, TargetZ);
	if(!ThrowPossible(Actor->posx, Actor->posy, Actor->posz,
			TargetX, TargetY, TargetZ, 0)){
		throw CANNOTTHROW;
	}

	int Delay = 2000;
	if(WorldType == PVP_ENFORCED){
		Delay = 1000;
	}
	CheckMana(Actor, ManaPoints, SoulPoints, Delay);

	int Animation = ANIMATION_ENERGY;
	if(FieldType == 1){
		Animation = ANIMATION_FIRE;
	}
	Missile(Actor->CrObject, Target, Animation);

	bool Peaceful = (WorldType == NON_PVP && Actor->IsPeaceful());
	CreateField(TargetX, TargetY, TargetZ, FieldType, Actor->ID, Peaceful);

	// TODO(fusion): Probably damaging fields?
	if(FieldType == 1 || FieldType == 2 || FieldType == 3){
		Actor->BlockLogout(60, true);
	}
}

void CreateField(TCreature *Actor, int ManaPoints, int SoulPoints, int FieldType){
	if(Actor == NULL){
		error("CreateField: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	// TODO(fusion): This is probably an inlined function `TCreature::GetForwardPosition`.
	int TargetX = Actor->posx;
	int TargetY = Actor->posy;
	int TargetZ = Actor->posz;
	switch(Actor->Direction){
		case DIRECTION_NORTH:	TargetY -= 1; break;
		case DIRECTION_EAST:	TargetX += 1; break;
		case DIRECTION_SOUTH:	TargetY += 1; break;
		case DIRECTION_WEST:	TargetX -= 1; break;
	}

	if(Actor->Type == PLAYER && !CheckRight(Actor->ID, ATTACK_EVERYWHERE)
			&& IsProtectionZone(TargetX, TargetY, TargetZ)){
		throw PROTECTIONZONE;
	}

	Object Target = GetMapContainer(TargetX, TargetY, TargetZ);
	CreateField(Actor, Target, ManaPoints, SoulPoints, FieldType);
}

void MassCreateField(TCreature *Actor, Object Target,
		int ManaPoints, int SoulPoints, int FieldType, int Radius){
	if(Actor == NULL){
		error("MassCreateField: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	if(!Target.exists()){
		error("MassCreateField: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	int TargetX, TargetY, TargetZ;
	GetObjectCoordinates(Target, &TargetX, &TargetY, &TargetZ);
	CheckAffectedPlayers(Actor, TargetX, TargetY, TargetZ);
	if(!ThrowPossible(Actor->posx, Actor->posy, Actor->posz,
			TargetX, TargetY, TargetZ, 0)){
		throw CANNOTTHROW;
	}

	int Delay = 2000;
	if(WorldType == PVP_ENFORCED){
		Delay = 1000;
	}
	CheckMana(Actor, ManaPoints, SoulPoints, Delay);

	if(Actor->posx != TargetX || Actor->posy != TargetY || Actor->posz != TargetZ){
		int Animation = ANIMATION_ENERGY;
		if(FieldType == 1){
			Animation = ANIMATION_FIRE;
		}
		Missile(Actor->CrObject, Target, Animation);
	}

	// TODO(fusion): Same as `KillAllMonsters`. Perhaps the `ExecuteCircleSpell`
	// function is getting inlined and TFieldImpact devirtualized?
	if(Radius >= NARRAY(Circle)){
		Radius = NARRAY(Circle) - 1;
	}

	bool Peaceful = (WorldType == NON_PVP && Actor->IsPeaceful());
	for(int R = 0; R <= Radius; R += 1){
		int CirclePoints = Circle[R].Count;
		for(int Point = 0; Point < CirclePoints; Point += 1){
			int FieldX = TargetX + Circle[R].x[Point];
			int FieldY = TargetY + Circle[R].y[Point];
			int FieldZ = TargetZ;

			if(IsProtectionZone(FieldX, FieldY, FieldZ)){
				continue;
			}

			if(!ThrowPossible(TargetX, TargetY, TargetZ, FieldX, FieldY, FieldZ, 0)){
				continue;
			}

			CreateField(FieldX, FieldY, FieldZ, FieldType, Actor->ID, Peaceful);
		}
	}

	if(FieldType == 1 || FieldType == 2 || FieldType == 3){
		Actor->BlockLogout(60, true);
	}
}

void CreateFieldWall(TCreature *Actor, Object Target,
		int ManaPoints, int SoulPoints, int FieldType, int Width){
	if(Actor == NULL){
		error("CreateFieldWall: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	if(!Target.exists()){
		error("CreateFieldWall: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	int ActorX = Actor->posx;
	int ActorY = Actor->posy;
	int ActorZ = Actor->posz;
	int TargetX, TargetY, TargetZ;
	GetObjectCoordinates(Target, &TargetX, &TargetY, &TargetZ);
	CheckAffectedPlayers(Actor, TargetX, TargetY, TargetZ);
	if(!ThrowPossible(ActorX, ActorY, ActorZ, TargetX, TargetY, TargetZ, 0)){
		throw CANNOTTHROW;
	}

	int Delay = 2000;
	if(WorldType == PVP_ENFORCED){
		Delay = 1000;
	}
	CheckMana(Actor, ManaPoints, SoulPoints, Delay);

	if(ActorX != TargetX || ActorY != TargetY || ActorZ != TargetZ){
		int Animation = ANIMATION_ENERGY;
		if(FieldType == 1){
			Animation = ANIMATION_FIRE;
		}
		Missile(Actor->CrObject, Target, Animation);
	}

	int StepX, StepY;
	int Direction = GetDirection(TargetX - ActorX, TargetY - ActorY);
	switch(Direction){
		case DIRECTION_NORTH:
		case DIRECTION_SOUTH:{
			StepX = 1;
			StepY = 0;
			break;
		}

		case DIRECTION_EAST:
		case DIRECTION_WEST:{
			StepX = 0;
			StepY = 1;
			break;
		}

		case DIRECTION_SOUTHWEST:
		case DIRECTION_NORTHEAST:{
			StepX = 1;
			StepY = 1;
			break;
		}

		case DIRECTION_SOUTHEAST:
		case DIRECTION_NORTHWEST:{
			StepX = -1;
			StepY = 1;
			break;
		}

		case DIRECTION_INVALID:{
			throw NOROOM;
		}

		default:{
			error("CreateFieldWall: Ungültige Richtung %d.\n", Direction);
			throw ERROR;
		}
	}

	bool Peaceful = (WorldType == NON_PVP && Actor->IsPeaceful());
	CreateField(TargetX, TargetY, TargetZ, FieldType, Actor->ID, Peaceful);
	for(int i = 1; i <= Width; i += 1){
		// NOTE(fusion): Forward.
		{
			int FieldX = TargetX + i * StepX;
			int FieldY = TargetY + i * StepY;
			int FieldZ = TargetZ;
			if(ThrowPossible(ActorX, ActorY, ActorZ, FieldX, FieldY, FieldZ, 0)
					&& !IsProtectionZone(FieldX, FieldY, FieldZ)){
				CreateField(FieldX, FieldY, FieldZ, FieldType, Actor->ID, Peaceful);
			}
		}

		// NOTE(fusion): Forward diagonal.
		if(StepX != 0 && StepY != 0){
			int FieldX = TargetX + i * StepX;
			int FieldY = TargetY + (i - 1) * StepY;
			int FieldZ = TargetZ;
			if(ThrowPossible(ActorX, ActorY, ActorZ, FieldX, FieldY, FieldZ, 0)
					&& !IsProtectionZone(FieldX, FieldY, FieldZ)){
				CreateField(FieldX, FieldY, FieldZ, FieldType, Actor->ID, Peaceful);
			}
		}

		// NOTE(fusion): Backward.
		{
			int FieldX = TargetX - i * StepX;
			int FieldY = TargetY - i * StepY;
			int FieldZ = TargetZ;
			if(ThrowPossible(ActorX, ActorY, ActorZ, FieldX, FieldY, FieldZ, 0)
					&& !IsProtectionZone(FieldX, FieldY, FieldZ)){
				CreateField(FieldX, FieldY, FieldZ, FieldType, Actor->ID, Peaceful);
			}
		}

		// NOTE(fusion): Backward Diagonal.
		if(StepX != 0 && StepY != 0){
			int FieldX = TargetX - i * StepX;
			int FieldY = TargetY - (i - 1) * StepY;
			int FieldZ = TargetZ;
			if(ThrowPossible(ActorX, ActorY, ActorZ, FieldX, FieldY, FieldZ, 0)
					&& !IsProtectionZone(FieldX, FieldY, FieldZ)){
				CreateField(FieldX, FieldY, FieldZ, FieldType, Actor->ID, Peaceful);
			}
		}
	}

	if(FieldType == 1 || FieldType == 2 || FieldType == 3){
		Actor->BlockLogout(60, true);
	}
}

void DeleteField(TCreature *Actor, Object Target, int ManaPoints, int SoulPoints){
	if(Actor == NULL){
		error("DeleteField: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	if(!Target.exists()){
		error("DeleteField: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	int TargetX, TargetY, TargetZ;
	GetObjectCoordinates(Target, &TargetX, &TargetY, &TargetZ);
	if(!FieldPossible(TargetX, TargetY, TargetZ, 0)){
		throw NOROOM;
	}

	CheckMana(Actor, ManaPoints, SoulPoints, 1000);

	Object Obj = GetFirstObject(TargetX, TargetY, TargetZ);
	while(Obj != NONE){
		Object Next = Obj.getNextObject();
		if(Obj.getObjectType().getFlag(MAGICFIELD)){
			Delete(Obj, -1);
		}
		Obj = Next;
	}

	GraphicalEffect(TargetX, TargetY, TargetZ, EFFECT_POFF);
}

void CleanupField(TCreature *Actor, Object Target, int ManaPoints, int SoulPoints){
	if(Actor == NULL){
		error("CleanupField: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	if(!Target.exists()){
		error("CleanupField: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	int TargetX, TargetY, TargetZ;
	GetObjectCoordinates(Target, &TargetX, &TargetY, &TargetZ);

	int Distance = std::min<int>(
			std::abs(Actor->posx - TargetX),
			std::abs(Actor->posy - TargetY));
	if(Distance > 1 || Actor->posz != TargetZ){
		throw OUTOFRANGE;
	}

	CheckMana(Actor, ManaPoints, SoulPoints, 1000);
	Object Obj = GetFirstObject(TargetX, TargetY, TargetZ);
	while(Obj != NONE){
		Object Next = Obj.getNextObject();
		ObjectType ObjType = Obj.getObjectType();
		// TODO(fusion): It seems that corpse type can be either 0 for human
		// corpses or 1 for other/monster corpses, so we're avoiding deleting
		// human corpses here.
		if(!ObjType.getFlag(UNMOVE) && !ObjType.isCreatureContainer()
		&& (!ObjType.getFlag(CORPSE) || ObjType.getAttribute(CORPSETYPE) != 0)){
			Delete(Obj, -1);
		}
		Obj = Next;
	}
	GraphicalEffect(TargetX, TargetY, TargetZ, EFFECT_POFF);
	Actor->BlockLogout(60, true);
}

void CleanupField(TCreature *Actor){
	if(Actor == NULL){
		error("CleanupField: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	if(Actor->Type != PLAYER){
		error("CleanupField: Zauberspruch kann nur von Spielern angewendet werden.\n");
		throw ERROR;
	}

	if(!CheckRight(Actor->ID, CLEANUP_FIELDS)){
		return;
	}

	// TODO(fusion): This is probably an inlined function `TCreature::GetForwardPosition`.
	int TargetX = Actor->posx;
	int TargetY = Actor->posy;
	int TargetZ = Actor->posz;
	switch(Actor->Direction){
		case DIRECTION_NORTH:	TargetY -= 1; break;
		case DIRECTION_EAST:	TargetX += 1; break;
		case DIRECTION_SOUTH:	TargetY += 1; break;
		case DIRECTION_WEST:	TargetX -= 1; break;
	}

	Object Target = GetMapContainer(TargetX, TargetY, TargetZ);
	CleanupField(Actor, Target, 0, 0);
}

void Teleport(TCreature *Actor, char *Param){
	if(Actor == NULL){
		error("Teleport: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	if(Param == NULL){
		error("Teleport: Param ist NULL.\n");
		throw ERROR;
	}

	if(Actor->Type != PLAYER){
		error("Teleport: Zauberspruch kann nur von Spielern angewendet werden.\n");
		throw ERROR;
	}

	int DestX = Actor->posx;
	int DestY = Actor->posy;
	int DestZ = Actor->posz;
	// TODO(fusion): The `HouseID` parameter of `SearchFreeField` can be 0xFFFF
	// as a wildcard to any house. This is probably only used for these special
	// commands and we should have a `HOUSEID_ANY` constant defined somewhere.
	//	If it is something else, the function will try to find a free field with
	// the same house id. I'm not sure why we use it with marks but I think we
	// should only use 0xFFFF here.
	uint16 HouseID = 0xFFFF;
	int MDGoStrength = Actor->Skills[SKILL_GO_STRENGTH]->MDAct;

	if(stricmp(Param, "up") == 0){
		if(!CheckRight(Actor->ID, TELEPORT_VERTICAL)){
			return;
		}
		DestZ -= 1;
	}else if(stricmp(Param, "down") == 0){
		if(!CheckRight(Actor->ID, TELEPORT_VERTICAL)){
			return;
		}
		DestZ += 1;
	}else if(stricmp(Param, "fast") == 0){
		if(!CheckRight(Actor->ID, MODIFY_GOSTRENGTH)){
			return;
		}
		MDGoStrength = 100;
	}else if(stricmp(Param, "fastest") == 0){
		if(!CheckRight(Actor->ID, MODIFY_GOSTRENGTH)){
			return;
		}
		MDGoStrength = 200;
	}else if(stricmp(Param, "slow") == 0
			|| stricmp(Param, "normal") == 0){
		if(!CheckRight(Actor->ID, MODIFY_GOSTRENGTH)){
			return;
		}
		MDGoStrength = 0;
	}else{
		int ParamX, ParamY, ParamZ;
		if(sscanf(Param, "%d,%d,%d", &ParamX, &ParamY, &ParamZ) == 3
		|| sscanf(Param, "[%d,%d,%d]", &ParamX, &ParamY, &ParamZ) == 3){
			if(!CheckRight(Actor->ID, TELEPORT_TO_COORDINATE)){
				return;
			}

			if(IsOnMap(ParamX, ParamY, ParamZ)){
				DestX = ParamX;
				DestY = ParamY;
				DestZ = ParamZ;
			}else if(IsOnMap(DestX + ParamX, DestY + ParamY, DestZ + ParamZ)){
				DestX += ParamX;
				DestY += ParamY;
				DestZ += ParamZ;
			}else{
				SendMessage(Actor->Connection, TALK_FAILURE_MESSAGE, "Invalid coordinates.");
				return;
			}
		}else{
			if(!CheckRight(Actor->ID, TELEPORT_TO_MARK)){
				return;
			}

			if(GetMarkPosition(Param, &ParamX, &ParamY, &ParamZ)){
				DestX = ParamX;
				DestY = ParamY;
				DestZ = ParamZ;
				// TODO(fusion): Not sure why we do this here.
				HouseID = GetHouseID(DestX, DestY, DestZ);
			}else{
				SendMessage(Actor->Connection, TALK_FAILURE_MESSAGE, "There is no mark of this name.");
				return;
			}
		}
	}

	if(DestX != Actor->posx || DestY != Actor->posy || DestZ != Actor->posz){
		if(!SearchFreeField(&DestX, &DestY, &DestZ, 1, HouseID, true)){
			throw NOROOM;
		}
		Object Dest = GetMapContainer(DestX, DestY, DestZ);
		Move(0, Actor->CrObject, Dest, -1, false, NONE);
		GraphicalEffect(DestX, DestY, DestZ, EFFECT_TELEPORT);
	}else if(MDGoStrength != Actor->Skills[SKILL_GO_STRENGTH]->MDAct){
		Actor->Skills[SKILL_GO_STRENGTH]->SetMDAct(MDGoStrength);
		AnnounceChangedCreature(Actor->ID, 4); // CREATURE_GO_STRENGTH_CHANGED ?
		GraphicalEffect(DestX, DestY, DestZ, EFFECT_MAGIC_BLUE);
	}
}

void TeleportToCreature(TCreature *Actor, const char *Name){
	if(Actor == NULL){
		error("TeleportToCreature: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	if(Actor->Type != PLAYER){
		error("TeleportToCreature: Zauberspruch kann nur von Spielern angewendet werden.\n");
		throw ERROR;
	}

	if(!CheckRight(Actor->ID, TELEPORT_TO_CHARACTER)){
		return;
	}

	if(Name == NULL || Name[0] == 0){
		SendMessage(Actor->Connection, TALK_FAILURE_MESSAGE, "You must enter a name.");
		return;
	}

	TPlayer *Player;
	bool IgnoreGamemasters = !CheckRight(Actor->ID, READ_GAMEMASTER_CHANNEL);
	switch(IdentifyPlayer(Name, false, IgnoreGamemasters, &Player)){
		case  0:	break; // PLAYERFOUND ?
		case -1:	throw PLAYERNOTONLINE;
		case -2:	throw NAMEAMBIGUOUS;
		default:{
			error("TeleportToCreature: Ungültiger Rückgabewert von IdentifyPlayer.\n");
			throw ERROR;
		}
	}

	if(Actor == Player){
		GraphicalEffect(Actor->CrObject, EFFECT_TELEPORT);
		return;
	}

	GraphicalEffect(Actor->CrObject, EFFECT_POFF);

	// TODO(fusion): I assume `SearchFreeField` won't modify the input position
	// so we're either teleporting to a nearby free position or to the player's
	// position if it's not protection zone.
	int DestX = Player->posx;
	int DestY = Player->posy;
	int DestZ = Player->posz;
	uint16 HouseID = GetHouseID(DestX, DestY, DestZ);
	if(!SearchFreeField(&DestX, &DestY, &DestZ, 1, HouseID, true)
			|| IsProtectionZone(DestX, DestY, DestZ)){
		throw NOROOM;
	}

	Object Dest = GetMapContainer(DestX, DestY, DestZ);
	Move(0, Actor->CrObject, Dest, -1, false, NONE);
	GraphicalEffect(DestX, DestY, DestZ, EFFECT_TELEPORT);
	Log("banish", "%s teleportiert sich zu %s.\n", Actor->Name, Player->Name);
}

void TeleportPlayerToMe(TCreature *Actor, const char *Name){
	if(Actor == NULL){
		error("TeleportPlayerToMe: Ungültige Kreatur übergeben.\n");
		throw ERROR;
	}

	if(Actor->Type != PLAYER){
		error("TeleportPlayerToMe: Zauberspruch kann nur von Spielern angewendet werden.\n");
		throw ERROR;
	}

	if(!CheckRight(Actor->ID, RETRIEVE)){
		return;
	}

	if(Name == NULL || Name[0] == 0){
		SendMessage(Actor->Connection, TALK_FAILURE_MESSAGE, "You must enter a name.");
		return;
	}

	TPlayer *Player;
	bool IgnoreGamemasters = !CheckRight(Actor->ID, READ_GAMEMASTER_CHANNEL);
	switch(IdentifyPlayer(Name, false, IgnoreGamemasters, &Player)){
		case  0:	break; // PLAYERFOUND ?
		case -1:	throw PLAYERNOTONLINE;
		case -2:	throw NAMEAMBIGUOUS;
		default:{
			error("TeleportPlayerToMe: Ungültiger Rückgabewert von IdentifyPlayer.\n");
			throw ERROR;
		}
	}

	GraphicalEffect(Player->CrObject, EFFECT_POFF);

	int DestX = Actor->posx;
	int DestY = Actor->posy;
	int DestZ = Actor->posz;
	uint16 HouseID = GetHouseID(DestX, DestY, DestZ);
	if(!SearchFreeField(&DestX, &DestY, &DestZ, 1, HouseID, false)){
		throw NOROOM;
	}

	Object Dest = GetMapContainer(DestX, DestY, DestZ);
	Move(0, Player->CrObject, Dest, -1, false, NONE);
	GraphicalEffect(DestX, DestY, DestZ, EFFECT_TELEPORT);
}

// Magic Init Functions
// =============================================================================
static void InitCircles(void){
	char FileName[4096];
	snprintf(FileName, sizeof(FileName), "%s/circles.dat", DATAPATH);
	std::ifstream IN(FileName, std::ios_base::in);
	if(IN.fail()){
		error("InitCircles: Kann Datei %s nicht öffnen.\n", FileName);
		throw "Cannot open \"circles.dat\"";
	}

	// TODO(fusion): This doesn't make a lot of sense because we have `Width`
	// and `Height` but not `CenterX` and `CenterY`.
	int Width, Height, Center;
	IN >> Width >> Height >> Center;

	if(Width < 0 || Width > 21 || Height < 0 || Height > 21){
		error("InitCircles: Ungültiges Dateiformat.\n");
		throw "Cannot process \"circles.dat\"";
	}

	if(Center > 0){
		for(int Y = 0; Y < Height; Y += 1)
		for(int X = 0; X < Width; X += 1){
			int Radius;
			IN >> Radius;
			if(Radius < NARRAY(Circle)){
				int PointIndex = Circle[Radius].Count;
				ASSERT(PointIndex < 32);
				Circle[Radius].x[PointIndex] = X - Center;
				Circle[Radius].y[PointIndex] = Y - Center;
				Circle[Radius].Count += 1;
			}
		}
	}

	// TODO(fusion): Probably check if we parsed the file successfully?
	//if(IN.fail()) { throw "..."; }
}

static TSpellList *CreateSpell(int SpellNr, ...){
	ASSERT(SpellNr < NARRAY(SpellList));
	int SyllableCount = 0;
	TSpellList *Spell = &SpellList[SpellNr];

	va_list ap;
	va_start(ap, SpellNr);
	while(true){
		const char *Syllable = va_arg(ap, const char*);
		if(!Syllable || Syllable[0] == 0){
			break;
		}

		for(int SyllableNr = 0;
				SyllableNr < NARRAY(SpellSyllable);
				SyllableNr += 1){
			if(strcmp(SpellSyllable[SyllableNr], Syllable) == 0){
				// TODO(fusion): I'm not sure it is a good idea to throw an
				// exception between `va_start` and `va_end`.
				if(SyllableCount > NARRAY(Spell->Syllable)){
					error("CreateSpell: Silbenzahl überschritten bei Spell %d.\n", SpellNr);
					throw "Spell has too many syllables";
				}

				Spell->Syllable[SyllableCount] = (uint8)SyllableNr;
				SyllableCount += 1;
				break;
			}
		}
	}
	va_end(ap);
	return Spell;
}

static void InitSpells(void){
	TSpellList *Spell;

	Spell = CreateSpell(1, "ex", "ura", "");
	Spell->Mana = 25;
	Spell->Level = 9;
	Spell->Flags = 8;
	Spell->Comment = "Light Healing";

	Spell = CreateSpell(2, "ex", "ura", "gran", "");
	Spell->Mana = 40;
	Spell->Level = 11;
	Spell->Flags = 8;
	Spell->Comment = "Intense Healing";

	Spell = CreateSpell(3, "ex", "ura", "vita", "");
	Spell->Mana = 160;
	Spell->Level = 20;
	Spell->Flags = 8;
	Spell->Comment = "Ultimate Healing";

	Spell = CreateSpell(4, "ad", "ura", "gran", "");
	Spell->Mana = 240;
	Spell->Level = 15;
	Spell->RuneGr = 79;
	Spell->RuneNr = 5;
	Spell->Flags = 8;
	Spell->Amount = 1;
	Spell->RuneLevel = 1;
	Spell->SoulPoints = 2;
	Spell->Comment = "Intense Healing Rune";

	Spell = CreateSpell(5, "ad", "ura", "vita", "");
	Spell->Mana = 400;
	Spell->Level = 24;
	Spell->RuneGr = 79;
	Spell->RuneNr = 13;
	Spell->Flags = 8;
	Spell->Amount = 1;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 3;
	Spell->Comment = "Ultimate Healing Rune";

	Spell = CreateSpell(6, "ut", "ani", "hur", "");
	Spell->Mana = 60;
	Spell->Level = 14;
	Spell->Flags = 2;
	Spell->Comment = "Haste";

	Spell = CreateSpell(7, "ad", "ori", "");
	Spell->Mana = 120;
	Spell->Level = 15;
	Spell->RuneGr = 79;
	Spell->RuneNr = 27;
	Spell->Flags = 9;
	Spell->Amount = 5;
	Spell->RuneLevel = 0;
	Spell->SoulPoints = 1;
	Spell->Comment = "Light Magic Missile";

	Spell = CreateSpell(8, "ad", "ori", "gran", "");
	Spell->Mana = 280;
	Spell->Level = 25;
	Spell->RuneGr = 79;
	Spell->RuneNr = 51;
	Spell->Flags = 9;
	Spell->Amount = 5;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 2;
	Spell->Comment = "Heavy Magic Missile";

	Spell = CreateSpell(9, "ut", "evo", "res", "para", "");
	Spell->Mana = 0;
	Spell->Level = 25;
	Spell->Flags = 1;
	Spell->Comment = "Summon Creature";

	Spell = CreateSpell(10, "ut", "evo", "lux", "");
	Spell->Mana = 20;
	Spell->Level = 8;
	Spell->Flags = 0;
	Spell->Comment = "Light";

	Spell = CreateSpell(11, "ut", "evo", "gran", "lux", "");
	Spell->Mana = 60;
	Spell->Level = 13;
	Spell->Flags = 0;
	Spell->Comment = "Great Light";

	Spell = CreateSpell(12, "ad", "eta", "sio", "");
	Spell->Mana = 200;
	Spell->Level = 16;
	Spell->RuneGr = 79;
	Spell->RuneNr = 30;
	Spell->Flags = 1;
	Spell->Amount = 1;
	Spell->RuneLevel = 5;
	Spell->SoulPoints = 3;
	Spell->Comment = "Convince Creature";

	Spell = CreateSpell(13, "ex", "evo", "mort", "hur", "");
	Spell->Mana = 250;
	Spell->Level = 38;
	Spell->Flags = 1;
	Spell->Comment = "Energy Wave";

	Spell = CreateSpell(14, "ad", "evo", "ina", "");
	Spell->Mana = 600;
	Spell->Level = 27;
	Spell->RuneGr = 79;
	Spell->RuneNr = 31;
	Spell->Flags = 0;
	Spell->Amount = 1;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 2;
	Spell->Comment = "Chameleon";

	Spell = CreateSpell(15, "ad", "ori", "flam", "");
	Spell->Mana = 160;
	Spell->Level = 17;
	Spell->RuneGr = 79;
	Spell->RuneNr = 42;
	Spell->Flags = 9;
	Spell->Amount = 2;
	Spell->RuneLevel = 2;
	Spell->SoulPoints = 2;
	Spell->Comment = "Fireball";

	Spell = CreateSpell(16, "ad", "ori", "gran", "flam", "");
	Spell->Mana = 480;
	Spell->Level = 23;
	Spell->RuneGr = 79;
	Spell->RuneNr = 44;
	Spell->Flags = 9;
	Spell->Amount = 2;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 3;
	Spell->Comment = "Great Fireball";

	Spell = CreateSpell(17, "ad", "evo", "mas", "flam", "");
	Spell->Mana = 600;
	Spell->Level = 27;
	Spell->RuneGr = 79;
	Spell->RuneNr = 45;
	Spell->Flags = 1;
	Spell->Amount = 2;
	Spell->RuneLevel = 5;
	Spell->SoulPoints = 4;
	Spell->Comment = "Firebomb";

	Spell = CreateSpell(18, "ad", "evo", "mas", "hur", "");
	Spell->Mana = 720;
	Spell->Level = 31;
	Spell->RuneGr = 79;
	Spell->RuneNr = 53;
	Spell->Flags = 1;
	Spell->Amount = 3;
	Spell->RuneLevel = 6;
	Spell->SoulPoints = 4;
	Spell->Comment = "Explosion";

	Spell = CreateSpell(19, "ex", "evo", "flam", "hur", "");
	Spell->Mana = 80;
	Spell->Level = 18;
	Spell->Flags = 9;
	Spell->Comment = "Fire Wave";

	Spell = CreateSpell(20, "ex", "iva", "para", "");
	Spell->Mana = 20;
	Spell->Level = 8;
	Spell->Flags = 0;
	Spell->Comment = "Find Person";

	Spell = CreateSpell(21, "ad", "ori", "vita", "vis", "");
	Spell->Mana = 880;
	Spell->Level = 45;
	Spell->RuneGr = 79;
	Spell->RuneNr = 8;
	Spell->Flags = 1;
	Spell->Amount = 1;
	Spell->RuneLevel = 15;
	Spell->SoulPoints = 5;
	Spell->Comment = "Sudden Death";

	Spell = CreateSpell(22, "ex", "evo", "vis", "lux", "");
	Spell->Mana = 100;
	Spell->Level = 23;
	Spell->Flags = 1;
	Spell->Comment = "Energy Beam";

	Spell = CreateSpell(23, "ex", "evo", "gran", "vis", "lux", "");
	Spell->Mana = 200;
	Spell->Level = 29;
	Spell->Flags = 1;
	Spell->Comment = "Great Energy Beam";

	Spell = CreateSpell(24, "ex", "evo", "gran", "mas", "vis", "");
	Spell->Mana = 1200;
	Spell->Level = 60;
	Spell->Flags = 3;
	Spell->Comment = "Ultimate Explosion";

	Spell = CreateSpell(25, "ad", "evo", "grav", "flam", "");
	Spell->Mana = 240;
	Spell->Level = 15;
	Spell->RuneGr = 79;
	Spell->RuneNr = 41;
	Spell->Flags = 1;
	Spell->Amount = 3;
	Spell->RuneLevel = 1;
	Spell->SoulPoints = 1;
	Spell->Comment = "Fire Field";

	Spell = CreateSpell(26, "ad", "evo", "grav", "pox", "");
	Spell->Mana = 200;
	Spell->Level = 14;
	Spell->RuneGr = 79;
	Spell->RuneNr = 25;
	Spell->Flags = 1;
	Spell->Amount = 3;
	Spell->RuneLevel = 0;
	Spell->SoulPoints = 1;
	Spell->Comment = "Poison Field";

	Spell = CreateSpell(27, "ad", "evo", "grav", "vis", "");
	Spell->Mana = 320;
	Spell->Level = 18;
	Spell->RuneGr = 79;
	Spell->RuneNr = 17;
	Spell->Flags = 1;
	Spell->Amount = 3;
	Spell->RuneLevel = 3;
	Spell->SoulPoints = 2;
	Spell->Comment = "Energy Field";

	Spell = CreateSpell(28, "ad", "evo", "mas", "grav", "flam", "");
	Spell->Mana = 780;
	Spell->Level = 33;
	Spell->RuneGr = 79;
	Spell->RuneNr = 43;
	Spell->Flags = 1;
	Spell->Amount = 4;
	Spell->RuneLevel = 6;
	Spell->SoulPoints = 4;
	Spell->Comment = "Fire Wall";

	Spell = CreateSpell(29, "ex", "ana", "pox", "");
	Spell->Mana = 30;
	Spell->Level = 10;
	Spell->Flags = 0;
	Spell->Comment = "Antidote";

	Spell = CreateSpell(30, "ad", "ito", "grav", "");
	Spell->Mana = 120;
	Spell->Level = 17;
	Spell->RuneGr = 79;
	Spell->RuneNr = 1;
	Spell->Flags = 0;
	Spell->Amount = 3;
	Spell->RuneLevel = 3;
	Spell->SoulPoints = 2;
	Spell->Comment = "Destroy Field";

	Spell = CreateSpell(31, "ad", "ana", "pox", "");
	Spell->Mana = 200;
	Spell->Level = 15;
	Spell->RuneGr = 79;
	Spell->RuneNr = 6;
	Spell->Flags = 0;
	Spell->Amount = 1;
	Spell->RuneLevel = 0;
	Spell->SoulPoints = 1;
	Spell->Comment = "Antidote Rune";

	Spell = CreateSpell(32, "ad", "evo", "mas", "grav", "pox", "");
	Spell->Mana = 640;
	Spell->Level = 29;
	Spell->RuneGr = 79;
	Spell->RuneNr = 29;
	Spell->Flags = 1;
	Spell->Amount = 4;
	Spell->RuneLevel = 5;
	Spell->SoulPoints = 3;
	Spell->Comment = "Poison Wall";

	Spell = CreateSpell(33, "ad", "evo", "mas", "grav", "vis", "");
	Spell->Mana = 1000;
	Spell->Level = 41;
	Spell->RuneGr = 79;
	Spell->RuneNr = 19;
	Spell->Flags = 1;
	Spell->Amount = 4;
	Spell->RuneLevel = 9;
	Spell->SoulPoints = 5;
	Spell->Comment = "Energy Wall";

	Spell = CreateSpell(34, "al", "evo", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Item";

	Spell = CreateSpell(35, "al", "evo", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Item";

	Spell = CreateSpell(37, "al", "ani", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Move";

	Spell = CreateSpell(38, "ut", "evo", "res", "ina", "para", "");
	Spell->Mana = 100;
	Spell->Level = 23;
	Spell->Flags = 0;
	Spell->Comment = "Creature Illusion";

	Spell = CreateSpell(39, "ut", "ani", "gran", "hur", "");
	Spell->Mana = 100;
	Spell->Level = 20;
	Spell->Flags = 2;
	Spell->Comment = "Strong Haste";

	Spell = CreateSpell(40, "al", "evo", "cogni", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Experience";

	Spell = CreateSpell(41, "al", "eta", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Change Data";

	Spell = CreateSpell(42, "ex", "evo", "pan", "");
	Spell->Mana = 120;
	Spell->Level = 14;
	Spell->SoulPoints = 1;
	Spell->Flags = 0;
	Spell->Comment = "Food";

	Spell = CreateSpell(44, "ut", "amo", "vita", "");
	Spell->Mana = 50;
	Spell->Level = 14;
	Spell->Flags = 0;
	Spell->Comment = "Magic Shield";

	Spell = CreateSpell(45, "ut", "ana", "vid", "");
	Spell->Mana = 440;
	Spell->Level = 35;
	Spell->Flags = 0;
	Spell->Comment = "Invisible";

	Spell = CreateSpell(46, "al", "evo", "cogni", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Skill Experience";

	Spell = CreateSpell(47, "al", "ani", "sio", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Teleport to Friend";

	Spell = CreateSpell(48, "ex", "evo", "con", "pox", "");
	Spell->Mana = 130;
	Spell->Level = 16;
	Spell->SoulPoints = 2;
	Spell->Flags = 0;
	Spell->Comment = "Poisoned Arrow";

	Spell = CreateSpell(49, "ex", "evo", "con", "flam", "");
	Spell->Mana = 290;
	Spell->Level = 25;
	Spell->SoulPoints = 3;
	Spell->Flags = 0;
	Spell->Comment = "Explosive Arrow";

	Spell = CreateSpell(50, "ad", "evo", "res", "flam", "");
	Spell->Mana = 600;
	Spell->Level = 27;
	Spell->RuneGr = 79;
	Spell->RuneNr = 48;
	Spell->Flags = 3;
	Spell->Amount = 2;
	Spell->RuneLevel = 7;
	Spell->SoulPoints = 3;
	Spell->Comment = "Soulfire";

	Spell = CreateSpell(51, "ex", "evo", "con", "");
	Spell->Mana = 100;
	Spell->Level = 13;
	Spell->SoulPoints = 1;
	Spell->Flags = 0;
	Spell->Comment = "Conjure Arrow";

	Spell = CreateSpell(52, "al", "liber", "sio", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Retrieve Friend";

	Spell = CreateSpell(53, "al", "evo", "res", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Summon Wild Creature";

	Spell = CreateSpell(54, "ad", "ana", "ani", "");
	Spell->Mana = 1400;
	Spell->Level = 54;
	Spell->RuneGr = 79;
	Spell->RuneNr = 18;
	Spell->Flags = 3;
	Spell->Amount = 1;
	Spell->RuneLevel = 18;
	Spell->SoulPoints = 3;
	Spell->Comment = "Paralyze";

	Spell = CreateSpell(55, "ad", "evo", "mas", "vis", "");
	Spell->Mana = 880;
	Spell->Level = 37;
	Spell->RuneGr = 79;
	Spell->RuneNr = 2;
	Spell->Flags = 3;
	Spell->Amount = 2;
	Spell->RuneLevel = 10;
	Spell->SoulPoints = 5;
	Spell->Comment = "Energybomb";

	Spell = CreateSpell(56, "ex", "evo", "gran", "mas", "pox", "");
	Spell->Mana = 600;
	Spell->Level = 50;
	Spell->Flags = 3;
	Spell->Comment = "Poison Storm";

	Spell = CreateSpell(57, "om", "ana", "liber", "para", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Banish Account";

	Spell = CreateSpell(58, "al", "iva", "tera", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Position";

	Spell = CreateSpell(60, "om", "ani", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Temple Teleport";

	Spell = CreateSpell(61, "om", "ana", "gran", "liber", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Delete Account";

	Spell = CreateSpell(62, "om", "amo", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Set Namerule";

	Spell = CreateSpell(63, "al", "evo", "vis", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Create Gold";

	Spell = CreateSpell(64, "al", "eta", "vita", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Change Profession or Sex";

	Spell = CreateSpell(65, "om", "isa", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Entry in Criminal Record";

	Spell = CreateSpell(66, "om", "ana", "hora", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Namelock";

	Spell = CreateSpell(67, "om", "ana", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Kick Player";

	Spell = CreateSpell(68, "om", "ana", "gran", "res", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Delete Character";

	Spell = CreateSpell(69, "om", "ana", "vis", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Banish IP Address";

	Spell = CreateSpell(70, "om", "ana", "res", "para", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Banish Character";

	Spell = CreateSpell(71, "al", "eta", "sio", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Invite Guests";

	Spell = CreateSpell(72, "al", "eta", "som", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Invite Subowners";

	Spell = CreateSpell(73, "al", "ana", "sio", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Kick Guest";

	Spell = CreateSpell(74, "al", "eta", "grav", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Edit Door";

	Spell = CreateSpell(75, "ut", "evo", "vis", "lux", "");
	Spell->Mana = 140;
	Spell->Level = 26;
	Spell->Flags = 2;
	Spell->Comment = "Ultimate Light";

	Spell = CreateSpell(76, "ex", "ani", "tera", "");
	Spell->Mana = 20;
	Spell->Level = 9;
	Spell->Flags = 2;
	Spell->Comment = "Magic Rope";

	Spell = CreateSpell(77, "ad", "evo", "res", "pox", "");
	Spell->Mana = 400;
	Spell->Level = 21;
	Spell->RuneGr = 79;
	Spell->RuneNr = 32;
	Spell->Flags = 3;
	Spell->Amount = 1;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 2;
	Spell->Comment = "Envenom";

	Spell = CreateSpell(78, "ad", "ito", "tera", "");
	Spell->Mana = 200;
	Spell->Level = 21;
	Spell->RuneGr = 79;
	Spell->RuneNr = 50;
	Spell->Amount = 3;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 3;
	Spell->Flags = 3;
	Spell->Comment = "Desintegrate";

	Spell = CreateSpell(79, "ex", "evo", "con", "mort", "");
	Spell->Mana = 140;
	Spell->Level = 17;
	Spell->SoulPoints = 2;
	Spell->Flags = 2;
	Spell->Comment = "Conjure Bolt";

	Spell = CreateSpell(80, "ex", "ori", "");
	Spell->Mana = 0;
	Spell->Level = 35;
	Spell->Flags = 7;
	Spell->Comment = "Berserk";

	Spell = CreateSpell(81, "ex", "ani", "hur", "para", "");
	Spell->Mana = 50;
	Spell->Level = 12;
	Spell->Flags = 2;
	Spell->Comment = "Levitate";

	Spell = CreateSpell(82, "ex", "ura", "gran", "mas", "res", "");
	Spell->Mana = 150;
	Spell->Level = 36;
	Spell->Flags = 10;
	Spell->Comment = "Mass Healing";

	Spell = CreateSpell(83, "ad", "ana", "mort", "");
	Spell->Mana = 600;
	Spell->Level = 27;
	Spell->RuneGr = 79;
	Spell->RuneNr = 56;
	Spell->Amount = 1;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 5;
	Spell->Flags = 3;
	Spell->Comment = "Animate Dead";

	Spell = CreateSpell(84, "ex", "ura", "sio", "para", "");
	Spell->Mana = 70;
	Spell->Level = 18;
	Spell->Flags = 10;
	Spell->Comment = "Heal Friend";

	Spell = CreateSpell(85, "ex", "ana", "mas", "mort", "");
	Spell->Mana = 500;
	Spell->Level = 30;
	Spell->Flags = 3;
	Spell->Comment = "Undead Legion";

	Spell = CreateSpell(86, "ad", "evo", "grav", "tera", "");
	Spell->Mana = 750;
	Spell->Level = 32;
	Spell->RuneGr = 79;
	Spell->RuneNr = 33;
	Spell->Amount = 3;
	Spell->RuneLevel = 9;
	Spell->SoulPoints = 5;
	Spell->Flags = 3;
	Spell->Comment = "Magic Wall";

	Spell = CreateSpell(87, "ex", "ori", "mort", "");
	Spell->Mana = 20;
	Spell->Level = 11;
	Spell->Flags = 3;
	Spell->Comment = "Force Strike";

	Spell = CreateSpell(88, "ex", "ori", "vis", "");
	Spell->Mana = 20;
	Spell->Level = 12;
	Spell->Flags = 3;
	Spell->Comment = "Energy Strike";

	Spell = CreateSpell(89, "ex", "ori", "flam", "");
	Spell->Mana = 20;
	Spell->Level = 12;
	Spell->Flags = 3;
	Spell->Comment = "Flame Strike";

	Spell = CreateSpell(90, "ex", "ana", "ina", "");
	Spell->Mana = 200;
	Spell->Level = 26;
	Spell->Flags = 2;
	Spell->Comment = "Cancel Invisibility";

	Spell = CreateSpell(91, "ad", "evo", "mas", "pox", "");
	Spell->Mana = 520;
	Spell->Level = 25;
	Spell->RuneGr = 79;
	Spell->RuneNr = 26;
	Spell->Flags = 3;
	Spell->Amount = 2;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 2;
	Spell->Comment = "Poisonbomb";

	Spell = CreateSpell(92, "ex", "eta", "vis", "");
	Spell->Mana = 80;
	Spell->Level = 41;
	Spell->Flags = 2;
	Spell->Comment = "Enchant Staff";

	Spell = CreateSpell(93, "ex", "eta", "res", "");
	Spell->Mana = 30;
	Spell->Level = 20;
	Spell->Flags = 3;
	Spell->Comment = "Challenge";

	Spell = CreateSpell(94, "ex", "evo", "grav", "vita", "");
	Spell->Mana = 220;
	Spell->Level = 27;
	Spell->Flags = 3;
	Spell->Comment = "Wild Growth";

	Spell = CreateSpell(95, "ex", "evo", "con", "vis", "");
	Spell->Mana = 800;
	Spell->Level = 59;
	Spell->SoulPoints = 3;
	Spell->Flags = 2;
	Spell->Comment = "Power Bolt";

	Spell = CreateSpell(96, "al", "iva", "cogni", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Quest Value";

	Spell = CreateSpell(97, "al", "eta", "cogni", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Set Quest Value";

	Spell = CreateSpell(98, "al", "ito", "tera", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Desintegrate Spell";

	Spell = CreateSpell(99, "al", "ani", "hur", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Levitate Gamemaster";

	Spell = CreateSpell(100, "al", "ana", "cogni", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Clear Quest Values";

	Spell = CreateSpell(101, "al", "ito", "mas", "res", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Kill All Creatures";

	Spell = CreateSpell(102, "al", "evo", "mas", "res", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Start Monsterraid";
}

void InitMagic(void){
	InitCircles();
	InitSpells();
	InitLog("banish");
}

void ExitMagic(void){
	// no-op
}

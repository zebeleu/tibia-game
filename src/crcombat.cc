#include "creature.hh"
#include "player.hh"
#include "config.hh"
#include "magic.hh"

#include "stubs.hh"

TCombat::TCombat(void){
	this->Master = NULL;
	this->EarliestAttackTime = 0;
	this->EarliestDefendTime = 0;
	this->LastDefendTime = 0;
	this->LatestAttackTime = 0;
	this->AttackMode = ATTACK_MODE_BALANCED;
	this->ChaseMode = CHASE_MODE_NONE;
	this->SecureMode = 1;
	this->AttackDest = 0;
	this->Following = false;
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

void TCombat::GetWeapon(void){
	this->Shield = NONE;
	this->Close = NONE;
	this->Missile = NONE;
	this->Throw = NONE;
	this->Wand = NONE;
	this->Ammo = NONE;
	this->Fist = true;

	// TODO(fusion): Check if `Master` is NULL?
	TCreature *Master = this->Master;
	if(!Master->CrObject.exists()){
		error("TCombat::GetWeapon: Kreatur-Objekt existiert nicht.\n");
		return;
	}

	// TODO(fusion): We're iterating over left and right hand slots. Make
	// it clearer perhaps.
	for(int Hand = 5; Hand <= 6; Hand += 1){
		Object Obj = GetBodyObject(Master->ID, Hand);
		if(Obj == NONE){
			continue;
		}

		ObjectType ObjType = Obj.getObjectType();

		if(ObjType.getFlag(RESTRICTLEVEL)){
			int CurrentLevel = Master->Skills[SKILL_LEVEL]->Get();
			int MinimumLevel = (int)ObjType.getAttribute(MINIMUMLEVEL);
			if(CurrentLevel < MinimumLevel){
				continue;
			}
		}

		if(ObjType.getFlag(RESTRICTPROFESSION) && Master->Type == PLAYER){
			uint32 ProfessionMask = ObjType.getAttribute(PROFESSIONS);
			uint8 Profession = ((TPlayer*)Master)->GetEffectiveProfession();
			if((ProfessionMask & (1 << Profession)) == 0){
				continue;
			}
		}

		if(ObjType.getFlag(SHIELD)){
			this->Shield = Obj;
		}

		if(ObjType.getFlag(WEAPON)){
			this->Close = Obj;
			this->Fist = false;
		}

		if(ObjType.getFlag(BOW)){
			this->Missile = Obj;
			this->Fist = false;
		}

		if(ObjType.getFlag(THROW)){
			this->Throw = Obj;
			this->Fist = false;
		}

		if(ObjType.getFlag(WAND)){
			this->Wand = Obj;
			this->Fist = false;
		}
	}
}

void TCombat::GetAmmo(void){
	if(this->Missile == NONE){
		if(this->Throw != NONE){
			this->Ammo = this->Throw;
		}else if(this->Wand != NONE){
			this->Ammo = this->Wand;
		}
		return;
	}

	// TODO(fusion): Check if `Master` is NULL?
	Object Ammo = GetBodyObject(this->Master->ID, 10);
	this->Ammo = NONE;
	if(Ammo != NONE){
		ObjectType AmmoType = Ammo.getObjectType();
		if(AmmoType.getFlag(AMMO)){
			ObjectType BowType = this->Missile.getObjectType();
			if(AmmoType.getAttribute(AMMOTYPE) == BowType.getAttribute(BOWAMMOTYPE)){
				this->Ammo = Ammo;
			}
		}
	}
}

void TCombat::CheckCombatValues(void){
	Object OldShield = this->Shield;
	Object OldClose = this->Close;
	Object OldMissile = this->Missile;
	Object OldThrow = this->Throw;
	Object OldWand = this->Wand;
	Object OldAmmo = this->Ammo;
	bool OldFist = this->Fist;

	this->GetWeapon();
	this->GetAmmo();
	if(OldShield != this->Shield
			|| OldClose != this->Close
			|| OldMissile != this->Missile
			|| OldThrow != this->Throw
			|| OldWand != this->Wand
			|| OldAmmo != this->Ammo
			|| OldFist != this->Fist){
		this->DelayAttack(2000);
	}
}

int TCombat::GetDistance(void){
	int Distance = 0;
	if(this->Close != NONE || this->Fist){
		Distance = 1;
	}else if(this->Throw != NONE){
		Distance = 2;
	}else if(this->Missile != NONE || this->Wand != NONE){
		Distance = 3;
	}
	return Distance;
}

void TCombat::SetAttackDest(uint32 TargetID, bool Follow){
	if(this->AttackDest == TargetID && this->Following == Follow){
		return;
	}

	TCreature *Master = this->Master;
	if(TargetID == 0 || TargetID == Master->ID){
		this->StopAttack(0);
		return;
	}

	TCreature *Target = GetCreature(TargetID);
	if(Target == NULL){
		this->StopAttack(0);
		return;
	}

	if(!Follow){
		if(Master->Type == PLAYER && Target->Type == PLAYER){
			if(this->SecureMode == 1 && WorldType == NORMAL
			&& !((TPlayer*)Master)->IsAttackJustified(TargetID)){
				this->StopAttack(0);
				throw SECUREMODE;
			}
		}

		if(Master->Type == PLAYER && !CheckRight(Master->ID, ATTACK_EVERYWHERE)){
			if(IsProtectionZone(Master->posx, Master->posy, Master->posz)
			|| IsProtectionZone(Target->posx, Target->posy, Target->posz)){
				this->StopAttack(0);
				throw PROTECTIONZONE;
			}
		}

		if(Master->Type == PLAYER && CheckRight(Master->ID, NO_ATTACK)){
			this->StopAttack(0);
			throw ATTACKNOTALLOWED;
		}

		if(Master->Type == PLAYER && Target->Type == PLAYER){
			if(((TPlayer*)Master)->GetRealProfession() == PROFESSION_NONE
					&& !CheckRight(Master->ID, ATTACK_EVERYWHERE)){
				this->StopAttack(0);
				throw ATTACKNOTALLOWED;
			}
		}

		if(WorldType == NON_PVP){
			if(!Master->IsPeaceful() || !Target->IsPeaceful()){
				if(Target->Type == NPC){
					this->StopAttack(0);
					throw ATTACKNOTALLOWED;
				}
			}

			if(Master->Type == PLAYER){
				if(Target->Type == NPC || !CheckRight(Master->ID, ATTACK_EVERYWHERE)){
					this->StopAttack(0);
					throw ATTACKNOTALLOWED;
				}
			}
		}
	}

	if(Master->Type == PLAYER && Target->Type != PLAYER){
		if(Target->Outfit.OutfitID == 0 && Target->Outfit.ObjectType == 0){
			this->StopAttack(0);
			throw TARGETLOST;
		}
	}

	int Distance = ObjectDistance(Master->CrObject, Target->CrObject);
	if(Distance > 8){
		this->StopAttack(0);
		throw TARGETLOST;
	}

	this->AttackDest = TargetID;
	this->Following = Follow;
	if(!Follow){
		Target->AttackStimulus(Master->ID);
		Master->BlockLogout(60, Target->Type == PLAYER);
		if(Master->Type == PLAYER && Target->Type == PLAYER){
			((TPlayer*)Master)->RecordAttack(TargetID);
		}
		this->LatestAttackTime = 0;
	}
}

void TCombat::CanToDoAttack(void){
	if(this->AttackDest == 0){
		return;
	}

	// TODO(fusion): There is some `CanAttack` function inlined here.
	TCreature *Master = this->Master;
	if(Master == NULL){
		error("TCombat::CanAttack: Kein Master gesetzt!\n");
		throw ERROR;
	}

	TCreature *Target = GetCreature(this->AttackDest);
	if(Target == NULL){
		this->StopAttack(0);
		throw TARGETLOST;
	}

	if(Master->Type == PLAYER && Target->Type != PLAYER){
		if(Target->Outfit.OutfitID == 0 && Target->Outfit.ObjectType == 0){
			this->StopAttack(0);
			throw TARGETLOST;
		}
	}

	if(!this->Following){
		if(Master->Type == PLAYER && Target->Type == PLAYER){
			if(this->SecureMode == 1 && WorldType == NORMAL
			&& !((TPlayer*)Master)->IsAttackJustified(Target->ID)){
				this->StopAttack(0);
				throw SECUREMODE;
			}
		}

		if(WorldType == NON_PVP){
			if(Master->IsPeaceful() && Target->IsPeaceful()){
				if(Master->Type != PLAYER || !CheckRight(Master->ID, ATTACK_EVERYWHERE)){
					this->StopAttack(0);
					throw ATTACKNOTALLOWED;
				}
			}
		}
	}

	int Distance = ObjectDistance(Master->CrObject, Target->CrObject);
	if(Distance > 8){
		this->StopAttack(0);
		throw TARGETLOST;
	}

	int ChaseMode = this->ChaseMode;
	if(this->Following){
		ChaseMode = CHASE_MODE_CLOSE;
	}

	if(ChaseMode == CHASE_MODE_CLOSE){
		if(Distance > 1){
			Master->ToDoGo(Target->posx, Target->posy, Target->posz, false, 3);
		}
	}else if(ChaseMode == CHASE_MODE_RANGE){
		if(Distance > 4){
			Master->ToDoGo(Target->posx, Target->posy, Target->posz, false, Distance - 4);
		}else if(Distance < 4){
			int DestX, DestY, DestZ;
			if(SearchFlightField(Master->ID, Target->ID, &DestX, &DestY, &DestZ)){
				Master->ToDoGo(DestX, DestY, DestZ, true, -1);
			}
		}
	}
}

void TCombat::Attack(void){
	if(this->AttackDest == 0 || this->Following){
		return;
	}

	// TODO(fusion): There is some `CanAttack` function inlined here.
	TCreature *Master = this->Master;
	if(Master == NULL){
		error("TCombat::CanAttack: Kein Master gesetzt!\n");
		throw ERROR;
	}

	TCreature *Target = GetCreature(this->AttackDest);
	if(Target == NULL){
		this->StopAttack(0);
		throw TARGETLOST;
	}

	// NOTE(fusion): This probably has something to do with calling `StopAttack`
	// with a non zero delay.
	if(this->LatestAttackTime != 0 && this->LatestAttackTime < RoundNr){
		this->StopAttack(0);
		return;
	}

	if(Master->Type == PLAYER && Target->Type != PLAYER){
		if(Target->Outfit.OutfitID == 0 && Target->Outfit.ObjectType == 0){
			this->StopAttack(0);
			throw TARGETLOST;
		}
	}

	if(Master->Type == PLAYER && Target->Type == PLAYER){
		if(this->SecureMode == 1 && WorldType == NORMAL
		&& !((TPlayer*)Master)->IsAttackJustified(Target->ID)){
			this->StopAttack(0);
			throw SECUREMODE;
		}
	}

	// TODO(fusion): It is weird that max attack distance is hardcoded.
	// Actually, it is related to the maximum visible distance on a
	// creature's viewport.
	int Distance = ObjectDistance(Master->CrObject, Target->CrObject);
	if(Distance > 8){
		this->StopAttack(0);
		throw TARGETLOST;
	}

	if(Master->Type == PLAYER && Target->Type == PLAYER){
		if(((TPlayer*)Master)->GetRealProfession() == PROFESSION_NONE
				&& !CheckRight(Master->ID, ATTACK_EVERYWHERE)){
			this->StopAttack(0);
			throw ATTACKNOTALLOWED;
		}
	}

	if(Master->Type == PLAYER){
		if(CheckRight(Master->ID, NO_ATTACK)){
			this->StopAttack(0);
			throw ATTACKNOTALLOWED;
		}
	}

	if(IsProtectionZone(Master->posx, Master->posy, Master->posz)
	|| IsProtectionZone(Target->posx, Target->posy, Target->posz)){
		this->StopAttack(0);
		throw PROTECTIONZONE;
	}

	Master->BlockLogout(60, Target->Type == PLAYER);
	Target->BlockLogout(60, false);

	if(Master->Type == PLAYER && Target->Type == PLAYER){
		((TPlayer*)Master)->RecordAttack(Target->ID);
	}

	this->DelayAttack(200);

	// TODO(fusion): This `Range` value doesn't make a lot of sense.
	int Range = this->GetDistance();
	if(Range == 1){
		if(Distance > 1){
			throw TARGETOUTOFRANGE;
		}
		this->CloseAttack(Target);
	}else{
		if(Range < 2 || Range > 3){
			throw ERROR;
		}

		// TODO(fusion): These are related to the maximum visible distance on
		// each coordinate.
		if(std::abs(Master->posx - Target->posx) > 7
		|| std::abs(Master->posy - Target->posy) > 5){
			throw TARGETOUTOFRANGE;
		}

		// NOTE(fusion): Originally, this was a single function `RangeAttack`
		// and I quickly realized that readability would be massively improved
		// by splitting it into `DistanceAttack` and `WandAttack`.
		if(this->Missile != NONE || this->Throw != NONE){
			this->DistanceAttack(Target);
		}else if(this->Wand != NONE){
			this->WandAttack(Target);
		}else{
			throw ERROR;
		}
	}

	this->DelayAttack(2000);

	if(Target->IsDead){
		this->StopAttack(0);
	}
}

void TCombat::StopAttack(int Delay){
	if(Delay == 0){
		this->AttackDest = 0;
		if(this->Master->Type == PLAYER){
			SendClearTarget(this->Master->Connection);
		}
	}else{
		this->LatestAttackTime = RoundNr + Delay;
	}
}

void TCombat::DelayAttack(int Milliseconds){
	uint32 EarliestAttackTime = ServerMilliseconds + Milliseconds;
	if(this->EarliestAttackTime < EarliestAttackTime){
		this->EarliestAttackTime = EarliestAttackTime;
	}
}

void TCombat::CloseAttack(TCreature *Target){
	int Attack;
	uint16 SkillNr;
	this->GetAttackValue(&Attack, &SkillNr);
	if(this->AttackMode == ATTACK_MODE_OFFENSIVE){
		Attack += (Attack * 2) / 10;
	}else if(this->AttackMode == ATTACK_MODE_DEFENSIVE){
		Attack -= (Attack * 4) / 10;
	}

	TCreature *Master = this->Master;
	Attack = Master->Skills[SkillNr]->ProbeValue(Attack, this->LearningPoints > 0);
	if(this->LearningPoints > 0){
		this->LearningPoints -= 1;
	}

	int Defense = Target->Combat.GetDefendDamage();
	int Damage = Attack - Defense;
	if(Damage < 0){
		Damage = 0;
	}

	int DamageDone = Target->Damage(Master, Damage, 1); // DAMAGE_PHYSICAL ?
	if(DamageDone > 0){
		this->LearningPoints = 30;
	}

	int Poison = GetRacePoison(Master->Race);
	if(Poison != 0){
		if(DamageDone > 0 || (Attack > Defense && (rand() % 5) == 0)){
			int PoisonDamage = random(Poison / 2, Poison);
			if(PoisonDamage > 0){
				Target->Damage(Master, PoisonDamage, DAMAGE_POISON_PERIODIC);
				if(Target->Type == PLAYER){
					SendMessage(Target->Connection, TALK_STATUS_MESSAGE, "You are poisoned.");
				}
			}
		}
	}

	Object Close = this->Close;
	if(Close != NONE){
		ObjectType CloseType = Close.getObjectType();
		if(CloseType.getFlag(WEAROUT)){
			uint32 RemainingUses = Close.getAttribute(REMAININGUSES);
			if(RemainingUses > 1){
				Change(Close, REMAININGUSES, RemainingUses - 1);
			}else{
				ObjectType WearOutType = CloseType.getAttribute(WEAROUTTARGET);
				Change(Close, WearOutType, 0);
				this->CheckCombatValues();
			}
		}
	}
}

void TCombat::WandAttack(TCreature *Target){
	ASSERT(this->Wand != NONE);

	TCreature *Master = this->Master;
	int Distance = std::min<int>(
			std::abs(Master->posx - Target->posx),
			std::abs(Master->posy - Target->posy));

	ObjectType WandType = this->Wand.getObjectType();
	if(Distance > (int)WandType.getAttribute(WANDRANGE)){
		throw TARGETOUTOFRANGE;
	}

	if(!ThrowPossible(Master->posx, Master->posy, Master->posz,
				Target->posx, Target->posy, Target->posz, 0)){
		throw TARGETHIDDEN;
	}

	int DamageType = WandType.getAttribute(WANDDAMAGETYPE);
	int AnimType = WandType.getAttribute(WANDMISSILE);
	int ManaConsumption = WandType.getAttribute(WANDMANACONSUMPTION);
	int AttackStrength = WandType.getAttribute(WANDATTACKSTRENGTH);
	int AttackVariation = (int)WandType.getAttribute(WANDATTACKVARIATION);

	// NOTE(fusion): Oof...
	try{
		CheckMana(Master, ManaConsumption, 0, 0);
	}catch(RESULT err){
		if(err == NOTENOUGHMANA){
			throw OUTOFAMMO;
		}else{
			throw err;
		}
	}

	int Damage = AttackStrength + random(-AttackVariation, AttackVariation);
	if(Target->Damage(Master, Damage, DamageType) > 0){
		this->LearningPoints = 30;
	}

	::Missile(Master->CrObject, Target->CrObject, AnimType);
}

void TCombat::DistanceAttack(TCreature *Target){
	ASSERT(this->Missile != NONE || this->Throw != NONE);

	int HitChance;
	int DamageType;
	int AnimType;
	int Fragility;
	int SpecialEffect;
	int EffectStrength;

	TCreature *Master = this->Master;
	int DistanceX = std::abs(Master->posx - Target->posx);
	int DistanceY = std::abs(Master->posy - Target->posy);
	int Distance = std::max<int>(DistanceX, DistanceY);

	if(this->Missile != NONE){
		this->GetAmmo();
		if(this->Ammo == NONE || !this->Ammo.exists()){
			throw OUTOFAMMO;
		}

		ObjectType BowType = this->Missile.getObjectType();
		ObjectType AmmoType = this->Ammo.getObjectType();
		if(Distance > (int)BowType.getAttribute(BOWRANGE)){
			throw TARGETOUTOFRANGE;
		}

		HitChance = 90;
		DamageType = DAMAGE_PHYSICAL;
		AnimType = AmmoType.getAttribute(AMMOMISSILE);
		Fragility = 100;
		SpecialEffect = AmmoType.getAttribute(AMMOSPECIALEFFECT);
		EffectStrength = AmmoType.getAttribute(AMMOEFFECTSTRENGTH);
	}else{
		ASSERT(this->Throw != NONE);
		ObjectType ThrowType = this->Throw.getObjectType();
		if(Distance > (int)ThrowType.getAttribute(THROWRANGE)){
			throw TARGETOUTOFRANGE;
		}

		HitChance = 75;
		DamageType = DAMAGE_PHYSICAL;
		AnimType = ThrowType.getAttribute(THROWMISSILE);
		Fragility = ThrowType.getAttribute(THROWFRAGILITY);
		SpecialEffect = ThrowType.getAttribute(THROWSPECIALEFFECT);
		EffectStrength = ThrowType.getAttribute(THROWEFFECTSTRENGTH);
	}

	if(!ThrowPossible(Master->posx, Master->posy, Master->posz,
				Target->posx, Target->posy, Target->posz, 0)){
		throw TARGETHIDDEN;
	}

	int Difficulty = (Distance >= 2) ? Distance : 5;
	bool Hit = Master->Skills[SKILL_DISTANCE]->Probe(
			Difficulty * 15, HitChance, this->LearningPoints > 0);
	if(this->LearningPoints > 0){
		this->LearningPoints -= 1;
	}

	int DropX = Target->posx;
	int DropY = Target->posy;
	int DropZ = Target->posz;
	if(Hit){
		int Attack;
		uint16 SkillNr;
		this->GetAttackValue(&Attack, &SkillNr);
		if(this->AttackMode == ATTACK_MODE_OFFENSIVE){
			Attack += (Attack * 2) / 10;
		}else if(this->AttackMode == ATTACK_MODE_DEFENSIVE){
			Attack -= (Attack * 4) / 10;
		}

		Attack = Master->Skills[SkillNr]->ProbeValue(Attack, this->LearningPoints > 0);
		if(this->LearningPoints > 0){
			this->LearningPoints -= 1;
		}

		// TODO(fusion): This doesn't make any sense. The disassembly looks
		// correct but I feel we're missing something here. Or maybe defense
		// doesn't work with ranged attacks but worn them out?
		if(this->Shield != NONE){
			Target->Combat.GetDefendDamage();
		}

		if(Target->Damage(Master, Attack, DamageType) > 0){
			this->LearningPoints = 30;
		}
	}else{
		if(DistanceX > 1 || DistanceY > 1){
			DropX += (rand() % 3) - 1;
			DropY += (rand() % 3) - 1;
		}

		if(!CoordinateFlag(DropX, DropY, DropZ, BANK)
		|| CoordinateFlag(DropX, DropY, DropZ, UNLAY)
		|| !ThrowPossible(Master->posx, Master->posy, Master->posz, DropX, DropY, DropZ, 0)){
			DropX = Target->posx;
			DropY = Target->posy;
		}
	}

	Object DropCon = GetMapContainer(DropX, DropY, DropZ);
	::Missile(Master->CrObject, DropCon, AnimType);

	if(SpecialEffect == 1){ // POISON ARROW
		if(Hit){
			Target->Damage(Master, EffectStrength, DAMAGE_POISON_PERIODIC);
		}
	}else if(SpecialEffect == 2){ // BURST ARROW
		int Damage = ComputeDamage(Master, 0, EffectStrength, EffectStrength);
		TDamageImpact Impact(Master, DAMAGE_PHYSICAL, Damage, false);
		CircleShapeSpell(Master, DropX, DropY, DropZ, -1,
				ANIMATION_NONE, 2, &Impact, EFFECT_BURST_ARROW);
	}

	if(random(0, 99) < Fragility){
		Delete(this->Ammo, 1);
	}else{
		Move(0, this->Ammo, DropCon, 1, false, NONE);
	}

	if(!Hit){
		GraphicalEffect(DropX, DropY, DropZ, EFFECT_POFF);
	}
}

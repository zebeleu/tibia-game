#include "creature.hh"
#include "player.hh"
#include "config.hh"

#include "stubs.hh"

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

void TCombat::Attack(void){
	if(this->AttackDest == 0 || this->Following){
		return;
	}

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
		if(Range < 1 || Range > 3){
			throw ERROR;
		}

		// TODO(fusion): These are related to the maximum visible distance on
		// each coordinate.
		if(std::abs(Master->posx - Target->posx) > 7
		|| std::abs(Master->posy - Target->posy) > 5){
			throw TARGETOUTOFRANGE;
		}

		this->RangeAttack(Target);
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

#include "creature.hh"
#include "monster.hh"
#include "enums.hh"

#include "stubs.hh"

TCreature::TCreature(void) :
		TSkillBase(),
		Combat(),
		ToDoList(0, 20, 10)
{
	this->Combat.Master = this;
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

void TCreature::Attack(void){
	this->Combat.Attack();
}

// TODO(fusion): This probably belongs elsewhere but we should come back to
// this when we're wrapping up creature files.
static bool IsCreaturePlayer(uint32 CreatureID){
	return CreatureID < 0x40000000;
}

// TODO(fusion): This probably belongs elsewhere but we should come back to
// this when we're wrapping up creature files.
static void AddKillStatistics(int AttackerRace, int DefenderRace){
	// NOTE(fusion): I think the race name can be "human" only for players,
	// which means we're probably tracking how many creatures are killed by
	// players with `KilledCreatures`, and how many players are killed by
	// creatures with `KilledPlayers`.

	if(strcmp(RaceData[AttackerRace].Name, "human") == 0){
		KilledCreatures[DefenderRace] += 1;
	}

	// NOTE(fusion): This seems to track how many players are killed by
	if(strcmp(RaceData[DefenderRace].Name, "human") == 0){
		KilledPlayers[AttackerRace] += 1;
	}
}

int TCreature::Damage(TCreature *Attacker, int Damage, int DamageType){
	if(this->IsDead || this->Type == NPC){
		return 0;
	}

	if(this->Type == PLAYER){
		if(this->Connection != NULL && Attacker != NULL){
			SendMarkCreature(this->Connection, Attacker->ID, COLOR_BLACK);
		}

		if(Attacker != NULL && Attacker->Type == PLAYER
				&& DamageType != DAMAGE_POISON_PERIODIC
				&& DamageType != DAMAGE_FIRE_PERIODIC
				&& DamageType != DAMAGE_ENERGY_PERIODIC){
			Damage = (Damage + 1) / 2;
		}
	}

	// NOTE(fusion): `Responsible` is either the attacker or its master. It is
	// always valid if `Attacker` is not NULL.
	TCreature *Responsible = Attacker;
	if(Attacker != NULL){
		uint32 MasterID = Attacker->GetMaster();
		if(MasterID != 0){
			Responsible = GetCreature(MasterID);
		}

		Attacker->BlockLogout(60, this->Type == PLAYER);
		if(Responsible != Attacker){
			Responsible->BlockLogout(60, this->Type == PLAYER);
		}

		if(this->Type == PLAYER && Responsible->Type == PLAYER){
			((TPlayer*)Responsible)->RecordAttack(this->ID);
		}
	}

	if(this->Type == PLAYER){
		if(CheckRight(this->ID, INVULNERABLE)){
			Damage = 0;
		}

		// NOTE(fusion): We're iterating over the victim's inventory to apply
		// damage reductions, while the damage is greater than zero.
		for(int Position = 1;
				Position <= 10 && Damage > 0;
				Position += 1){
			Object Obj = GetBodyObject(this->ID, Position);
			if(Obj == NONE){
				continue;
			}

			ObjectType ObjType = Obj.getObjectType();
			if(ObjType.getFlag(PROTECTION) && ObjType.getFlag(CLOTHES)
					&& (int)ObjType.getAttribute(BODYPOSITION) == Position
					&& (ObjType.getAttribute(PROTECTIONDAMAGETYPES) & DamageType) != 0){
				int DamageReduction = ObjType.getAttribute(DAMAGEREDUCTION);
				Damage = (Damage * (100 - DamageReduction)) / 100;
				if(ObjType.getFlag(WEAROUT)){
					// TODO(fusion): Ugh... The try..catch block might be used only
					// when changing the object's type.
					try{
						uint32 RemainingUses = Obj.getAttribute(REMAININGUSES);
						if(RemainingUses > 1){
							Change(Obj, REMAININGUSES, RemainingUses - 1);
						}else{
							ObjectType WearOutType = ObjType.getAttribute(WEAROUTTARGET);
							Change(Obj, WearOutType, 0);
						}
					}catch(RESULT err){
						error("TCreature::Damage: Exception %d beim Abnutzen von Objekt %d.\n",
								err, ObjType.TypeID);
					}
				}
			}else if(ObjType.getFlag(PROTECTION) && !ObjType.getFlag(CLOTHES)){
				error("TCreature::Damage: Objekt %d hat PROTECTION, aber nicht CLOTHES.\n",
						ObjType.TypeID);
			}
		}
	}

	if(Damage <= 0){
		GraphicalEffect(this->CrObject, EFFECT_POFF);
		return 0;
	}

	if(DamageType == DAMAGE_POISON_PERIODIC){
		if(RaceData[this->Race].NoPoison){
			return 0;
		}

		if(Damage > this->Skills[SKILL_POISON]->TimerValue()){
			this->PoisonDamageOrigin = Attacker->ID;
			this->SetTimer(SKILL_POISON, Damage, 3, 3, -1);
		}

		this->DamageStimulus(Attacker->ID, Damage, DamageType);
		return Damage;
	}else if(DamageType == DAMAGE_FIRE_PERIODIC){
		if(RaceData[this->Race].NoBurning){
			return 0;
		}

		this->FireDamageOrigin = Attacker->ID;
		this->SetTimer(SKILL_BURNING, Damage / 10, 8, 8, -1);
		this->DamageStimulus(Attacker->ID, Damage, DamageType);
		return Damage;
	}else if(DamageType == DAMAGE_ENERGY_PERIODIC){
		if(RaceData[this->Race].NoEnergy){
			return 0;
		}

		// TODO(fusion): Shouldn't we use `Damage / 25` here?
		this->EnergyDamageOrigin = Attacker->ID;
		this->SetTimer(SKILL_ENERGY, Damage / 20, 10, 10, -1);
		this->DamageStimulus(Attacker->ID, Damage, DamageType);
		return Damage;
	}

	if((DamageType == DAMAGE_PHYSICAL && RaceData[this->Race].NoHit)
	|| (DamageType == DAMAGE_POISON && RaceData[this->Race].NoPoison)
	|| (DamageType == DAMAGE_FIRE && RaceData[this->Race].NoBurning)
	|| (DamageType == DAMAGE_ENERGY && RaceData[this->Race].NoEnergy)
	|| (DamageType == DAMAGE_LIFEDRAIN && RaceData[this->Race].NoLifeDrain)){
		GraphicalEffect(this->CrObject, EFFECT_BLOCK_HIT);
		return 0;
	}

	if(DamageType == DAMAGE_PHYSICAL){
		Damage -= this->Combat.GetArmorStrength();
		if(Damage <= 0){
			GraphicalEffect(this->CrObject, EFFECT_BLOCK_HIT);
			return 0;
		}
	}

	this->DamageStimulus(Attacker->ID, Damage, DamageType);

	// NOTE(fusion): Remove non-player illusion. Might as well be an inlined
	// function.
	if(this->Type != PLAYER
			&& this->Outfit.OutfitID == 0
			&& this->Outfit.ObjectType == 0){
		this->SetTimer(SKILL_ILLUSION, 0, 0, 0, -1);
		this->Outfit = this->OrgOutfit;
		AnnounceChangedCreature(this->ID, 3); // CREATURE_OUTFIT_CHANGED ?
		NotifyAllCreatures(this->CrObject, 2, NONE); // CREATURE_APPEAR ?
	}

	if(DamageType == DAMAGE_MANADRAIN){
		int ManaPoints = this->Skills[SKILL_MANA]->Get();
		if(Damage > ManaPoints){
			Damage = ManaPoints;
		}

		if(Damage > 0){
			this->Skills[SKILL_MANA]->Change(-Damage);
			if(this->Type == PLAYER && this->Connection != NULL){
				SendMessage(this->Connection, TALK_STATUS_MESSAGE,
						"You lose %d mana.", Damage);
			}
			GraphicalEffect(this->CrObject, EFFECT_MAGIC_RED);
			TextualEffect(this->CrObject, COLOR_BLUE, "%d", Damage);
		}

		return Damage;
	}

	if(this->Skills[SKILL_MANASHIELD]->TimerValue() > 0
			|| this->Skills[SKILL_MANASHIELD]->Get() > 0){
		// NOTE(fusion): We only send these if the attack was fully absorbed,
		// else it'd be overwritten by whatever effect and messages we send
		// next, when the victim's hitpoints are actually touched.
		int ManaPoints = this->Skills[SKILL_MANA]->Get();
		if(Damage <= ManaPoints){
			this->Skills[SKILL_MANA]->Change(-Damage);
			GraphicalEffect(this->CrObject, EFFECT_MANA_HIT);
			TextualEffect(this->CrObject, COLOR_BLUE, "%d", Damage);
			if(this->Type == PLAYER && this->Connection != NULL){
				if(Attacker != NULL){
					SendMessage(this->Connection, TALK_STATUS_MESSAGE,
							"You lose %d mana blocking an attack by %s.",
							Damage, Attacker->Name);
				}else{
					SendMessage(this->Connection, TALK_STATUS_MESSAGE,
							"You lose %d mana.", Damage);
				}
				SendPlayerData(this->Connection);
			}
			return Damage;
		}

		this->Skills[SKILL_MANA]->Set(0);
		Damage -= ManaPoints;
	}

	int HitPoints = this->Skills[SKILL_HITPOINTS]->Get();
	if(Damage > HitPoints){
		Damage = HitPoints;
	}

	this->Skills[SKILL_HITPOINTS]->Change(-Damage);
	if(Responsible != NULL){
		ASSERT(Attacker != NULL);
		if(Responsible == Attacker){
			this->Combat.AddDamageToCombatList(Attacker->ID, Damage);
		}else{
			this->Combat.AddDamageToCombatList(Attacker->ID, Damage / 2);
			this->Combat.AddDamageToCombatList(Responsible->ID, Damage / 2);
		}
	}

	int HitEffect = EFFECT_NONE;
	int TextColor = COLOR_BLACK;
	int SplashLiquid = LIQUID_NONE;
	if(DamageType == DAMAGE_PHYSICAL){
		switch(RaceData[this->Race].Blood){
			case BT_BLOOD:{
				HitEffect = EFFECT_BLOOD_HIT;
				TextColor = COLOR_RED;
				SplashLiquid = LIQUID_BLOOD;
				break;
			}
			case BT_SLIME:{
				HitEffect = EFFECT_POISON_HIT;
				TextColor = COLOR_LIGHTGREEN;
				SplashLiquid = LIQUID_SLIME;
				break;
			}
			case BT_BONES:{
				HitEffect = EFFECT_BONE_HIT;
				TextColor = COLOR_LIGHTGRAY;
				break;
			}
			case BT_FIRE:{
				HitEffect = EFFECT_FIRE_HIT;
				TextColor = COLOR_ORANGE;
				break;
			}
			case BT_ENERGY:{
				HitEffect = EFFECT_ENERGY_HIT;
				TextColor = COLOR_LIGHTBLUE;
				break;
			}
			default:{
				error("TCreature::Damage: Ungültiger Bluttyp %d für Rasse %d.\n",
						RaceData[this->Race].Blood, this->Race);
				break;
			}
		}
	}else if(Damage == DAMAGE_POISON){
		HitEffect = EFFECT_POISON;
		TextColor = COLOR_LIGHTGREEN;
	}else if(DamageType == DAMAGE_FIRE){
		HitEffect = EFFECT_FIRE_HIT;
		TextColor = COLOR_ORANGE;
	}else if(DamageType == DAMAGE_ENERGY){
		HitEffect = EFFECT_ENERGY_HIT;
		TextColor = COLOR_LIGHTBLUE;
	}else if(DamageType == DAMAGE_LIFEDRAIN){
		HitEffect = EFFECT_MAGIC_RED;
		TextColor = COLOR_RED;
	}else{
		// TODO(fusion): The original decompiled function would return here but
		// I don't think it's a good idea because it would skip death handling.
		error("TCreature::Damage: Ungültiger Schadenstyp %d.\n", DamageType);
		//return Damage;
	}

	if(HitEffect != EFFECT_NONE){
		GraphicalEffect(this->CrObject, HitEffect);
		TextualEffect(this->CrObject, TextColor, "%d", Damage);
		if(SplashLiquid != LIQUID_NONE){
			CreatePool(GetMapContainer(this->CrObject),
						GetSpecialObject(BLOOD_SPLASH),
						SplashLiquid);
		}
	}

	if(this->Type == PLAYER && this->Connection != NULL){
		if(Attacker != NULL){
			SendMessage(this->Connection, TALK_STATUS_MESSAGE,
					"You lose %d hitpoint%s due to an attack by %s.",
					Damage, (Damage == 1 ? "" : "s"), Attacker->Name);
		}else{
			SendMessage(this->Connection, TALK_STATUS_MESSAGE,
					"You lose %d hitpoint%s.",
					Damage, (Damage == 1 ? "" : "s"));
		}
		SendPlayerData(this->Connection);
	}

	if(Damage == HitPoints){
		if(this->Type == PLAYER){
			for(int Position = 1; Position <= 10; Position += 1){
				Object Obj = GetBodyObject(this->ID, Position);
				if(Obj == NONE){
					continue;
				}

				ObjectType ObjType = Obj.getObjectType();
				if(ObjType.getFlag(CLOTHES) && (int)ObjType.getAttribute(BODYPOSITION) == Position){
					ObjectType AmuletOfLossType = GetNewObjectType(77, 12);
					if(ObjType == AmuletOfLossType){
						Log("game", "%s stirbt mit Amulett of Loss.\n", this->Name);
						this->LoseInventory = 0;
						Delete(Obj, -1);
						// TODO(fusion): Shouldn't we break here? We could also
						// just check if there is an amulet of loss in the necklace
						// container instead of iterating over all of them.
					}
				}
			}
		}

		int OldLevel = this->Skills[SKILL_LEVEL]->Get();
		this->Death();
		if(Attacker != NULL && this->Type == PLAYER){
			Attacker->BlockLogout(900, true);
			if(Responsible != Attacker){
				Responsible->BlockLogout(900, true);
			}
		}

		uint32 MurdererID = 0;
		char Remark[30] = {};
		if(Attacker == NULL){
			AddKillStatistics(0, this->Race);
			if(DamageType == DAMAGE_PHYSICAL){
				strcpy(Remark, "a hit");
			}else if(DamageType == DAMAGE_POISON){
				strcpy(Remark, "poison");
			}else if(DamageType == DAMAGE_FIRE){
				strcpy(Remark, "fire");
			}else if(DamageType == DAMAGE_ENERGY){
				strcpy(Remark, "energy");
			}else{
				// NOTE(fusion): We probably don't expect any other damage type
				// as the cause of death when there is no attacker.
				error("TCreature::Damage: Ungültiger Schadenstyp %d als Todesursache.\n", DamageType);
			}
		}else{
			AddKillStatistics(Attacker->Race, this->Race);
			strcpy(this->Murderer, Attacker->Name);

			if(Responsible->Type == PLAYER){
				MurdererID = Responsible->ID;
			}

			if(Attacker->Type != PLAYER){
				strcpy(Remark, Attacker->Name);
			}
		}

		if(this->Type == PLAYER){
			if(MurdererID != 0 && MurdererID != this->ID){
				bool Justified = true;
				if(IsCreaturePlayer(MurdererID)){
					TPlayer *Murderer = GetPlayer(MurdererID);
					if(Murderer != NULL){
						Justified = Murderer->IsAttackJustified(this->ID);
						Murderer->RecordMurder(this->ID);
					}
				}
				CharacterDeathOrder(this, OldLevel, MurdererID, Remark, !Justified);
			}

			uint32 MostDangerousID = this->Combat.GetMostDangerousAttacker();
			if(MostDangerousID != 0
					&& MostDangerousID != MurdererID
					&& MostDangerousID != this->ID
					&& IsCreaturePlayer(MostDangerousID)){
				bool Justified = true;
				TPlayer *MostDangerous = GetPlayer(MostDangerousID);
				if(MostDangerous != NULL){
					Justified = MostDangerous->IsAttackJustified(this->ID);
					MostDangerous->RecordMurder(this->ID);
				}

				// TODO(fusion): The original function is confusing at this point
				// but it seems correct that the remark is included only with the
				// murderer.
				CharacterDeathOrder(this, OldLevel, MostDangerousID, "", !Justified);
			}
		}
	}

	AnnounceChangedCreature(this->ID, 1); // CREATURE_HITPOINTS_CHANGED ?
	return Damage;
}

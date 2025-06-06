#include "cr.hh"
#include "config.hh"
#include "enums.hh"
#include "info.hh"

#include "stubs.hh"

TRaceData RaceData[MAX_RACES];
int KilledCreatures[MAX_RACES];
int KilledPlayers[MAX_RACES];

priority_queue<uint32, uint32> ToDoQueue(5000, 1000);
//priority_queue<uint32, TAttackWave*> AttackWaveQueue(100, 100);

static uint32 NextCreatureID;
static int FirstFreeCreature;
static TCreature *HashList[1000];
static matrix<uint32> *FirstChainCreature;
static vector<TCreature*> CreatureList(0, 10000, 1000);

// TCreature
// =============================================================================
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

TCreature::~TCreature(void){
	// TODO(fusion): Bruuhh... these exceptions...
	if(this->IsDead){
		int Race = this->Race;
		int PoolLiquid = LIQUID_NONE;
		if(RaceData[Race].Blood == BT_BLOOD){
			PoolLiquid = LIQUID_BLOOD;
		}else if(RaceData[Race].Blood == BT_SLIME){
			PoolLiquid = LIQUID_SLIME;
		}

		if(PoolLiquid != LIQUID_NONE){
			try{
				CreatePool(GetMapContainer(this->CrObject),
						GetSpecialObject(BLOOD_POOL),
						PoolLiquid);
			}catch(RESULT r){
				if(r != NOROOM && r != DESTROYED){
					error("TCreature::~TCreature: Kann Blutlache nicht setzen (Exc %d, Pos [%d,%d,%d]).\n",
							r, this->posx, this->posy, this->posz);
				}
			}
		}

		ObjectType CorpseType = (this->Sex == 1) // MALE ?
				? RaceData[Race].MaleCorpse
				: RaceData[Race].FemaleCorpse;

		if(CorpseType.getFlag(MAGICFIELD)){
			Object Obj = GetFirstObject(this->posx, this->posy, this->posz);
			while(Obj != NONE){
				Object Next = Obj.getNextObject();
				if(Obj.getObjectType().getFlag(MAGICFIELD)){
					try{
						Delete(Obj, -1);
					}catch(RESULT r){
						error("TCreature::~TCreature: Exception %d beim Löschen eines Feldes.\n", r);
					}
				}
				Obj = Next;
			}
		}

		try{
			Object Con = GetMapContainer(this->posx, this->posy, this->posz);
			Object Corpse = Create(Con, CorpseType, 0);
			Log("game", "Tod von %s: LoseInventory=%d.\n", this->Name, this->LoseInventory);

			if(this->Type == PLAYER){
				char Help[128];
				sprintf(Help, "You recognize %s", this->Name);
				if(this->Murderer[0] != 0){
					if(this->Sex == 1){ // MALE ?
						strcat(Help, ". He was killed by ");
					}else{
						strcat(Help, ". She was killed by ");
					}
					strcat(Help, this->Murderer);
				}
				Change(Corpse, TEXTSTRING, AddDynamicString(Help));
			}

			if(this->LoseInventory != 0){ // LOSE_INVENTORY_NONE ?
				for(int Position = 1; Position <= 10; Position += 1){
					Object Item = GetBodyObject(this->ID, Position);
					if(Item == NONE){
						continue;
					}

					if(this->LoseInventory != 2 // LOSE_INVENTORY_ALL ?
							&& !Item.getObjectType().getFlag(CONTAINER)
							&& random(0, 9) != 0){
						continue;
					}

					Move(0, Item, Corpse, -1, false, NONE);
				}
			}

			if(this->Type == PLAYER && this->LoseInventory != 2){ // LOSE_INVENTORY_ALL ?
				((TPlayer*)this)->SaveInventory();
			}
		}catch(RESULT r){
			error("TCreature::~TCreature: Kann Leiche/Inventory nicht ablegen (Exc %d, Pos [%d,%d,%d], %s).\n",
					r, this->posx, this->posy, this->posz, this->Name);
		}
	}

	if(this->CrObject != NONE && this->CrObject.exists()){
		this->DelOnMap();
	}

	this->ToDoClear();

	if(this->Type == PLAYER && this->Connection != NULL){
		this->Connection->Logout(30, true);
	}

	this->DelInCrList();

	if(this->ID != 0){
		this->DelID();
	}

	for(TKnownCreature *KnownCreature = this->FirstKnowingConnection;
			KnownCreature != NULL;
			KnownCreature = KnownCreature->Next){
		if(KnownCreature->CreatureID != this->ID){
			error("TCreature::~TCreature: Verkettungsfehler bei Kreatur %u.\n", this->ID);
		}
		KnownCreature->State = KNOWNCREATURE_FREE;
	}
}

void TCreature::SetID(uint32 CharacterID){
	if(this->ID != 0){
		error("TCreature::SetID: ID ist schon gesetzt.\n");
	}

	uint32 CreatureID = 0;
	if(CharacterID == 0){
		bool Found = false;
		for(int Attempts = 0; Attempts < 16; Attempts += 1){
			CreatureID = NextCreatureID++;
			if(GetCreature(CreatureID) == NULL){
				Found = true;
				break;
			}
		}

		if(!Found){
			error("TCreature::SetID: 16x hintereinander doppelte ID."
					" Verwende nun doppelte ID %d\n", CreatureID);
		}
	}else{
		CreatureID = CharacterID;
		if(GetCreature(CreatureID) != NULL){
			error("TCreature::SetID: Doppelte Character-ID %d gefunden.\n", CharacterID);
		}
	}

	uint32 ListIndex = CreatureID % NARRAY(HashList);
	this->ID = CreatureID;
	this->NextHashEntry = HashList[ListIndex];
	HashList[ListIndex] = this;
}

void TCreature::DelID(void){
	uint32 ListIndex = this->ID % NARRAY(HashList);
	TCreature *First = HashList[ListIndex];
	if(First == NULL){
		error("TCreature::DelID: Hasheintrag nicht gefunden id = %d\n", this->ID);
		return;
	}

	if(First->ID == this->ID){
		HashList[ListIndex] = this->NextHashEntry;
	}else{
		TCreature *Prev = First;
		TCreature *Current = First->NextHashEntry;
		while(true){
			if(Current == NULL){
				error("TCreature::DelID: id=%d nicht gefunden.\n", this->ID);
				return;
			}

			if(Current->ID == this->ID){
				Prev->NextHashEntry = this->NextHashEntry;
				break;
			}

			Prev = Current;
			Current = Current->NextHashEntry;
		}
	}
}

void TCreature::SetInCrList(void){
	*CreatureList.at(FirstFreeCreature) = this;
	FirstFreeCreature += 1;
}

void TCreature::DelInCrList(void){
	// TODO(fusion): See note in `ProcessCreatures`.
	for(int Index = 0; Index < FirstFreeCreature; Index += 1){
		TCreature **Current = CreatureList.at(Index);
		if(*Current == this){
			TCreature **Last = CreatureList.at(FirstFreeCreature - 1);
			*Current = *Last;
			*Last = NULL;
			FirstFreeCreature -= 1;

			// TODO(fusion): The original function wouldn't break here. Maybe it
			// is possible to have duplicates in `CreatureList`?
			//break;
		}
	}
}

void TCreature::StartLogout(bool Force, bool StopFight){
	this->LoggingOut = true;
	if(Force || LagDetected()){
		this->LogoutAllowed = true;
	}

	if(this->Type == PLAYER && this->Connection != NULL){
		this->Connection->Logout(0, true);
	}

	this->Combat.StopAttack(StopFight ? 0 : 60);
}

int TCreature::LogoutPossible(void){
	if(!this->LogoutAllowed && !this->IsDead && !GameEnding()){
		if(this->EarliestLogoutRound > RoundNr && !LagDetected()){
			return 1; // LOGOUT_COMBAT ?
		}

		if(IsNoLogoutField(this->posx, this->posy, this->posz)){
			return 2; // LOGOUT_FIELD ?
		}

		this->LogoutAllowed = true;
	}

	return 0; // LOGOUT_OK ?
}

void TCreature::BlockLogout(int Delay, bool BlockProtectionZone){
	if(WorldType == NON_PVP){
		BlockProtectionZone = false;
	}

	if(this->Type == PLAYER && !CheckRight(this->ID, NO_LOGOUT_BLOCK)){
		if(BlockProtectionZone || this->EarliestProtectionZoneRound > RoundNr){
			uint32 EarliestProtectionZoneRound = RoundNr + Delay;
			if(this->EarliestProtectionZoneRound < EarliestProtectionZoneRound){
				this->EarliestProtectionZoneRound = EarliestProtectionZoneRound;
			}
		}else if(this->Connection == NULL){
			// NOTE(fusion): This is a failsafe to avoid extending the earliest
			// logout round of a player that got disconnected in combat.
			return;
		}

		uint32 EarliestLogoutRound = RoundNr + Delay;
		if(this->EarliestLogoutRound < EarliestLogoutRound){
			this->EarliestLogoutRound = EarliestLogoutRound;
		}

		((TPlayer*)this)->CheckState();
	}
}

int TCreature::GetHealth(void){
	int MaxHitPoints = this->Skills[SKILL_HITPOINTS]->Max;
	if(MaxHitPoints <= 0){
		if(!this->IsDead){
			error("TCreature::GetHealth: MaxHitpoints von %s ist %d, obwohl sie nicht tot ist.\n",
					this->Name, MaxHitPoints);
		}
		return 0;
	}

	int CurrentHitPoints = this->Skills[SKILL_HITPOINTS]->Get();
	int Health = CurrentHitPoints * 100 / MaxHitPoints;
	if(Health <= 0){
		Health = (int)(CurrentHitPoints != 0);
	}
	return Health;
}

int TCreature::GetSpeed(void){
	TSkill *GoStrength = this->Skills[SKILL_GO_STRENGTH];
	if(GoStrength == NULL){
		error("TCreature::GetSpeed: Kein Skill GOSTRENGTH vorhanden.\n");
		return 0;
	}

	return GoStrength->Get() * 2 + 80;
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
					}catch(RESULT r){
						error("TCreature::Damage: Exception %d beim Abnutzen von Objekt %d.\n",
								r, ObjType.TypeID);
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

void TCreature::Death(void){
	this->IsDead = true;
	this->LoggingOut = true;
}

bool TCreature::MovePossible(int x, int y, int z, bool Execute, bool Jump){
	bool Result;

	if(Jump){
		Result = JumpPossible(x, y, z, false);
	}else{
		Result = CoordinateFlag(x, y, z, BANK)
			&& !CoordinateFlag(x, y, z, UNPASS);
	}

	if(Result && !Execute && CoordinateFlag(x, y, z, AVOID)){
		Result = false;
	}

	return Result;
}

bool TCreature::IsPeaceful(void){
	return true;
}

uint32 TCreature::GetMaster(void){
	return 0;
}

void TCreature::TalkStimulus(uint32 SpeakerID, const char *Text){
	// no-op
}

void TCreature::DamageStimulus(uint32 AttackerID, int Damage, int DamageType){
	// no-op
}

void TCreature::IdleStimulus(void){
	// no-op
}

void TCreature::CreatureMoveStimulus(uint32 CreatureID, int Type){
	if(CreatureID == 0 || CreatureID == this->ID
			|| this->IsDead
			|| this->Combat.AttackDest != CreatureID
			|| this->Combat.ChaseMode != CHASE_MODE_CLOSE
			|| this->Combat.EarliestAttackTime <= (ServerMilliseconds + 200)){
		return;
	}

	// TODO(fusion): Find out what `Type` is here.
	if(Type != 2 // STIMULUS_TYPE??
			|| !this->LockToDo
			|| this->ActToDo >= this->NrToDo
			|| this->ToDoList.at(this->ActToDo)->Code != TDAttack){
		return;
	}

	TCreature *Target = GetCreature(this->Combat.AttackDest);
	if(Target == NULL){
		return;
	}

	int Distance = ObjectDistance(this->CrObject, Target->CrObject);
	if(Distance <= 1){
		return;
	}

	// TODO(fusion): Review this.
	try{
		if(this->ToDoClear() && this->Type == PLAYER){
			SendSnapback(this->Connection);
		}
		this->ToDoWait(200);
		this->ToDoAttack();
		this->ToDoStart();
	}catch(RESULT r){
		if(this->Type == PLAYER){
			SendResult(this->Connection, r);
		}
		this->ToDoClear();
		this->ToDoWait(this->Combat.EarliestAttackTime);
		this->ToDoStart();
	}
}

void TCreature::AttackStimulus(uint32 AttackerID){
	// no-op
}

// Creature Management
// =============================================================================
bool IsCreaturePlayer(uint32 CreatureID){
	return CreatureID < 0x40000000;
}

TCreature *GetCreature(uint32 CreatureID){
	if(CreatureID == 0){
		return NULL;
	}

	TCreature *Creature = HashList[CreatureID % NARRAY(HashList)];
	while(Creature != NULL && Creature->ID != CreatureID){
		Creature = Creature->NextHashEntry;
	}

	return Creature;
}

TCreature *GetCreature(Object Obj){
	return GetCreature(Obj.getCreatureID());
}

void InsertChainCreature(TCreature *Creature, int CoordX, int CoordY){
	if(Creature == NULL){
		// TODO(fusion): Maybe a typo on the name of the function? I thought it
		// could be some type of macro because there was no function name mismatch
		// until now.
		error("DeleteChainCreature: Übegebene Kreatur existiert nicht.\n");
		return;
	}

	if(CoordX == 0){
		CoordX = Creature->posx;
	}

	if(CoordY == 0){
		CoordY = Creature->posy;
	}

	int ChainX = CoordX / 16;
	int ChainY = CoordY / 16;
	uint32 *FirstID = FirstChainCreature->at(ChainX, ChainY);
	Creature->NextChainCreature = *FirstID;
	*FirstID = Creature->ID;
}

void DeleteChainCreature(TCreature *Creature){
	if(Creature == NULL){
		error("DeleteChainCreature: Übegebene Kreatur existiert nicht.\n");
		return;
	}

	// NOTE(fusion): All creatures in each 16x16 region form a creature linked
	// list, despite its current floor. Whether that is a good idea is a whole
	// other matter.
	int ChainX = Creature->posx / 16;
	int ChainY = Creature->posy / 16;
	uint32 *FirstID = FirstChainCreature->at(ChainX, ChainY);

	if(*FirstID == Creature->ID){
		*FirstID = Creature->NextChainCreature;
	}else{
		uint32 CurrentID = *FirstID;
		while(true){
			if(CurrentID == 0){
				error("DeleteChainCreature: Kreatur nicht gefunden.\n");
				return;
			}

			TCreature *Current = GetCreature(CurrentID);
			if(Current == NULL){
				error("DeleteChainCreature: Kreatur existiert nicht.\n");
				return;
			}

			if(Current->NextChainCreature == Creature->ID){
				Current->NextChainCreature = Creature->NextChainCreature;
				break;
			}

			CurrentID = Current->NextChainCreature;
		}
	}
}

void MoveChainCreature(TCreature *Creature, int CoordX, int CoordY){
	if(Creature == NULL){
		error("DeleteChainCreature: Übegebene Kreatur existiert nicht.\n");
		return;
	}

	int NewChainX = CoordX / 16;
	int NewChainY = CoordY / 16;
	int OldChainX = Creature->posx / 16;
	int OldChainY = Creature->posy / 16;

	if(NewChainX != OldChainX || NewChainY != OldChainY){
		DeleteChainCreature(Creature);
		InsertChainCreature(Creature, CoordX, CoordY);
	}
}

void ProcessCreatures(void){
	for(int Index = 0; Index < FirstFreeCreature; Index += 1){
		TCreature *Creature = *CreatureList.at(Index);
		if(Creature == NULL){
			error("ProcessCreatures: Kreatur %d existiert nicht.\n", Index);
			continue;
		}

		// TODO(fusion): It is weird that we do check the connection all the time
		// and most of the time it is redundant because functions will check if
		// it's NULL before attempting anything.

		int FoodRegen = Creature->Skills[SKILL_FED]->Get();
		if(FoodRegen > 0 && (RoundNr % FoodRegen) == 0 && !Creature->IsDead
				&& !IsProtectionZone(Creature->posx, Creature->posy, Creature->posz)){
			Creature->Skills[SKILL_HITPOINTS]->Change(1);
			Creature->Skills[SKILL_MANA]->Change(4);
			if(Creature->Type == PLAYER){
				SendPlayerData(Creature->Connection);
			}
		}

		if(Creature->Type == PLAYER){
			if(Creature->Connection != NULL){
				((TPlayer*)Creature)->CheckState();
			}

			if(Creature->EarliestLogoutRound != 0 && Creature->EarliestLogoutRound <= RoundNr){
				((TPlayer*)Creature)->ClearPlayerkillingMarks();
				Creature->EarliestLogoutRound = 0;
			}
		}

		if(!Creature->IsDead && Creature->Skills[SKILL_HITPOINTS]->Get() <= 0){
			error("ProcessCreatures: Kreatur %s ist nicht tot, obwohl sie keine HP mehr hat.\n", Creature->Name);
			Creature->Death();
		}

		if(Creature->LoggingOut && Creature->LogoutPossible() == 0){ // LOGOUT_POSSIBLE ?
			if(Creature->IsDead && Creature->Skills[SKILL_HITPOINTS]->Get() > 0){
				error("ProcessCreatures: Kreatur %s hat HP, obwohl sie tot ist.\n", Creature->Name);
				Creature->Skills[SKILL_HITPOINTS]->Set(0);
			}

			// TODO(fusion): Creatures are removed from `CreatureList` with a swap
			// and pop. Since we're iterating it RIGHT NOW, we need to process the
			// the current index AGAIN because it'll now contain the creature that
			// was previously at the end of the list. The annoying part here is that
			// this removal occurs implicitly in the creature's destructor.
			delete Creature;
			Index -= 1;
		}
	}
}

void ProcessSkills(void){
	for(int Index = 0; Index < FirstFreeCreature; Index += 1){
		TCreature *Creature = *CreatureList.at(Index);
		if(Creature == NULL){
			error("ProcessSkills: Kreatur %d existiert nicht.\n", Index);
			continue;
		}

		Creature->ProcessSkills();
	}
}

void MoveCreatures(int Delay){
	ServerMilliseconds += Delay;
	while(ToDoQueue.Entries > 0){
		auto Entry = *ToDoQueue.Entry->at(1);
		uint32 ExecutionTime = Entry.Key;
		uint32 CreatureID = Entry.Data;
		if(ExecutionTime > ServerMilliseconds){
			break;
		}

		ToDoQueue.deleteMin();
		TCreature *Creature = GetCreature(CreatureID);
		if(Creature != NULL){
			Creature->Execute();
		}
	}
}

// Kill Statistics
// =============================================================================
void InitKillStatistics(void);//TODO
void ExitKillStatistics(void);//TODO
void AddKillStatistics(int AttackerRace, int DefenderRace){
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

// Race
// =============================================================================
TRaceData::TRaceData(void) :
		Skill(1, 5, 5),
		Talk(1, 5, 5),
		Item(1, 5, 5),
		Spell(1, 5, 5)
{
	this->Name[0] = 0;
	this->Article[0] = 0;
	this->Outfit.OutfitID = 0;
	this->Outfit.ObjectType = 0;
	this->Blood = BT_BLOOD;
	this->ExperiencePoints = 0;
	this->FleeThreshold = 0;
	this->Attack = 0;
	this->Defend = 0;
	this->Armor = 0;
	this->Poison = 0;
	this->SummonCost = 0;
	this->LoseTarget = 0;
	this->Strategy[0] = 100;
	this->Strategy[1] = 0;
	this->Strategy[2] = 0;
	this->Strategy[3] = 0;
	this->KickBoxes = false;
	this->KickCreatures = false;
	this->SeeInvisible = false;
	this->Unpushable = false;
	this->DistanceFighting = false;
	this->NoSummon = false;
	this->NoIllusion = false;
	this->NoConvince = false;
	this->NoBurning = false;
	this->NoPoison = false;
	this->NoEnergy = false;
	this->NoHit = false;
	this->NoLifeDrain = false;
	this->NoParalyze = false;
	this->Skills = 0;
	this->Talks = 0;
	this->Items = 0;
	this->Spells = 0;
}

bool IsRaceValid(int Race){
	return Race >= 1 && Race < MAX_RACES;
}

int GetRaceByName(const char *RaceName){
	int Result = 0;
	for(int Race = 1; Race < MAX_RACES; Race += 1){
		if(stricmp(RaceName, RaceData[Race].Name) == 0){
			Result = Race;
			break;
		}
	}
	return Result;
}

void LoadRaces(void); //TODO

// Monster Raid
// =============================================================================
void LoadMonsterRaids(void); //TODO

// Initialization
// =============================================================================
void InitCr(void){
	NextCreatureID = 0x40000000;
	FirstFreeCreature = 0;
	FirstChainCreature = new matrix<uint32>(
				SectorXMin * 2, SectorXMax * 2 + 1,
				SectorYMin * 2, SectorYMax * 2 + 1,
				0);

	LoadRaces();
	LoadMonsterRaids();
	InitCrskill();
	InitPlayer();
	InitNonplayer();
	InitKillStatistics();
}

void ExitCr(void){
	ExitKillStatistics();
	ExitPlayer();
	ExitNonplayer();
	ExitCrskill();

	delete FirstChainCreature;
}

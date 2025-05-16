#include "creature.hh"
#include "enums.hh"

// TSkill REGULAR FUNCTIONS
//==============================================================================
TSkill::TSkill(int SkNr, TCreature *Master){
	this->Reset();
	this->SkNr = (uint16)SkNr;
	this->Master = Master;
}

int TSkill::Get(void){
	int Value = this->Act;
	if(Value < this->Min)
		Value = this->Min;
	Value += this->MDAct + this->DAct;
	return Value;
}

int TSkill::GetProgress(void){
	int Result = 0;
	if(this->NextLevel > this->LastLevel){
		Result = (this->NextLevel - this->Exp) * 100 / (this->NextLevel - this->LastLevel);

		// TODO(fusion): This feels too much for reporting a mostly *impossible* error.
		if(Result < 0 || Result > 100){
			error("TSkill::GetProgress: Berechnungsfehler Exp %d, Last %d, Next %d, Prozent %d.\n",
					this->Exp, this->LastLevel, this->NextLevel, Result);

			const char *MasterName = "(Unknown";
			if(this->Master != NULL){
				MasterName = this->Master->Name;
			}
			error("# Spieler %s - Skill %d\n", MasterName, this->SkNr);
			Result = 0;
		}
	}
	return Result;
}

void TSkill::Check(void){
	if(this->Act > this->Max){
		this->Act = this->Max;
	}
}

void TSkill::Change(int Amount){
	this->Set(this->Act + Amount);

	// TODO(fusion): Probably `TSkill::Check` inlined? It doesn't make a whole
	// lot of sense because `TSkill::Set` also checks the value. Maybe a custom
	// version of it does something differently?
	if(this->Act > this->Max){
		this->Act = this->Max;
	}
}

void TSkill::SetMDAct(int MDAct){
	this->MDAct = MDAct;
	if(this->SkNr == 4 && this->Master && this->Master->Type == PLAYER){
		// TODO(fusion): Same as `TSkill::Process`.
		((TPlayer*)this->Master)->CheckState();
	}
}

void TSkill::Load(int Act, int Max, int Min, int DAct, int MDAct,
		int Cycle, int MaxCycle, int Count, int MaxCount, int AddLevel,
		int Exp, int FactorPercent, int NextLevel, int Delta){
	this->Act = Act;
	this->Max = Max;
	this->Min = Min;
	this->DAct = DAct;
	this->MDAct = MDAct;
	this->Cycle = Cycle;
	this->MaxCycle = MaxCycle;
	this->Count = Count;
	this->MaxCount = MaxCount;
	this->AddLevel = AddLevel;
	this->Exp = Exp;
	this->FactorPercent = FactorPercent;
	this->NextLevel = NextLevel;
	this->Delta = Delta;

	// TODO(fusion): Shouldn't we also do the same for `NextLevel`?
	this->LastLevel = this->GetExpForLevel(Act);

	TCreature *Master = this->Master;
	if(Master && Cycle != 0){
		int SkNr = this->SkNr;
		if(SkNr >= NARRAY(Master->Skills)){
			error("TSkillBase::SetTimer: Ungueltige SkNr: %d\n", SkNr);
			return;
		}

		// TODO(fusion): I'm not entirely sure wtf is going on here.
		TSkill *Other = Master->Skills[SkNr];
		Master->DelTimer(SkNr);
		if(Other->SetTimer(Cycle, Count, MaxCount, FactorPercent)){
			Master->TimerList[Master->FirstFreeTimer] = Other;
			Master->FirstFreeTimer += 1;
		}
	}
}


void TSkill::Save(int *Act, int *Max, int *Min, int *DAct, int *MDAct,
		int *Cycle, int *MaxCycle, int *Count, int *MaxCount, int *AddLevel,
		int *Exp, int *FactorPercent, int *NextLevel, int *Delta){
	*Act = this->Act;
	*Max = this->Max;
	*Min = this->Min;
	*DAct = this->DAct;
	*MDAct = this->MDAct;
	*Cycle = this->Cycle;
	*MaxCycle = this->MaxCycle;
	*Count = this->Count;
	*MaxCount = this->MaxCount;
	*AddLevel = this->AddLevel;
	*Exp = this->Exp;
	*FactorPercent = this->FactorPercent;
	*NextLevel = this->NextLevel;
	*Delta = this->Delta;
}

// TSkill VIRTUAL FUNCTIONS
//==============================================================================
TSkill::~TSkill(void){
	if(this->Master != NULL){
		this->Master->DelTimer(this->SkNr);
	}
}

void TSkill::Set(int Value){
	if(Value > this->Max)
		Value = this->Max;
	this->Act = Value;
}

void TSkill::Increase(int Amount){
	// no-op
}

void TSkill::Decrease(int Amount){
	// no-op
}

int TSkill::GetExpForLevel(int Level){
	return 0;
}

void TSkill::Advance(int Range){
	// no-op
}

void TSkill::ChangeSkill(int FactorPercent, int Delta){
	// no-op
}

int TSkill::ProbeValue(int Max, bool Increase){
	return 0;
}

bool TSkill::Probe(int Diff, int Prob, bool Increase){
	return false;
}

bool TSkill::Process(void){
	bool Result = this->Cycle == 0;
	if(Result){
		if(this->Master && this->Master->Type == PLAYER){
			// TODO(fusion): Verify if a simple pointer cast works. Maybe `dynamic_cast`?
			((TPlayer*)this->Master)->CheckState();
		}
	}else if(this->Count <= 0){
		this->Count = this->MaxCount;
		// NOTE(fusion): `Range` should move `Cycle` towards ZERO.
		int Range = (this->Cycle < 1) ? +1 : -1;
		this->Cycle += Range;
		this->Event(Range);
	}else{
		this->Count -= 1;
	}

	return Result;
}

bool TSkill::SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue){
	this->Cycle = Cycle;
	this->Count = Count;
	this->MaxCount = MaxCount;
	if(this->Master && this->Master->Type == PLAYER){
		// TODO(fusion): Same as `TSkill::Process`.
		((TPlayer*)this->Master)->CheckState();
	}
	return true;
}

bool TSkill::DelTimer(void){
	this->Cycle = 0;
	this->Count = 0;
	this->MaxCount = 0;
	if(this->Master && this->Master->Type == PLAYER){
		// TODO(fusion): Same as `TSkill::Process`.
		((TPlayer*)this->Master)->CheckState();
	}
	return true;
}

int TSkill::TimerValue(void){
	return this->Cycle;
}

bool TSkill::Jump(int Range){
	return true;
}

void TSkill::Event(int Range){
	// no-op
}

void TSkill::Reset(void){
	this->Act = 0;
	this->Max = INT_MAX;
	this->Min = 0;
	this->DAct = 0;
	this->MDAct = 0;
	this->FactorPercent = 1000;
	this->LastLevel = 0;
	this->NextLevel = INT_MAX;
	this->Exp = 0;
	this->Delta = INT_MAX;
	this->Cycle = 0;
	this->MaxCycle = 0;
	this->Count = 0;
	this->MaxCount = 0;
	this->AddLevel = 0;
}

// TSkillLevel
//==============================================================================
TSkillLevel::~TSkillLevel(void){
	if(this->Master != NULL){
		this->Master->DelTimer(this->SkNr);
	}
}

void TSkillLevel::Increase(int Amount){
	if (Amount < 0) {
		error("TSkillLevel::Increase: Amount negativ (%d).\n", Amount);
		return;
	}

	// BUG(fusion): No bounds check as in `TSkillLevel::Decrease`?

	int Range = 0;
	this->Exp += Amount;
	while(this->Exp >= this->NextLevel){
		this->Act += 1;
		this->LastLevel = this->GetExpForLevel(this->Act);
		this->NextLevel = this->GetExpForLevel(this->Act + 1);
		if(this->NextLevel < 0){
			// BUG(fusion): We don't check if `Master` is valid here?
			error("TSkillLevel::Increase: Skill vor Überlauf (%s, Skill %d).\n", this->Master->Name, this->SkNr);
			this->NextLevel = this->Exp;
			this->Exp -= 1000000;
			break;
		}
		Range += 1;
	}

	if(Range != 0){
		this->Jump(Range);
	}

	if(this->Master == NULL){
		error("TSkillLevel::Increase: GetMaster liefert NULL zurueck.\n");
		return;
	}

	if(this->Master->Type == PLAYER){
		SendPlayerData(this->Master->Connection);
	}
}

void TSkillLevel::Decrease(int Amount){
	if(Amount < 0){
		error("TSkillLevel::Decrease: Amount negativ (%d).\n", Amount);
		return;
	}

	// TODO(fusion): This is some weird ass comparison.
	if(Amount > this->Exp && this->Exp > 100000){
		error("TSkillLevel::Decrease: Amount zu gross(%d).\n", Amount);
		return;
	}

	int Range = 0;
	if(Amount < this->Exp){
		this->Exp -= Amount;
	}else{
		this->Exp = 0;
	}

	// TODO(fusion): This could probably be an oversight but the decompiled
	// function was calling `GetExpForLevel` instead of using `LastLevel`
	// which makes me wonder whether `LastLevel` is properly initialized.
	while(this->Exp < this->LastLevel){
		this->Act -= 1;
		this->NextLevel = this->LastLevel;
		this->LastLevel = this->GetExpForLevel(this->Act);
	}

	if(Range != 0){
		this->Jump(Range);
	}

	if(this->Master == NULL){
		error("TSkillLevel::Decrease: GetMaster liefert NULL zurueck.\n");
		return;
	}

	if(this->Master->Type == PLAYER){
		SendPlayerData(this->Master->Connection);
	}
}

int TSkillLevel::GetExpForLevel(int Level){
	if(Level < 1){
		error("TSkillLevel::GetExpForLevel: Ungültiger Level %d.\n", Level);
		return 0;
	}

	if(Level > 500){
		error("TSkillLevel::GetExpForLevel: Level=%d; Formel gegen Überlauf sichern.\n", Level);
		return -1;
	}

	if(this->Delta <= 0){
		error("TSkillLevel::GetExpForLevel: Ungültiger Delta-Wert %d.\n", this->Delta);
		return 0;
	}

	return ((((Level - 6) * Level + 17) * Level - 12) / 6) * this->Delta;
}

bool TSkillLevel::Jump(int Range){
	if(this->Master == NULL){
		error("TSkillLevel::Jump: GetMaster liefert NULL zurueck!\n");
		return false;
	}

	this->Master->Skills[SKILL_HITPOINTS   ]->Advance(Range);
	this->Master->Skills[SKILL_MANA        ]->Advance(Range);
	this->Master->Skills[SKILL_GO_STRENGTH ]->Advance(Range);
	this->Master->Skills[SKILL_CARRY_WEIGHT]->Advance(Range);

	AnnounceChangedCreature(this->Master->ID, 4);
	this->Master->Combat.CheckCombatValues();

	if(this->Master->Type == PLAYER){
		// TODO(fusion): Probably `TSkill::Get` inlined?
		int Level = this->Act;
		if(Level < this->Min)
			Level = this->Min;
		Level += this->MDAct + this->DAct;
		// ==

		int FromLevel = Level - Range;
		int ToLevel = Level;
		if(Range > 0){
			SendMessage(this->Master->Connection, TALK_ANONYMOUS_BROADCAST,
					"You advanced from Level %d to Level %d.", FromLevel, ToLevel);
		}else if(Range < 0){
			SendMessage(this->Master->Connection, TALK_ANONYMOUS_BROADCAST,
					"You were downgraded from Level %d to Level %d.", FromLevel, ToLevel);
		}
	}

	return true;
}

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
	if(Creature->EarliestSpellTime < EarliestSpellTime)
		Creature->EarliestSpellTime = EarliestSpellTime;
}

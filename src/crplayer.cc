#include "cr.hh"
#include "info.hh"

#include "stubs.hh"

uint8 TPlayer::GetRealProfession(void){
	return this->Profession;
}

uint8 TPlayer::GetEffectiveProfession(void){
	uint8 Profession = this->Profession;
	if(Profession >= 10){
		Profession -= 10;
	}
	return Profession;
}

uint8 TPlayer::GetActiveProfession(void){
	uint8 Profession;
	if(CheckRight(this->ID, PREMIUM_ACCOUNT)){
		Profession = this->Profession;
	}else{
		Profession = this->GetEffectiveProfession();
	}
	return Profession;
}

bool TPlayer::GetActivePromotion(void){
	return CheckRight(this->ID, PREMIUM_ACCOUNT)
		&& this->Profession >= 10;
}

void TPlayer::CheckState(void){
	if(this->Connection != NULL){
		uint8 State = 0;

		if(this->Skills[SKILL_POISON]->TimerValue() > 0){
			State |= 0x01;
		}

		if(this->Skills[SKILL_BURNING]->TimerValue() > 0){
			State |= 0x02;
		}

		if(this->Skills[SKILL_ENERGY]->TimerValue() > 0){
			State |= 0x04;
		}

		// TODO(fusion): I think the result from `Get()` here tells whether the
		// player has some drunk suppression item equipped?
		if(this->Skills[SKILL_DRUNKEN]->TimerValue() > 0
				&& this->Skills[SKILL_DRUNKEN]->Get() == 0){
			State |= 0x08;
		}

		if(this->Skills[SKILL_MANASHIELD]->TimerValue() > 0
				|| this->Skills[SKILL_MANASHIELD]->Get() > 0){
			State |= 0x10;
		}

		if(this->Skills[SKILL_GO_STRENGTH]->MDAct < 0){
			State |= 0x20;
		}else if(this->Skills[SKILL_GO_STRENGTH]->MDAct > 0){
			State |= 0x40;
		}

		if(RoundNr < this->EarliestLogoutRound){
			State |= 0x80;
		}

		if(State != this->OldState){
			SendPlayerState(this->Connection, State);
			this->OldState = State;
		}
	}
}

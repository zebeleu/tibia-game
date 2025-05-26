#include "player.hh"
#include "enums.hh"

#include "stubs.hh"

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

		// TODO(fusion): Not sure about this one.
		if(this->Skills[SKILL_DRUNK]->TimerValue() > 0 && this->Skills[SKILL_DRUNK]->Get() == 0){
			State |= 0x08;
		}

		if(this->Skills[SKILL_MANASHIELD]->TimerValue() > 0 || this->Skills[SKILL_MANASHIELD]->Get() > 0){
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

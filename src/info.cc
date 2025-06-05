#include "info.hh"
#include "cr.hh"

#include "stubs.hh"

bool JumpPossible(int x, int y, int z, bool AvoidPlayers){
	bool HasBank = false;
	Object Obj = GetFirstObject(x, y, z);
	while(Obj != NONE){
		ObjectType ObjType = Obj.getObjectType();

		if(ObjType.getFlag(BANK)){
			HasBank = true;
		}

		if(ObjType.getFlag(UNPASS) && ObjType.getFlag(UNMOVE)){
			return false;
		}

		if(AvoidPlayers && ObjType.isCreatureContainer()){
			TCreature *Creature = GetCreature(Obj);
			if(Creature != NULL && Creature->Type == PLAYER){
				return false;
			}
		}

		Obj = Obj.getNextObject();
	}
	return HasBank;
}

bool SearchFreeField(int *x, int *y, int *z, int Distance, uint16 HouseID, bool Jump){
	int OffsetX = 0;
	int OffsetY = 0;
	int CurrentDistance = 0;
	int CurrentDirection = DIRECTION_EAST;
	while(CurrentDistance <= Distance){
		int FieldX = *x + OffsetX;
		int FieldY = *y + OffsetY;
		int FieldZ = *z;

		// TODO(fusion): This is probably some form of the `TCreature::MovePossible`
		// function inlined.
		bool MovePossible;
		if(Jump){
			MovePossible = JumpPossible(FieldX, FieldY, FieldZ, true);
		}else{
			MovePossible = CoordinateFlag(FieldX, FieldY, FieldZ, BANK)
					&& !CoordinateFlag(FieldX, FieldY, FieldZ, UNPASS);

			// TODO(fusion): This one I'm not so sure.
			if(MovePossible && CoordinateFlag(FieldX, FieldY, FieldZ, AVOID)){
				MovePossible = CoordinateFlag(FieldX, FieldY, FieldZ, BED);
			}
		}

		if(MovePossible){
			if(HouseID == HOUSEID_ANY || !IsHouse(FieldX, FieldY, FieldZ)
			|| (HouseID != 0 && HouseID == GetHouseID(FieldX, FieldY, FieldZ))){
				*x = FieldX;
				*y = FieldY;
				return true;
			}
		}


		// NOTE(fusion): We're spiraling out from the initial coordinate.
		// TODO(fusion): This function used directions different from the ones
		// used by creatures and defined in `enums.hh` so I made it use them
		// instead, LOL.
		if(CurrentDirection == DIRECTION_NORTH){
			OffsetY -= 1;
			if(OffsetY <= -CurrentDistance){
				CurrentDirection = DIRECTION_WEST;
			}
		}else if(CurrentDirection == DIRECTION_WEST){
			OffsetX -= 1;
			if(OffsetX <= -CurrentDistance){
				CurrentDirection = DIRECTION_SOUTH;
			}
		}else if(CurrentDirection == DIRECTION_SOUTH){
			OffsetY += 1;
			if(OffsetY >= CurrentDistance){
				CurrentDirection = DIRECTION_EAST;
			}
		}else{
			ASSERT(CurrentDirection == DIRECTION_EAST);
			OffsetX += 1;
			if(OffsetX > CurrentDistance){
				CurrentDistance = OffsetX;
				CurrentDirection = DIRECTION_NORTH;
			}
		}
	}

	return false;
}

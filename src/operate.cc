#include "operate.hh"

#include "stubs.hh"

// TODO(fusion): This could have been a simple return value.
void CheckContainerDestination(Object Obj, Object Con){
	Object Help = Con;
	while(Help != NONE && !Help.getObjectType().isMapContainer()){
		if(Help == Obj){
			throw CROSSREFERENCE;
		}
		Help = Help.getContainer();
	}

	ObjectType ConType = Con.getObjectType();
	if(!ConType.getFlag(CHEST)){
		int ConObjects = CountObjectsInContainer(Con);
		int ConCapacity = (int)ConType.getAttribute(CAPACITY);
		if(ConObjects >= ConCapacity){
			throw CONTAINERFULL;
		}
	}
}

void CheckInventoryDestination(Object Obj, Object Con, bool Split){
	ObjectType ObjType = Obj.getObjectType();
	int ConPosition = GetObjectBodyPosition(Con);
	bool HandContainer = ConPosition == INVENTORY_RIGHTHAND
					|| ConPosition == INVENTORY_LEFTHAND;

	if(!HandContainer && ConPosition != INVENTORY_AMMO){
		if(!ObjType.getFlag(CLOTHES)){
			throw WRONGPOSITION;
		}

		int ObjPosition = (int)ObjType.getAttribute(BODYPOSITION);
		if(ObjPosition == 0){
			throw WRONGPOSITION2;
		}else if(ObjPosition != ConPosition){
			throw WRONGCLOTHES;
		}
	}

	uint32 CreatureID = GetObjectCreatureID(Con);
	if(ObjType.isTwoHanded() && HandContainer){
		Object RightHand = GetBodyObject(CreatureID, INVENTORY_RIGHTHAND);
		if(RightHand != NONE && RightHand != Obj){
			throw HANDSNOTFREE;
		}

		Object LeftHand = GetBodyObject(CreatureID, INVENTORY_LEFTHAND);
		if(LeftHand != NONE && LeftHand != Obj){
			throw HANDSNOTFREE;
		}

		return;
	}

	if(GetBodyObject(CreatureID, ConPosition) != NONE){
		throw NOROOM;
	}

	if(HandContainer){
		for(int Position = INVENTORY_HAND_FIRST;
				Position <= INVENTORY_HAND_LAST;
				Position += 1){
			Object Other = GetBodyObject(CreatureID, Position);
			if(Other != NONE){
				ObjectType OtherType = Other.getObjectType();
				if(OtherType.isTwoHanded()){
					throw HANDBLOCKED;
				}

				if(Split || Other != Obj){
					if(OtherType.isWeapon() && ObjType.isWeapon()){
						throw ONEWEAPONONLY;
					}
				}
			}
		}
	}
}

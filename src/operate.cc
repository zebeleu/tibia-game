#include "operate.hh"
#include "info.hh"

#include "stubs.hh"

// operate.cc
void AnnounceMovingCreature(uint32 CreatureID, Object Con);
void CheckTopMoveObject(uint32 CreatureID, Object Obj, Object Ignore);
void CheckMoveObject(uint32 CreatureID, Object Obj, bool Take);
void CheckMapDestination(uint32 CreatureID, Object Obj, Object MapCon);
void CheckDepotSpace(uint32 CreatureID, Object Source, Object Destination, int Count);
void CheckWeight(uint32 CreatureID, Object Obj, int Count);
void NotifyCreature(uint32 CreatureID, Object Obj, bool Inventory);
void NotifyCreature(uint32 CreatureID, ObjectType Type, bool Inventory);
//void NotifyAllCreatures(Object Obj, int Type, Object OldCon);
void NotifyTrades(Object Obj);
void NotifyDepot(uint32 CreatureID, Object Obj, int Count);
void CloseContainer(Object Con, bool Force);

// moveuse.cc
void MovementEvent(Object Obj, Object Start, Object Dest);
void CollisionEvent(Object Obj, Object Dest);
void SeparationEvent(Object Obj, Object Start);


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

void Move(uint32 CreatureID, Object Obj, Object Con, int Count, bool NoMerge, Object Ignore){
	if(!Obj.exists()){
		error("Move: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	if(!Con.exists()){
		int ObjX, ObjY, ObjZ;
		ObjectType ObjType = Obj.getObjectType();
		GetObjectCoordinates(Obj, &ObjX, &ObjY, &ObjZ);
		error("Move: Zielobjekt existiert nicht.\n");
		error("# Objekt %d an [%d,%d,%d]\n", ObjType.TypeID, ObjX, ObjY, ObjZ);
		throw ERROR;
	}

	ObjectType ConType = Con.getObjectType();
	if(!ConType.getFlag(CONTAINER)){
		error("Move: Zielobjekt ist kein Container.\n");
		throw ERROR;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(ObjType.getFlag(CUMULATIVE) && (Count == 0 || Count > (int)Obj.getAttribute(AMOUNT))){
		error("Move: Ungültige Anzahl %d von %d an Teilen bei kumulierbarem Objekt.\n",
				Count, (int)Obj.getAttribute(AMOUNT));
		throw ERROR;
	}

	Object OldCon = Obj.getContainer();
	if(!NoMerge && ConType.isMapContainer()
			&& ObjType.getFlag(CUMULATIVE)
			&& OldCon != Con){
		int ConX, ConY, ConZ;
		GetObjectCoordinates(Con, &ConX, &ConY, &ConZ);
		Object Top = GetTopObject(ConX, ConY, ConZ, true);
		if(Top != NONE){
			try{
				Merge(CreatureID, Obj, Top, Count, Ignore);
				return; // <-- EARLY RETURN IF `Merge` SUCCEEDS.
			}catch(RESULT r){
				if(r == DESTROYED){
					throw;
				}
			}
		}
	}

	// TODO(fusion): The only event inside `Merge` that could modify the object
	// and still throw some non `DESTROYED` error is the separation event.
	//	From what I've seen, it is used exclusively with non CUMULATIVE objects
	// like depot switches to perform "step out" events but using them with
	// CUMULATIVE objects could be problematic as the event is triggered on the
	// whole object which includes both the remainder and moved parts, meaning
	// that transforming the object then would already make the merge "invalid".

	if(!Obj.exists()){
		error("Move: Übergebenes Objekt existiert nicht mehr.\n");
	}

	int ObjCount = 1;
	if(ObjType.getFlag(CUMULATIVE)){
		ObjCount = (int)Obj.getAttribute(AMOUNT);
	}

	if(Count == -1){
		Count = ObjCount;
	}

	bool Split = Count < ObjCount;
	uint32 ObjOwnerID = GetObjectCreatureID(Obj);
	uint32 ConOwnerID = GetObjectCreatureID(Con);
	CheckTopMoveObject(CreatureID, Obj, Ignore);
	if(ConType.isMapContainer()){
		CheckMoveObject(CreatureID, Obj, false);
		CheckMapDestination(CreatureID, Obj, Con);
	}else{
		CheckMoveObject(CreatureID, Obj, true);
		if(ConType.isBodyContainer()){
			CheckInventoryDestination(Obj, Con, Split);
		}else{
			CheckContainerDestination(Obj, Con);
		}

		if(Split){
			CheckDepotSpace(CreatureID, NONE, Con, 1);
		}else{
			CheckDepotSpace(CreatureID, Obj, Con, CountObjects(Obj));
		}

		if(ConOwnerID != 0){
			CheckWeight(ConOwnerID, Obj, Count);
		}
	}

	// NOTE(fusion): `Obj` becomes the split part while `Remainder`, the remainder.
	Object Remainder = NONE;
	if(Split){
		Remainder = Obj;
		Obj = SplitObject(Obj, Count);
	}

	if(OldCon != Con){
		SeparationEvent(Obj, OldCon);
	}

	// TODO(fusion): There is surely a way to reduce these creature checks?
	// perhaps split them into two linear paths, with a few duplications but
	// easier to read.

	if(!ObjType.isCreatureContainer() && Remainder == NONE){
		AnnounceChangedObject(Obj, 0); // OBJECT_DISAPPEAR ?
		NotifyTrades(Obj);
		NotifyDepot(CreatureID, Obj, CountObjects(Obj));
	}

	// TODO(fusion): This could probably be moved to the end of the function
	// along with the other `CloseContainer`.
	if(ObjType.getFlag(CONTAINER) && CreatureID == 0){
		CloseContainer(Obj, true);
	}

	if(ObjType.isCreatureContainer()){
		uint32 MovingCreatureID = Obj.getCreatureID();
		TCreature *Creature = GetCreature(MovingCreatureID);
		if(Creature != NULL){
			Creature->NotifyTurn(Con);
		}else{
			error("Move: Kreatur mit ungültiger ID %d.\n", MovingCreatureID);
		}
		AnnounceMovingCreature(MovingCreatureID, Con);
	}

	MoveObject(Obj, Con);

	if(!ObjType.isCreatureContainer()){
		AnnounceChangedObject(Obj, 1); // OBJECT_APPEAR
		NotifyTrades(Obj);
		NotifyDepot(CreatureID, Obj, -CountObjects(Obj));
	}

	if(Remainder != NONE){
		AnnounceChangedObject(Remainder, 2); // OBJECT_CHANGED
		NotifyTrades(Remainder);
	}

	if(ObjType.isCreatureContainer()){
		uint32 MovingCreatureID = Obj.getCreatureID();
		TCreature *Creature = GetCreature(MovingCreatureID);
		if(Creature != NULL){
			Creature->NotifyGo();
		}else{
			error("Move: Kreatur mit ungültiger ID %d.\n", MovingCreatureID);
		}
	}

	NotifyCreature(ObjOwnerID, Obj, OldCon.getObjectType().isBodyContainer());
	NotifyCreature(ConOwnerID, Obj, Con.getObjectType().isBodyContainer());

	if(ObjType.getFlag(CONTAINER)){
		CloseContainer(Obj, false);
	}

	MovementEvent(Obj, OldCon, Con);
	CollisionEvent(Obj, Con);
	NotifyAllCreatures(Obj, 3, OldCon); // CREATURE_MOVE_STIMULUS_ ?
}

void Merge(uint32 CreatureID, Object Obj, Object Dest, int Count, Object Ignore){
	if(!Obj.exists()){
		error("Merge: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	if(!Dest.exists()){
		error("Merge: Zielobjekt existiert nicht.\n");
		throw ERROR;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(ObjType.getFlag(CUMULATIVE) && (Count == 0 || Count > (int)Obj.getAttribute(AMOUNT))){
		error("Merge: Ungültige Anzahl %d an Teilen bei kumulierbarem Objekt.\n", Count);
		throw ERROR;
	}

	if(Obj == Dest){
		return;
	}

	if(ObjType != Dest.getObjectType()){
		throw NOMATCH;
	}

	if(!ObjType.getFlag(CUMULATIVE)){
		throw NOTCUMULABLE;
	}

	int ObjCount = Obj.getAttribute(AMOUNT);
	int DestCount = Dest.getAttribute(AMOUNT);
	if(Count == -1){
		Count = ObjCount;
	}

	if((Count + DestCount) > 100){
		throw TOOMANYPARTS;
	}

	Object DestCon = Dest.getContainer();
	uint32 DestOwnerID = GetObjectCreatureID(Dest);
	CheckTopMoveObject(CreatureID, Obj, Ignore);
	if(DestCon.getObjectType().isMapContainer()){
		CheckMoveObject(CreatureID, Obj, false);
		CheckMapDestination(CreatureID, Obj, DestCon);
	}else{
		CheckMoveObject(CreatureID, Obj, true);
		CheckDepotSpace(CreatureID, Obj, DestCon, 0);
		if(DestOwnerID != 0){
			CheckWeight(DestOwnerID, Obj, Count);
		}
	}

	Object ObjCon = Obj.getContainer();
	uint32 ObjOwnerID = GetObjectCreatureID(Obj);

	// TODO(fusion): What is even going on here?
	if(ObjType.getFlag(MOVEMENTEVENT)){
		error("Merge: Movement-Event für kumulierbares Objekt %d wird nicht ausgelöst.\n", ObjType.TypeID);
	}

	if(ObjCon != DestCon){
		SeparationEvent(Obj, ObjCon);
	}

	if(Count < ObjCount){
		ChangeObject(Obj, AMOUNT, ObjCount - Count);
		AnnounceChangedObject(Obj, 2); // OBJECT_CHANGED ?
		NotifyTrades(Obj);
		ChangeObject(Dest, AMOUNT, DestCount + Count);
	}else{
		AnnounceChangedObject(Obj, 0); // OBJECT_DISAPPEAR ?
		NotifyTrades(Obj);
		NotifyDepot(CreatureID, Obj, 1);
		MergeObjects(Obj, Dest);
	}

	AnnounceChangedObject(Dest, 2); // OBJECT_CHANGED ?
	NotifyTrades(Dest);
	NotifyCreature(ObjOwnerID, Dest, ObjCon.getObjectType().isBodyContainer());
	NotifyCreature(DestOwnerID, Dest, DestCon.getObjectType().isBodyContainer());
	CollisionEvent(Dest, DestCon);
	NotifyAllCreatures(Dest, 2, NONE); // CREATURE_MOVE_STIMULUS_ ?
}

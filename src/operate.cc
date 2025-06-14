#include "operate.hh"
#include "config.hh"
#include "info.hh"

#include "stubs.hh"

// TODO(fusion): The radii parameters for `TFindCreatures` are commonly around
// 16 and 14 for x and y respectively so there is probably some constant involved.
//	Also, since we're talking about radii, these values are quite large when you
// consider the regular client's viewport dimensions of 15x11 visible fields or
// 18x14 total fields.

void AnnounceMovingCreature(uint32 CreatureID, Object Con){
	TCreature *Creature = GetCreature(CreatureID);
	if(Creature == NULL){
		error("AnnounceMovingCreature: Kreatur %d existiert nicht.\n", CreatureID);
		return;
	}

	int ConX, ConY, ConZ;
	GetObjectCoordinates(Con, &ConX, &ConY, &ConZ);

	int SearchRadiusX = 16 + (std::abs(Creature->posx - ConX) / 2) + 1;
	int SearchRadiusY = 14 + (std::abs(Creature->posy - ConY) / 2) + 1;
	int SearchCenterX = (Creature->posx + ConX) / 2;
	int SearchCenterY = (Creature->posy + ConY) / 2;
	TFindCreatures Search(SearchRadiusX, SearchRadiusY, SearchCenterX, SearchCenterY, FIND_PLAYERS);
	while(true){
		uint32 CharacterID = Search.getNext();
		if(CharacterID == 0){
			break;
		}

		TPlayer *Player = GetPlayer(CharacterID);
		if(Player == NULL || Player->Connection == NULL){
			continue;
		}

		SendMoveCreature(Player->Connection, CreatureID, ConX, ConY, ConZ);
	}
}

void AnnounceChangedCreature(uint32 CreatureID, int Type){
	TCreature *Creature = GetCreature(CreatureID);
	if(Creature == NULL){
		error("AnnounceChangedCreature: Kreatur %d existiert nicht (Type=%d).\n", CreatureID, Type);
		return;
	}

	for(TKnownCreature *KnownCreature = Creature->FirstKnowingConnection;
			KnownCreature != NULL;
			KnownCreature = KnownCreature->Next){
		TConnection *KnowingConnection = KnownCreature->Connection;
		if(KnowingConnection->State != CONNECTION_GAME
				|| KnownCreature->State == KNOWNCREATURE_OUTDATED){
			continue;
		}

		if(KnowingConnection->IsVisible(Creature->posx, Creature->posy, Creature->posz)){
			switch(Type){
				case CREATURE_HEALTH_CHANGED: SendCreatureHealth(KnowingConnection, CreatureID); break;
				case CREATURE_LIGHT_CHANGED:  SendCreatureLight(KnowingConnection, CreatureID); break;
				case CREATURE_OUTFIT_CHANGED: SendCreatureOutfit(KnowingConnection, CreatureID); break;
				case CREATURE_SPEED_CHANGED:  SendCreatureSpeed(KnowingConnection, CreatureID); break;
				case CREATURE_SKULL_CHANGED:  SendCreatureSkull(KnowingConnection, CreatureID); break;
				case CREATURE_PARTY_CHANGED:  SendCreatureParty(KnowingConnection, CreatureID); break;
			}
		}else{
			KnownCreature->State = KNOWNCREATURE_OUTDATED;
		}
	}
}

void AnnounceChangedField(Object Obj, int Type){
	if(!Obj.exists()){
		error("AnnounceChangedField: Übergebenes Objekt existiert nicht.\n");
		return;
	}

	int ObjX, ObjY, ObjZ;
	GetObjectCoordinates(Obj, &ObjX, &ObjY, &ObjZ);
	TFindCreatures Search(16, 14, ObjX, ObjY, FIND_PLAYERS);
	while(true){
		uint32 CharacterID = Search.getNext();
		if(CharacterID == 0){
			break;
		}

		TPlayer *Player = GetPlayer(CharacterID);
		if(Player == NULL || Player->Connection == NULL){
			continue;
		}

		if(!Player->Connection->IsVisible(ObjX, ObjY, ObjZ)){
			continue;
		}

		switch(Type){
			case OBJECT_DELETED: SendDeleteField(Player->Connection, ObjX, ObjY, ObjZ, Obj); break;
			case OBJECT_CREATED: SendAddField(Player->Connection,    ObjX, ObjY, ObjZ, Obj); break;
			case OBJECT_CHANGED: SendChangeField(Player->Connection, ObjX, ObjY, ObjZ, Obj); break;
			default:{
				error("AnnounceChangedField: Ungültiger Typ %d.\n", Type);
				return;
			}
		}
	}
}

void AnnounceChangedContainer(Object Obj, int Type){
	if(!Obj.exists()){
		error("AnnounceChangedContainer: Übergebenes Objekt existiert nicht.\n");
		return;
	}

	Object Con = Obj.getContainer();
	TFindCreatures Search(1, 1, Obj, FIND_PLAYERS);
	while(true){
		uint32 CharacterID = Search.getNext();
		if(CharacterID == 0){
			break;
		}

		TPlayer *Player = GetPlayer(CharacterID);
		if(Player == NULL || Player->Connection == NULL){
			continue;
		}

		for(int ContainerNr = 0;
				ContainerNr <= NARRAY(Player->OpenContainer);
				ContainerNr += 1){
			if(Player->GetOpenContainer(ContainerNr) != Con){
				continue;
			}

			switch(Type){
				case OBJECT_DELETED: SendDeleteInContainer(Player->Connection, ContainerNr, Obj); break;
				case OBJECT_CREATED: SendCreateInContainer(Player->Connection, ContainerNr, Obj); break;
				case OBJECT_CHANGED: SendChangeInContainer(Player->Connection, ContainerNr, Obj); break;
				default:{
					error("AnnounceChangedContainer: Ungültiger Typ %d.\n", Type);
					return;
				}
			}
		}
	}
}

void AnnounceChangedInventory(Object Obj, int Type){
	if(!Obj.exists()){
		error("AnnounceChangedInventory: Übergebenes Objekt existiert nicht.\n");
		return;
	}

	uint32 OwnerID = GetObjectCreatureID(Obj);
	TCreature *Creature = GetCreature(OwnerID);
	if(Creature == NULL){
		error("AnnounceChangedInventory: Objekt liegt in keinem Inventory.\n");
		return;
	}

	if(Creature->Type != PLAYER){
		return;
	}

	int Position = GetObjectBodyPosition(Obj);
	switch(Type){
		case OBJECT_DELETED: SendDeleteInventory(Creature->Connection, Position); break;
		case OBJECT_CREATED: SendSetInventory(Creature->Connection, Position, Obj); break;
		case OBJECT_CHANGED: SendSetInventory(Creature->Connection, Position, Obj); break;
		default:{
			error("AnnounceChangedInventory: Ungültiger Typ %d.\n", Type);
			return;
		}
	}
}

void AnnounceChangedObject(Object Obj, int Type){
	if(!Obj.exists()){
		error("AnnounceChangedObject: Übergebenes Objekt existiert nicht.\n");
		return;
	}

	Object Con = Obj.getContainer();
	ObjectType ConType = Con.getObjectType();
	if(ConType.isMapContainer()){
		AnnounceChangedField(Obj, Type);
	}else if(ConType.isBodyContainer()){
		AnnounceChangedInventory(Obj, Type);
	}else{
		AnnounceChangedContainer(Obj, Type);
	}
}

void AnnounceGraphicalEffect(int x, int y, int z, int Type){
	if(!IsOnMap(x, y, z)){
		return;
	}

	TFindCreatures Search(16, 14, x, y, FIND_PLAYERS);
	while(true){
		uint32 CharacterID = Search.getNext();
		if(CharacterID == 0){
			break;
		}

		TPlayer *Player = GetPlayer(CharacterID);
		if(Player == NULL || Player->Connection == NULL){
			continue;
		}

		if(!Player->Connection->IsVisible(x, y, z)){
			continue;
		}

		SendGraphicalEffect(Player->Connection, x, y, z, Type);
	}
}

void AnnounceTextualEffect(int x, int y, int z, int Color, const char *Text){
	if(!IsOnMap(x, y, z)){
		return;
	}

	TFindCreatures Search(16, 14, x, y, FIND_PLAYERS);
	while(true){
		uint32 CharacterID = Search.getNext();
		if(CharacterID == 0){
			break;
		}

		TPlayer *Player = GetPlayer(CharacterID);
		if(Player == NULL || Player->Connection == NULL){
			continue;
		}

		if(!Player->Connection->IsVisible(x, y, z)){
			continue;
		}

		SendTextualEffect(Player->Connection, x, y, z, Color, Text);
	}
}

void AnnounceMissile(int OrigX, int OrigY, int OrigZ,
		int DestX, int DestY, int DestZ, int Type){
	if(!IsOnMap(OrigX, OrigY, OrigZ) || !IsOnMap(DestX, DestY, DestZ)){
		return;
	}

	int SearchRadiusX = 16 + (std::abs(OrigX - DestX) / 2) + 1;
	int SearchRadiusY = 14 + (std::abs(OrigY - DestY) / 2) + 1;
	int SearchCenterX = (OrigX + DestX) / 2;
	int SearchCenterY = (OrigY + DestY) / 2;
	TFindCreatures Search(SearchRadiusX, SearchRadiusY, SearchCenterX, SearchCenterY, FIND_PLAYERS);
	while(true){
		uint32 CharacterID = Search.getNext();
		if(CharacterID == 0){
			break;
		}

		TPlayer *Player = GetPlayer(CharacterID);
		if(Player == NULL || Player->Connection == NULL){
			continue;
		}

		if(!Player->Connection->IsVisible(OrigX, OrigY, OrigY)
		&& !Player->Connection->IsVisible(DestX, DestY, DestZ)){
			continue;
		}

		SendMissileEffect(Player->Connection,
				OrigX, OrigY, OrigY,
				DestX, DestY, DestZ, Type);
	}
}

void CheckTopMoveObject(uint32 CreatureID, Object Obj, Object Ignore){
	if(!Obj.exists()){
		error("CheckTopMoveObject: Objekt existiert nicht.\n");
		throw ERROR;
	}

	if(CreatureID == 0 || !IsCreaturePlayer(CreatureID)){
		return;
	}

	Object Con = Obj.getContainer();
	ObjectType ConType = Con.getObjectType();
	if(!ConType.isMapContainer()){
		return;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(ObjType.isCreatureContainer() && Obj.getCreatureID() == CreatureID){
		return;
	}

	Object Best = NONE;
	bool BestIsCreature = false;
	Object Help = GetFirstContainerObject(Con);
	while(Help != NONE){
		if(Help != Ignore){
			ObjectType HelpType = Help.getObjectType();

			// NOTE(fusion): We're looking for the top move object. The creature
			// check here makes sure only the first creature found is kept as the
			// best candidate, since creatures on a map container are in a sequence.
			if(Best == NONE || (!HelpType.getFlag(UNMOVE) && (!BestIsCreature || !HelpType.isCreatureContainer()))){
				Best = Help;
				BestIsCreature = HelpType.isCreatureContainer();
			}

			if(GetObjectPriority(Help) == PRIORITY_LOW){
				break;
			}
		}
		Help = Help.getNextObject();
	}

	if(Obj != Best){
		throw NOTACCESSIBLE;
	}
}

void CheckTopUseObject(uint32 CreatureID, Object Obj){
	if(!Obj.exists()){
		error("CheckTopUseObject: Objekt existiert nicht.\n");
		throw ERROR;
	}

	if(CreatureID == 0 || !IsCreaturePlayer(CreatureID)){
		return;
	}

	Object Con = Obj.getContainer();
	ObjectType ConType = Con.getObjectType();
	if(!ConType.isMapContainer()){
		return;
	}

	Object Best = NONE;
	Object Help = GetFirstContainerObject(Con);
	while(Help != NONE){
		ObjectType HelpType = Help.getObjectType();
		if(Best == NONE || (!HelpType.isCreatureContainer() && !HelpType.getFlag(LIQUIDPOOL))){
			Best = Help;
		}

		if(HelpType.getFlag(FORCEUSE) || GetObjectPriority(Help) == PRIORITY_LOW){
			break;
		}

		Help = Help.getNextObject();
	}

	if(Obj != Best){
		throw NOTACCESSIBLE;
	}
}

void CheckTopMultiuseObject(uint32 CreatureID, Object Obj){
	if(!Obj.exists()){
		error("CheckTopMultiuseObject: Objekt existiert nicht.\n");
		throw ERROR;
	}

	if(CreatureID == 0 || !IsCreaturePlayer(CreatureID)){
		return;
	}

	Object Con = Obj.getContainer();
	ObjectType ConType = Con.getObjectType();
	if(!ConType.isMapContainer()){
		return;
	}

	Object Best = NONE;
	Object Help = GetFirstContainerObject(Con);
	while(Help != NONE){
		ObjectType HelpType = Help.getObjectType();
		if(Best == NONE || !HelpType.getFlag(LIQUIDPOOL)){
			Best = Help;
		}

		if(HelpType.getFlag(FORCEUSE) || GetObjectPriority(Help) >= PRIORITY_CREATURE){
			break;
		}

		Help = Help.getNextObject();
	}

	if(Obj != Best){
		throw NOTACCESSIBLE;
	}
}

void CheckMoveObject(uint32 CreatureID, Object Obj, bool Take){
	if(CreatureID == 0){
		return;
	}

	if(!ObjectAccessible(CreatureID, Obj, 1)){
		throw NOTACCESSIBLE;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(ObjType.getFlag(UNMOVE)){
		throw NOTMOVABLE;
	}

	if(ObjType.isCreatureContainer() && Obj.getCreatureID() != CreatureID){
		TCreature *MovingCreature = GetCreature(Obj);
		if(MovingCreature == NULL){
			error("CheckMoveObject: Kreatur existiert nicht.\n");
			throw ERROR;
		}

		if(GetRaceUnpushable(MovingCreature->Race) && (WorldType != NON_PVP || !MovingCreature->IsPeaceful())){
			throw NOTMOVABLE;
		}
	}

	if(Take && !ObjType.getFlag(TAKE)){
		throw NOTTAKABLE;
	}
}

// NOTE(fusion): This is a helper function for `CheckMapDestination` and `CheckMapPlace`
// and improves the readability of otherwise two convoluted functions.
static bool IsMapBlocked(int DestX, int DestY, int DestZ, ObjectType Type){
	if(Type.isCreatureContainer()){
		return false;
	}

	bool HasBank = CoordinateFlag(DestX, DestY, DestZ, BANK);
	if(HasBank && !CoordinateFlag(DestX, DestY, DestZ, UNPASS)){
		return false;
	}

	if(!Type.getFlag(UNPASS)){
		if(HasBank && !CoordinateFlag(DestX, DestY, DestZ, UNLAY)){
			return false;
		}

		if(Type.getFlag(HANG)){
			bool HasHook = CoordinateFlag(DestX, DestY, DestZ, HOOKSOUTH)
						|| CoordinateFlag(DestX, DestY, DestZ, HOOKEAST);
			if(HasHook && !CoordinateFlag(DestX, DestY, DestZ, HANG)){
				return false;
			}
		}
	}

	return true;
}

void CheckMapDestination(uint32 CreatureID, Object Obj, Object MapCon){
	if(CreatureID == 0){
		return;
	}

	int DestX, DestY, DestZ;
	ObjectType ObjType = Obj.getObjectType();
	GetObjectCoordinates(MapCon, &DestX, &DestY, &DestZ);
	if(IsMapBlocked(DestX, DestY, DestZ, ObjType)){
		throw NOROOM;
	}

	int OrigX, OrigY, OrigZ;
	GetObjectCoordinates(Obj, &OrigX, &OrigY, &OrigZ);
	if(ObjType.isCreatureContainer()){
		if(std::abs(OrigX - DestX) > 1
				|| std::abs(OrigY - DestY) > 1
				|| std::abs(OrigZ - DestZ) > 1){
			throw OUTOFRANGE;
		}

		if(DestZ == (OrigZ - 1)){
			if(GetHeight(OrigX, OrigY, OrigZ) < 24){ // JUMP_HEIGHT ?
				throw NOROOM;
			}
		}else if(DestZ == (OrigZ + 1)){
			if(GetHeight(DestX, DestY, DestZ) < 24){ // JUMP_HEIGHT ?
				throw NOWAY;
			}
		}

		TCreature *MovingCreature = GetCreature(Obj);
		if(MovingCreature == NULL){
			error("CheckMapDestination: Moving creature is NULL.");
			throw ERROR;
		}

		if(OrigZ == DestZ || MovingCreature->Type != MONSTER){
			if(!MovingCreature->MovePossible(DestX, DestY, DestZ, true, OrigZ != DestZ)){
				if(CreatureID == MovingCreature->ID){
					throw MOVENOTPOSSIBLE;
				}else{
					throw NOROOM;
				}
			}

			if(CreatureID != MovingCreature->ID){
				if(CoordinateFlag(DestX, DestY, DestZ, AVOID)){
					throw NOROOM;
				}

				if(IsProtectionZone(OrigX, OrigY, OrigZ) && !IsProtectionZone(DestX, DestY, DestZ)){
					throw PROTECTIONZONE;
				}
			}
		}
	}else{
		if(!ObjType.getFlag(TAKE) && !ObjectInRange(CreatureID, MapCon, 2)){
			throw OUTOFRANGE;
		}
	}

	// TODO(fusion): This looks awfully similar to `ObjectAccessible` outside
	// from the exceptions and floor check.
	if(ObjType.getFlag(HANG)){
		bool HookSouth = CoordinateFlag(DestX, DestY, DestZ, HOOKSOUTH);
		bool HookEast = CoordinateFlag(DestX, DestY, DestZ, HOOKEAST);
		if(HookSouth || HookEast){
			TCreature *Creature = GetCreature(CreatureID);
			if(Creature == NULL){
				error("CheckMapDestination: Ausführende Kreatur existiert nicht.\n");
				throw ERROR;
			}

			if(Creature->posz > DestZ){
				throw UPSTAIRS;
			}else if(Creature->posz < DestZ){
				throw DOWNSTAIRS;
			}

			if(HookSouth){
				if(Creature->posy < DestY
						|| Creature->posy > (DestY + 1)
						|| Creature->posx < (DestX - 1)
						|| Creature->posx > (DestX + 1)){
					throw OUTOFRANGE;
				}
			}
			
			if(HookEast){
				if(Creature->posx < DestX
						|| Creature->posx > (DestX + 1)
						|| Creature->posy < (DestY - 1)
						|| Creature->posy > (DestY + 1)){
					throw OUTOFRANGE;
				}
			}

			return;
		}
	}

	if(!ThrowPossible(OrigX, OrigY, OrigZ, DestX, DestY, DestZ, 1)){
		throw CANNOTTHROW;
	}
}

void CheckMapPlace(uint32 CreatureID, ObjectType Type, Object MapCon){
	int DestX, DestY, DestZ;
	GetObjectCoordinates(MapCon, &DestX, &DestY, &DestZ);

	if(!Type.getFlag(UNMOVE) && !CoordinateFlag(DestX, DestY, DestZ, BANK)){
		if(!Type.getFlag(HANG)){
			throw NOROOM;
		}

		if(!CoordinateFlag(DestX, DestY, DestZ, HOOKSOUTH)
		&& !CoordinateFlag(DestX, DestY, DestZ, HOOKEAST)){
			throw NOROOM;
		}
	}

	// TODO(fusion): The original function used a slightly modified version of
	// `IsMapBlocked` which wouldn't check if there was already a HANG item at
	// the destination but since this function seems to only be called from
	// `Create` which sets `CreatureID` to zero, I assume this was probably an
	// oversight?
	if(CreatureID != 0 && IsMapBlocked(DestX, DestY, DestZ, Type)){
		throw NOROOM;
	}
}

void CheckContainerDestination(Object Obj, Object Con){
	if(IsHeldByContainer(Con, Obj)){
		throw CROSSREFERENCE;
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

void CheckContainerPlace(ObjectType Type, Object Con, Object OldObj){
	if(Type == GetSpecialObject(DEPOT_CHEST)){
		return;
	}

	ObjectType ConType = Con.getObjectType();
	if(!ConType.getFlag(CHEST)){
		if(Type.getFlag(UNMOVE)){
			throw NOTMOVABLE;
		}

		if(!Type.getFlag(TAKE)){
			throw NOTTAKABLE;
		}

		if(OldObj == NONE){
			int ConObjects = CountObjectsInContainer(Con);
			int ConCapacity = (int)ConType.getAttribute(CAPACITY);
			if(ConObjects >= ConCapacity){
				throw CONTAINERFULL;
			}
		}
	}
}

void CheckDepotSpace(uint32 CreatureID, Object Source, Object Destination, int Count){
	if(CreatureID == 0){
		return;
	}

	TCreature *Creature = GetCreature(CreatureID);
	if(Creature == NULL){
		error("CheckDepotSpace: Kreatur %u existiert nicht.\n", CreatureID);
		throw ERROR;
	}

	if(Destination == NONE){
		error("CheckDepotSpace: Ziel existiert nicht.\n");
		throw ERROR;
	}

	if(Creature->Type == PLAYER){
		TPlayer *Player = (TPlayer*)Creature;
		if(Player->Depot == NONE || Count <= Player->DepotSpace){
			return;
		}

		if(!IsHeldByContainer(Source, Player->Depot)
		&& IsHeldByContainer(Destination, Player->Depot)){
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
	if(HandContainer && ObjType.isTwoHanded()){
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

void CheckInventoryPlace(ObjectType Type, Object Con, Object OldObj){
	// TODO(fusion): This is awfully similar to `CheckInventoryDestination` with
	// a few subtle differences. Perhaps they use the same underlying function?

	int ConPosition = GetObjectBodyPosition(Con);
	bool HandContainer = ConPosition == INVENTORY_RIGHTHAND
					|| ConPosition == INVENTORY_LEFTHAND;

	if(!HandContainer && ConPosition != INVENTORY_AMMO){
		if(!Type.getFlag(CLOTHES)){
			throw WRONGPOSITION;
		}

		int TypePosition = (int)Type.getAttribute(BODYPOSITION);
		if(TypePosition == 0){
			throw WRONGPOSITION2;
		}else if(TypePosition != ConPosition){
			throw WRONGCLOTHES;
		}
	}

	uint32 CreatureID = GetObjectCreatureID(Con);
	if(HandContainer && Type.isTwoHanded()){
		Object RightHand = GetBodyObject(CreatureID, INVENTORY_RIGHTHAND);
		if(RightHand != NONE && RightHand != OldObj){
			throw HANDSNOTFREE;
		}

		Object LeftHand = GetBodyObject(CreatureID, INVENTORY_LEFTHAND);
		if(LeftHand != NONE && LeftHand != OldObj){
			throw HANDSNOTFREE;
		}

		return;
	}

	if(GetBodyObject(CreatureID, ConPosition) != NONE
	&& GetBodyObject(CreatureID, ConPosition) != OldObj){
		throw NOROOM;
	}

	if(HandContainer){
		for(int Position = INVENTORY_HAND_FIRST;
				Position <= INVENTORY_HAND_LAST;
				Position += 1){
			Object Other = GetBodyObject(CreatureID, Position);
			if(Other != NONE && Other != OldObj){
				ObjectType OtherType = Other.getObjectType();
				if(OtherType.isTwoHanded()){
					throw HANDBLOCKED;
				}

				if(OtherType.isWeapon() && Type.isWeapon()){
					throw ONEWEAPONONLY;
				}
			}
		}
	}
}

void CheckWeight(uint32 CreatureID, Object Obj, int Count){
	if(GetObjectCreatureID(Obj) == CreatureID){
		return;
	}

	TCreature *Creature = GetCreature(CreatureID);
	if(Creature == NULL){
		error("CheckWeight: Kreatur %d existiert nicht.\n", CreatureID);
		throw ERROR;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(!ObjType.getFlag(TAKE)){
		throw TOOHEAVY;
	}

	if(Creature->Type == PLAYER){
		if(CheckRight(CreatureID, UNLIMITED_CAPACITY)){
			return;
		}

		if(CheckRight(CreatureID, ZERO_CAPACITY)){
			throw TOOHEAVY;
		}
	}

	TSkill *CarryStrength = Creature->Skills[SKILL_CARRY_STRENGTH];
	if(CarryStrength == NULL){
		error("CheckWeight: Skill CARRYSTRENGTH existiert nicht.\n");
		throw ERROR;
	}

	int ObjWeight;
	if(ObjType.getFlag(CUMULATIVE)){
		ObjWeight = GetWeight(Obj, Count);
	}else{
		ObjWeight = GetCompleteWeight(Obj);
	}

	int InventoryWeight = GetInventoryWeight(CreatureID);
	int MaxWeight = CarryStrength->Get() * 100;
	if((InventoryWeight + ObjWeight) > MaxWeight){
		throw TOOHEAVY;
	}
}

void CheckWeight(uint32 CreatureID, ObjectType Type, uint32 Value, int OldWeight){
	TCreature *Creature = GetCreature(CreatureID);
	if(Creature == NULL){
		error("CheckWeight: Kreatur %d existiert nicht.\n", CreatureID);
		throw ERROR;
	}

	if(!Type.getFlag(TAKE)){
		throw TOOHEAVY;
	}

	if(Creature->Type == PLAYER){
		if(CheckRight(CreatureID, UNLIMITED_CAPACITY)){
			return;
		}

		if(CheckRight(CreatureID, ZERO_CAPACITY)){
			throw TOOHEAVY;
		}
	}

	TSkill *CarryStrength = Creature->Skills[SKILL_CARRY_STRENGTH];
	if(CarryStrength == NULL){
		error("CheckWeight: Skill CARRYSTRENGTH existiert nicht.\n");
		throw ERROR;
	}

	int TypeWeight = (int)Type.getAttribute(WEIGHT);
	if(Type.getFlag(CUMULATIVE) && (int)Value > 1){
		TypeWeight *= (int)Value;
	}

	if(TypeWeight > OldWeight){
		int InventoryWeight = GetInventoryWeight(CreatureID);
		int MaxWeight = CarryStrength->Get() * 100;
		if((InventoryWeight + TypeWeight) > MaxWeight){
			throw TOOHEAVY;
		}
	}
}

void NotifyCreature(uint32 CreatureID, Object Obj, bool Inventory){
	if(CreatureID == 0){
		return;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(!ObjType.isCreatureContainer()){
		TCreature *Creature = GetCreature(CreatureID);
		if(Creature == NULL){
			error("NotifyCreature: Kreatur existiert nicht.\n");
			return;
		}

		if(Inventory && ObjType.getFlag(LIGHT)){
			AnnounceChangedCreature(CreatureID, CREATURE_LIGHT_CHANGED);
		}

		Creature->NotifyChangeInventory();
	}
}

void NotifyCreature(uint32 CreatureID, ObjectType Type, bool Inventory){
	if(CreatureID == 0){
		return;
	}

	// TODO(fusion): Shouldn't we check if `Type` is creature container too?

	TCreature *Creature = GetCreature(CreatureID);
	if(Creature == NULL){
		error("NotifyCreature: Kreatur existiert nicht.\n");
		return;
	}

	if(Inventory && Type.getFlag(LIGHT)){
		AnnounceChangedCreature(CreatureID, CREATURE_LIGHT_CHANGED);
	}

	Creature->NotifyChangeInventory();
}

void NotifyAllCreatures(Object Obj, int Type, Object OldCon){
	if(!Obj.exists()){
		error("NotifyAllCreatures: Übergebenes Objekt existiert nicht.\n");
		return;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(!ObjType.isCreatureContainer()){
		return;
	}

	int ObjX, ObjY, ObjZ;
	uint32 CreatureID = Obj.getCreatureID();
	GetObjectCoordinates(Obj, &ObjX, &ObjY, &ObjZ);

	constexpr int StimulusRadius = 10;
	int SearchRadiusX = StimulusRadius;
	int SearchRadiusY = StimulusRadius;
	int SearchCenterX = ObjX;
	int SearchCenterY = ObjY;
	if(Type == OBJECT_MOVED){
		if(!OldCon.exists()){
			error("NotifyAllCreatures: Übergebener alter Container existiert nicht.\n");
			return;
		}

		// NOTE(fusion): Report stimulus as `OBJECT_CHANGED`.
		Type = OBJECT_CHANGED;

		// NOTE(fusion): Perform a secondary search if the distances are beyond
		// the stimulus diameter or, extend the primary search otherwise.
		int OldX, OldY, OldZ;
		GetObjectCoordinates(OldCon, &OldX, &OldY, &OldZ);
		int DistanceX = std::abs(ObjX - OldX);
		int DistanceY = std::abs(ObjY - OldY);
		if(DistanceX > (StimulusRadius * 2) || DistanceY > (StimulusRadius * 2)){
			TFindCreatures Search(StimulusRadius, StimulusRadius, OldX, OldY, FIND_ALL);
			while(true){
				uint32 SpectatorID = Search.getNext();
				if(SpectatorID == 0){
					break;
				}

				// TODO(fusion): Check if the spectator is NULL?
				TCreature *Spectator = GetCreature(SpectatorID);
				Spectator->CreatureMoveStimulus(CreatureID, Type);
			}
		}else{
			SearchRadiusX = StimulusRadius + (DistanceX + 1) / 2;
			SearchRadiusY = StimulusRadius + (DistanceY + 1) / 2;
			SearchCenterX = (ObjX + OldX) / 2;
			SearchCenterY = (ObjY + OldY) / 2;
		}
	}

	TFindCreatures Search(SearchRadiusX, SearchRadiusY, SearchCenterX, SearchCenterY, FIND_ALL);
	while(true){
		uint32 SpectatorID = Search.getNext();
		if(SpectatorID == 0){
			break;
		}

		// TODO(fusion): Check if the spectator is NULL?
		TCreature *Spectator = GetCreature(SpectatorID);
		Spectator->CreatureMoveStimulus(CreatureID, Type);
	}
}

void NotifyTrades(Object Obj){
	if(!Obj.exists()){
		error("NotifyTrades: Übergebenes Objekt existiert nicht.\n");
		return;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(ObjType.isCreatureContainer()){
		return;
	}

	// TODO(fusion): These radii seem to be correct but I'm skeptical because
	// you're required to be close to the trade object to even start the trade.
	// Maybe you can walk around after a trade is started?
	TFindCreatures Search(12, 10, Obj, FIND_PLAYERS);
	while(true){
		uint32 CharacterID = Search.getNext();
		if(CharacterID == 0){
			break;
		}

		TPlayer *Player = GetPlayer(CharacterID);
		if(Player == NULL){
			error("NotifyTrade: Spieler existiert nicht.\n");
			continue;
		}

		Object TradeObject = Player->InspectTrade(true, 0);
		if(TradeObject != NONE){
			if(IsHeldByContainer(Obj, TradeObject) || IsHeldByContainer(TradeObject, Obj)){
				SendCloseTrade(Player->Connection);
				SendMessage(Player->Connection, TALK_FAILURE_MESSAGE, "Trade cancelled.");
				Player->RejectTrade();
			}
		}
	}
}

void NotifyDepot(uint32 CreatureID, Object Obj, int Count){
	if(CreatureID == 0){
		return;
	}

	TCreature *Creature = GetCreature(CreatureID);
	if(Creature == NULL){
		error("NotifyDepot: Kreatur %u existiert nicht.\n", CreatureID);
		// TODO(fusion): We should probably also throw here.
		//throw ERROR;
		return;
	}

	if(!Obj.exists()){
		error("NotifyDepot: Objekt existiert nicht.\n");
		throw ERROR;
	}

	if(Creature->Type == PLAYER){
		TPlayer *Player = (TPlayer*)Creature;
		if(Player->Depot == NONE){
			return;
		}

		if(IsHeldByContainer(Obj, Player->Depot)){
			Player->DepotSpace += Count;
			print(3, "Neuer Depot-Freiraum für %s: %d\n",
					Player->Name, Player->DepotSpace);
		}
	}
}

void CloseContainer(Object Con, bool Force){
	if(!Con.exists()){
		error("CloseContainer: Übergebener Container existiert nicht.\n");
		return;
	}

	ObjectType ConType = Con.getObjectType();
	if(ConType.isCreatureContainer()){
		return;
	}

	// TODO(fusion): Similar to `NotifyTrades`.
	TFindCreatures Search(12, 10, Con, FIND_PLAYERS);
	while(true){
		uint32 CharacterID = Search.getNext();
		if(CharacterID == 0){
			break;
		}

		TPlayer *Player = GetPlayer(CharacterID);
		if(Player == NULL){
			error("CloseContainer: Spieler existiert nicht.\n");
			continue;
		}

		for(int ContainerNr = 0;
				ContainerNr < NARRAY(Player->OpenContainer);
				ContainerNr += 1){
			Object OpenCon = Player->GetOpenContainer(ContainerNr);
			if(IsHeldByContainer(OpenCon, Con)){
				if(Force || !ObjectAccessible(Player->ID, Con, 1)){
					Player->SetOpenContainer(ContainerNr, NONE);
					SendCloseContainer(Player->Connection, ContainerNr);
				}else{
					// TODO(fusion): Why are we doing this? Is the container being refreshed?
					SendContainer(Player->Connection, ContainerNr);
				}
			}
		}
	}
}

Object Create(Object Con, ObjectType Type, uint32 Value){
	if(!Con.exists()){
		error("Create: Zielobjekt existiert nicht.\n");
		throw ERROR;
	}

	ObjectType ConType = Con.getObjectType();
	if(!ConType.getFlag(CONTAINER) && !ConType.getFlag(CHEST)){
		error("Create: Zielobjekt ist kein Container.\n");
		throw ERROR;
	}

	// TODO(fusion): The map container does indeed use type id zero but it seems
	// it is also considered a "no type" id so we might also want to add an alias
	// to it as `isValid()`, `isVoid()`, `isNone()`, or `isNull()` perhaps.
	if(Type.isMapContainer()){
		error("Create: Objekttyp existiert nicht.\n");
		throw ERROR;
	}

	if(ConType.isMapContainer()){
		CheckMapPlace(0, Type, Con);
	}else if(ConType.isBodyContainer()){
		CheckInventoryPlace(Type, Con, NONE);
	}else{
		CheckContainerPlace(Type, Con, NONE);
	}

	uint32 ConOwnerID = GetObjectCreatureID(Con);
	if(ConOwnerID != 0){
		CheckWeight(ConOwnerID, Type, Value, 0);
	}

	// NOTE(fusion): Attempt to merge with top object.
	if(ConType.isMapContainer() && Type.getFlag(CUMULATIVE)){
		int ConX, ConY, ConZ;
		GetObjectCoordinates(Con, &ConX, &ConY, &ConZ);
		Object Top = GetTopObject(ConX, ConY, ConZ, true);
		if(Top != NONE && Top.getObjectType() == Type){
			uint32 Count = Top.getAttribute(AMOUNT);
			if(Value != 0){
				Count += Value;
			}else{
				Count += 1;
			}

			if(Count <= 100){
				Change(Top, AMOUNT, Count);
				return Top;
			}
		}
	}

	// TODO(fusion): I feel these usages of `Value` should be exclusive?

	Object Obj = SetObject(Con, Type, (Type.isCreatureContainer() ? Value : 0));

	if(Type.getFlag(CUMULATIVE)){
		ChangeObject(Obj, AMOUNT, Value);
	}

	if(Type.getFlag(MAGICFIELD)){
		ChangeObject(Obj, RESPONSIBLE, Value);
	}

	if(Type.getFlag(LIQUIDPOOL)){
		ChangeObject(POOLLIQUIDTYPE, Value);
	}

	if(Type.getFlag(LIQUIDCONTAINER)){
		ChangeObject(CONTAINERLIQUIDTYPE, Value);
	}

	if(Type.getFlag(KEY)){
		ChangeObject(KEYNUMBER, Value);
	}

	if(Type.getFlag(RUNE)){
		ChangeObject(CHARGES, Value);
	}

	if(Type.isCreatureContainer()){
		// BUG(fusion): We should just check this before creating the object.
		// Also, using `Delete` instead of `DeleteObject` here is problematic
		// because the object's placement wasn't yet broadcasted.
		TCreature *Creature = GetCreature(Value);
		if(Creature == NULL){
			error("Create: Ungültige Kreatur-ID %u\n", Value);
			Delete(Obj, -1);
			throw ERROR;
		}

		// IMPORTANT(fusion): Creature body containers are created here and their
		// type ids match inventory slots exactly. We're looping in reverse because
		// `SetObject` will insert objects at the front of the object chain.
		for(int Position = INVENTORY_LAST;
				Position >= INVENTORY_FIRST;
				Position -= 1){
			SetObject(Obj, ObjectType(Position), 0);
		}

		Creature->NotifyCreate();
	}

	AnnounceChangedObject(Obj, OBJECT_CREATED);
	NotifyTrades(Obj);
	NotifyCreature(ConOwnerID, Obj, ConType.isBodyContainer());
	MovementEvent(Obj, Con, Con); // TODO(fusion): This one doesn't make a lot of sense.
	CollisionEvent(Obj, Con);
	NotifyAllCreatures(Obj, OBJECT_CREATED, NONE);
	return Obj;
}

Object Copy(Object Con, Object Source){
	if(!Con.exists()){
		error("Copy: Ziel existiert nicht.\n");
		throw ERROR;
	}

	ObjectType ConType = Con.getObjectType();
	if(!ConType.getFlag(CONTAINER) && !ConType.getFlag(CHEST)){
		error("Copy: Ziel ist kein Container.\n");
		throw ERROR;
	}

	if(!Source.exists()){
		error("Copy: Quelle existiert nicht.\n");
		throw ERROR;
	}

	ObjectType SourceType = Source.getObjectType();
	if(SourceType.isCreatureContainer()){
		error("Copy: Quelle ist eine Kreatur.\n");
		throw ERROR;
	}

	if(ConType.isMapContainer()){
		CheckMapDestination(0, Source, Con);
	}else if(ConType.isBodyContainer()){
		// TODO(fusion): Not sure about this third param.
		CheckInventoryDestination(Source, Con, true);
	}else{
		CheckContainerDestination(Source, Con);
	}

	uint32 ConOwnerID = GetObjectCreatureID(Con);
	if(ConOwnerID != 0){
		CheckWeight(ConOwnerID, Source, -1);
	}

	// TODO(fusion): Using `Copy` recursively here should work because the object
	// is placed when `CopyObject` is called. The thing is, announce and notify
	// functions will do meaningless work there (I think, check what happens when
	// we're actually running).
	Object Obj = CopyObject(Con, Source);
	if(SourceType.getFlag(CONTAINER)){
		Object Help = GetFirstContainerObject(Source);
		while(Help != NONE){
			Copy(Obj, Help);
			Help = Help.getNextObject();
		}
	}

	AnnounceChangedObject(Obj, OBJECT_CREATED);
	NotifyTrades(Obj);
	NotifyCreature(ConOwnerID, Obj, ConType.isBodyContainer());
	MovementEvent(Obj, Con, Con); // TODO(fusion): Same as `Create`.
	CollisionEvent(Obj, Con);
	NotifyAllCreatures(Obj, OBJECT_CREATED, NONE);
	return Obj;
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

	// NOTE(fusion): Refresh object type just in case a merge event modified it.
	ObjType = Obj.getObjectType();

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

	// NOTE(fusion): `Obj` becomes the split part while `Remainder`, the remaining part.
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
		AnnounceChangedObject(Obj, OBJECT_DELETED);
		NotifyTrades(Obj);
		NotifyDepot(CreatureID, Obj, CountObjects(Obj));
	}

	// TODO(fusion): This could probably be moved to the end of the function
	// along with the other `CloseContainer`.
	//	Probably the other way around, since events could modify the object
	// and I don't think it makes a difference if we close it before or after
	// performing the move.
	//	Except it does because the object's position will be different lol.
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
		AnnounceChangedObject(Obj, OBJECT_CREATED);
		NotifyTrades(Obj);
		NotifyDepot(CreatureID, Obj, -CountObjects(Obj));
	}

	if(Remainder != NONE){
		AnnounceChangedObject(Remainder, OBJECT_CHANGED);
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
	NotifyAllCreatures(Obj, OBJECT_MOVED, OldCon);
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
		AnnounceChangedObject(Obj, OBJECT_CHANGED);
		NotifyTrades(Obj);
		ChangeObject(Dest, AMOUNT, DestCount + Count);
	}else{
		AnnounceChangedObject(Obj, OBJECT_DELETED);
		NotifyTrades(Obj);
		NotifyDepot(CreatureID, Obj, 1);
		MergeObjects(Obj, Dest);
	}

	AnnounceChangedObject(Dest, OBJECT_CHANGED);
	NotifyTrades(Dest);
	NotifyCreature(ObjOwnerID, Dest, ObjCon.getObjectType().isBodyContainer());
	NotifyCreature(DestOwnerID, Dest, DestCon.getObjectType().isBodyContainer());
	CollisionEvent(Dest, DestCon);
	NotifyAllCreatures(Dest, OBJECT_CHANGED, NONE);
}

void Change(Object Obj, ObjectType NewType, uint32 Value){
	if(Obj.exists()){
		error("Change: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	ObjectType OldType = Obj.getObjectType();
	if(OldType.isCreatureContainer() || NewType.isCreatureContainer()){
		error("Change: Kann keine Kreaturen verändern.\n");
		throw ERROR;
	}

	// TODO(fusion): Same thing as `Create`. The zero type id is also used as
	// a "no type" id.
	if(NewType.isMapContainer()){
		Delete(Obj, -1);
		return;
	}

	if(OldType.getFlag(CONTAINER)){
		if(!NewType.getFlag(CONTAINER) && GetFirstContainerObject(Obj) != NONE){
			error("Change: Zielobjekt %d für %d ist kein Container mehr.\n",
					NewType.TypeID, OldType.TypeID);
			throw EMPTYCONTAINER;
		}

		if(NewType.getFlag(CONTAINER)){
			int ObjectCount = CountObjectsInContainer(Obj);
			int NewCapacity = (int)NewType.getAttribute(CAPACITY);
			if(ObjectCount < NewCapacity){
				error("Change: Zielobjekt %d für %d ist kleinerer Container.\n",
						NewType.TypeID, OldType.TypeID);
				throw EMPTYCONTAINER;
			}
		}
	}

	if(OldType.getFlag(CHEST) && !NewType.getFlag(CHEST)){
		error("Change: Schatztruhe muß Schatztruhe bleiben.\n");
		throw ERROR;
	}

	if(OldType.getFlag(CUMULATIVE)
			&& OldType != NewType
			&& Obj.getAttribute(AMOUNT) > 1){
		throw SPLITOBJECT;
	}

	Object Con = Obj.getContainer();
	ObjectType ConType = Con.getObjectType();
	uint32 ObjOwnerID = GetObjectCreatureID(Obj);
	if(ObjOwnerID != 0 && OldType != NewType){
		if(ConType.isBodyContainer()){
			CheckInventoryPlace(NewType, Con, Obj);
		}else{
			CheckContainerPlace(NewType, Con, Obj);
		}

		uint32 Count = 1;
		if(OldType.getFlag(CUMULATIVE)){
			Count = Obj.getAttribute(AMOUNT);
		}

		int OldWeight = GetWeight(Obj, -1);
		CheckWeight(ObjOwnerID, NewType, Count, OldWeight);
	}

	if(OldType.getFlag(CONTAINER) && !NewType.getFlag(CONTAINER)){
		CloseContainer(Obj, true);
	}

	ChangeObject(Obj, NewType);

	// TODO(fusion): Same thing as `Create`. I feel these usages of `Value`
	// should be exclusive.

	if(NewType.getFlag(CUMULATIVE) && !OldType.getFlag(CUMULATIVE)){
		ChangeObject(Obj, AMOUNT, Value);
	}

	if(NewType.getFlag(MAGICFIELD) && !OldType.getFlag(MAGICFIELD)){
		ChangeObject(Obj, RESPONSIBLE, Value);
	}

	if(NewType.getFlag(LIQUIDPOOL) && !OldType.getFlag(LIQUIDPOOL)){
		ChangeObject(Obj, POOLLIQUIDTYPE, Value);
	}

	if(NewType.getFlag(LIQUIDCONTAINER) && !OldType.getFlag(LIQUIDCONTAINER)){
		ChangeObject(Obj, CONTAINERLIQUIDTYPE, Value);
	}

	if(NewType.getFlag(KEY) && !OldType.getFlag(KEY)){
		ChangeObject(Obj, KEYNUMBER, Value);
	}

	if(NewType.getFlag(RUNE) && !OldType.getFlag(RUNE)){
		ChangeObject(Obj, CHARGES, Value);
	}

	AnnounceChangedObject(Obj, OBJECT_CHANGED);
	NotifyTrades(Obj);
	NotifyCreature(ObjOwnerID, Obj, ConType.isBodyContainer());
	NotifyAllCreatures(Obj, OBJECT_CHANGED, NONE);
}

void Change(Object Obj, INSTANCEATTRIBUTE Attribute, uint32 Value){
	if(!Obj.exists()){
		error("Change: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	if(Attribute == CONTENT
			|| Attribute == CHESTQUESTNUMBER
			|| Attribute == DOORLEVEL
			|| Attribute == DOORQUESTNUMBER
			|| Attribute == DOORQUESTVALUE
			|| Attribute == ABSTELEPORTDESTINATION
			|| Attribute == REMAININGEXPIRETIME){
		error("Change: Kann Attribut %d nicht ändern.\n", Attribute);
		throw ERROR;
	}

	uint32 ObjOwnerID = GetObjectCreatureID(Obj);
	if(ObjOwnerID != 0 && Attribute == AMOUNT && Value > Obj.getAttribute(AMOUNT)){
		int OldWeight = GetWeight(Obj, -1);
		CheckWeight(ObjOwnerID, Obj.getObjectType(), Value, OldWeight);
	}

	ChangeObject(Obj, Attribute, Value);
	if(Attribute == AMOUNT
			|| Attribute == CONTAINERLIQUIDTYPE
			|| Attribute == POOLLIQUIDTYPE){
		AnnounceChangedObject(Obj, OBJECT_CHANGED);
	}

	NotifyTrades(Obj);
	NotifyCreature(ObjOwnerID, Obj, Obj.getContainer().getObjectType().isBodyContainer());
}

void Delete(Object Obj, int Count){
	if(!Obj.exists()){
		error("Delete: Übergebenes Objekt existiert nicht.\n");
		throw ERROR;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(ObjType.getFlag(CUMULATIVE) && Count > (int)Obj.getAttribute(AMOUNT)){
		error("Delete: Ungültige Anzahl %d an Teilen bei kumulierbarem Objekt.\n", Count);
		throw ERROR;
	}

	Object Remainder = NONE;
	if(ObjType.getFlag(CUMULATIVE) && Count != -1
			&& Count < (int)Obj.getAttribute(AMOUNT)){
		Remainder = Obj;
		Obj = SplitObject(Obj, Count);
	}else{
		NotifyTrades(Obj);
	}

	if(ObjType.getFlag(CONTAINER) || ObjType.getFlag(CHEST)){
		if(ObjType.getFlag(CONTAINER)){
			CloseContainer(Obj, true);
		}

		Object Help = GetFirstContainerObject(Obj);
		while(Help != NONE){
			Object Next = Help.getNextObject();
			DeleteObject(Help);
			Help = Next;
		}
	}

	Object Con = Obj.getContainer();
	uint32 ObjOwnerID = GetObjectCreatureID(Obj);
	SeparationEvent(Obj, Con);

	// BUG(fusion): Object may have been destroyed by the separation event? This
	// looks suspicious because even if the object is properly destroyed, its
	// remainder could be left unannounced.
	//	If it's a container, its objects don't get a separation event trigger,
	// and the separation event trigger on the container itself is only triggered
	// after it is empty.
	if(Obj.exists()){
		if(Remainder == NONE){
			AnnounceChangedObject(Obj, OBJECT_DELETED);
		}

		NotifyAllCreatures(Obj, OBJECT_DELETED, NONE);
		if(ObjType.isCreatureContainer()){
			TCreature *Creature = GetCreature(ObjOwnerID);
			if(Creature != NULL){
				Creature->NotifyDelete();
			}else{
				error("Delete: Kreatur mit ungültiger ID %d.\n", ObjOwnerID);
			}
		}

		DeleteObject(Obj);

		if(Remainder != NONE){
			AnnounceChangedObject(Remainder, OBJECT_CHANGED);
			NotifyTrades(Remainder);
		}

		NotifyCreature(ObjOwnerID, ObjType, Con.getObjectType().isBodyContainer());
	}
}

void Empty(Object Con, int Remainder){
	if(!Con.exists()){
		error("Empty: Übergebener Container existiert nicht.\n");
		throw ERROR;
	}

	if(Remainder < 0){
		error("Empty: Ungültiger Rest %d.\n", Remainder);
		throw ERROR;
	}

	ObjectType ConType = Con.getObjectType();
	if(!ConType.getFlag(CONTAINER)){
		error("Empty: Übergebenes Objekt ist kein Container.\n");
		throw ERROR;
	}

	CloseContainer(Con, true);

	bool IsCorpse = ConType.getFlag(CORPSE);
	int ObjectCount = CountObjectsInContainer(Con);
	Object Help = GetFirstContainerObject(Con);
	while(Help != NONE && ObjectCount > Remainder){
		Object Next = Help.getNextObject();
		if(IsCorpse){
			Delete(Help, -1);
		}else{
			Move(0, Help, Con.getContainer(), -1, false, NONE);
		}
		Help = Next;
		ObjectCount -= 1;
	}
}

void GraphicalEffect(int x, int y, int z, int Type){
	AnnounceGraphicalEffect(x, y, z, Type);
}

void GraphicalEffect(Object Obj, int Type){
	if(Obj.exists()){
		int ObjX, ObjY, ObjZ;
		GetObjectCoordinates(Obj, &ObjX, &ObjY, &ObjZ);
		AnnounceGraphicalEffect(ObjX, ObjY, ObjZ, Type);
	}
}

void TextualEffect(Object Obj, int Color, const char *Format, ...){
	if(!Obj.exists()){
		error("TextualEffect: Übergebener Mapcontainer existiert nicht.\n");
		return;
	}

	char Buffer[10];
	va_list ap;
	va_start(ap, Format);
	vsnprintf(Buffer, sizeof(Buffer), Format, ap);
	va_end(ap);

	int ObjX, ObjY, ObjZ;
	GetObjectCoordinates(Obj, &ObjX, &ObjY, &ObjZ);
	AnnounceTextualEffect(ObjX, ObjY, ObjZ, Color, Buffer);
}

void Missile(Object Start, Object Dest, int Type){
	if(!Start.exists()){
		error("Missile: Übergebener Startpunkt existiert nicht.\n");
		return;
	}

	if(!Dest.exists()){
		error("Missile: Übergebener Zielpunkt existiert nicht.\n");
		return;
	}

	int StartX, StartY, StartZ;
	int DestX, DestY, DestZ;
	GetObjectCoordinates(Start, &StartX, &StartY, &StartZ);
	GetObjectCoordinates(Dest, &DestX, &DestY, &DestZ);
	AnnounceMissile(StartX, StartY, StartZ, DestX, DestY, DestZ, Type);
}

void Look(uint32 CreatureID, Object Obj){
	// TODO(fusion): Honestly one of the first things we should focus on, after
	// the server is running, is to implement a small stack buffer writer/formatter
	// to retire `strcpy`, `strcat`, and `sprintf`.

	TPlayer *Player = GetPlayer(CreatureID);
	if(Player == NULL){
		error("Look: Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(!Obj.exists()){
		error("Look: Objekt existiert nicht.\n");
		throw ERROR;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(ObjType.isCreatureContainer()){
		TCreature *Target = GetCreature(Obj);
		if(Target == NULL){
			error("Look: Object %d hat keine Kreatur!\n", ObjType.TypeID);
			return;
		}

		if(Target->Type == PLAYER){
			TPlayer *TargetPlayer = (TPlayer*)Target;
			bool Yourself = (CreatureID == TargetPlayer->ID);
			char Pronoun[4] = {};
			char Name[50] = {};
			char Vocation[50] = {};

			if(TargetPlayer->Sex == 1){
				strcpy(Pronoun, "He");
			}else{
				strcpy(Pronoun, "She");
			}

			uint8 Profession = TargetPlayer->GetActiveProfession();
			if(Yourself){
				strcpy(Name, "yourself");

				if(Profession != PROFESSION_NONE){
					char Help[30];
					GetProfessionName(Help, Profession, true, false);
					snprintf(Vocation, sizeof(Vocation), "You are %s", Help);
				}else{
					strcpy(Vocation, "You have no vocation");
				}
			}else{
				int Level = 1;
				if(TargetPlayer->Skills[SKILL_LEVEL] != NULL){
					Level = TargetPlayer->Skills[SKILL_LEVEL]->Get();
				}
				snprintf(Name, sizeof(Name), "%s (Level %d)", TargetPlayer->Name, Level);

				if(Profession != PROFESSION_NONE){
					char Help[30];
					GetProfessionName(Help, Profession, true, false);
					snprintf(Vocation, sizeof(Vocation), "%s is %s", Pronoun, Help);
				}else{
					snprintf(Vocation, sizeof(Vocation), "%s has no vocation", Pronoun);
				}
			}

			if(TargetPlayer->Guild[0] != 0){
				char Membership[200] = {};
				if(Yourself){
					strcpy(Membership, "You are ");
				}else{
					snprintf(Membership, sizeof(Membership), "%s id ", Pronoun);
				}

				if(TargetPlayer->Rank[0] != 0){
					strcat(Membership, TargetPlayer->Rank);
				}else{
					strcat(Membership, "a member");
				}

				strcat(Membership, " of the ");
				strcat(Membership, TargetPlayer->Guild);
				if(TargetPlayer->Title[0] != 0){
					strcat(Membership, " (");
					strcat(Membership, TargetPlayer->Title);
					strcat(Membership, ")");
				}

				SendMessage(Player->Connection, TALK_INFO_MESSAGE,
						"You see %s. %s. %s.", Name, Vocation, Membership);
			}else{
				SendMessage(Player->Connection, TALK_INFO_MESSAGE,
						"You see %s. %s.", Name, Vocation);
			}
		}else{
			SendMessage(Player->Connection, TALK_INFO_MESSAGE, "You see %s.", Target->Name);
		}
	}else{
		char Description[500] = {};
		char Help[500] = {};

		snprintf(Description, sizeof(Description), "You see %s", GetName(Obj));

		if(ObjType.getFlag(LEVELDOOR)){
			snprintf(Help, sizeof(Help), " for level %u",
					Obj.getAttribute(DOORLEVEL));
			strcat(Description, Help);
		}

		if(ObjType.getFlag(CONTAINER)){
			snprintf(Help, sizeof(Help), " (Vol:%u)",
					ObjType.getAttribute(CAPACITY));
			strcat(Description, Help);
		}

		if(ObjType.getFlag(WEAPON)){
			snprintf(Help, sizeof(Help), " (Atk:%u Def:%u)",
					ObjType.getAttribute(WEAPONATTACKVALUE),
					ObjType.getAttribute(WEAPONDEFENDVALUE));
			strcat(Description, Help);
		}

		if(ObjType.getFlag(THROW)){
			snprintf(Help, sizeof(Help), " (Atk:%u Def:%u)",
					ObjType.getAttribute(THROWATTACKVALUE),
					ObjType.getAttribute(THROWDEFENDVALUE));
			strcat(Description, Help);
		}

		if(ObjType.getFlag(SHIELD)){
			snprintf(Help, sizeof(Help), " (Def:%u)",
					ObjType.getAttribute(SHIELDDEFENDVALUE));
			strcat(Description, Help);
		}

		if(ObjType.getFlag(ARMOR)){
			snprintf(Help, sizeof(Help), " (Arm:%u)",
					ObjType.getAttribute(ARMORVALUE));
			strcat(Description, Help);
		}

		if(ObjType.getFlag(KEY)){
			snprintf(Help, sizeof(Help), " (Key:%04u)",
					Obj.getAttribute(KEYNUMBER));
			strcat(Description, Help);
		}

		if(ObjType.getFlag(KEYDOOR) && CheckRight(Player->ID, SHOW_KEYHOLE_NUMBERS)){
			snprintf(Help, sizeof(Help), " (Door:%04u)",
					Obj.getAttribute(KEYHOLENUMBER));
			strcat(Description, Help);
		}

		if(ObjType.getFlag(SHOWDETAIL)){
			if(ObjType.getFlag(WEAROUT) && ObjType.getAttribute(TOTALUSES) > 1){
				uint32 RemainingUses = Obj.getAttribute(REMAININGUSES);
				snprintf(Help, sizeof(Help), " that has %u charge%s left",
						RemainingUses, (RemainingUses != 1 ? "s" : ""));
				strcat(Description, Help);
			}else if(ObjType.getFlag(EXPIRE) || ObjType.getFlag(EXPIRESTOP)){
				uint32 SecondsLeft;
				if(ObjType.getFlag(EXPIRE)){
					SecondsLeft = CronInfo(Obj, false);
				}else{
					SecondsLeft = Obj.getAttribute(SAVEDEXPIRETIME);
				}

				uint32 MinutesLeft = (SecondsLeft + 59) / 60;
				if(MinutesLeft == 0){
					snprintf(Help, sizeof(Help), " that is brand-new");
				}else{
					snprintf(Help, sizeof(Help), " that has energy for %u minutes%s left",
							MinutesLeft, (MinutesLeft != 1 ? "s" : ""));
				}
				strcat(Description, Help);
			}else{
				error("Look: Objekt %d hat Flag SHOWDETAIL, aber weder WEAROUT noch EXPIRE.\n", ObjType.TypeID);
			}
		}

		if(ObjType.getFlag(RESTRICTLEVEL) || ObjType.getFlag(RESTRICTPROFESSION)){
			strcat(Description, ".\nIt can only be wielded by ");
			if(ObjType.getFlag(RESTRICTPROFESSION)){
				int ProfessionCount = 0;
				uint32 ProfessionMask = ObjType.getAttribute(PROFESSIONS);

				if(ProfessionMask & 1){
					ProfessionCount += 1;
					strcpy(Help, "players without vocation");
				}else{
					strcpy(Help, "");
				}

				// NOTE(fusion): We're iterating professions in reverse because
				// the string is built in reverse to avoid extra processing for
				// adding commas and the final "and ...".
				for(int Profession = 4; Profession >= 1; Profession -= 1){
					if((ProfessionMask & (1 << Profession)) == 0){
						continue;
					}

					char Temp[200] = {};
					if(ProfessionCount == 1){
						snprintf(Temp, sizeof(Temp), " and %s", Help);
					}else if(ProfessionCount > 1){
						snprintf(Temp, sizeof(Temp), ", %s", Help);
					}

					switch(Profession){
						case PROFESSION_KNIGHT:		strcpy(Help, "knights"); break;
						case PROFESSION_PALADIN:	strcpy(Help, "paladins"); break;
						case PROFESSION_SORCERER:	strcpy(Help, "sorcerers"); break;
						case PROFESSION_DRUID:		strcpy(Help, "druids"); break;
					}

					strcat(Help, Temp);
				}
			}else{
				strcpy(Help, "players");
			}

			if(ObjType.getFlag(RESTRICTLEVEL)){
				char Temp[200] = {};
				snprintf(Temp, sizeof(Temp), " of level %u or higher",
						ObjType.getAttribute(MINIMUMLEVEL));
				strcat(Help, Temp);
			}

			strcat(Description, Help);
		}

		if(ObjType.getFlag(RUNE)){
			int MagicLevel;
			GetMagicItemDescription(Obj, Help, &MagicLevel);
			if(MagicLevel != 0){
				char Temp[200] = {};
				snprintf(Temp, sizeof(Temp), " for magic level %d", MagicLevel);
				strcat(Description, Temp);
			}

			strcat(Description, ". It\'s an \"");
			strcat(Description, Help);
			strcat(Description, "\"-spell");

			snprintf(Help, sizeof(Help), " (%ux)", Obj.getAttribute(CHARGES));
			strcat(Description, Help);
		}

		if(ObjType.getFlag(BED)){
			uint32 Sleeper = Obj.getAttribute(TEXTSTRING);
			if(Sleeper != 0){
				snprintf(Help, sizeof(Help), ". %s is sleeping there",
						GetDynamicString(Sleeper));
				strcat(Description, Help);
			}
		}

		if(ObjType.getFlag(NAMEDOOR)){
			int ObjX, ObjY, ObjZ;
			GetObjectCoordinates(Obj, &ObjX, &ObjY, &ObjZ);

			uint16 HouseID = GetHouseID(ObjX, ObjY, ObjZ);
			if(HouseID != 0){
				// TODO(fusion): Make `GetHouseOwner` return "Nobody" when there
				// is no owner name.
				snprintf(Help, sizeof(Help),
						". It belongs to house \'%s\'. %s owns this house",
						GetHouseName(HouseID), GetHouseOwner(HouseID));
				strcat(Description, Help);
			}else{
				error("Look: NameDoor auf [%d,%d,%d] gehört zu keinem Haus.\n", ObjX, ObjY, ObjZ);
			}
		}

		strcat(Description, ".");

		if((ObjType.getFlag(LIQUIDCONTAINER) && Obj.getAttribute(CONTAINERLIQUIDTYPE) == LIQUID_NONE)
		|| (ObjType.getFlag(LIQUIDPOOL)      && Obj.getAttribute(POOLLIQUIDTYPE)      == LIQUID_NONE)){
			strcat(Description, " It is empty.");
		}

		// TODO(fusion): This looks like an inlined function because we'd have
		// already returned at the beginning if `CreatureID` was zero.
		if(CreatureID == 0 || ObjectInRange(CreatureID, Obj, 1)){
			if(ObjType.getFlag(TAKE) && GetWeight(Obj, -1) > 0){
				int ObjWeight = GetCompleteWeight(Obj);
				if(ObjType.getFlag(CUMULATIVE)
						&& Obj.getAttribute(AMOUNT) > 1
						&& IsCountable(ObjType.getName(1))){
					snprintf(Help, sizeof(Help), "\nThey weigh %d.%02d oz.",
							ObjWeight / 100, ObjWeight % 100);
				}else{
					snprintf(Help, sizeof(Help), "\nIt weighs %d.%02d oz.",
							ObjWeight / 100, ObjWeight % 100);
				}
				strcat(Description, Help);
			}

			const char *ObjInfo = GetInfo(Obj);
			if(ObjInfo != NULL){
				snprintf(Help, sizeof(Help), "\n%s.", ObjInfo);
				strcat(Description, Help);
			}
		}

		if(ObjType.getFlag(TEXT)){
			int FontSize = (int)ObjType.getAttribute(FONTSIZE);
			if(FontSize == 0){
				uint32 Text = Obj.getAttribute(TEXTSTRING);
				if(Text != 0){
					snprintf(Help, sizeof(Help), "\n%s.", GetDynamicString(Text));
					strcat(Description, Help);
				}
			}else if(FontSize >= 2){
				uint32 Text = Obj.getAttribute(TEXTSTRING);
				if(Text != 0){
					// TODO(fusion): Again, `CreatureID` can't be zero here.
					if(CreatureID == 0 || ObjectInRange(CreatureID, Obj, FontSize)){
						uint32 Editor = Obj.getAttribute(EDITOR);
						if(Editor != 0){
							snprintf(Help, sizeof(Help), "\n%s has written: %s",
									GetDynamicString(Editor),
									GetDynamicString(Text));
						}else{
							snprintf(Help, sizeof(Help), "\nYou read: %s",
									GetDynamicString(Text));
						}
						strcat(Description, Help);
					}else{
						strcat(Description, "\nYou are too far away to read it.");
					}
				}else{
					strcat(Description, "\nNothing is written on it.");
				}
			}
		}

		SendMessage(Player->Connection, TALK_INFO_MESSAGE, "%s", Description);
	}
}

void Talk(uint32 CreatureID, int Mode, const char *Addressee, const char *Text, bool CheckSpamming){
	TCreature *Creature = GetCreature(CreatureID);
	if(Creature == NULL){
		error("Talk: Übergebene Kreatur %d existiert nicht.\n", CreatureID);
		throw ERROR;
	}

	if(Text == NULL){
		error("Talk: Text ist NULL (Sprecher: %s, Modus: %d).\n", Creature->Name, Mode);
		throw ERROR;
	}

	int TextLen = (int)strlen(Text);
	if(TextLen >= 256){
		error("Talk: Text ist zu lang (Sprecher: %s, Länge: %d).\n", Creature->Name, TextLen);
		throw ERROR;
	}

	int Channel = 0;
	if(Mode == TALK_CHANNEL_CALL
			|| Mode == TALK_GAMEMASTER_CHANNELCALL
			|| Mode == TALK_HIGHLIGHT_CHANNELCALL
			|| Mode == TALK_ANONYMOUS_CHANNELCALL){
		// TODO(fusion): Check if `Addressee` is NULL?
		Channel = atoi(Addressee);
		if(Channel < 0 || Channel >= Channels){
			error("Talk: Ungültiger Kanal %d.\n", Channel);
			return;
		}
	}

	print(2, "%s: \"%s\"\n", Creature->Name, Text);

	if(Creature->Type == PLAYER){
		TPlayer *Player = (TPlayer*)Creature;

		// TODO(fusion): Some `IsTalkMuteable` inlined function?
		bool TalkMuteable = Mode == TALK_SAY
				|| Mode == TALK_WHISPER
				|| Mode == TALK_YELL
				|| Mode == TALK_PRIVATE_MESSAGE
				|| ((Mode == TALK_CHANNEL_CALL
					|| Mode == TALK_HIGHLIGHT_CHANNELCALL)
						&& Channel >= 1 && Channel <= 7);

		int Muting = 0;
		if(CheckSpamming){
			Muting = Player->CheckForMuting();
			if(Muting > 0 && TalkMuteable){
				SendMessage(Player->Connection, TALK_FAILURE_MESSAGE,
						"You are still muted for %d second%s.",
						Muting, (Muting != 1 ? "s" : ""));
				return;
			}
		}

		int SpellType = 0;
		if(Muting == 0){
			SpellType = CheckForSpell(CreatureID, Text);
			if(SpellType < 0 || SpellType == 1 || SpellType == 5){
				return;
			}

			if(SpellType != 0){
				Mode = TALK_SAY;
			}
		}

		if(Mode == TALK_YELL){
			if(Player->EarliestYellRound > RoundNr){
				throw EXHAUSTED;
			}

			// TODO(fusion): This would be valid if `Text` was `char*`, but I
			// wanted to improve the quality of string handling and parameters
			// across the board, and now we need some scratch buffer to actually
			// modify the text.
			strUpper(Text);
		}

		if(Mode == TALK_CHANNEL_CALL && Channel == 5){
			if(Player->EarliestTradeChannelRound > RoundNr){
				SendMessage(Player->Connection, TALK_FAILURE_MESSAGE,
						"You may only place one offer in two minutes.");
				return;
			}

			Player->EarliestTradeChannelRound = RoundNr + 120;
		}

		if(CheckSpamming && SpellType == 0 && TalkMuteable
				&& !CheckRight(Player->ID, NO_BANISHMENT)){
			Muting = Player->RecordTalk();
			if(Muting > 0){
				SendMessage(Player->Connection, TALK_FAILURE_MESSAGE,
						"You are muted for %d second%s.",
						Muting, (Muting != 1 ? "s" : ""));
				return;
			}
		}
	}

	if(Mode == TALK_PRIVATE_MESSAGE
			|| Mode == TALK_GAMEMASTER_ANSWER
			|| Mode == TALK_PLAYER_ANSWER
			|| Mode == TALK_GAMEMASTER_MESSAGE
			|| Mode == TALK_GAMEMASTER_MESSAGE
			|| Mode == TALK_ANONYMOUS_MESSAGE){
		// TODO(fusion): These assumed the creature was a player but it wasn't checked.
		if(Creature->Type != PLAYER){
			error("Talk: Expected player creature for message talk modes.");
			throw ERROR;
		}

		if(Addressee == NULL){
			error("Talk: Adressat für Botschaft nicht angegeben.\n");
			throw ERROR;
		}

		TPlayer *Player = (TPlayer*)Creature;
		TPlayer *Receiver;
		bool IgnoreGamemasters = !CheckRight(Player->ID, READ_GAMEMASTER_CHANNEL);
		switch(IdentifyPlayer(Addressee, false, IgnoreGamemasters, &Receiver)){
			case  0:	break; // PLAYERFOUND ?
			case -1:	throw PLAYERNOTONLINE;
			case -2:	throw NAMEAMBIGUOUS;
			default:{
				error("Talk: Ungültiger Rückgabewert von IdentifyPlayer.\n");
				throw ERROR;
			}
		}

		if(Mode == TALK_PRIVATE_MESSAGE){
			Muting = Player->RecordMessage(Receiver->ID);
			if(Muting > 0){
				SendMessage(Player->Connection, TALK_FAILURE_MESSAGE,
						"You have addressed too many players. You are muted for %d second%s.",
						Muting, (Muting != 1 ? "s" : ""));
				return;
			}
		}

		if(Mode != TALK_ANONYMOUS_MESSAGE){
			uint32 StatementID = LogCommunication(CreatureID, Mode, 0, Text);
			LogListener(StatementID, Player);
			SendTalk(Receiver->Connection, LogListener(StatementID, Receiver),
					(Mode == TALK_GAMEMASTER_ANSWER ? "Gamemaster" : Player->Name),
					Mode, Text, 0);
		}else{
			SendMessage(Receiver->Connection, TALK_ADMIN_MESSAGE, "%s", Text);
		}

		SendMessage(Player->Connection, TALK_FAILURE_MESSAGE, "Message sent to %s.",
				(Mode == TALK_PLAYER_ANSWER ? "Gamemaster" : Receiver->Name));
		return;
	}else if(Mode == TALK_SAY
			|| Mode == TALK_WHISPER
			|| Mode == TALK_YELL
			|| Mode == TALK_ANIMAL_LOW
			|| Mode == TALK_ANIMAL_LOUD){
		uint32 StatementID;
		if(Creature->Type == PLAYER){
			StatementID = LogCommunication(CreatureID, Mode, 0, Text);
		}else{
			StatementID = 0;
		}

		int Radius;
		if(Mode == TALK_YELL || Mode == TALK_ANIMAL_LOUD){
			Radius = 30; // RANGE_YELL ?
		}else{
			Radius = 7; // RANGE_SAY ?
		}

		TFindCreatures Search(Radius, Radius, Creature->posx, Creature->posy, FIND_PLAYERS);
		while(true){
			uint32 SpectatorID = Search.findNext();
			if(SpectatorID == 0){
				break;
			}

			TPlayer *Spectator = GetPlayer(SpectatorID);
			if(Spectator == NULL || Spectator->Connection == NULL){
				continue;
			}

			int DistanceX = std::abs(Spectator->posx - Creature->posx);
			int DistanceY = std::abs(Spectator->posy - Creature->posy);
			int DistanceZ = std::abs(Spectator->posz - Creature->posz);
			if(Mode == TALK_SAY || Mode == TALK_ANIMAL_LOW){
				if(DistanceX > 7 || DistanceY > 5 || DistanceZ > 0){
					continue;
				}
			}else if(Mode == TALK_WHISPER){
				if(DistanceX > 7 || DistanceY > 5 || DistanceZ > 0){
					continue;
				}

				if(DistanceX > 1 && DistanceY > 1){
					SendTalk(Spectator->Connection, 0, Creature->Name, Mode,
							Creature->posx, Creature->posy, Creature->posz,
							"pspsps");
					continue;
				}
			}else if(Mode == TALK_YELL || Mode == TALK_ANIMAL_LOUD){
				if(DistanceX > 30 || DistanceY > 30){
					continue;
				}

				// TODO(fusion): This seems to be correct. Underground yells
				// aren't multi floor.
				if(DistanceZ > 0 && (Spectator->posz > 7 || Creature->posz > 7)){
					continue;
				}
			}

			SendTalk(Spectator->Connection,
					LogListener(StatementID, Spectator),
					Creature->Name, Mode,
					Creature->posx, Creature->posy, Creature->posz, Text);
		}
	}else if(Mode == TALK_GAMEMASTER_BROADCAST){
		uint32 StatementID = LogCommunication(CreatureID, Mode, 0, Text);
		TConnection *Connection = GetFirstConnection();
		while(Connection != NULL){
			// TODO(fusion): Same as `AdvanceGame`. Definitely some inlined
			// function to check whether the connection is in a valid state.
			if(Connection->State == CONNECTION_LOGIN
					|| Connection->State == CONNECTION_GAME
					|| Connection->State == CONNECTION_DEAD
					|| Connection->State == CONNECTION_LOGOUT){
				SendTalk(Connection,
						LogListener(StatementID, Connection->GetPlayer()),
						Creature->Name, Mode, Text, 0);
			}

			Connection = GetNextConnection();
		}
	}else if(Mode == TALK_ANONYMOUS_BROADCAST){
		TConnection *Connection = GetFirstConnection();
		while(Connection != NULL){
			// TODO(fusion): Same as above.
			if(Connection->State == CONNECTION_LOGIN
					|| Connection->State == CONNECTION_GAME
					|| Connection->State == CONNECTION_DEAD
					|| Connection->State == CONNECTION_LOGOUT){
				SendMessage(Connection, TALK_ADMIN_MESSAGE, Text);
			}

			Connection = GetNextConnection();
		}
	}else if(Mode == TALK_CHANNEL_CALL
			|| Mode == TALK_GAMEMASTER_CHANNELCALL
			|| Mode == TALK_HIGHLIGHT_CHANNELCALL
			|| Mode == TALK_ANONYMOUS_CHANNELCALL){
		uint32 StatementID = 0;
		if(Mode != TALK_ANONYMOUS_CHANNELCALL){
			StatementID = LogCommunication(CreatureID, Mode, Channel, Text);
		}

		// TODO(fusion): We should probably cleanup these global iterators.
		for(uint32 SubscriberID = GetFirstSubscriber(Channel);
				SubscriberID != 0;
				SubscriberID = GetNextSubscriber()){
			TPlayer *Subscriber = GetPlayer(SubscriberID);
			if(Subscriber == NULL){
				continue;
			}

			// TODO(fusion): We should probably review this. What if both player
			// guild names are empty? Is it checked elsewhere?
			if(Channel == 0 && (Creature->Type != PLAYER
					|| strcmp(((TPlayer*)Creature)->Guild, Subscriber->Guild) != 0)){
				continue;
			}

			// TODO(fusion): There was some weird logic here to select the statement
			// id and it didn't seem to make a difference since `LogListener` was
			// always called, regardless.
			SendTalk(Subscriber->Connection,
					LogListener(StatementID, Subscriber),
					Creature->Name, Mode, Channel, Text);
		}
	}else{
		// TODO(fusion): Put private messages into this if-else chain.
		error("Talk: Ungültiger Sprechmodus %d.\n", Mode);
	}

	if(Mode == TALK_SAY && Creature->Type == PLAYER){
		TFindCreatures Search(3, 3, CreatureID, FIND_NPCS);
		while(true){
			uint32 SpectatorID = Search.getNext();
			if(SpectatorID == 0){
				break;
			}

			TCreature *Spectator = GetCreature(SpectatorID);
			if(Spectator != NULL){
				Spectator->TalkStimulus(CreatureID, Text);
			}else{
				error("Talk: Kreatur existiert nicht.\n");
			}
		}
	}
}

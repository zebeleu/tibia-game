#include "cr.hh"

#include "stubs.hh"

// TShortway
// =============================================================================
struct TShortwayPoint {
	TShortwayPoint(void){
		this->x = 0;
		this->y = 0;
		this->Waypoints = -1;
		this->Waylength = -1;
		this->Heuristic = -1;
		this->Predecessor = NULL;
		this->NextToExpand = NULL;
	}

	int x;
	int y;
	int Waypoints;
	int Waylength;
	int Heuristic;
	TShortwayPoint *Predecessor;
	TShortwayPoint *NextToExpand;
};

struct TShortway{
	TShortway(TCreature *Creature, int VisibleX, int VisibleY);
	~TShortway(void);
	void FillMap(void);
	void ClearMap(void);
	void Expand(TShortwayPoint *Node);
	bool Calculate(int DestX, int DestY, bool MustReach, int MaxSteps);

	// DATA
	// =================
	matrix<TShortwayPoint> *Map;
	TShortwayPoint *FirstToExpand;
	TCreature *Creature;
	int VisibleX;
	int VisibleY;
	int StartX;
	int StartY;
	int StartZ;
	int MinWaypoints;
};

TShortway::TShortway(TCreature *Creature, int VisibleX, int VisibleY){
	if(Creature == NULL){
		error("TShortway::TShortway: Übergebene Kreatur ist NULL.\n");
		return;
	}

	if(VisibleX < 1 || VisibleX > 100 || VisibleY < 1 || VisibleY > 100){
		error("TShortway::TShortway: Ungültiger Sichtbarkeitsbereich %d*%d.\n", VisibleX, VisibleY);
		return;
	}

	this->Creature = Creature;
	this->VisibleX = VisibleX;
	this->VisibleY = VisibleY;
	this->StartX = Creature->posx;
	this->StartY = Creature->posy;
	this->StartZ = Creature->posz;
	this->Map = new matrix<TShortwayPoint>(
				-(VisibleX + 1), +(VisibleX + 1),
				-(VisibleY + 1), +(VisibleY + 1));
	this->FillMap();
}

TShortway::~TShortway(void){
	if(this->Map != NULL){
		delete this->Map;
	}
}

void TShortway::FillMap(void){
	this->MinWaypoints = 1000;

	for(int X = -this->VisibleX; X <= this->VisibleX; X += 1)
	for(int Y = -this->VisibleY; Y <= this->VisibleY; Y += 1){
		int FieldX = this->StartX + X;
		int FieldY = this->StartY + Y;
		int FieldZ = this->StartZ;

		int Waypoints = -1;
		Object Obj = GetFirstObject(FieldX, FieldY, FieldZ);
		if(Obj.exists()){
			ObjectType ObjType = Obj.getObjectType();
			if(ObjType.getFlag(BANK) && !ObjType.getFlag(UNPASS)){
				Waypoints = (int)ObjType.getAttribute(WAYPOINTS);
				if(Waypoints == 0){
					error("TShortway::FillMap: Ungültiger Wegpunkte-Wert %d für Bank %d.\n",
							Waypoints, ObjType.TypeID);
					Waypoints = -1;
				}

				if(!this->Creature->MovePossible(FieldX, FieldY, FieldZ, false, false)){
					Waypoints = -1;
				}

				if(Waypoints > 0 && Waypoints < this->MinWaypoints){
					this->MinWaypoints = Waypoints;
				}
			}
		}

		TShortwayPoint *Node = this->Map->at(X, Y);
		Node->x = X;
		Node->y = Y;
		Node->Waypoints = Waypoints;
	}
}

void TShortway::ClearMap(void){
	for(int X = -this->VisibleX; X <= this->VisibleX; X += 1)
	for(int Y = -this->VisibleY; Y <= this->VisibleY; Y += 1){
		TShortwayPoint *Node = this->Map->at(X, Y);
		Node->Waylength = INT_MAX;
		Node->Heuristic = INT_MAX;
		Node->Predecessor = NULL;
		Node->NextToExpand = NULL;
	}
}

void TShortway::Expand(TShortwayPoint *Node){
	if(Node == NULL){
		error("TShortway::Expand: Übergebener Knoten ist NULL.\n");
		return;
	}

	this->FirstToExpand = Node->NextToExpand;

	int MinNeighborWaylength = Node->Waylength + Node->Waypoints;
	if(MinNeighborWaylength >= this->Map->at(0, 0)->Waylength){
		return;
	}

	for(int OffsetX = -1; OffsetX <= 1; OffsetX += 1)
	for(int OffsetY = -1; OffsetY <= 1; OffsetY += 1){
		if(OffsetX == 0 && OffsetY == 0){
			continue;
		}

		TShortwayPoint *Neighbor = this->Map->at(Node->x + OffsetX, Node->y + OffsetY);

		// NOTE(fusion): The minimum neighbor waylength already contains the cost
		// of a single step. Diagonal steps are three times more expensive so we
		// add waypoints two more times.
		int NeighborWaylength = MinNeighborWaylength;
		if(OffsetX != 0 && OffsetY != 0){
			NeighborWaylength += Node->Waylength * 2;
		}

		if(NeighborWaylength < Neighbor->Waylength){
			Neighbor->Waylength = NeighborWaylength;
			Neighbor->Predecessor = Node;
			if((Neighbor->x != 0 || Neighbor->y != 0) && Neighbor->Waypoints != -1){
				// NOTE(fusion): Remove from expand list if it was already expanded upon.
				if(Neighbor->Heuristic != INT_MAX){
					TShortwayPoint *Prev = NULL;
					TShortwayPoint *Cur = this->FirstToExpand;
					while(Cur != NULL && Cur != Neighbor){
						Prev = Cur;
						Cur = Cur->NextToExpand;
					}

					if(Cur != Neighbor){
						error("TShortway::Expand: Knoten steht nicht in der ExpandList.\n");
					}else if(Prev == NULL){
						this->FirstToExpand = Neighbor->NextToExpand;
					}else{
						Prev->NextToExpand = Neighbor->NextToExpand;
					}
				}

				// NOTE(fusion): Compute heuristic using the manhattan distance.
				int Distance = std::abs(Neighbor->x) + std::abs(Neighbor->y);
				Neighbor->Heuristic = Neighbor->Waylength
						+ Neighbor->Waypoints * 1
						+ this->MinWaypoints * (Distance - 1);

				// NOTE(fusion): Insert into expand list.
				{
					TShortwayPoint *Prev = NULL;
					TShortwayPoint *Cur = this->FirstToExpand;
					while(Cur != NULL && Cur->Heuristic < Neighbor->Heuristic){
						Prev = Cur;
						Cur = Cur->NextToExpand;
					}

					if(Prev == NULL){
						this->FirstToExpand = Neighbor;
					}else{
						Prev->NextToExpand = Neighbor;
					}
					Neighbor->NextToExpand = Cur;
				}
			}
		}
	}
}

bool TShortway::Calculate(int DestX, int DestY, bool MustReach, int MaxSteps){
	if(this->Map == NULL){
		error("TShortway::Calculate: Karte existiert nicht.\n");
		return false;
	}

	// NOTE(fusion): Transform dest to relative coordinates.
	DestX -= this->StartX;
	DestY -= this->StartY;

	// NOTE(fusion): Check if already at destination.
	if(DestX == 0 && DestY == 0){
		return true;
	}

	// NOTE(fusion): Check if out of range.
	if(std::abs(DestX) > this->VisibleX || std::abs(DestY) > this->VisibleY){
		return false;
	}

	// NOTE(fusion): Find shortest path from the destination to the origin.
	this->ClearMap();
	this->FirstToExpand = this->Map->at(DestX, DestY);
	this->FirstToExpand->Waylength = 0;
	while(this->FirstToExpand != NULL){
		this->Expand(this->FirstToExpand);
	}

	// NOTE(fusion): Check if the origin was reached from the destination.
	TShortwayPoint *Node = this->Map->at(0, 0);
	if(Node->Waylength == INT_MAX){
		return false;
	}

	// NOTE(fusion): Walk back from the origin to reconstruct the path.
	Node = Node->Predecessor;
	while(Node != NULL && MaxSteps > 0){
		int MaxDistance = std::max<int>(
				std::abs(Node->x - DestX),
				std::abs(Node->y - DestY));
		if(!MustReach && MaxDistance <= 1){
			break;
		}

		TToDoEntry TD = {};
		TD.Code = TDGo;
		TD.Go.x = this->StartX + Node->x;
		TD.Go.y = this->StartY + Node->y;
		TD.Go.z = this->StartZ;
		Creature->ToDoAdd(TD);

		Node = Node->Predecessor;
		MaxSteps -= 1;
	}

	return true;
}

// TCreature
// =============================================================================
bool TCreature::SetOnMap(void){
	int LoginX = this->posx;
	int LoginY = this->posy;
	int LoginZ = this->posz;
	bool Player = (this->Type == PLAYER);
	if(!SearchLoginField(&LoginX, &LoginY, &LoginZ, 1, Player)){
		bool Found = false;
		if(IsHouse(LoginX, LoginY, LoginZ)){
			uint16 HouseID = GetHouseID(LoginX, LoginY, LoginZ);
			GetExitPosition(HouseID, &LoginX, &LoginY, &LoginZ);
			Found = SearchLoginField(&LoginX, &LoginY, &LoginZ, 1, Player);
		}

		if(!Found){
			LoginX = this->startx;
			LoginY = this->starty;
			LoginZ = this->startz;
		}
	}

	Object Con = GetMapContainer(LoginX, LoginY, LoginZ);
	if(Con == NONE){
		error("TCreature::SetOnMap: Kartencontainer für Punkt [%d,%d,%d] existiert nicht.\n",
				LoginX, LoginY, LoginZ);
		return false;
	}

	this->posx = LoginX;
	this->posy = LoginY;
	this->posz = LoginZ;

	// NOTE(fusion): `Create` automatically sets `this->CrObject` and creates
	// its body container slots.
	bool Result = true;
	try{
		Create(Con, TYPEID_CREATURE_CONTAINER, this->ID);
	}catch(RESULT r){
		error("TCreature::SetOnMap: Kann Kreatur nicht setzen ([%d,%d,%d] - Exception %d).\n",
				LoginX, LoginY, LoginZ, r);
		if(this->Type == PLAYER){
			SendResult(this->Connection, r);
		}
		Result = false;
	}
	return Result;
}

bool TCreature::DelOnMap(void){
	Object Obj = this->CrObject;
	if(Obj == NONE){
		return true;
	}

	// TODO(fusion): I feel `Delete` should also manage `this->CrObject` automatically.
	bool Result = true;
	this->CrObject = NONE;
	try{
		Delete(Obj, -1);
	}catch(RESULT r){
		error("TCreature::DelOnMap: Error Deleting CreatureObject: %d\n", r);
		Result = false;
	}
	return Result;
}

void TCreature::Attack(void){
	this->Combat.Attack();
}

//void TCreature::Execute(void);

uint32 TCreature::CalculateDelay(void){
	uint32 Delay = 0;
	TToDoEntry *TD = this->ToDoList.at(this->ActToDo);
	switch(TD->Code){
		case TDWait:{
			// TODO(fusion): I'm not sure about having `EarliestWalkTime` here.
			uint32 WaitTime = TD->Wait.Time;
			if(WaitTime < this->EarliestWalkTime){
				WaitTime = this->EarliestWalkTime;
			}

			if(WaitTime > ServerMilliseconds){
				Delay = WaitTime - ServerMilliseconds;
			}
			break;
		}

		case TDGo:{
			if(this->EarliestWalkTime > ServerMilliseconds){
				Delay = this->EarliestWalkTime - ServerMilliseconds;
			}
			break;
		}

		case TDUse:{
			if(TD->Use.Obj2 != 0){
				if(this->EarliestMultiuseTime > ServerMilliseconds){
					Delay = this->EarliestMultiuseTime - ServerMilliseconds;
				}
			}
			break;
		}

		case TDAttack:{
			uint32 EarliestAttackTime = this->Combat.EarliestAttackTime;
			if(EarliestAttackTime < this->EarliestSpellTime){
				EarliestAttackTime = this->EarliestSpellTime;
			}

			if(EarliestAttackTime > ServerMilliseconds){
				Delay = EarliestAttackTime - ServerMilliseconds;
			}
			break;
		}

		default:{
			break;
		}
	}
	return Delay;
}

bool TCreature::ToDoClear(void){
	bool SnapbackNecessary = false;
	for(int i = 0; i < this->NrToDo; i += 1){
		TToDoEntry *TD = this->ToDoList.at(i);
		switch(TD->Code){
			case TDGo:{
				if(this->ActToDo <= i){
					SnapbackNecessary = true;
				}
				break;
			}

			case TDTalk:{
				DeleteDynamicString(TD->Talk.Text);
				DeleteDynamicString(TD->Talk.Addressee);
				break;
			}

			case TDChangeState:{
				if(this->ActToDo <= i && this->Type == NPC){
					ChangeNPCState(this, TD->ChangeState.NewState, false);
				}
				break;
			}

			default:{
				break;
			}
		}
	}

	this->LockToDo = false;
	this->ActToDo = 0;
	this->NrToDo = 0;
	this->Stop = false;
	return SnapbackNecessary;
}

void TCreature::ToDoAdd(TToDoEntry TD){
	if(this->LockToDo){
		if(this->ToDoClear() && this->Type == PLAYER){
			SendSnapback(this->Connection);
		}
	}

	*this->ToDoList.at(this->NrToDo) = TD;
	this->NrToDo += 1;
}

void TCreature::ToDoStop(void){
	if(this->LockToDo){
		this->Stop = true;
	}else if(this->Type == PLAYER){
		SendSnapback(this->Connection);
	}
}

void TCreature::ToDoStart(void){
	if(this->NrToDo != 0){
		this->LockToDo = true;
		this->ActToDo = 0;

		uint32 Delay = this->CalculateDelay();
		if(Delay < 1){
			Delay = 1;
		}

		uint32 NextWakeup = ServerMilliseconds + Delay;
		ToDoQueue.insert(NextWakeup, this->ID);
		this->NextWakeup = NextWakeup;
	}
}

void TCreature::ToDoYield(void){
	if(!this->LockToDo){
		this->ToDoWait(0);
		this->ToDoStart();
	}
}

void TCreature::ToDoWait(int Delay){
	TToDoEntry TD = {};
	TD.Code = TDWait;
	TD.Wait.Time = ServerMilliseconds + Delay;
	this->ToDoAdd(TD);
}

void TCreature::ToDoGo(int DestX, int DestY, int DestZ, bool MustReach, int MaxSteps){
	if(this->posz < DestZ){
		throw DOWNSTAIRS;
	}else if(this->posz > DestZ){
		throw UPSTAIRS;
	}

	if(this->LockToDo){
		TToDoEntry *Last = this->ToDoList.at(this->NrToDo - 1);
		if(Last->Code == TDGo
				&& Last->Go.x == DestX
				&& Last->Go.y == DestY
				&& Last->Go.z == DestZ){
			// TODO(fusion): Why? Shouldn't we just return here?
			throw NOERROR;
		}
	}

	int DistanceX = std::abs(DestX - this->posx);
	int DistanceY = std::abs(DestY - this->posy);
	int MaxDistance = std::max<int>(DistanceX, DistanceY);
	if(MaxDistance == 0 || (!MustReach && MaxDistance <= 1)){
		return;
	}

	// NOTE(fusion): The number of steps between two points is the same as the
	// their manhattan distance, if we exclude diagonal movement. We can skip
	// the path finder if we know we're step away from the destination.
	if(DistanceX + DistanceY == 1){
		TToDoEntry TD = {};
		TD.Code = TDGo;
		TD.Go.x = DestX;
		TD.Go.y = DestY;
		TD.Go.z = DestZ;
		this->ToDoAdd(TD);
	}else{
		int VisibleX = (this->Type == PLAYER) ? 7 : 10;
		int VisibleY = (this->Type == PLAYER) ? 7 : 10;
		TShortway Shortway(this, VisibleX, VisibleY);
		if(!Shortway.Calculate(DestX, DestY, MustReach, MaxSteps)){
			this->ToDoClear();
			if(this->Type == PLAYER){
				SendSnapback(this->Connection);
			}
			throw NOWAY;
		}
	}
}

void TCreature::ToDoRotate(int Direction){
	if(Direction != DIRECTION_NORTH
			&& Direction != DIRECTION_EAST
			&& Direction != DIRECTION_SOUTH
			&& Direction != DIRECTION_WEST){
		throw ERROR;
	}

	TToDoEntry TD = {};
	TD.Code = TDRotate;
	TD.Rotate.Direction = Direction;
	this->ToDoAdd(TD);
}

void TCreature::ToDoMove(int ObjX, int ObjY, int ObjZ, ObjectType Type, uint8 RNum,
						int DestX, int DestY, int DestZ, uint8 Count){
	Object Obj = GetObject(this->ID, ObjX, ObjY, ObjZ, RNum, Type);
	if(!Obj.exists()){
		throw NOTACCESSIBLE;
	}

	// TODO(fusion): This could be an inlined function.
	if(ObjX != 0xFFFF){ // SPECIAL_COORDINATE ?
		if(this->posz > ObjZ){
			throw UPSTAIRS;
		}else if(this->posz < ObjZ){
			throw DOWNSTAIRS;
		}

		if(!ObjectInRange(this->ID, Obj, 1)){
			this->ToDoGo(ObjX, ObjY, ObjZ, false, INT_MAX);
		}
	}

	int Delay = 100;
	if(Obj.getObjectType().isCreatureContainer()){
		Object DestBank = GetFirstObject(DestX, DestY, DestZ);
		if(DestBank == NONE || !DestBank.getObjectType().getFlag(BANK)){
			throw NOTACCESSIBLE;
		}

		TCreature *Creature = GetCreature(Obj);
		if(Creature == NULL){
			error("TCreature::ToDoMove: Kreatur existiert nicht.\n");
			throw ERROR;
		}

		Delay = 1000;
		if(this->EarliestWalkTime > ServerMilliseconds){
			Delay += (int)(this->EarliestWalkTime - ServerMilliseconds);
		}
	}

	this->ToDoWait(Delay);

	TToDoEntry TD = {};
	TD.Code = TDMove;
	TD.Move.Obj = Obj.ObjectID;
	TD.Move.x = DestX;
	TD.Move.y = DestY;
	TD.Move.z = DestZ;
	TD.Move.Count = Count;
	this->ToDoAdd(TD);
}

void TCreature::ToDoMove(Object Obj, int DestX, int DestY, int DestZ, uint8 Count){
	if(!Obj.exists()){
		throw NOTACCESSIBLE;
	}

	if(!ObjectInRange(this->ID, Obj, 1)){
		this->ToDoGo(DestX, DestY, DestZ, false, INT_MAX);
	}

	int Delay = 100;
	if(Obj.getObjectType().isCreatureContainer()){
		// TODO(fusion): We don't add the delay until the earliest walk time
		// like in the other variant above.
		Delay = 1000;
	}

	this->ToDoWait(Delay);

	TToDoEntry TD = {};
	TD.Code = TDMove;
	TD.Move.Obj = Obj.ObjectID;
	TD.Move.x = DestX;
	TD.Move.y = DestY;
	TD.Move.z = DestZ;
	TD.Move.Count = Count;
	this->ToDoAdd(TD);
}

void TCreature::ToDoTrade(int ObjX, int ObjY, int ObjZ, ObjectType Type, uint8 RNum,
						uint32 TradePartner){
	Object Obj = GetObject(this->ID, ObjX, ObjY, ObjZ, RNum, Type);
	if(!Obj.exists()){
		throw NOTACCESSIBLE;
	}

	ObjectType ObjType = Obj.getObjectType();
	if(ObjType.getFlag(UNMOVE)){
		throw NOTMOVABLE;
	}

	if(!ObjType.getFlag(TAKE)){
		throw NOTTAKABLE;
	}

	if(ObjType.isCreatureContainer()){
		error("TCreature::ToDoTrade: Objekt ist eine Kreatur.\n");
		throw ERROR;
	}

	if(TradePartner == 0){
		error("TCreature::ToDoTrade: Handelspartner ist Null.\n");
		throw ERROR;
	}

	TCreature *Creature = GetCreature(TradePartner);
	if(Creature == NULL){
		throw PLAYERNOTONLINE;
	}

	if(Creature->Type != PLAYER){
		error("TCreature::ToDoTrade: Handelspartner von %s ist kein Spieler.\n", this->Name);
		throw ERROR;
	}

	// TODO(fusion): This could be an inlined function.
	if(ObjX != 0xFFFF){ // SPECIAL_COORDINATE ?
		if(this->posz > ObjZ){
			throw UPSTAIRS;
		}else if(this->posz < ObjZ){
			throw DOWNSTAIRS;
		}

		if(!ObjectInRange(this->ID, Obj, 1)){
			this->ToDoGo(ObjX, ObjY, ObjZ, false, INT_MAX);
		}
	}

	TToDoEntry TD = {};
	TD.Code = TDTrade;
	TD.Trade.Obj = Obj.ObjectID;
	TD.Trade.Partner = TradePartner;
	this->ToDoAdd(TD);
}

void TCreature::ToDoUse(uint8 Count, int ObjX1, int ObjY1, int ObjZ1, ObjectType Type1, uint8 RNum1,
						uint8 Dummy, int ObjX2, int ObjY2, int ObjZ2, ObjectType Type2, uint8 RNum2){
	Object Obj1 = GetObject(this->ID, ObjX1, ObjY1, ObjZ1, RNum1, Type1);
	if(!Obj1.exists()){
		throw NOTACCESSIBLE;
	}

	Object Obj2 = NONE;
	if(Count >= 2){
		Obj2 = GetObject(this->ID, ObjX2, ObjY2, ObjZ2, RNum2, Type2);
		if(!Obj2.exists()){
			throw NOTACCESSIBLE;
		}
	}

	// TODO(fusion): This could be an inlined function.
	if(ObjX1 != 0xFFFF){ // SPECIAL_COORDINATE ?
		if(this->posz > ObjZ1){
			throw UPSTAIRS;
		}else if(this->posz < ObjZ1){
			throw DOWNSTAIRS;
		}

		if(!ObjectInRange(this->ID, Obj1, 1)){
			this->ToDoGo(ObjX1, ObjY1, ObjZ1, false, INT_MAX);
		}
	}

	this->ToDoWait(100);

	TToDoEntry TD = {};
	TD.Code = TDUse;
	TD.Use.Obj1 = Obj1.ObjectID;
	TD.Use.Obj2 = Obj2.ObjectID;
	// TODO(fusion): This is the value sent by the client on single object use
	// that contains the next free container index.
	TD.Use.Dummy = Dummy;
	this->ToDoAdd(TD);
}

void TCreature::ToDoUse(uint8 Count, Object Obj1, Object Obj2){
	if(!Obj1.exists()){
		throw NOTACCESSIBLE;
	}

	if(Count <= 1){
		Obj2 = NONE;
	}else if(!Obj2.exists()){
		throw NOTACCESSIBLE;
	}

	if(!ObjectInRange(this->ID, Obj1, 1)){
		int ObjX1, ObjY1, ObjZ1;
		GetObjectCoordinates(Obj1, &ObjX1, &ObjY1, &ObjZ1);
		this->ToDoGo(ObjX1, ObjY1, ObjZ1, false, INT_MAX);
	}

	this->ToDoWait(100);

	TToDoEntry TD = {};
	TD.Code = TDUse;
	TD.Use.Obj1 = Obj1.ObjectID;
	TD.Use.Obj2 = Obj2.ObjectID;
	// TODO(fusion): Not sure why we set this value at all.
	TD.Use.Dummy = Count;
	this->ToDoAdd(TD);
}

void TCreature::ToDoTurn(int ObjX, int ObjY, int ObjZ, ObjectType Type, uint8 RNum){
	Object Obj = GetObject(this->ID, ObjX, ObjY, ObjZ, RNum, Type);
	if(!Obj.exists()){
		throw NOTACCESSIBLE;
	}

	// TODO(fusion): This could be an inlined function.
	if(ObjX != 0xFFFF){ // SPECIAL_COORDINATE ?
		if(this->posz > ObjZ){
			throw UPSTAIRS;
		}else if(this->posz < ObjZ){
			throw DOWNSTAIRS;
		}

		if(!ObjectInRange(this->ID, Obj, 1)){
			this->ToDoGo(ObjX, ObjY, ObjZ, false, INT_MAX);
		}
	}

	this->ToDoWait(100);

	TToDoEntry TD = {};
	TD.Code = TDTurn;
	TD.Turn.Obj = Obj.ObjectID;
	this->ToDoAdd(TD);
}

void TCreature::ToDoAttack(void){
	this->Combat.CanToDoAttack();
	if(this->Combat.GetDistance() != 1){
		this->ToDoWait(100);
	}

	TToDoEntry TD = {};
	TD.Code = TDAttack;
	this->ToDoAdd(TD);
}

void TCreature::ToDoTalk(int Mode, const char *Addressee, const char *Text, bool CheckSpamming){
	TToDoEntry TD = {};

	if(Text == NULL || Text[0] == 0){
		error("TCreature::ToDoTalk: Text ist NULL bei %s.\n", this->Name);

		// TODO(fusion): The original code would attempt to call `AddDynamicString`
		// even after this check but it doesn't check whether `Text` is NULL and
		// calls `strlen` immediately on it which could be a problem.
		TD.Talk.Text = 0;
	}else{
		TD.Talk.Text = AddDynamicString(Text);
	}

	if(Addressee == NULL){
		TD.Talk.Addressee = 0;
	}else{
		TD.Talk.Addressee = AddDynamicString(Addressee);
	}

	TD.Code = TDTalk;
	TD.Talk.Mode = Mode;
	TD.Talk.CheckSpamming = CheckSpamming;
	this->ToDoAdd(TD);
}

void TCreature::ToDoChangeState(int NewState){
	TToDoEntry TD = {};
	TD.Code = TDChangeState;
	TD.ChangeState.NewState = NewState;
	this->ToDoAdd(TD);
}

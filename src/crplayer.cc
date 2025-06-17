#include "cr.hh"
#include "info.hh"
#include "operate.hh"
#include "query.hh"
#include "thread.hh"

#include "stubs.hh"

static Semaphore PlayerMutex(1);
static int FirstFreePlayer;
static vector<TPlayer*> PlayerList(0, 100, 10, NULL);

static Semaphore PlayerDataPoolMutex(1);
static TPlayerData PlayerDataPool[2000];

static TPlayerIndexInternalNode PlayerIndexHead;
static store<TPlayerIndexInternalNode, 100> PlayerIndexInternalNodes;
static store<TPlayerIndexLeafNode, 100> PlayerIndexLeafNodes;

// TPlayer
// =============================================================================
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

// Player Utility
// =============================================================================
int GetNumberOfPlayers(void){
	return FirstFreePlayer;
}

TPlayer *GetPlayer(uint32 CharacterID){
	TCreature *Creature = GetCreature(CharacterID);
	if(Creature != NULL && Creature->Type != PLAYER){
		error("GetPlayer: Kreatur ist kein Spieler.\n");
		Creature = NULL;
	}
	return (TPlayer*)Creature;
}

TPlayer *GetPlayer(const char *Name){
	TPlayer *Result = NULL;
	for(int Index = 0; Index < FirstFreePlayer; Index += 1){
		TPlayer *Player = *PlayerList.at(Index);
		if(stricmp(Player->Name, Name) == 0){
			Result = Player;
			break;
		}
	}
	return Result;
}

bool IsPlayerOnline(const char *Name){
	// TODO(fusion): What is `PlayerMutex` actually doing and where is it used?
	bool Result;
	PlayerMutex.down();
	Result = (GetPlayer(Name) != NULL);
	PlayerMutex.up();
	return Result;
}

int IdentifyPlayer(const char *Name, bool ExactMatch, bool IgnoreGamemasters, TPlayer **OutPlayer){
	if(Name == NULL){
		error("IdentifyPlayer: Name ist NULL.\n");
		return -1; // NOTFOUND ?
	}

	if(Name[0] == 0){
		error("IdentifyPlayer: Name ist leer.\n");
		return -1; // NOTFOUND ?
	}

	int NameLength = (int)strlen(Name);
	if(!ExactMatch){
		if(Name[NameLength - 1] != '~'){
			ExactMatch = true;
		}else{
			NameLength -= 1;
		}
	}

	int Hits = 0;
	for(int Index = 0; Index < FirstFreePlayer; Index += 1){
		TPlayer *Player = *PlayerList.at(Index);
		if(stricmp(Player->Name, Name, NameLength) == 0){
			if(NameLength == (int)strlen(Player->Name)){
				*OutPlayer = Player;
				return 0; // FOUND ?
			}else if(!ExactMatch){
				if(IgnoreGamemasters && CheckRight(Player->ID, NO_STATISTICS)){
					continue;
				}

				*OutPlayer = Player;
				Hits += 1;
			}
		}
	}

	if(Hits == 0){
		return -1; // NOTFOUND ?
	}else if(Hits > 1){
		return -2; // AMBIGUOUS ?
	}

	return 0; // FOUND ?
}

void LogoutAllPlayers(void){
	print(1, "LogoutAllPlayers: Werde alle Spieler ausloggen!\n");
	for(int Index = 0; Index < FirstFreePlayer; Index += 1){
		TPlayer *Player = *PlayerList.at(Index);

		// TODO(fusion): Should we be checking if `Player` is NULL, because
		// `GetPlayer` and `IdentifyPlayer` do not.
		if(Player == NULL){
			error("LogoutAllPlayers: Eintrag %d in der Playerlist ist NULL.\n", Index);
			continue;
		}

		// TODO(fusion): Same thing as with `ProcessCreatures`. Players are removed
		// from `PlayerList` by the player's destructor, in a swap and pop fashion.
		// What a disaster.
		delete Player;
		Index -= 1;
	}
}

void CloseProcessedRequests(uint32 CharacterID){
	for(int Index = 0; Index < FirstFreePlayer; Index += 1){
		TPlayer *Player = *PlayerList.at(Index);
		if(Player == NULL){
			error("CloseProcessedRequests: Spieler %d existiert nicht.\n", Index);
			continue;
		}

		SendCloseRequest(Player->Connection);
		Player->Request = 0;
	}
}

void NotifyBuddies(uint32 CharacterID, const char *Name, bool Login){
	if(Name == NULL || Name[0] == 0){
		error("NotifyBuddies: Name existiert nicht.\n");
		return;
	}

	bool Hide = CheckRight(CharacterID, NO_STATISTICS);
	for(int Index = 0; Index < FirstFreePlayer; Index += 1){
		TPlayer *Player = *PlayerList.at(Index);
		if(Player->Connection == NULL || Player->PlayerData == NULL){
			continue;
		}

		if(Hide && !CheckRight(Player->ID, READ_GAMEMASTER_CHANNEL)){
			continue;
		}

		TPlayerData *PlayerData = Player->PlayerData;
		for(int BuddyIndex = 0;
				BuddyIndex < PlayerData->Buddies;
				BuddyIndex += 1){
			if(PlayerData->Buddy[BuddyIndex] == CharacterID){
				if(strcmp(PlayerData->BuddyName[BuddyIndex], Name) == 0){
					SendBuddyStatus(Player->Connection, CharacterID, Login);
				}else{
					SendBuddyData(Player->Connection, CharacterID, Name, Login);
					strcpy(PlayerData->BuddyName[BuddyIndex], Name);
				}
			}
		}
	}
}

void CreatePlayerList(bool Online){
	// TODO(fusion): Same as `WriteKillStatistics` for names.

	// TODO(fusion): We can avoid allocating these buffers when `Online` is false.
	// We'll be sure when we dive into `writer.cc` and `reader.cc` and learn more
	// about these "orders".

	char *PlayerNames = new char[FirstFreePlayer * 30];
	int *PlayerLevels = new int[FirstFreePlayer];
	int *PlayerProfessions = new int[FirstFreePlayer];
	int NumberOfPlayers = -1;
	if(Online){
		NumberOfPlayers = 0;
		for(int Index = 0; Index < FirstFreePlayer; Index += 1){
			TPlayer *Player = *PlayerList.at(Index);
			if(CheckRight(Player->ID, NO_STATISTICS)){
				continue;
			}

			strcpy(&PlayerNames[Index * 30], Player->Name);
			PlayerLevels[Index] = Player->Skills[SKILL_LEVEL]->Get();
			PlayerProfessions[Index] = Player->GetActiveProfession();
			NumberOfPlayers += 1;
		}
		Log("load", "%d %d\n", (int)time(NULL), FirstFreePlayer);
	}

	PlayerlistOrder(NumberOfPlayers, PlayerNames, PlayerLevels, PlayerProfessions);
}

void PrintPlayerPositions(void){
	for(int Index = 0; Index < FirstFreePlayer; Index += 1){
		TPlayer *Player = *PlayerList.at(Index);
		Log("players", "[%d,%d,%d] %d %d\n",
				Player->posx, Player->posy, Player->posz,
				Player->Skills[SKILL_LEVEL]->Get(),
				Player->GetActiveProfession());
	}
}

void LoadDepot(TPlayerData *PlayerData, int DepotNr, Object Con){
	if(PlayerData == NULL){
		error("LoadDepot: PlayerData ist NULL.\n");
		throw ERROR;
	}

	if(!Con.exists()){
		error("LoadDepot: Übergebener Container existiert nicht.\n");
		throw ERROR;
	}

	if(!Con.getObjectType().getFlag(CONTAINER)){
		error("LoadDepot: Übergebenes Objekt ist kein Container.\n");
		throw ERROR;
	}

	if(DepotNr < 0 || DepotNr >= 9){ // MAX_DEPOT ?
		error("LoadDepot: Ungültige Depotnummer %d.\n", DepotNr);
		throw ERROR;
	}

	if(PlayerData->Depot[DepotNr] == NULL){
		Create(Con, GetSpecialObject(DEPOT_CHEST), 0);
	}else{
		try{
			TReadBuffer ReadBuffer(
					PlayerData->Depot[DepotNr],
					PlayerData->DepotSize[DepotNr]);
			LoadObjects(&ReadBuffer, Con);
		}catch(const char *str){
			error("LoadDepot: Kann Depot nicht lesen (%s).\n", str);
			throw ERROR;
		}
	}
}

void SaveDepot(TPlayerData *PlayerData, int DepotNr, Object Con){
	if(PlayerData == NULL){
		error("SaveDepot: PlayerData ist NULL.\n");
		throw ERROR;
	}

	if(!Con.exists()){
		error("SaveDepot: Übergebener Container existiert nicht.\n");
		throw ERROR;
	}

	if(!Con.getObjectType().getFlag(CONTAINER)){
		error("SaveDepot: Übergebenes Objekt ist kein Container.\n");
		throw ERROR;
	}

	if(DepotNr < 0 || DepotNr >= 9){ // MAX_DEPOT ?
		error("SaveDepot: Ungültige Depotnummer %d.\n", DepotNr);
		throw ERROR;
	}

	PlayerData->Dirty = true;
	if(PlayerData->Depot[DepotNr] != NULL){
		delete PlayerData->Depot[DepotNr];
		PlayerData->Depot[DepotNr] = NULL;
	}

	try{
		TDynamicWriteBuffer HelpBuffer(KB(16));
		SaveObjects(GetFirstContainerObject(Con), &HelpBuffer, false);

		PlayerData->Depot[DepotNr] = new uint8[HelpBuffer.Size];
		PlayerData->DepotSize[DepotNr] = HelpBuffer.Size;
		memcpy(PlayerData->Depot[DepotNr], HelpBuffer.Data, HelpBuffer.Size);
	}catch(const char *str){
		error("SaveDepot: Kann Depot nicht schreiben (%s).\n", str);
		PlayerData->Depot[DepotNr] = NULL;
		PlayerData->DepotSize[DepotNr] = 0;
	}
}

void GetProfessionName(char *Buffer, int Profession, bool Article, bool Capitals){
	Buffer[0] = 0;

	if(Article){
		if(Profession == PROFESSION_ELITE_KNIGHT
		|| Profession == PROFESSION_ELDER_DRUID){
			strcat(Buffer, "an ");
		}else{
			strcat(Buffer, "a ");
		}
	}

	switch(Profession){
		case PROFESSION_NONE:				strcat(Buffer, "None"); break;
		case PROFESSION_KNIGHT:				strcat(Buffer, "Knight"); break;
		case PROFESSION_PALADIN:			strcat(Buffer, "Paladin"); break;
		case PROFESSION_SORCERER:			strcat(Buffer, "Sorcerer"); break;
		case PROFESSION_DRUID:				strcat(Buffer, "Druid"); break;
		case PROFESSION_ELITE_KNIGHT:		strcat(Buffer, "Elite Knight"); break;
		case PROFESSION_ROYAL_PALADIN:		strcat(Buffer, "Royal Paladin"); break;
		case PROFESSION_MASTER_SORCERER:	strcat(Buffer, "Master Sorcerer"); break;
		case PROFESSION_ELDER_DRUID:		strcat(Buffer, "Elder Druid"); break;
		// NOTE(fusion): Not in the original function but w/e.
		default:							strcat(Buffer, "Unknown"); break;
	}

	if(!Capitals){
		strLower(Buffer);
	}
}

void SendExistingRequests(TConnection *Connection){
	if(Connection == NULL){
		error("SendExistingRequests: Verbindung ist NULL.\n");
		return;
	}

	// TODO(fusion): This function would originally use `alloca` to allocate
	// the `Players` array.

	int NumPlayers = 0;
	TPlayer **Players = new TPlayer*[FirstFreePlayer];
	for(int Index = 0; Index < FirstFreePlayer; Index += 1){
		TPlayer *Player = *PlayerList.at(Index);
		if(Player == NULL){
			error("SendExistingRequests: Spieler %d existiert nicht.\n", Index);
			continue;
		}

		if(Player->Request != 0 && Player->RequestProcessingGamemaster == 0){
			Players[NumPlayers] = Player;
			NumPlayers += 1;
		}
	}

	// NOTE(fusion): Little selection sort by request timestamp.
	for(int i = 0; i < (NumPlayers - 1); i += 1){
		for(int j = i + 1; j < NumPlayers; j += 1){
			if(Players[j]->RequestTimestamp < Players[i]->RequestTimestamp){
				std::swap(Players[i], Players[j]);
			}
		}
	}

	for(int i = 0; i < NumPlayers; i += 1){
		TPlayer *Player = Players[i];
		SendTalk(Connection, 0, Player->Name, TALK_GAMEMASTER_REQUEST,
				GetDynamicString(Player->Request),
				(RoundNr - Player->RequestTimestamp));
	}

	delete []Players;
}

// Player Pool
// =============================================================================
void SavePlayerPoolSlot(TPlayerData *Slot){
	if(Slot == NULL){
		error("SavePlayerPoolSlot: Slot existiert nicht.\n");
		return;
	}

	if(Slot->Dirty){
		SavePlayerData(Slot);
		Slot->Dirty = false;
	}
}

void FreePlayerPoolSlot(TPlayerData *Slot){
	if(Slot == NULL){
		error("FreePlayerPoolSlot: Slot ist NULL.\n");
		return;
	}

	print(3, "Gebe Slot von Charakter %u frei.\n", Slot->CharacterID);
	if(Slot->CharacterID == 0){
		return;
	}

	if(Slot->Sticky > 0){
		error("FreePlayerPoolSlot: Slot wird noch benötigt; darf nicht freigegeben werden.\n");
		return;
	}

	if(Slot->Locked != 0 && Slot->Locked != getpid()){
		error("FreePlayerPoolSlot: Slot ist von einem anderen Thread gesperrt.\n");
		return;
	}

	if(Slot->Dirty){
		SavePlayerData(Slot);
		Slot->Dirty = false;
	}

	delete[] Slot->Inventory;
	for(int DepotNr = 0; DepotNr < 9; DepotNr += 1){ // MAX_DEPOT ?
		delete[] Slot->Depot[DepotNr];
	}

	Slot->CharacterID = 0;
}

TPlayerData *GetPlayerPoolSlot(uint32 CharacterID){
	if(CharacterID == 0){
		error("GetPlayerPoolSlot: CharacterID ist Null.\n");
		return NULL;
	}

	TPlayerData *Slot = NULL;
	for(int i = 0; i < NARRAY(PlayerDataPool); i += 1){
		if(PlayerDataPool[i].CharacterID == CharacterID){
			Slot = &PlayerDataPool[i];
			break;
		}
	}
	return Slot;
}

TPlayerData *AssignPlayerPoolSlot(uint32 CharacterID, bool DontWait){
	if(CharacterID == 0){
		error("AssignPlayerPoolSlot: CharacterID ist Null.\n");
		return NULL;
	}

	print(3, "Reserviere Slot für Charakter %u.\n", CharacterID);

	// TODO(fusion): `PlayerDataPoolMutex` usage here could be improved, perhaps
	// with some guard class.

	TPlayerData *Slot = NULL;
	while(true){
		PlayerDataPoolMutex.down();
		Slot = GetPlayerPoolSlot(CharacterID);
		if(Slot == NULL){
			break;
		}

		if(Slot->Locked == 0){
			Slot->Locked = getpid();
			PlayerDataPoolMutex.up();
			return Slot;
		}

		if(DontWait){
			Slot->Sticky += 1;
			PlayerDataPoolMutex.up();
			return Slot;
		}

		PlayerDataPoolMutex.up();
		DelayThread(0, 100);
	}

	// NOTE(fusion): Player data for `CharacterID` isn't loaded so we need to
	// assign it one slot and load it.
	ASSERT(Slot == NULL);

	// NOTE(fusion): Try to find an empty slot.
	for(int i = 0; i < NARRAY(PlayerDataPool); i += 1){
		if(PlayerDataPool[i].CharacterID == 0){
			Slot = &PlayerDataPool[i];
			break;
		}
	}

	if(Slot == NULL){
		// NOTE(fusion): Try to find a non-empty slot that is not being used and
		// doesn't need to be saved.
		for(int i = 0; i < NARRAY(PlayerDataPool); i += 1){
			if(PlayerDataPool[i].Locked == 0
					&& PlayerDataPool[i].Sticky == 0
					&& !PlayerDataPool[i].Dirty){
				Slot = &PlayerDataPool[i];
				FreePlayerPoolSlot(Slot);
				break;
			}
		}
	}

	if(Slot == NULL){
		// NOTE(fusion): Try to find a non-empty slot that is not being used.
		for(int i = 0; i < NARRAY(PlayerDataPool); i += 1){
			if(PlayerDataPool[i].Locked == 0 && PlayerDataPool[i].Sticky == 0){
				Slot = &PlayerDataPool[i];
				FreePlayerPoolSlot(Slot);
				break;
			}
		}
	}

	if(Slot == NULL){
		PlayerDataPoolMutex.up();
		error("AssignPlayerPoolSlot: Kein Slot mehr frei.\n");
		return NULL;
	}

	memset(Slot, 0, sizeof(TPlayerData));
	Slot->CharacterID = CharacterID;
	Slot->Locked = getpid();
	PlayerDataPoolMutex.up();

	print(3, "Lade Daten für Spieler %u.\n", CharacterID);

	if(!PlayerDataExists(CharacterID)){
		Log("game", "Spieler %u loggt sich zum ersten Mal ein.\n", CharacterID);
	}

	if(!LoadPlayerData(Slot)){
		Slot->CharacterID = 0;
		Slot->Locked = 0;
		Slot = NULL;
	}

	return Slot;
}

TPlayerData *AttachPlayerPoolSlot(uint32 CharacterID, bool DontWait){
	if(CharacterID == 0){
		error("AttachPlayerPoolSlot: CharacterID ist Null.\n");
		return NULL;
	}

	print(3, "Attache Slot von Spieler %u.\n", CharacterID);

	// TODO(fusion): Same as `AssignPlayerPoolSlot`.

	while(true){
		TPlayerData *Slot = NULL;
		PlayerDataPoolMutex.down();
		Slot = GetPlayerPoolSlot(CharacterID);
		if(Slot == NULL){
			PlayerDataPoolMutex.up();
			error("AttachPlayerPoolSlot: Daten des Charakters sind nicht vorhanden.\n");
			return NULL;
		}

		if(Slot->Locked == 0){
			Slot->Locked = getpid();
			PlayerDataPoolMutex.up();
			return Slot;
		}

		if(DontWait){
			PlayerDataPoolMutex.up();
			return NULL;
		}

		PlayerDataPoolMutex.up();
		DelayThread(0, 100);
	}
}

void AttachPlayerPoolSlot(TPlayerData *Slot, bool DontWait){
	if(Slot == NULL){
		error("AttachPlayerPoolSlot: Slot ist NULL.\n");
		return;
	}

	print(3, "Attache Slot von Spieler %u.\n", Slot->CharacterID);
	while(true){
		PlayerDataPoolMutex.down();
		if(Slot->Locked == 0){
			Slot->Locked = getpid();
			PlayerDataPoolMutex.up();
			return;
		}

		if(DontWait){
			PlayerDataPoolMutex.up();
			return;
		}

		PlayerDataPoolMutex.up();
		DelayThread(0,100);
	}
}

void IncreasePlayerPoolSlotSticky(TPlayerData *Slot){
	if(Slot == NULL){
		error("IncreasePlayerPoolSlotSticky: Slot ist NULL.\n");
		return;
	}

	PlayerDataPoolMutex.down();
	Slot->Sticky += 1;
	PlayerDataPoolMutex.up();
}

void DecreasePlayerPoolSlotSticky(TPlayerData *Slot){
	if(Slot == NULL){
		error("DecreasePlayerPoolSlotSticky: Slot ist NULL.\n");
		return;
	}

	PlayerDataPoolMutex.down();
	Slot->Sticky -= 1;
	PlayerDataPoolMutex.up();
}

void DecreasePlayerPoolSlotSticky(uint32 CharacterID){
	if(CharacterID == 0){
		error("DecreasePlayerPoolSlotSticky: CharacterID ist Null.\n");
		return;
	}

	PlayerDataPoolMutex.down();
	TPlayerData *Slot = GetPlayerPoolSlot(CharacterID);
	if(Slot != NULL){
		Slot->Sticky -= 1;
	}else{
		error("DecreasePlayerPoolSlotSticky: Slot von Spieler %u nicht gefunden.\n", CharacterID);
	}
	PlayerDataPoolMutex.up();
}

void ReleasePlayerPoolSlot(TPlayerData *Slot){
	if(Slot == NULL){
		error("ReleasePlayerPoolSlot: Slot ist NULL.\n");
		return;
	}

	print(3, "Gebe Slot von Spieler %u frei.\n", Slot->CharacterID);
	if(Slot->Locked == 0){
		error("ReleasePlayerPoolSlot: Slot ist nicht gesperrt.\n");
		return;
	}

	if(Slot->Locked != getpid()){
		error("ReleasePlayerPoolSlot: Slot ist von einem anderen Thread gesperrt.\n");
		return;
	}

	Slot->Locked = 0;
}

void SavePlayerPoolSlots(void){
	time_t Now = time(NULL);
	print(3, "Speichere alle Spielerdaten...\n");
	for(int i = 0; i < NARRAY(PlayerDataPool); i += 1){
		TPlayerData *Slot = &PlayerDataPool[i];
		if(Slot->CharacterID == 0
				|| Slot->Locked != 0
				|| Slot->Sticky > 0
				|| !Slot->Dirty){
			continue;
		}

		// TODO(fusion): Logged out for at least 15 minutes?
		if((Now - Slot->LastLogoutTime) < 900){
			continue;
		}

		AttachPlayerPoolSlot(Slot, true);
		if(Slot->Locked == getpid()){
			SavePlayerPoolSlot(Slot);
			ReleasePlayerPoolSlot(Slot);
		}
	}
}

void InitPlayerPool(void){
	memset(PlayerDataPool, 0, sizeof(PlayerDataPool));
}

void ExitPlayerPool(void){
	// TODO(fusion): I assume the order in `ExitAll` is such to make this work?
	for(int i = 0; i < NARRAY(PlayerDataPool); i += 1){
		TPlayerData *Slot = &PlayerDataPool[i];
		if(Slot->CharacterID == 0){
			continue;
		}

		while(Slot->Locked != 0){
			print(2, "Warte auf Freigabe von Slot %d...\n", i);
			DelayThread(1, 0);
		}

		while(Slot->Sticky > 0){
			DelayThread(1, 0);
			AttachPlayerPoolSlot(Slot, false);
			SendMails(Slot);
			DecreasePlayerPoolSlotSticky(Slot);
			ReleasePlayerPoolSlot(Slot);
		}

		AttachPlayerPoolSlot(Slot, false);
		FreePlayerPoolSlot(Slot);
		ReleasePlayerPoolSlot(Slot);
	}
	print(1, "Alle Spielerdaten gespeichert.\n");
}

// Player Index
// =============================================================================
int GetPlayerIndexEntryNumber(const char *Name, int Position){
	if(Name == NULL){
		error("GetPlayerIndexEntryNumber: Name ist NULL.\n");
		return 0;
	}

	if(Position < (int)strlen(Name)){
		int ch = Name[Position];
		if(ch >= 'A' && ch <= 'Z'){
			return ch - 'A';
		}else if(ch >= 'a' && ch <= 'z'){
			return ch - 'a';
		}
	}
	return 0;
}

void InsertPlayerIndex(TPlayerIndexInternalNode *Node,
		int Position, const char *Name, uint32 CharacterID){
	while(true){
		if(Node == NULL){
			error("InsertPlayerIndex: Node ist NULL.\n");
			return;
		}

		if(Name == NULL){
			error("InsertPlayerIndex: Name ist NULL.\n");
			return;
		}

		int ChildIndex = GetPlayerIndexEntryNumber(Name, Position);
		TPlayerIndexNode *Child = Node->Child[ChildIndex];
		if(Child == NULL){
			TPlayerIndexLeafNode *Leaf = PlayerIndexLeafNodes.getFreeItem();
			memset(Leaf, 0, sizeof(TPlayerIndexLeafNode));
			Leaf->InternalNode = false;
			Leaf->Count = 1;
			strcpy(Leaf->Entry[0].Name, Name);
			Leaf->Entry[0].CharacterID = CharacterID;
			Node->Child[ChildIndex] = Leaf;
			return;
		}

		if(Child->InternalNode){
			Position += 1;
			Node = (TPlayerIndexInternalNode*)Child;
			continue;
		}

		TPlayerIndexLeafNode *ChildLeaf = (TPlayerIndexLeafNode*)Child;
		for(int i = 0; i < ChildLeaf->Count; i += 1){
			// NOTE(fusion): Check if entry is already in the index?
			// TODO(fusion): We should probably be using `stricmp` here.
			if(strcmp(ChildLeaf->Entry[i].Name, Name) == 0){
				return;
			}
		}

		if(ChildLeaf->Count < NARRAY(ChildLeaf->Entry)){
			strcpy(ChildLeaf->Entry[ChildLeaf->Count].Name, Name);
			ChildLeaf->Entry[ChildLeaf->Count].CharacterID = CharacterID;
			ChildLeaf->Count += 1;
			return;
		}

		// NOTE(fusion): Child leaf node is full so we need to create a new
		// internal node and move all previous leaf entries to it. Note that
		// the position is also increased here, so entries should get different
		// child indices.
		TPlayerIndexInternalNode *ChildInternal = PlayerIndexInternalNodes.getFreeItem();
		memset(ChildInternal, 0, sizeof(TPlayerIndexInternalNode));
		ChildInternal->InternalNode = true;
		Node->Child[ChildIndex] = ChildInternal;

		for(int i = 0; i < ChildLeaf->Count; i += 1){
			InsertPlayerIndex(ChildInternal, Position + 1,
					ChildLeaf->Entry[i].Name,
					ChildLeaf->Entry[i].CharacterID);
		}
		PlayerIndexLeafNodes.putFreeItem(ChildLeaf);

		Position += 1;
		Node = ChildInternal;
	}
}

TPlayerIndexEntry *SearchPlayerIndex(const char *Name){
	if(Name == NULL){
		error("SearchPlayerIndex: Name ist NULL.\n");
		return NULL;
	}

	TPlayerIndexNode *Node = &PlayerIndexHead;
	for(int Position = 0; Node->InternalNode; Position += 1){
		int ChildIndex = GetPlayerIndexEntryNumber(Name, Position);
		Node = ((TPlayerIndexInternalNode*)Node)->Child[ChildIndex];
		if(Node == NULL){
			return NULL;
		}
	}

	ASSERT(!Node->InternalNode);
	TPlayerIndexLeafNode *Leaf = (TPlayerIndexLeafNode*)Node;
	for(int i = 0; i < Leaf->Count; i += 1){
		if(stricmp(Leaf->Entry[i].Name, Name) == 0){
			return &Leaf->Entry[i];
		}
	}

	return NULL;
}

bool PlayerExists(const char *Name){
	if(Name == NULL){
		error("PlayerExists: Name ist NULL.\n");
		return false;
	}

	return SearchPlayerIndex(Name) != NULL;
}

uint32 GetCharacterID(const char *Name){
	if(Name == NULL){
		error("GetCharacterID: Name ist NULL.\n");
		return 0;
	}

	uint32 Result = 0;
	TPlayerIndexEntry *Entry = SearchPlayerIndex(Name);
	if(Entry != NULL){
		Result = Entry->CharacterID;
	}
	return Result;
}

const char *GetCharacterName(const char *Name){
	if(Name == NULL){
		error("GetCharacterName: Name ist NULL.\n");
		return NULL;
	}

	const char *Result = "Unknown";
	TPlayerIndexEntry *Entry = SearchPlayerIndex(Name);
	if(Entry != NULL){
		Result = Entry->Name;
	}
	return Result;
}

void InitPlayerIndex(void){
	memset(&PlayerIndexHead, 0, sizeof(TPlayerIndexInternalNode));
	PlayerIndexHead.InternalNode = true;

	// TODO(fusion): The `QueryBufferSize` parameter to `TQueryManagerConnection`
	// is probably related to the size of both these arrays. There gotta be some
	// type of limit per load query or we'd easily overflow these buffers. It may
	// be hardcoded.
	uint32 CharacterIDs[10000];
	char Names[10000][30];
	TQueryManagerConnection *QueryManager = new TQueryManagerConnection(360007);
	if(!QueryManager->isConnected()){
		error("InitPlayerIndex: Kann nicht zum Query-Manager verbinden.\n");
		return;
	}

	int MinimumCharacterID = 0;
	while(true){
		int NumberOfPlayers;
		int Ret = QueryManager->loadPlayers(MinimumCharacterID,
				&NumberOfPlayers, Names, CharacterIDs);
		if(Ret != 0){
			error("InitPlayerIndex: Kann Spielerdaten nicht ermitteln.\n");
			break;
		}

		for(int i = 0; i < NumberOfPlayers; i += 1){
			InsertPlayerIndex(&PlayerIndexHead, 0, Names[i], CharacterIDs[i]);
		}

		if(NumberOfPlayers < 10000){
			break;
		}

		MinimumCharacterID = CharacterIDs[9999] + 1;
	}

	delete QueryManager;
}

void ExitPlayerIndex(void){
	// no-op
}

// Initialization
// =============================================================================
void InitPlayer(void){
	InitPlayerPool();
	CreatePlayerList(true);
	InitPlayerIndex();
}

void ExitPlayer(void){
	ExitPlayerPool();
	CreatePlayerList(false);
	ExitPlayerIndex();
}

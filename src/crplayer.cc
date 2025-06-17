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
TPlayer::TPlayer(TConnection *Connection, uint32 CharacterID):
		TCreature(),
		AttackedPlayers(0, 10, 10),
		FormerAttackedPlayers(0, 10, 10)
{
	this->Type = PLAYER;
	this->LoseInventory = LOSE_INVENTORY_SOME;
	this->Profession = PROFESSION_NONE;
	this->AccountID = 0;
	this->Guild[0] = 0;
	this->Rank[0] = 0;
	this->Title[0] = 0;
	this->IPAddress[0] = 0;
	this->Depot = NONE;
	this->DepotNr = 0;
	this->DepotSpace = 0;
	this->ConstructError = NOERROR;
	this->PlayerData = NULL;
	this->TradeObject = NONE;
	this->TradePartner = 0;
	this->TradeAccepted = false;
	this->OldState = 0;
	this->Request = 0;
	this->RequestTimestamp = 0;
	this->RequestProcessingGamemaster = 0;
	this->TutorActivities = 0;
	this->NumberOfAttackedPlayers = 0;
	this->Aggressor = false;
	this->NumberOfFormerAttackedPlayers = 0;
	this->FormerAggressor = false;
	this->FormerLogoutRound = 0;
	this->PartyLeader = 0;
	this->PartyLeavingRound = 0;
	this->TalkBufferFullTime = 0;
	this->MutingEndRound = 0;
	this->NumberOfMutings = 0;

	memset(this->Rights, 0, sizeof(this->Rights));

	for(int SpellNr = 0;
			SpellNr < NARRAY(this->SpellList);
			SpellNr += 1){
		this->SpellList[SpellNr] = 0;
	}

	for(int QuestNr = 0;
			QuestNr < NARRAY(this->QuestValues);
			QuestNr += 1){
		this->QuestValues[QuestNr] = 0;
	}

	for(int ContainerNr = 0;
			ContainerNr < NARRAY(this->OpenContainer);
			ContainerNr += 1){
		this->OpenContainer[ContainerNr] = NONE;
	}

	STATIC_ASSERT(NARRAY(this->Addressees) == NARRAY(this->AddresseesTimes));
	for(int AddresseeNr = 0;
			AddresseeNr < NARRAY(this->Addressees);
			AddresseeNr += 1){
		this->Addressees[AddresseeNr] = 0;
		this->AddresseesTimes[AddresseeNr] = 0;
	}

	TPlayerData *Slot = AttachPlayerPoolSlot(CharacterID, false);
	if(Slot == NULL){
		error("TPlayer::TPlayer: PlayerData-Slot nicht gefunden.\n");
		this->ConstructError = ERROR;
		return;
	}

	SendMails(Slot);

	this->PlayerData = Slot;
	this->Connection = Connection;
	strcpy(this->Name, Slot->Name);
	this->SetID(CharacterID);

	InsertPlayerIndex(&PlayerIndexHead, 0, this->Name, CharacterID);
	this->LoadData();
	this->AccountID = Slot->AccountID;
	strcpy(this->IPAddress, Connection->GetIPAddress());

	STATIC_ASSERT(sizeof(this->Rights) == sizeof(Slot->Rights));
	memcpy(this->Rights, Slot->Rights, sizeof(this->Rights));

	this->Sex = Slot->Sex;
	strcpy(this->Guild, Slot->Guild);
	strcpy(this->Rank, Slot->Rank);
	strcpy(this->Title, Slot->Title);

	if(Slot->PlayerkillerEnd < (int)time(NULL)){
		Slot->PlayerkillerEnd = 0;
	}

	this->CheckOutfit();
	this->SetInList();

	// NOTE(fusion): Soul regen. Could be some inlined function `CheckSoul`.
	{
		TSkill *Soul = this->Skills[SKILL_SOUL];
		Soul->Max = (this->GetActivePromotion() ? 200 : 100);
		Soul->Check();

		// TODO(fusion): We're rounding up to an extra regen cycle here but I'm
		// not sure it is correct.
		int Timer = Soul->TimerValue();
		if(Timer >= 15){
			int Interval = (this->GetActivePromotion() ? 15 : 120);
			int Cycle = (Timer + Interval - 1) / Interval;
			int Count = (Timer + Interval - 1) % Interval;
			this->SetTimer(SKILL_SOUL, Cycle, Count, Interval, -1);
		}
	}

	// TODO(fusion): Handle login after death?
	if(this->Skills[SKILL_HITPOINTS]->Get() <= 0){
		for(int SkillNr = 0;
				SkillNr < NARRAY(this->Skills);
				SkillNr += 1){
			this->DelTimer(SkillNr);
			this->Skills[SkillNr]->SetMDAct(0);
			this->Skills[SkillNr]->DAct = 0;
		}

		this->Skills[SKILL_HITPOINTS]->Set(this->Skills[SKILL_HITPOINTS]->Max);
		this->Skills[SKILL_MANA     ]->Set(this->Skills[SKILL_MANA     ]->Max);
		this->Outfit = this->OrgOutfit;
		this->posx = this->startx;
		this->posy = this->starty;
		this->posz = this->startz;
	}

	if(Slot->LastLoginTime == 0 && CheckRight(CharacterID, GAMEMASTER_OUTFIT)){
		Log("game", "Gamemaster-Charakter %s loggt zum ersten Mal ein -> Level 2 setzen.\n", this->Name);
		this->Skills[SKILL_LEVEL]->Act = 2;
		this->Skills[SKILL_LEVEL]->Exp = 100;
		this->Skills[SKILL_LEVEL]->LastLevel = 100;
		this->Skills[SKILL_LEVEL]->NextLevel = 200;
	}

	if(CoordinateFlag(this->posx, this->posy, this->posz, BED)){
		this->Regenerate();
	}

	uint16 HouseID = GetHouseID(this->posx, this->posy, this->posz);
	if(HouseID != 0
			&& !IsInvited(HouseID, this, Slot->LastLogoutTime)
			&& !CheckRight(CharacterID, ENTER_HOUSES)){
		GetExitPosition(HouseID, &this->posx, &this->posy, &this->posz);
	}

	if(!CheckRight(CharacterID, PREMIUM_ACCOUNT)){
		if(IsPremiumArea(this->startx, this->starty, this->startz)){
			Log("game", "Spieler %s wird aus PayArea-Stadt ausgebürgert und erhält neue Heimatstadt.\n", this->Name);
			GetStartPosition(&this->startx, &this->starty, &this->startz, (this->Profession == PROFESSION_NONE));
		}

		if(IsPremiumArea(this->posx, this->posy, this->posz)){
			Log("game", "Spieler %s wird aus PayArea geworfen und in seine Heimatstadt gesetzt.\n", this->Name);
			this->posx = this->startx;
			this->posy = this->starty;
			this->posz = this->startz;
		}
	}else{
		Log("game", "Spieler besitzt Premium Account.\n");
	}

	this->SetOnMap();
	Connection->EnterGame();
	SendInitGame(Connection, CharacterID);
	SendRights(Connection);
	SendFullScreen(Connection);
	GraphicalEffect(this->CrObject, EFFECT_ENERGY);
	this->LoadInventory(Slot->LastLoginTime == 0);
	this->NotifyChangeInventory();
	SendAmbiente(Connection);
	AnnounceChangedCreature(CharacterID, CREATURE_LIGHT_CHANGED);
	SendPlayerSkills(Connection);
	this->CheckState();
	this->SendBuddies();

	if(Slot->LastLoginTime != 0){
		char TimeString[100];
		struct tm LastLogin = GetLocalTimeTM(Slot->LastLoginTime);
		strftime(TimeString, sizeof(TimeString), "%d. %b %Y %X %Z", &LastLogin);
		SendMessage(Connection, TALK_LOGIN_MESSAGE,
				"Your last visit in Tibia: %s.", TimeString);
	}else if(!CheckRight(this->ID, GAMEMASTER_OUTFIT)){
		Log("game", "Spieler %s loggt zum ersten Mal ein -> Outfitwahl.\n", this->Name);
		SendMessage(Connection, TALK_LOGIN_MESSAGE,
				"Welcome to Tibia! Please choose your outfit.");
		SendOutfit(Connection);
	}

	Slot->LastLoginTime = time(NULL);
}

TPlayer::~TPlayer(void){
	LogoutOrder(this);
	if(this->ConstructError != NOERROR){
		this->DelInList();
		return;
	}

	ASSERT(this->PlayerData);
	TPlayerData *Slot = this->PlayerData;
	Slot->LastLogoutTime = time(NULL);
	this->SaveData();

	if(!this->IsDead){
		Log("game", "Spieler %s loggt aus.\n", this->Name);
		GraphicalEffect(this->posx, this->posy, this->posz, EFFECT_POFF);
		this->SaveInventory();
	}else{
		Log("game", "Spieler %s ist gestorben.\n", this->Name);

		// NOTE(fusion): This is a disaster. We're deleting the slot's inventory
		// here so `~TCreature` can handle dropping loot and then re-generate it
		// with `SaveInventory` if `LoseInventory` is not `LOSE_INVENTORY_ALL`,
		// which makes sense but is poorly executed.
		delete[] Slot->Inventory;
		Slot->Inventory = NULL;

		bool ResetCharacter = false;
		if(this->Profession != PROFESSION_NONE && this->Skills[SKILL_LEVEL]->Get() <= 5){
			Log("game", "Setze Spieler %s komplett zurück wegen Level.\n", this->Name);
			ResetCharacter = true;
		}else if(this->Skills[SKILL_HITPOINTS]->Max <= 0){
			Log("game", "Setze Spieler %s komplett zurück wegen MaxHitpoints.\n", this->Name);
			ResetCharacter = true;
		}

		if(ResetCharacter){
			Slot->CurrentOutfit = Slot->OriginalOutfit;
			Slot->startx = 0;
			Slot->starty = 0;
			Slot->startz = 0;
			Slot->posx = 0;
			Slot->posy = 0;
			Slot->posz = 0;
			Slot->Profession = PROFESSION_NONE;

			for(int SpellNr = 0;
					SpellNr < NARRAY(Slot->SpellList);
					SpellNr += 1){
				Slot->SpellList[SpellNr] = 0;
			}

			for(int QuestNr = 0;
					QuestNr < NARRAY(Slot->QuestValues);
					QuestNr += 1){
				Slot->QuestValues[QuestNr] = 0;
			}

			// TODO(fusion): This one looks sketchy.
			for(int SkillNr = 0;
					SkillNr < NARRAY(Slot->Minimum);
					SkillNr += 1){
				Slot->Minimum[SkillNr] = INT_MIN;
			}

			this->LoseInventory = LOSE_INVENTORY_ALL;
		}

		if(Slot->PlayerkillerEnd != 0){
			this->LoseInventory = LOSE_INVENTORY_ALL;
		}

		if(CheckRight(this->ID, KEEP_INVENTORY)){
			this->LoseInventory = LOSE_INVENTORY_NONE;
		}
	}

	if(CheckRight(this->ID, READ_GAMEMASTER_CHANNEL)){
		CloseProcessedRequests(this->ID);
	}

	if(this->Request != 0){
		if(this->RequestProcessingGamemaster == 0){
			DeleteGamemasterRequest(this->Name);
		}else{
			TCreature *Gamemaster = GetCreature(this->RequestProcessingGamemaster);
			if(Gamemaster != NULL){
				if(Gamemaster->Type == PLAYER){
					SendFinishRequest(Gamemaster->Connection, this->Name);
				}else{
					error("GetPlayer: Kreatur ist kein Spieler.\n");
				}
			}
		}
		this->Request = 0;
	}

	LeaveAllChannels(this->ID);

	if(this->GetPartyLeader(false) != 0){
		::LeaveParty(this->ID, true);
	}

	this->ClearPlayerkillingMarks();
	this->DelInList();

	Slot->Dirty = true;
	// TODO(fusion): Something is telling me that `Slot->Sticky` is also poorly managed.
	DecreasePlayerPoolSlotSticky(Slot);
	ReleasePlayerPoolSlot(Slot);
}

void TPlayer::SetInList(void){
	this->SetInCrList();

	PlayerMutex.down();
	*PlayerList.at(FirstFreePlayer) = this;
	FirstFreePlayer += 1;
	PlayerMutex.up();

	NotifyBuddies(this->ID, this->Name, true);
	IncrementPlayersOnline();
	if(this->Profession == PROFESSION_NONE){
		IncrementNewbiesOnline();
	}
}

void TPlayer::DelInList(void){
	NotifyBuddies(this->ID, this->Name, false);

	for(int Index = 0; Index < FirstFreePlayer; Index += 1){
		if(*PlayerList.at(Index) == this){
			// TODO(fusion): This can't be right? If the player at `Index` changes
			// before entering the critical section, we could end up removing the
			// wrong player from the list..
			PlayerMutex.down();
			FirstFreePlayer -= 1;
			*PlayerList.at(Index) = *PlayerList.at(FirstFreePlayer);
			*PlayerList.at(FirstFreePlayer) = NULL;
			PlayerMutex.up();

			DecrementPlayersOnline();
			if(this->Profession == PROFESSION_NONE){
				DecrementNewbiesOnline();
			}

			break;
		}
	}
}

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

		if(Player->Request != 0 && Player->RequestProcessingGamemaster == CharacterID){
			SendCloseRequest(Player->Connection);
			Player->Request = 0;
		}
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
	TQueryManagerConnection QueryManager(360007);
	if(!QueryManager.isConnected()){
		error("InitPlayerIndex: Kann nicht zum Query-Manager verbinden.\n");
		return;
	}

	int MinimumCharacterID = 0;
	while(true){
		int NumberOfPlayers;
		int Ret = QueryManager.loadPlayers(MinimumCharacterID,
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

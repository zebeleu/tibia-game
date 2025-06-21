#include "connections.hh"
#include "cr.hh"
#include "info.hh"
#include "threads.hh"

#include "stubs.hh"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

static Semaphore ConnectionMutex(1);
static int ConnectionIterator;
static TConnection *FirstSendingConnection;
static TConnection Connections[MAX_CONNECTIONS];

enum ClientCommand: int {
	CL_CMD_LOGIN					= 10,
	CL_CMD_LOGIN_TREATED			= 11,
	CL_CMD_LOGOUT					= 20,
	CL_CMD_PING						= 30,
	CL_CMD_GO						= 100,
	CL_CMD_GO_NORTH					= 101,
	CL_CMD_GO_EAST					= 102,
	CL_CMD_GO_SOUTH					= 103,
	CL_CMD_GO_WEST					= 104,
	CL_CMD_GO_STOP					= 105,
	CL_CMD_GO_NORTHEAST				= 106,
	CL_CMD_GO_SOUTHEAST				= 107,
	CL_CMD_GO_SOUTHWEST				= 108,
	CL_CMD_GO_NORTHWEST				= 109,
	CL_CMD_ROTATE_NORTH				= 111,
	CL_CMD_ROTATE_EAST				= 112,
	CL_CMD_ROTATE_SOUTH				= 113,
	CL_CMD_ROTATE_WEST				= 114,
	CL_CMD_MOVE_OBJECT				= 120,
	CL_CMD_TRADE_OBJECT				= 125,
	CL_CMD_INSPECT_TRADE			= 126,
	CL_CMD_ACCEPT_TRADE				= 127,
	CL_CMD_REJECT_TRADE				= 128,
	CL_CMD_USE_OBJECT				= 130,
	CL_CMD_USE_TWO_OBJECTS			= 131,
	CL_CMD_USE_ON_CREATURE			= 132,
	CL_CMD_TURN_OBJECT				= 133,
	CL_CMD_CLOSE_CONTAINER			= 135,
	CL_CMD_UP_CONTAINER				= 136,
	CL_CMD_EDIT_TEXT				= 137,
	CL_CMD_EDIT_LIST				= 138,
	CL_CMD_LOOK_AT_POINT			= 140,
	CL_CMD_TALK						= 150,
	CL_CMD_GET_CHANNELS				= 151,
	CL_CMD_JOIN_CHANNEL				= 152,
	CL_CMD_LEAVE_CHANNEL			= 153,
	CL_CMD_PRIVATE_CHANNEL			= 154,
	CL_CMD_PROCESS_REQUEST			= 155,
	CL_CMD_REMOVE_REQUEST			= 156,
	CL_CMD_CANCEL_REQUEST			= 157,
	CL_CMD_SET_TACTICS				= 160,
	CL_CMD_ATTACK					= 161,
	CL_CMD_FOLLOW					= 162,
	CL_CMD_INVITE_TO_PARTY			= 163,
	CL_CMD_JOIN_PARTY				= 164,
	CL_CMD_REVOKE_INVITATION		= 165,
	CL_CMD_PASS_LEADERSHIP			= 166,
	CL_CMD_LEAVE_PARTY				= 167,
	CL_CMD_OPEN_CHANNEL				= 170,
	CL_CMD_INVITE_TO_CHANNEL		= 171,
	CL_CMD_EXCLUDE_FROM_CHANNEL		= 172,
	CL_CMD_CANCEL					= 190,
	CL_CMD_REFRESH_FIELD			= 201,
	CL_CMD_REFRESH_CONTAINER		= 202,
	CL_CMD_GET_OUTFIT				= 210,
	CL_CMD_SET_OUTFIT				= 211,
	CL_CMD_ADD_BUDDY				= 220,
	CL_CMD_REMOVE_BUDDY				= 221,
	CL_CMD_BUG_REPORT				= 230,
	CL_CMD_RULE_VIOLATION			= 231,
	CL_CMD_ERROR_FILE_ENTRY			= 232,
};

enum ServerCommand: int {
	SV_CMD_LOGIN_ERROR				= 20,
	SV_CMD_LOGIN_PREMIUM			= 21,
	SV_CMD_LOGIN_WAITINGLIST		= 22,
};

// TConnection
// =============================================================================
TConnection::TConnection(void){
	this->State = CONNECTION_FREE;
}

void TConnection::Process(void){
	if(this->InGame()){
		uint32 LastCommand = (RoundNr - this->TimeStamp);
		if(LastCommand == 30 || LastCommand == 60){
			SendPing(this);
		}

		uint32 LastAction = (RoundNr - this->TimeStampAction);
		if(LastAction == 900 && !CheckRight(this->CharacterID, NO_LOGOUT_BLOCK)){
			SendMessage(this, TALK_ADMIN_MESSAGE,
					"You have been idle for %d minutes. You will be disconnected"
					" in one minute if you are still idle then.", 15);
		}

		if(LastAction >= 960 && !CheckRight(this->CharacterID, NO_LOGOUT_BLOCK)){
			this->Logout(0, true);
		}else if(!GameRunning() || !this->ConnectionIsOk || (LastCommand >= 90)){
			this->Logout(0, false);
		}
	}else if(this->State == CONNECTION_LOGIN){
		if(!GameRunning() || !this->ConnectionIsOk){
			this->Disconnect();
		}
	}else if(this->State == CONNECTION_LOGOUT){
		// NOTE(fusion): `TimeStamp` has the logout round which is set by
		// `TConnection::Logout`.
		if(!GameRunning() || !this->ConnectionIsOk || this->TimeStamp <= RoundNr){
			this->Disconnect();
		}
	}
}

void TConnection::ResetTimer(int Command){
	if(this->InGame()){
		this->TimeStamp = RoundNr;
		if(Command != CL_CMD_PING
				&& Command != CL_CMD_GO_STOP
				&& Command != CL_CMD_CANCEL
				&& Command != CL_CMD_REFRESH_FIELD
				&& Command != CL_CMD_REFRESH_CONTAINER){
			this->TimeStampAction = RoundNr;
		}
	}
}

void TConnection::EmergencyPing(void){
	if(this->InGame()){
		uint32 LastCommand = (RoundNr - this->TimeStamp);
		if(LastCommand < 80){
			// TODO(fusion): This is only called by `NetLoadCheck`, when it detects
			// lag, which can only happen after some set number of rounds, usually
			// higher than 100. This is all to say this subtraction below is very
			// unlikely to wrap. Nevertheless, we should also have some helper
			// functions to do saturating addition or subtraction for both signed
			// and unsigned integers.
			this->TimeStamp = RoundNr - 100;
		}
		SendPing(this);
	}
}

pid_t TConnection::GetPID(void){
	if(this->State == CONNECTION_FREE){
		error("TConnection::GetPID: Verbindung ist nicht zugewiesen.\n");
		return 0;
	}

	return this->PID;
}

int TConnection::GetSocket(void){
	if(this->State == CONNECTION_FREE || this->State == CONNECTION_ASSIGNED){
		error("TConnection::GetSocket: Verbindung ist nicht angeschlossen.\n");
		return -1;
	}

	return this->Socket;
}

const char *TConnection::GetIPAddress(void){
	if(this->State == CONNECTION_FREE || this->State == CONNECTION_ASSIGNED){
		error("TConnection::GetIPAddress: Verbindung ist nicht angeschlossen.\n");
		return "Unknown";
	}

	return this->IPAddress;
}

void TConnection::Free(void){
	this->State = CONNECTION_FREE;
}

void TConnection::Assign(void){
	if(this->State != CONNECTION_FREE){
		error("TConnection::Assign: Verbindung ist nicht frei.\n");
	}

	this->State = CONNECTION_ASSIGNED;
	this->PID = getpid();
}

void TConnection::Connect(int Socket){
	if(this->State != CONNECTION_ASSIGNED){
		error("TConnection::Connect: Verbindung ist keinem Thread zugewiesen.\n");
	}

	this->State = CONNECTION_CONNECTED;
	this->Socket = Socket;
	this->ConnectionIsOk = true;
	this->ClosingIsDelayed = true;
	this->RandomSeed = rand();

	struct sockaddr_in RemoteAddr;
	socklen_t RemoteAddrLen = sizeof(RemoteAddr);
	getpeername(Socket, (struct sockaddr*)&RemoteAddr, &RemoteAddrLen);
	strcpy(this->IPAddress, inet_ntoa(RemoteAddr.sin_addr));
}

void TConnection::Login(void){
	if(this->State != CONNECTION_CONNECTED){
		error("TConnection::Connect: Ungültiger Verbindungs-Zustand %d.\n", this->State);
	}

	this->State = CONNECTION_LOGIN;
}

bool TConnection::JoinGame(TReadBuffer *Buffer){
	if(this->State != CONNECTION_LOGIN){
		error("TConnection::JoinGame: Ungültiger Verbindungszustand %d.\n", this->State);
	}

	this->ClearKnownCreatureTable(false);
	
	try{
		this->TerminalType = (int)Buffer->readWord();
		this->TerminalVersion = (int)Buffer->readWord();
		this->CharacterID = Buffer->readQuad();
	}catch(const char *str){
		error("TConnection::JoinGame: Fehler beim Auslesen des Puffers (%s).\n", str);
	}

	if(this->TerminalType != 1 && this->TerminalType != 2){
		error("TConnection::JoinGame: Unbekannter Terminal-Typ %d.\n", this->TerminalType);
		return false;
	}

	this->TerminalOffsetX = 8;
	this->TerminalOffsetY = 6;
	this->TerminalWidth = 18;
	this->TerminalHeight = 14;

	TPlayer *Player = ::GetPlayer(this->CharacterID);
	if(Player == NULL){
		Player = new TPlayer(this, this->CharacterID);
		if(Player->ConstructError != NOERROR){
			delete Player;
			return false;
		}
	}else{
		if(Player->IsDead){
			Log("game", "Spieler %s ist gerade am Sterben - Einloggen gescheitert.\n", Player->Name);
			DecreasePlayerPoolSlotSticky(this->CharacterID);
			return false;
		}

		if(Player->LoggingOut && Player->LogoutPossible() == 0){
			Log("game", "Spieler %s loggt gerade aus - Einloggen gescheitert.\n", Player->Name);
			DecreasePlayerPoolSlotSticky(this->CharacterID);
			return false;
		}

		TConnection *OldConnection = Player->Connection;
		Player->ClearConnection();
		if(OldConnection != NULL){
			OldConnection->CharacterID = 0;
			OldConnection->Logout(0, true);
		}

		DecrementIsOnlineOrder(this->CharacterID);
		Player->TakeOver(this);
	}

	strcpy(this->Name, Player->Name);
	this->TimeStamp = RoundNr;
	this->TimeStampAction = RoundNr;
	return true;
}

void TConnection::EnterGame(void){
	if(this->State != CONNECTION_LOGIN){
		error("TConnection::EnterGame: Ungültiger Verbindungszustand %d.\n", this->State);
	}

	this->State = CONNECTION_GAME;
}

void TConnection::Die(void){
	if(this->State == CONNECTION_GAME){
		this->State = CONNECTION_DEAD;
	}
}

void TConnection::Logout(int Delay, bool StopFight){
	if(!this->InGame() && this->State != CONNECTION_LOGOUT){
		error("TConnection::Logout: Ungültiger Verbindungszustand %d.\n", this->State);
	}

	this->State = CONNECTION_LOGOUT;
	if(this->CharacterID != 0){
		TPlayer *Player = ::GetPlayer(this->CharacterID);
		if(Player != NULL){
			Player->ClearConnection();
			Player->StartLogout(false, StopFight);
		}
	}

	if(Delay < 0){
		Delay = 0;
	}

	this->CharacterID = 0;
	this->TimeStamp = RoundNr + (uint32)Delay;
	this->ClosingIsDelayed = false;
}

void TConnection::Close(bool Delay){
	if(this->State == CONNECTION_FREE || this->State == CONNECTION_ASSIGNED){
		error("TConnection::Close: Ungültiger Verbindungszustand %d.\n", this->State);
	}

	if(this->State == CONNECTION_CONNECTED){
		this->State = CONNECTION_DISCONNECTED;
	}

	this->ConnectionIsOk = false;
	this->ClosingIsDelayed = Delay;
}

void TConnection::Disconnect(void){
	if(this->State == CONNECTION_FREE || this->State == CONNECTION_ASSIGNED){
		error("TConnection::Close: Ungültiger Verbindungszustand %d.\n", this->State);
	}

	this->ClearKnownCreatureTable(true);
	this->ConnectionIsOk = false;
	this->State = CONNECTION_DISCONNECTED;
	kill(this->PID, SIGHUP);
}

TPlayer *TConnection::GetPlayer(void){
	if(!this->Live()){
		error("TConnection::GetPlayer: Ungültiger Verbindungszustand %d.\n", this->State);
		return NULL;
	}

	TPlayer *Player = NULL;
	if(this->CharacterID != 0){
		Player = ::GetPlayer(this->CharacterID);
	}
	return Player;
}

const char *TConnection::GetName(void){
	if(!this->Live()){
		error("TConnection::GetName: Ungültiger Verbindungszustand %d.\n", this->State);
		return "";
	}

	return this->Name;
}

void TConnection::GetPosition(int *x, int *y, int *z){
	TPlayer *Player = this->GetPlayer();
	if(Player != NULL){
		*x = Player->posx;
		*y = Player->posy;
		*z = Player->posz;
	}else{
		*x = 0;
		*y = 0;
		*z = 0;
	}
}

bool TConnection::IsVisible(int x, int y, int z){
	int PlayerX, PlayerY, PlayerZ;
	this->GetPosition(&PlayerX, &PlayerY, &PlayerZ);

	// NOTE(fusion): The player can see only floors above ground when above
	// ground, or two floors up and down when underground.
	if(PlayerZ <= 7){
		if(z > 7){
			return false;
		}
	}else{
		if(std::abs(PlayerZ - z) > 2){
			return false;
		}
	}

	int MinX = (PlayerX - this->TerminalOffsetX) + (PlayerZ - z);
	int MinY = (PlayerY - this->TerminalOffsetY) + (PlayerZ - z);
	int MaxX = MinX + this->TerminalWidth - 1;
	int MaxY = MinY + this->TerminalHeight - 1;
	return x >= MinX && x <= MaxX
		&& y >= MinY && y <= MaxY;
}

KNOWNCREATURESTATE TConnection::KnownCreature(uint32 ID, bool UpdateFollows){
	int EntryIndex = -1;
	for(int i = 0; i < NARRAY(this->KnownCreatureTable); i += 1){
		if(this->KnownCreatureTable[i].CreatureID == ID){
			EntryIndex = i;
			break;
		}
	}

	if(EntryIndex == -1){
		return KNOWNCREATURE_FREE;
	}

	KNOWNCREATURESTATE Result = this->KnownCreatureTable[EntryIndex].State;
	if(Result == KNOWNCREATURE_OUTDATED && UpdateFollows){
		this->KnownCreatureTable[EntryIndex].State = KNOWNCREATURE_UPTODATE;
	}
	return Result;
}

uint32 TConnection::NewKnownCreature(uint32 NewID){
	uint32 OldID = 0;
	int EntryIndex = -1;
	for(int i = 0; i < NARRAY(this->KnownCreatureTable); i += 1){
		if(this->KnownCreatureTable[i].CreatureID == NewID){
			OldID = NewID;
			EntryIndex = i;
			break;
		}
	}

	if(EntryIndex == -1){
		for(int i = 0; i < NARRAY(this->KnownCreatureTable); i += 1){
			if(this->KnownCreatureTable[i].State == KNOWNCREATURE_FREE){
				OldID = this->KnownCreatureTable[i].CreatureID;
				EntryIndex = i;
				break;
			}
		}
	}

	if(EntryIndex == -1){
		for(int i = 0; i < NARRAY(this->KnownCreatureTable); i += 1){
			TCreature *Creature = GetCreature(this->KnownCreatureTable[i].CreatureID);
			if(Creature == NULL || !this->IsVisible(Creature->posx, Creature->posy, Creature->posz)){
				OldID = this->KnownCreatureTable[i].CreatureID;
				EntryIndex = i;
				this->UnchainKnownCreature(OldID);
				break;
			}
		}
	}

	if(EntryIndex == -1){
		print(3, "KnownCreatureTable ausgelastet.\n");
		return 0;
	}

	if(this->KnownCreatureTable[EntryIndex].State != KNOWNCREATURE_FREE){
		error("TUserCom::NewKnownCreature: Slot ist nicht gelöscht.\n");
	}

	this->KnownCreatureTable[EntryIndex].State = KNOWNCREATURE_UPTODATE;
	this->KnownCreatureTable[EntryIndex].CreatureID = NewID;

	TCreature *Creature = GetCreature(NewID);
	if(Creature != NULL){
		this->KnownCreatureTable[EntryIndex].Next = Creature->FirstKnowingConnection;
		Creature->FirstKnowingConnection = &this->KnownCreatureTable[EntryIndex];
	}else{
		error("TUserCom::NewKnownCreature: Kreatur %u existiert nicht.\n", NewID);
	}

	return OldID;
}

void TConnection::ClearKnownCreatureTable(bool Unchain){
	for(int i = 0; i < NARRAY(this->KnownCreatureTable); i += 1){
		if(Unchain && this->KnownCreatureTable[i].State != KNOWNCREATURE_FREE){
			this->UnchainKnownCreature(this->KnownCreatureTable[i].CreatureID);
		}
		this->KnownCreatureTable[i].State = KNOWNCREATURE_FREE;
		this->KnownCreatureTable[i].CreatureID = 0;
		this->KnownCreatureTable[i].Connection = this;
	}
}

void TConnection::UnchainKnownCreature(uint32 ID){
	TCreature *Creature = GetCreature(ID);
	if(Creature == NULL){
		error("TUserCom::UnchainKnownCreature: Kreatur %u existiert nicht.\n", ID);
		return;
	}

	if(Creature->FirstKnowingConnection == NULL){
		error("TUserCom::UnchainKnownCreature: Kreatur %u kennt niemand.\n", ID);
		return;
	}

	TKnownCreature *Prev = NULL;
	TKnownCreature *Cur = Creature->FirstKnowingConnection;
	while(Cur != NULL){
		if(Cur->Connection == this){
			break;
		}
		Prev = Cur;
		Cur = Cur->Next;
	}

	if(Cur == NULL){
		error("TUserCom::UnchainKnownCreature: Kreatur %u ist nicht bekannt.\n", ID);
		return;
	}

	Cur->State = KNOWNCREATURE_FREE;
	if(Prev == NULL){
		Creature->FirstKnowingConnection = Cur->Next;
	}else{
		Prev->Next = Cur->Next;
	}
}

// Connection Utility
// =============================================================================
TConnection *AssignFreeConnection(void){
	TConnection *Connection = NULL;
	ConnectionMutex.down();
	for(int i = 0; i < MAX_CONNECTIONS; i += 1){
		if(Connections[i].State == CONNECTION_FREE){
			Connection = &Connections[i];
			Connection->Assign();
			break;
		}
	}
	ConnectionMutex.up();
	return Connection;
}

TConnection *GetFirstConnection(void){
	ConnectionIterator = 0;
	return GetNextConnection();
}

TConnection *GetNextConnection(void){
	TConnection *NextConnection = NULL;
	while(ConnectionIterator < MAX_CONNECTIONS){
		TConnection *Connection = &Connections[ConnectionIterator];
		ConnectionIterator += 1;
		if(Connection->State != CONNECTION_FREE){
			NextConnection = Connection;
			break;
		}
	}
	return NextConnection;
}

void ProcessConnections(void){
	TConnection *Connection = GetFirstConnection();
	while(Connection != NULL){
		Connection->Process();
		Connection = GetNextConnection();
	}
}

void InitConnections(void){
	ConnectionIterator = 0;
	FirstSendingConnection = NULL;
	for(int i = 0; i < MAX_CONNECTIONS; i += 1){
		Connections[i].State = CONNECTION_FREE;
	}
}

void ExitConnections(void){
	// no-op
}

#ifndef TIBIA_PLAYER_HH_
#define TIBIA_PLAYER_HH_ 1

#include "main.hh"
#include "creature.hh"

struct TPlayerData {
	uint32 CharacterID;
	pid_t Locked;
	int Sticky;
	bool Dirty;
	int Race;
	TOutfit OriginalOutfit;
	TOutfit CurrentOutfit;
	time_t LastLoginTime;
	time_t LastLogoutTime;
	int startx;
	int starty;
	int startz;
	int posx;
	int posy;
	int posz;
	int Profession;
	int PlayerkillerEnd;
	int Actual[25];
	int Maximum[25];
	int Minimum[25];
	int DeltaAct[25];
	int MagicDeltaAct[25];
	int Cycle[25];
	int MaxCycle[25];
	int Count[25];
	int MaxCount[25];
	int AddLevel[25];
	int Experience[25];
	int FactorPercent[25];
	int NextLevel[25];
	int Delta[25];
	uint8 SpellList[256];
	int QuestValues[500];
	int MurderTimestamps[20];
	uint8 *Inventory;
	int InventorySize;
	uint8 *Depot[9];
	int DepotSize[9];
	ulong AccountID;
	int Sex;
	char Name[30];
	uint8 Rights[12];
	char Guild[31];
	char Rank[31];
	char Title[31];
	int Buddies;
	uint32 Buddy[100];
	char BuddyName[100][30];
	uint32 EarliestYellRound;
	uint32 EarliestTradeChannelRound;
	uint32 EarliestSpellTime;
	uint32 EarliestMultiuseTime;
	uint32 TalkBufferFullTime;
	uint32 MutingEndRound;
	uint32 Addressees[20];
	uint32 AddresseesTimes[20];
	int NumberOfMutings;
};

struct TPlayer: TCreature {
	// REGULAR FUNCTIONS
	// =========================================================================
	void CheckState(void);

	// VIRTUAL FUNCTIONS
	// =========================================================================
	// TODO

	// DATA
	// =========================================================================
	//TCreature super_TCreature;	// IMPLICIT
	ulong AccountID;
	char Guild[31];
	char Rank[31];
	char Title[31];
	char IPAddress[16];
	uint8 Rights[12];
	Object Depot;
	int DepotNr;
	int DepotSpace;
	RESULT ConstructError;
	TPlayerData *PlayerData;
	Object TradeObject;
	uint32 TradePartner;
	bool TradeAccepted;
	int OldState;
	uint32 Request;
	int RequestTimestamp;
	uint32 RequestProcessingGamemaster;
	int TutorActivities;
	uint8 SpellList[256];
	int QuestValues[500];
	Object OpenContainer[16];
	vector<long_unsigned_int> AttackedPlayers;
	int NumberOfAttackedPlayers;
	bool Aggressor;
	vector<long_unsigned_int> FormerAttackedPlayers;
	int NumberOfFormerAttackedPlayers;
	bool FormerAggressor;
	uint32 FormerLogoutRound;
	uint32 PartyLeader;
	uint32 PartyLeavingRound;
	uint32 TalkBufferFullTime;
	uint32 MutingEndRound;
	int NumberOfMutings;
	uint32 Addressees[20];
	uint32 AddresseesTimes[20];
};

#endif //TIBIA_PLAYER_HH_

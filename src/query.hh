#ifndef TIBIA_QUERY_HH_
#define TIBIA_QUERY_HH_ 1

#include "common.hh"
#include "threads.hh"

enum : int {
	QUERY_OK		= 0,
	QUERY_ERROR		= 1,
	QUERY_FAILED	= 3,
};

struct TQueryManagerConnection{
	TQueryManagerConnection(int QueryBufferSize);
	~TQueryManagerConnection(void);

	void connect(void);
	void disconnect(void);
	int write(const uint8 *Buffer, int Size);
	int read(uint8 *Buffer, int Size, int Timeout);
	bool isConnected(void){
		return this->Socket != -1;
	}

	void prepareQuery(int QueryType);
	void sendFlag(bool Flag);
	void sendByte(uint8 Byte);
	void sendWord(uint16 Word);
	void sendQuad(uint32 Quad);
	void sendString(const char *String);
	void sendBytes(const uint8 *Buffer, int Count);

	bool getFlag(void);
	uint8 getByte(void);
	uint16 getWord(void);
	uint32 getQuad(void);
	void getString(char *Buffer, int MaxLength);
	void getBytes(uint8 *Buffer, int Count);

	int executeQuery(int Timeout, bool AutoReconnect);

//==

	int loadWorldConfig(int *WorldType, int *RebootTime,
				int *IPAddress, int *Port,
				int *MaxPlayers, int *PremiumPlayerBuffer,
				int *MaxNewbies, int *PremiumNewbieBuffer);
	int loadPlayers(uint32 MinimumCharacterID, int *NumberOfPlayers,
			char (*Names)[30], uint32 *CharacterIDs);

	int loginGame(uint32 AccountID, const char *PlayerName, const char *Password,
			const char *IPAddress, bool PrivateWorld, bool PremiumAccountRequired,
			bool GamemasterRequired, uint32 *CharacterID, int *Sex, char *Guild,
			char *Rank, char *Title, int *NumberOfBuddies, uint32 *BuddyIDs,
			char (*BuddyNames)[30], uint8 *Rights, bool *PremiumAccountActivated);
	int logoutGame(uint32 CharacterID, int Level, const char *Profession,
			const char *Residence, time_t LastLoginTime, int TutorActivities);
	int createPlayerlist(int NumberOfPlayers, const char **Names, int *Levels,
			const char (*Professions)[30], bool *NewRecord);
	int decrementIsOnline(uint32 CharacterID);
	int clearIsOnline(int *NumberOfAffectedPlayers);
	int addBuddy(uint32 AccountID, uint32 BuddyID);
	int removeBuddy(uint32 AccountID, uint32 BuddyID);

	int logKilledCreatures(int NumberOfRaces, const char **Names,
			int *KilledPlayers,int *KilledCreatures);

	int logCharacterDeath(uint32 CharacterID, int Level, uint32 OffenderID,
			const char *Remark, bool Unjustified, time_t Time);

	int setNotation(uint32 GamemasterID, const char *PlayerName, const char *IPAddress,
			const char *Reason, const char *Comment, uint32 *BanishmentID);
	int setNamelock(uint32 GamemasterID, const char *PlayerName, const char *IPAddress,
			const char *Reason, const char *Comment);
	int banishAccount(uint32 GamemasterID, const char *PlayerName, const char *IPAddress,
			const char *Reason, const char *Comment, bool *FinalWarning, int *Days,
			uint32 *BanishmentID);
	int reportStatement(uint32 ReporterID, const char *PlayerName, const char *Reason,
			const char *Comment, uint32 BanishmentID, uint32 StatementID, int NumberOfStatements,
			uint32 *StatementIDs, int *TimeStamps, uint32 *CharacterIDs, char (*Channels)[30],
			char (*Texts)[256]);
	int banishIPAddress(uint32 GamemasterID, const char *PlayerName, const char *IPAddress,
			const char *Reason, const char *Comment);

	int insertHouses(int NumberOfHouses, uint16 *HouseIDs, char **Names,
			int *Rents, char **Descriptions, int *Sizes, int *PositionsX,
			int *PositionsY, int *PositionsZ, char (*Towns)[30], bool *Guildhouses);

	int finishAuctions(int *NumberOfAuctions, uint16 *HouseIDs,
			uint32 *CharacterIDs, char (*CharacterNames)[30], int *Bids);
	int excludeFromAuctions(uint32 CharacterIDs, bool Banish);

	int transferHouses(int *NumberOfTransfers, uint16 *HouseIDs,
			uint32 *NewOwnerIDs, char (*NewOwnerNames) [30], int *Prices);
	int cancelHouseTransfer(uint16 HouseID);
	int evictFreeAccounts(int *NumberOfEvictions, uint16 *HouseIDs, uint32 *OwnerIDs);
	int evictDeletedCharacters(int *NumberOfEvictions, uint16 *HouseIDs);
	int evictExGuildleaders(int NumberOfGuildhouses, int *NumberOfEvictions,
			uint16 *HouseIDs, uint32 *Guildleaders);
	int deleteHouseOwner(uint16 HouseID);

	int getAuctions(int *NumberOfAuctions, uint16 *HouseIDs);
	int startAuction(uint16 HouseID);
	int getHouseOwners(int *NumberOfHouses, uint16 *HouseIDs,
			uint32 *OwnerIDs, char (*OwnerNames)[30], int *PaidUntils);
	int insertHouseOwner(uint16 HouseID, uint32 OwnerID, int PaidUntil);
	int updateHouseOwner(uint16 HouseID, uint32 OwnerID, int PaidUntil);

	// DATA
	// =================
	int BufferSize;
	uint8 *Buffer;
	TReadBuffer ReadBuffer;
	TWriteBuffer WriteBuffer;
	int Socket;
	bool QueryOk;
};

struct TQueryManagerConnectionPool{
	TQueryManagerConnectionPool(int Connections);
	void init(void);
	void exit(void);

	// DATA
	// =================
	int NumberOfConnections;
	TQueryManagerConnection *QueryManagerConnection;
	bool *QueryManagerConnectionFree;
	Semaphore FreeQueryManagerConnections;
	Semaphore QueryManagerConnectionMutex;
};

struct TQueryManagerPoolConnection{
	TQueryManagerPoolConnection(TQueryManagerConnectionPool *Pool);
	~TQueryManagerPoolConnection(void);

	// TODO(fusion): I don't know if this was the indended way to access this
	// structure but it is only a small wrapper and accessing members directly
	// would be too verbose, specially when it is also named `QueryManagerConnection`.

	TQueryManagerConnection *operator->(void){
		return this->QueryManagerConnection;
	}

	operator bool(void) const {
		return this->QueryManagerConnection != NULL;
	}

	// DATA
	// =================
	TQueryManagerConnectionPool *QueryManagerConnectionPool;
	TQueryManagerConnection *QueryManagerConnection;
};

#endif //TIBIA_QUERY_HH_

#ifndef TIBIA_QUERY_HH_
#define TIBIA_QUERY_HH_ 1

#include "common.hh"
#include "threads.hh"

struct TQueryManagerConnection{
	TQueryManagerConnection(int QueryBufferSize);
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
	void decrementIsOnline(uint32 CharacterID);

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

	bool isConnected(void){
		return this->Socket >= 0;
	}

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

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

	// DATA
	// =================
	int NumberOfConnections;
	TQueryManagerConnection *QueryManagerConnection;
	bool *QueryManagerConnectionFree;
	Semaphore FreeQueryManagerConnections;
	Semaphore QueryManagerConnectionMutex;
};

struct TQueryManagerPoolConnection{
	TQueryManagerConnectionPool *QueryManagerConnectionPool;
	TQueryManagerConnection *QueryManagerConnection;
};

#endif //TIBIA_QUERY_HH_


struct TDatabasePoolConnection {
    TDatabaseConnectionPool *DatabaseConnectionPool;
    TDatabaseConnection *DatabaseConnection;
    bool TransactionRunning;
};

struct TDatabaseConnectionPool {
    int NumberOfConnections;
    struct TDatabaseSettings DatabaseSettings;
    struct TDatabaseConnection **DatabaseConnection;
    bool *DatabaseConnectionFree;
    struct Semaphore FreeDatabaseConnections;
    struct Semaphore DatabaseConnectionMutex;
};

// TODO(fusion): Opaque struct?
struct TDatabaseConnection {};

struct TPreparedQuery {
    int Number;
    char *Query;
    char *DBTypes;
    char *CTypes;
    char *Name;
};

struct TDirectReplyData {
    ulong CharacterID;
    char Message[100];
};

struct TWriterThreadReply {
    enum TWriterThreadReplyType ReplyType;
    void *Data;
};

struct TWriterThreadOrder {
    enum TWriterThreadOrderType OrderType;
    void *Data;
};

struct TPunishmentOrderData {
    ulong GamemasterID;
    char GamemasterName[30];
    char CriminalName[30];
    char CriminalIPAddress[16];
    int Reason;
    int Action;
    char Comment[200];
    int NumberOfStatements;
    struct vector<TReportedStatement> *ReportedStatements;
    ulong StatementID;
    bool IPBanishment;
};

struct TLogoutOrderData {
    ulong CharacterID;
    int Level;
    int Profession;
    time_t LastLoginTime;
    int TutorActivities;
    char Residence[30];
};

struct TCharacterDeathOrderData {
    ulong CharacterID;
    int Level;
    ulong Offender;
    char Remark[30];
    bool Unjustified;
    time_t Time;
};

struct TNameLockOrderData {
    ulong GamemasterID;
    char GamemasterName[30];
    char CharacterName[30];
    bool OldNameVisible;
};

struct TNotationOrderData {
    ulong GamemasterID;
    char GamemasterName[30];
    char CharacterName[30];
    char Comment[200];
};

struct TProtocolThreadOrder {
    char ProtocolName[20];
    char Text[256];
};

struct TPlayerlistOrderData {
    int NumberOfPlayers;
    char *PlayerNames;
    int *PlayerLevels;
    int *PlayerProfessions;
};

struct TBuddyOrderData {
    ulong AccountID;
    ulong Buddy;
};

struct TKillStatisticsOrderData {
    int NumberOfRaces;
    char *RaceNames;
    int *KilledPlayers;
    int *KilledCreatures;
};

struct TReaderThreadOrder {
    enum TReaderThreadOrderType OrderType;
    int SectorX;
    int SectorY;
    int SectorZ;
    ulong CharacterID;
};

struct TReaderThreadReply {
    enum TReaderThreadReplyType ReplyType;
    int SectorX;
    int SectorY;
    int SectorZ;
    uchar *Data;
    int Size;
};

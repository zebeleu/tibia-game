
struct TXTEASymmetricKey {
    int (**_vptr.TXTEASymmetricKey)(...); // VTABLE?
    uint8 m_SymmetricKey[16];
};


// NOTE(fusion): Some minimal processing up until here. Then I realized the file would be huge.
//==================================================================================================
//==================================================================================================
//==================================================================================================

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

struct TPlayerIndexNode {
    bool InternalNode;
};

struct TPlayerIndexInternalNode {
    struct TPlayerIndexNode super_TPlayerIndexNode;
    struct TPlayerIndexNode *Child[27];
};

struct TPlayerIndexEntry {
    char Name[30];
    ulong CharacterID;
};

struct TPlayerIndexLeafNode {
    struct TPlayerIndexNode super_TPlayerIndexNode;
    int Count;
    struct TPlayerIndexEntry Entry[10];
};

struct THouseGuest {
    char Name[60];
};

struct THouse {
    ushort ID;
    char Name[50];
    char Description[500];
    int Size;
    ulong Rent;
    int DepotNr;
    bool NoAuction;
    bool GuildHouse;
    int ExitX;
    int ExitY;
    int ExitZ;
    int CenterX;
    int CenterY;
    int CenterZ;
    ulong OwnerID;
    char OwnerName[30];
    int LastTransition;
    int PaidUntil;
    struct vector<THouseGuest> Subowner;
    int Subowners;
    struct vector<THouseGuest> Guest;
    int Guests;
    int Help;
};

struct TWaitinglistEntry {
    struct TWaitinglistEntry *Next;
    char Name[30];
    ulong NextTry;
    bool FreeAccount;
    bool Newbie;
    bool Sleeping;
};

struct TAttackWave {
    int x;
    int y;
    int z;
    int Spread;
    int Race;
    int MinCount;
    int MaxCount;
    int Radius;
    int Lifetime;
    ulong Message;
    int ExtraItems;
    struct vector<TItemData> ExtraItem;
};

struct TReportedStatement {
    ulong StatementID;
    ulong TimeStamp;
    ulong CharacterID;
    int Mode;
    int Channel;
    char Text[256];
};

struct TListener {
    ulong StatementID;
    ulong CharacterID;
};

struct TDelayedMail {
    ulong CharacterID;
    int DepotNumber;
    uchar *Packet;
    int PacketSize;
};

struct TShortwayPoint {
    int x;
    int y;
    int Waypoints;
    int Waylength;
    int Heuristic;
    struct TShortwayPoint *Predecessor;
    struct TShortwayPoint *NextToExpand;
};

struct TMonsterhome {
    int Race;
    int x;
    int y;
    int z;
    int Radius;
    int MaxMonsters;
    int ActMonsters;
    int RegenerationTime;
    int Timer;
};



struct TChannel {
    ulong Moderator;
    char ModeratorName[30];
    struct vector<long_unsigned_int> Subscriber;
    int Subscribers;
    struct vector<long_unsigned_int> InvitedPlayer;
    int InvitedPlayers;
};

struct THelpDepot {
    ulong CharacterID;
    struct Object Box;
    int DepotNr;
};

struct TParty {
    ulong Leader;
    struct vector<long_unsigned_int> Member;
    int Members;
    struct vector<long_unsigned_int> InvitedPlayer;
    int InvitedPlayers;
};

struct TMoveUseAction {
    enum ActionType Action;
    int Parameters[5];
};

struct TMoveUseRule {
    int FirstCondition;
    int LastCondition;
    int FirstAction;
    int LastAction;
};

struct TMoveUseCondition {
    enum ModifierType Modifier;
    enum ConditionType Condition;
    int Parameters[5];
};

struct THouseArea {
    ushort ID;
    int SQMPrice;
    int DepotNr;
};

struct TStatement {
    ulong StatementID;
    ulong TimeStamp;
    ulong CharacterID;
    int Mode;
    int Channel;
    ulong Text;
    bool Reported;
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

struct TMoveUseDatabase {
    struct vector<TMoveUseRule> Rules;
    int NumberOfRules;
};

struct TFindCreatures {
    int startx;
    int starty;
    int endx;
    int endy;
    int blockx;
    int blocky;
    ulong ActID;
    ulong SkipID;
    int Mask;
    bool finished;
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

struct TQueryManagerConnection {
    int BufferSize;
    uchar *Buffer;
    struct TReadBuffer ReadBuffer;
    struct TWriteBuffer WriteBuffer;
    int Socket;
    bool QueryOk;
    undefined field6_0x2d;
    undefined field7_0x2e;
    undefined field8_0x2f;
};

struct TQueryManagerPoolConnection {
    struct TQueryManagerConnectionPool *QueryManagerConnectionPool;
    struct TQueryManagerConnection *QueryManagerConnection;
};

struct TQueryManagerConnectionPool {
    int NumberOfConnections;
    struct TQueryManagerConnection *QueryManagerConnection;
    bool *QueryManagerConnectionFree;
    struct Semaphore FreeQueryManagerConnections;
    struct Semaphore QueryManagerConnectionMutex;
};

// NOTE(fusion): Probably bigint structures for RSA decoding.
struct vlong_flex_unit {
    uint n;
    uint *a;
    uint z;
};

struct vlong {
    int (**_vptr.vlong)(...);
    struct vlong_value *value;
    int negative;
};

struct vlong_value {
    struct vlong_flex_unit super_vlong_flex_unit;
    uint share;
};

struct vlong_montgomery {
    struct vlong R;
    struct vlong R1;
    struct vlong m;
    struct vlong n1;
    struct vlong T;
    struct vlong k;
    uint N;
};

// NOTE(fusion): Oh yes.
struct TRSAPrivateKey {
    int (**_vptr.TRSAPrivateKey)(...);
    struct vlong m_PrimeP;
    struct vlong m_PrimeQ;
    struct vlong m_U;
    struct vlong m_DP;
    struct vlong m_DQ;
};

struct TShortway {
    struct matrix<TShortwayPoint> *Map;
    struct TShortwayPoint *FirstToExpand;
    struct TCreature *cr;
    int VisibleX;
    int VisibleY;
    int StartX;
    int StartY;
    int StartZ;
    int MinWaypoints;
};

struct Object {
    uint32 ObjectID;
};

struct TSkill {
    int (**_vptr.TSkill)(...); // VTABLE?
    int DAct;
    int MDAct;
    uint16 SkNr;
    TCreature *Master;
    int Act;
    int Max;
    int Min;
    int FactorPercent;
    int LastLevel;
    int NextLevel;
    int Delta;
    int Exp;
    int Cycle;
    int MaxCycle;
    int Count;
    int MaxCount;
    int AddLevel;
};

struct TSkillAdd {
    TSkill super_TSkill; // INHERITANCE?
};

struct TSkillHitpoints {
    TSkillAdd super_TSkillAdd; // INHERITANCE?
};

struct TSkillSoulpoints {
    TSkillAdd super_TSkillAdd; // INHERITANCE?
};

struct TSkillProbe {
    TSkill super_TSkill; // INHERITANCE?
};

struct TSkillMana {
    TSkillAdd super_TSkillAdd; // INHERITANCE?
};

struct TSkillGoStrength {
    TSkillAdd super_TSkillAdd; // INHERITANCE?
};

struct TSkillLight {
    TSkill super_TSkill; // INHERITANCE?
};

struct TSkillEnergy {
    TSkill super_TSkill; // INHERITANCE?
};

struct TSkillCarryStrength {
    TSkillAdd super_TSkillAdd; // INHERITANCE?
};

struct TSkillPoison {
    TSkill super_TSkill; // INHERITANCE?
};

struct TSkillBurning {
    TSkill super_TSkill; // INHERITANCE?
};

struct TSkillFed {
    TSkill super_TSkill; // INHERITANCE?
};

struct TSkillIllusion {
    TSkill super_TSkill; // INHERITANCE?
};

struct TSkillLevel {
    TSkill super_TSkill; // INHERITANCE?
};

struct TSkillBase {
    TSkill *Skills[25];
    TSkill *TimerList[25];
    uint16 FirstFreeTimer;
};

struct TImpact {
    int (**_vptr.TImpact)(...); // VTABLE?
};

struct TSummonImpact {
    TImpact super_TImpact; // INHERITANCE?
    TCreature *Actor;
    int Race;
    int Maximum;
};

struct TSpeedImpact {
    TImpact super_TImpact; // INHERITANCE?
    TCreature *Actor;
    int Percent;
    int Duration;
};

struct THealingImpact {
    TImpact super_TImpact; // INHERITANCE?
    TCreature *Actor;
    int Power;
};

struct TOutfitImpact {
    TImpact super_TImpact; // INHERITANCE?
    TCreature *Actor;
    TOutfit Outfit;
    int Duration;
};

struct TFieldImpact {
    TImpact super_TImpact; // INHERITANCE?
    TCreature *Actor;
    int FieldType;
};

struct TDrunkenImpact {
    TImpact super_TImpact; // INHERITANCE?
    TCreature *Actor;
    int Power;
    int Duration;
};

struct TStrengthImpact {
    TImpact super_TImpact; // INHERITANCE?
    TCreature *Actor;
    int Skills;
    int Percent;
    int Duration;
};

struct TDamageImpact {
    TImpact super_TImpact; // INHERITANCE?
    TCreature *Actor;
    int DamageType;
    int Power;
    bool AllowDefense;
};

struct TCombatEntry {
    uint32 ID;
    uint32 Damage;
    int TimeStamp;
};

struct TCombat {
    TCreature *Master;
    uint32 EarliestAttackTime;
    uint32 EarliestDefendTime;
    uint32 LastDefendTime;
    uint32 LatestAttackTime;
    uint32 AttackMode;
    uint32 ChaseMode;
    uint32 SecureMode;
    uint32 AttackDest;
    bool Following;
    Object Shield;
    Object Close;
    Object Missile;
    Object Throw;
    Object Wand;
    Object Ammo;
    bool Fist;
    uint32 CombatDamage;
    int ActCombatEntry;
    TCombatEntry CombatList[20];
    int LearningPoints;
};

struct TOutfit {
    int OutfitID;
	union{
		uint16 ObjectType;
		uint8 Colors[4];
	};
};

struct TCreature {
    int (**_vptr.TCreature)(...); // CREATURE VTABLE?
    TSkillBase super_TSkillBase; // INHERITANCE?
    TCombat Combat;
    uint32 ID;
    TCreature *NextHashEntry;
    uint32 NextChainCreature;
    char Name[31];
    char Murderer[31];
    TOutfit OrgOutfit;
    TOutfit Outfit;
    int startx;
    int starty;
    int startz;
    int posx;
    int posy;
    int posz;
    int Sex;
    int Race;
    int Direction;
    int Radius;
    CreatureType Type;
    bool IsDead;
    int LoseInventory;
    bool LoggingOut;
    bool LogoutAllowed;
    uint32 EarliestLogoutRound;
    uint32 EarliestProtectionZoneRound;
    uint32 EarliestYellRound;
    uint32 EarliestTradeChannelRound;
    uint32 EarliestSpellTime;
    uint32 EarliestMultiuseTime;
    uint32 EarliestWalkTime;
    uint32 LifeEndRound;
    TKnownCreature *FirstKnowingConnection;
    int SummonedCreatures;
    uint32 FireDamageOrigin;
    uint32 PoisonDamageOrigin;
    uint32 EnergyDamageOrigin;
    Object CrObject;
    vector<TToDoEntry> ToDoList;
    int ActToDo;
    int NrToDo;
    uint32 NextWakeup;
    bool Stop;
    bool LockToDo;
    uint8 Profession;
    TConnection *Connection;
};


struct TKnownCreature {
    KNOWNCREATURESTATE State;
    uint32 CreatureID;
    TKnownCreature *Next;
    TConnection *Connection;
};

struct TConnection {
    uint8 InData[2048];
    int InDataSize;
    bool SigIOPending;
    bool WaitingForACK;
    uint8 OutData[16384];
    uint8 field5_0x4806;
    uint8 field6_0x4807;
    int NextToSend;
    int NextToCommit;
    int NextToWrite;
    bool Overflow;
    bool WillingToSend;
    uint8 field12_0x4816;
    uint8 field13_0x4817;
    TConnection *NextSendingConnection;
    uint32 RandomSeed;
	CONNECTIONSTATE State;
    pid_t PID;
    int Socket;
    char IPAddress[16];
    TXTEASymmetricKey SymmetricKey;
    bool ConnectionIsOk;
    bool ClosingIsDelayed;
    uint8 field23_0x4852;
    uint8 field24_0x4853;
    uint32 TimeStamp;
    uint32 TimeStampAction;
    int TerminalType;
    int TerminalVersion;
    int TerminalOffsetX;
    int TerminalOffsetY;
    int TerminalWidth;
    int TerminalHeight;
    uint32 CharacterID;
    char Name[31];
    uint8 field35_0x4897;
    TKnownCreature KnownCreatureTable[150];
};

struct TCircle {
    int x[32];
    int y[32];
    int Count;
};

struct TSpellList {
    uint8 Syllable[10];
    uint8 RuneGr;
    uint8 RuneNr;
    char *Comment;
    uint16 Level;
    uint16 RuneLevel;
    uint16 Flags;
    int Mana;
    int SoulPoints;
    int Amount;
};

struct TToDoEntry {
    ToDoType Code;
    union{
        struct{
            uint32 Time;
        } Wait;

        struct{
            int x;
            int y;
            int z;
        } Go;

        struct{
            int Direction;
        } Rotate;

        struct{
            uint32 Obj;
            int x;
            int y;
            int z;
            int Count;
        } Move;

        struct{
            uint32 Obj;
            uint32 Partner;
        } Trade;

        struct{
            uint32 Obj1;
            uint32 Obj2;
            int Dummy;
        } Use;

        struct{
            uint32 Obj;
        } Turn;

        struct{
            uint32 Text; // POINTER?
            int Mode;
            uint32 Addressee;
            bool CheckSpamming;
        } Talk;

        struct{
            int NewState;
        } ChangeState;
    };
};

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

struct TDatabaseSettings {
    char Product[30];
    char Database[30];
    char Login[30];
    char Password[30];
    char Host[30];
    char Port[6];
};

struct Semaphore {
    int value;
    struct pthread_mutex_t mutex;
    struct pthread_cond_t condition;
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

struct TSharedMemory {
    int Command;
    char CommandBuffer[256];
    ulong RoundNr;
    ulong ObjectCounter;
    ulong Errors;
    int PlayersOnline;
    int NewbiesOnline;
    int PrintBufferPosition;
    char PrintBuffer[200][128];
    enum GAMESTATE GameState;
    pid_t GameThreadPID;
};

struct TPlayerIndexNode {
    bool InternalNode;
};

struct TPlayerIndexInternalNode {
    struct TPlayerIndexNode super_TPlayerIndexNode;
    struct TPlayerIndexNode *Child[27];
};

union storeitem<TPlayerIndexInternalNode> {
    union storeitem<TPlayerIndexInternalNode> *next;
    struct TPlayerIndexInternalNode data;
};

struct storeunit<TPlayerIndexInternalNode,100> {
    union storeitem<TPlayerIndexInternalNode> item[100];
};

struct listnode<storeunit<TPlayerIndexInternalNode,_100>_> { // Original name: listnode<storeunit<TPlayerIndexInternalNode, 100> >
    struct listnode<storeunit<TPlayerIndexInternalNode,_100>_> *next;
    struct listnode<storeunit<TPlayerIndexInternalNode,_100>_> *prev;
    struct storeunit<TPlayerIndexInternalNode,100> data;
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

union storeitem<TPlayerIndexLeafNode> {
    union storeitem<TPlayerIndexLeafNode> *next;
    struct TPlayerIndexLeafNode data;
};

struct storeunit<TPlayerIndexLeafNode,100> {
    union storeitem<TPlayerIndexLeafNode> item[100];
};

struct TNode {
    int Type;
    int Data;
    struct TNode *Left;
    struct TNode *Right;
};

union storeitem<TNode> {
    union storeitem<TNode> *next;
    struct TNode data;
};

struct store<TPlayerIndexInternalNode,100> {
    struct list<storeunit<TPlayerIndexInternalNode,_100>_> *Units;
    union storeitem<TPlayerIndexInternalNode> *firstFreeItem;
};

struct list<storeunit<TPlayerIndexInternalNode,_100>_> { // Original name: list<storeunit<TPlayerIndexInternalNode, 100> >
    struct listnode<storeunit<TPlayerIndexInternalNode,_100>_> *firstNode;
    struct listnode<storeunit<TPlayerIndexInternalNode,_100>_> *lastNode;
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

union storeitem<TWaitinglistEntry> {
    union storeitem<TWaitinglistEntry> *next;
    struct TWaitinglistEntry data;
};

struct storeunit<TWaitinglistEntry,100> {
    union storeitem<TWaitinglistEntry> item[100];
};

struct listnode<storeunit<TWaitinglistEntry,_100>_> { // Original name: listnode<storeunit<TWaitinglistEntry, 100> >
    struct listnode<storeunit<TWaitinglistEntry,_100>_> *next;
    struct listnode<storeunit<TWaitinglistEntry,_100>_> *prev;
    struct storeunit<TWaitinglistEntry,100> data;
};

struct ObjectType {
    int TypeID;
};

struct TItemData {
    struct ObjectType Type;
    int Maximum;
    int Probability;
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

struct TSkillData {
    int Nr;
    int Actual;
    int Minimum;
    int Maximum;
    int NextLevel;
    int FactorPercent;
    int AddLevel;
};

struct store<TWaitinglistEntry,100> {
    struct list<storeunit<TWaitinglistEntry,_100>_> *Units;
    union storeitem<TWaitinglistEntry> *firstFreeItem;
};

struct list<storeunit<TWaitinglistEntry,_100>_> { // Original name: list<storeunit<TWaitinglistEntry, 100> >
    struct listnode<storeunit<TWaitinglistEntry,_100>_> *firstNode;
    struct listnode<storeunit<TWaitinglistEntry,_100>_> *lastNode;
};

struct TDynamicStringTableBlock {
    int FreeEntries;
    int TotalTextLength;
    bool Dirty;
    uchar EntryType[256];
    ushort StringOffset[256];
    char Text[32768];
};

struct listnode<TDynamicStringTableBlock> {
    struct listnode<TDynamicStringTableBlock> *next;
    struct listnode<TDynamicStringTableBlock> *prev;
    struct TDynamicStringTableBlock data;
};

struct listIterator<TDynamicStringTableBlock> {
    struct listnode<TDynamicStringTableBlock> *actNode;
};

struct TNonplayer {
    struct TCreature super_TCreature; // INHERITANCE?
    enum STATE State;
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

struct fifo<TListener> {
    struct TListener *Entry;
    int Size;
    int Head;
    int Tail;
};

struct TDelayedMail {
    ulong CharacterID;
    int DepotNumber;
    uchar *Packet;
    int PacketSize;
};

struct list<storeunit<TPlayerIndexLeafNode,_100>_> { // Original name: list<storeunit<TPlayerIndexLeafNode, 100> >
    struct listnode<storeunit<TPlayerIndexLeafNode,_100>_> *firstNode;
    struct listnode<storeunit<TPlayerIndexLeafNode,_100>_> *lastNode;
};

struct listnode<storeunit<TPlayerIndexLeafNode,_100>_> { // Original name: listnode<storeunit<TPlayerIndexLeafNode, 100> >
    struct listnode<storeunit<TPlayerIndexLeafNode,_100>_> *next;
    struct listnode<storeunit<TPlayerIndexLeafNode,_100>_> *prev;
    struct storeunit<TPlayerIndexLeafNode,100> data;
};

struct store<TPlayerIndexLeafNode,100> {
    struct list<storeunit<TPlayerIndexLeafNode,_100>_> *Units;
    union storeitem<TPlayerIndexLeafNode> *firstFreeItem;
};

struct listIterator<storeunit<TWaitinglistEntry,_100>_> { // Original name: listIterator<storeunit<TWaitinglistEntry, 100> >
    struct listnode<storeunit<TWaitinglistEntry,_100>_> *actNode;
};

struct TSpellData {
    enum SpellShapeType Shape;
    int ShapeParam1;
    int ShapeParam2;
    int ShapeParam3;
    int ShapeParam4;
    enum SpellImpactType Impact;
    int ImpactParam1;
    int ImpactParam2;
    int ImpactParam3;
    int ImpactParam4;
    int Delay;
};

struct storeunit<TNode,256> {
    union storeitem<TNode> item[256];
};

struct listnode<storeunit<TNode,_256>_> { // Original name: listnode<storeunit<TNode, 256> >
    struct listnode<storeunit<TNode,_256>_> *next;
    struct listnode<storeunit<TNode,_256>_> *prev;
    struct storeunit<TNode,256> data;
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

struct TCronEntry {
    struct Object Obj;
    ulong RoundNr;
    int Previous;
    int Next;
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

struct TStaticStringTableBlock {
    int TotalTextLength;
    char Text[65536];
};

struct listnode<TStaticStringTableBlock> {
    struct listnode<TStaticStringTableBlock> *next;
    struct listnode<TStaticStringTableBlock> *prev;
    struct TStaticStringTableBlock data;
};

struct list<TStaticStringTableBlock> {
    struct listnode<TStaticStringTableBlock> *firstNode;
    struct listnode<TStaticStringTableBlock> *lastNode;
};

struct listIterator<TStaticStringTableBlock> {
    struct listnode<TStaticStringTableBlock> *actNode;
};

struct list<storeunit<TNode,_256>_> { // Original name: list<storeunit<TNode, 256> >
    struct listnode<storeunit<TNode,_256>_> *firstNode;
    struct listnode<storeunit<TNode,_256>_> *lastNode;
};

struct TObjectType {
    char *Name;
    char *Description;
    uchar Flags[9];
    ulong Attributes[62];
    int AttributeOffsets[18];
};

struct TCondition {
    int Type;
    ulong Text;
    struct TNode *Expression;
    int Property;
    int Number;
};

struct TSector {
    struct Object MapCon[32][32];
    ulong TimeStamp;
    uchar Status;
    uchar MapFlags;
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

struct listIterator<storeunit<TPlayerIndexLeafNode,_100>_> { // Original name: listIterator<storeunit<TPlayerIndexLeafNode, 100> >
    struct listnode<storeunit<TPlayerIndexLeafNode,_100>_> *actNode;
};

struct TPlayerData {
    ulong CharacterID;
    pid_t Locked;
    int Sticky;
    bool Dirty;
    int Race;
    struct TOutfit OriginalOutfit;
    struct TOutfit CurrentOutfit;
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
    uchar SpellList[256];
    int QuestValues[500];
    int MurderTimestamps[20];
    uchar *Inventory;
    int InventorySize;
    uchar *Depot[9];
    int DepotSize[9];
    ulong AccountID;
    int Sex;
    char Name[30];
    uchar Rights[12];
    char Guild[31];
    char Rank[31];
    char Title[31];
    int Buddies;
    ulong Buddy[100];
    char BuddyName[100][30];
    ulong EarliestYellRound;
    ulong EarliestTradeChannelRound;
    ulong EarliestSpellTime;
    ulong EarliestMultiuseTime;
    ulong TalkBufferFullTime;
    ulong MutingEndRound;
    ulong Addressees[20];
    ulong AddresseesTimes[20];
    int NumberOfMutings;
};

struct TPlayer {
    struct TCreature super_TCreature;
    ulong AccountID;
    char Guild[31];
    char Rank[31];
    char Title[31];
    char IPAddress[16];
    uchar Rights[12];
    struct Object Depot;
    int DepotNr;
    int DepotSpace;
    enum RESULT ConstructError;
    struct TPlayerData *PlayerData;
    struct Object TradeObject;
    ulong TradePartner;
    bool TradeAccepted;
    int OldState;
    ulong Request;
    int RequestTimestamp;
    ulong RequestProcessingGamemaster;
    int TutorActivities;
    uchar SpellList[256];
    int QuestValues[500];
    struct Object OpenContainer[16];
    struct vector<long_unsigned_int> AttackedPlayers;
    int NumberOfAttackedPlayers;
    bool Aggressor;
    struct vector<long_unsigned_int> FormerAttackedPlayers;
    int NumberOfFormerAttackedPlayers;
    bool FormerAggressor;
    ulong FormerLogoutRound;
    ulong PartyLeader;
    ulong PartyLeavingRound;
    ulong TalkBufferFullTime;
    ulong MutingEndRound;
    int NumberOfMutings;
    ulong Addressees[20];
    ulong AddresseesTimes[20];
};

struct TParty {
    ulong Leader;
    struct vector<long_unsigned_int> Member;
    int Members;
    struct vector<long_unsigned_int> InvitedPlayer;
    int InvitedPlayers;
};

struct listIterator<storeunit<TNode,_256>_> { // Original name: listIterator<storeunit<TNode, 256> >
    struct listnode<storeunit<TNode,_256>_> *actNode;
};

struct TMark {
    char Name[20];
    int x;
    int y;
    int z;
};

struct store<TNode,256> {
    struct list<storeunit<TNode,_256>_> *Units;
    union storeitem<TNode> *firstFreeItem;
};

struct TMoveUseAction {
    enum ActionType Action;
    int Parameters[5];
};

struct list<TDynamicStringTableBlock> {
    struct listnode<TDynamicStringTableBlock> *firstNode;
    struct listnode<TDynamicStringTableBlock> *lastNode;
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

struct TAction {
    int Type;
    ulong Text;
    int Number;
    struct TNode *Expression;
    struct TNode *Expression2;
    struct TNode *Expression3;
};

struct TBehaviour {
    struct vector<TCondition> Condition;
    struct vector<TAction> Action;
    int Conditions;
    int Actions;
};

struct THouseArea {
    ushort ID;
    int SQMPrice;
    int DepotNr;
};

struct listIterator<storeunit<TPlayerIndexInternalNode,_100>_> { // Original name: listIterator<storeunit<TPlayerIndexInternalNode, 100> >
    struct listnode<storeunit<TPlayerIndexInternalNode,_100>_> *actNode;
};


struct TDepotInfo {
    char Town[20];
    int Size;
};

struct fifoIterator<TStatement> {
    struct fifo<TStatement> *Fifo;
    int Position;
};

struct fifo<TStatement> {
    struct TStatement *Entry;
    int Size;
    int Head;
    int Tail;
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

struct fifoIterator<TListener> {
    struct fifo<TListener> *Fifo;
    int Position;
};

struct TWriteStream {
    int (**_vptr.TWriteStream)(...);
};

struct TReadStream {
    int (**_vptr.TReadStream)(...);
};

struct TReadBuffer {
    struct TReadStream super_TReadStream;
    uchar *Data;
    int Size;
    int Position;
};

struct TWriteBuffer {
    struct TWriteStream super_TWriteStream;
    uchar *Data;
    int Size;
    int Position;
};

struct TDynamicWriteBuffer {
    struct TWriteBuffer super_TWriteBuffer;
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

struct TObject {
    ulong ObjectID;
    struct Object NextObject;
    struct Object Container;
    struct ObjectType Type;
    ulong Attributes[4];
};

struct TObjectBlock {
    struct TObject Object[32768];
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

typedef int (ThreadFunction)(void *);
struct TThreadStarter {
    ThreadFunction *Function;
    void *Argument;
    bool Detach;
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

struct TWriteBinaryFile {
    struct TWriteStream super_TWriteStream;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    FILE *File;
    char Filename[4096];
};

struct TWriteScriptFile {
    FILE *File;
    char Filename[4096];
    int Line;
};

struct TReadBinaryFile {
    struct TReadStream super_TReadStream;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    FILE *File;
    char Filename[4096];
    int FileSize;
};

struct TReadScriptFile {
    enum TOKEN Token;
    FILE *File[3];
    char Filename[3][4096];
    int Line[3];
    char String[4000];
    int RecursionDepth;
    uchar *Bytes;
    int Number;
    int CoordX;
    int CoordY;
    int CoordZ;
    char Special;
};

struct TNPC {
    struct TNonplayer super_TNonplayer;
    ulong Interlocutor;
    int Topic;
    int Price;
    int Amount;
    int TypeID;
    ulong Data;
    ulong LastTalk;
    struct vector<long_unsigned_int> QueuedPlayers;
    struct vector<long_unsigned_int> QueuedAddresses;
    int QueueLength;
    struct TBehaviourDatabase *Behaviour;
};

struct TBehaviourDatabase {
    struct vector<TBehaviour> Behaviour;
    int Behaviours;
};

struct TMonster {
    struct TNonplayer super_TNonplayer;
    int Home;
    ulong Master;
    ulong Target;
};

struct TRaceData {
    char Name[30];
    char Article[3];
    struct TOutfit Outfit;
    struct ObjectType MaleCorpse;
    struct ObjectType FemaleCorpse;
    enum BloodType Blood;
    int ExperiencePoints;
    int FleeThreshold;
    int Attack;
    int Defend;
    int Armor;
    int Poison;
    int SummonCost;
    int LoseTarget;
    int Strategy[4];
    bool KickBoxes;
    bool KickCreatures;
    bool SeeInvisible;
    bool Unpushable;
    bool DistanceFighting;
    bool NoSummon;
    bool NoIllusion;
    bool NoConvince;
    bool NoBurning;
    bool NoPoison;
    bool NoEnergy;
    bool NoHit;
    bool NoLifeDrain;
    bool NoParalyze;
    int Skills;
    struct vector<TSkillData> Skill;
    int Talks;
    struct vector<long_unsigned_int> Talk;
    int Items;
    struct vector<TItemData> Item;
    int Spells;
    struct vector<TSpellData> Spell;
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

struct TQueryManagerSettings {
    char Host[50];
    int Port;
    char Password[30];
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

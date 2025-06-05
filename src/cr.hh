#ifndef TIBIA_CREATURE_HH_
#define TIBIA_CREATURE_HH_ 1

#include "common.hh"
#include "connection.hh"
#include "containers.hh"
#include "enums.hh"
#include "map.hh"

struct TCreature;

// Creature Data
// =============================================================================
struct TOutfit{
	int OutfitID;
	union{
		uint16 ObjectType;
		uint8 Colors[4];
	};
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

struct TItemData {
	ObjectType Type;
	int Maximum;
	int Probability;
};

struct TSpellData {
	SpellShapeType Shape;
	int ShapeParam1;
	int ShapeParam2;
	int ShapeParam3;
	int ShapeParam4;
	SpellImpactType Impact;
	int ImpactParam1;
	int ImpactParam2;
	int ImpactParam3;
	int ImpactParam4;
	int Delay;
};

struct TRaceData {
	TRaceData(void);

	// DATA
	// =================
	char Name[30];
	char Article[3];
	TOutfit Outfit;
	ObjectType MaleCorpse;
	ObjectType FemaleCorpse;
	BloodType Blood;
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
	vector<TSkillData> Skill;
	int Talks;
	vector<uint32> Talk; // POINTER? Probably a reference from `AddDynamicString`?
	int Items;
	vector<TItemData> Item;
	int Spells;
	vector<TSpellData> Spell;
};

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
	uint32 AccountID;
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

// TSkillBase 
// =============================================================================
struct TSkill{
	TSkill(int SkNr, TCreature *Master);
	int Get(void);
	int GetProgress(void);
	void Check(void);
	void Change(int Amount);
	void SetMDAct(int MDAct);
	void Load(int Act, int Max, int Min, int DAct, int MDAct,
			int Cycle, int MaxCycle, int Count, int MaxCount, int AddLevel,
			int Exp, int FactorPercent, int NextLevel, int Delta);
	void Save(int *Act, int *Max, int *Min, int *DAct, int *MDAct,
			int *Cycle, int *MaxCycle, int *Count, int *MaxCount, int *AddLevel,
			int *Exp, int *FactorPercent, int *NextLevel, int *Delta);

	// VIRTUAL FUNCTIONS
	// =================
	virtual ~TSkill(void);																// VTABLE[ 0]
	// Duplicate destructor that also calls operator delete.							// VTABLE[ 1]
	virtual void Set(int Value);														// VTABLE[ 2]
	virtual void Increase(int Amount);													// VTABLE[ 3]
	virtual void Decrease(int Amount);													// VTABLE[ 4]
	virtual int GetExpForLevel(int Level);												// VTABLE[ 5]
	virtual void Advance(int Range);													// VTABLE[ 6]
	virtual void ChangeSkill(int FactorPercent, int Delta);								// VTABLE[ 7]
	virtual int ProbeValue(int Max, bool Increase);										// VTABLE[ 8]
	virtual bool Probe(int Diff, int Prob, bool Increase);								// VTABLE[ 9]
	virtual bool Process(void);															// VTABLE[10]
	virtual bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue);		// VTABLE[11]
	virtual bool DelTimer(void);														// VTABLE[12]
	virtual int TimerValue(void);														// VTABLE[13]
	virtual bool Jump(int Range);														// VTABLE[14]
	virtual void Event(int Range);														// VTABLE[15]
	virtual void Reset(void);															// VTABLE[16]

	// DATA
	// =================
	//void *VTABLE;		// IMPLICIT
	int DAct;			// Delta Value - Probably from equipment.
	int MDAct;			// Delta Magic Value - Probably from spells.
	uint16 SkNr;
	TCreature *Master;
	int Act;			// Actual Value (?)
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

struct TSkillLevel: TSkill {
	TSkillLevel(int SkNr, TCreature *Master) : TSkill(SkNr, Master) {}
	void Increase(int Amount) override;
	void Decrease(int Amount) override;
	int GetExpForLevel(int Level) override;
	bool Jump(int Range) override;
};

struct TSkillProbe: TSkill {
	TSkillProbe(int SkNr, TCreature *Master) : TSkill(SkNr, Master) {}
	void Increase(int Amount) override;
	void Decrease(int Amount) override;
	int GetExpForLevel(int Level) override;
	void ChangeSkill(int FactorPercent, int Delta) override;
	int ProbeValue(int Max, bool Increase) override;
	bool Probe(int Diff, int Prob, bool Increase) override;
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	bool Jump(int Range) override;
	void Event(int Range) override;
};

struct TSkillAdd: TSkill {
	TSkillAdd(int SkNr, TCreature *Master) : TSkill(SkNr, Master) {}
	void Advance(int Range) override;
};

struct TSkillHitpoints: TSkillAdd {
	TSkillHitpoints(int SkNr, TCreature *Master) : TSkillAdd(SkNr, Master) {}
	void Set(int Value) override;
};

struct TSkillMana: TSkillAdd {
	TSkillMana(int SkNr, TCreature *Master) : TSkillAdd(SkNr, Master) {}
	void Set(int Value) override;
};

struct TSkillGoStrength: TSkillAdd {
	TSkillGoStrength(int SkNr, TCreature *Master) : TSkillAdd(SkNr, Master) {}
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	void Event(int Range) override;
};

struct TSkillCarryStrength: TSkillAdd {
	TSkillCarryStrength(int SkNr, TCreature *Master) : TSkillAdd(SkNr, Master) {}
	void Set(int Value) override;
};

struct TSkillSoulpoints: TSkillAdd {
	TSkillSoulpoints(int SkNr, TCreature *Master) : TSkillAdd(SkNr, Master) {}
	void Set(int Value) override;
	int TimerValue(void) override;
	void Event(int Range) override;
};

struct TSkillFed: TSkill {
	TSkillFed(int SkNr, TCreature *Master) : TSkill(SkNr, Master) {}
	void Event(int Range) override;
};

struct TSkillLight: TSkill {
	TSkillLight(int SkNr, TCreature *Master) : TSkill(SkNr, Master) {}
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	void Event(int Range) override;
};

struct TSkillIllusion: TSkill {
	TSkillIllusion(int SkNr, TCreature *Master) : TSkill(SkNr, Master) {}
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	void Event(int Range) override;
};

struct TSkillPoison: TSkill {
	TSkillPoison(int SkNr, TCreature *Master) : TSkill(SkNr, Master) {}
	bool Process(void) override;
	bool SetTimer(int Cycle, int Count, int MaxCount, int AdditionalValue) override;
	void Event(int Range) override;
	void Reset(void) override;
};

struct TSkillBurning: TSkill {
	TSkillBurning(int SkNr, TCreature *Master) : TSkill(SkNr, Master) {}
	void Event(int Range) override;
};

struct TSkillEnergy: TSkill {
	TSkillEnergy(int SkNr, TCreature *Master) : TSkill(SkNr, Master) {}
	void Event(int Range) override;
};

struct TSkillBase{
	TSkillBase(void);
	~TSkillBase(void);
	bool NewSkill(uint16 SkillNo, TCreature *Creature);
	bool SetSkills(int Race);
	void ProcessSkills(void);
	bool SetTimer(uint16 SkNr, int Cycle, int Count, int MaxCount, int AdditionalValue);
	void DelTimer(uint16 SkNr);

	// DATA
	// =================
	TSkill *Skills[25];
	TSkill *TimerList[25];
	uint16 FirstFreeTimer;
};

// TCombat
// =============================================================================
struct TCombatEntry{
	uint32 ID;
	uint32 Damage;
	uint32 TimeStamp;
};

struct TCombat{
	TCombat(void);
	void GetWeapon(void);
	void GetAmmo(void);
	void CheckCombatValues(void);
	void GetAttackValue(int *Value, int *SkillNr);
	void GetDefendValue(int *Value, int *SkillNr);
	int GetAttackDamage(void);
	int GetDefendDamage(void);
	int GetArmorStrength(void);
	int GetDistance(void);
	void ActivateLearning(void);
	void SetAttackMode(uint8 AttackMode);
	void SetChaseMode(uint8 ChaseMode);
	void SetSecureMode(uint8 SecureMode);
	void SetAttackDest(uint32 TargetID, bool Follow);
	void CanToDoAttack(void);
	void StopAttack(int Delay);
	void DelayAttack(int Milliseconds);
	void Attack(void);
	void CloseAttack(TCreature *Target);
	void DistanceAttack(TCreature *Target);
	void WandAttack(TCreature *Target);
	void AddDamageToCombatList(uint32 Attacker, uint32 Damage);
	uint32 GetDamageByCreature(uint32 CreatureID);
	uint32 GetMostDangerousAttacker(void);
	void DistributeExperiencePoints(uint32 Exp);


	// DATA
	// =================
	TCreature *Master;
	uint32 EarliestAttackTime;
	uint32 EarliestDefendTime;
	uint32 LastDefendTime;
	uint32 LatestAttackTime;
	uint8 AttackMode;
	uint8 ChaseMode;
	uint8 SecureMode;
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

// TCreature
// =============================================================================
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
			uint32 Text; // POINTER? Probably a reference from `AddDynamicString`?
			int Mode;
			uint32 Addressee;
			bool CheckSpamming;
		} Talk;

		struct{
			int NewState;
		} ChangeState;
	};
};

struct TCreature: TSkillBase {
	TCreature(void);
	void Attack(void);
	int Damage(TCreature *Attacker, int Damage, int DamageType);
	void StartLogout(bool Force, bool StopFight);
	void BlockLogout(int Delay, bool BlockProtectionZone);
	void ToDoGo(int DestX, int DestY, int DestZ, bool Dest, int MaxSteps);

	// VIRTUAL FUNCTIONS
	// =================
	virtual ~TCreature(void);															// VTABLE[ 0]
	// Duplicate destructor that also calls operator delete.							// VTABLE[ 1]
	virtual void Death(void);															// VTABLE[ 2]
	virtual bool MovePossible(int x, int y, int z, bool Execute, bool Jump);			// VTABLE[ 3]
	virtual bool IsPeaceful(void);														// VTABLE[ 4]
	virtual uint32 GetMaster(void);														// VTABLE[ 5]
	virtual void TalkStimulus(uint32 SpeakerID, const char *Text);						// VTABLE[ 6]
	virtual void DamageStimulus(uint32 AttackerID, int Damage, int DamageType);			// VTABLE[ 7]
	virtual void IdleStimulus(void);													// VTABLE[ 8]
	virtual void CreatureMoveStimulus(uint32 CreatureID, int Type);						// VTABLE[ 9]
	virtual void AttackStimulus(uint32 AttackerID);										// VTABLE[10]

	// DATA
	// =================
	//void *VTABLE;					// IMPLICIT
	//TSkillBase super_TSkillBase;	// IMPLICIT
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

// TNonPlayer
// =============================================================================
struct TNonplayer: TCreature {
	//TNonplayer(void);

	// DATA
	// =================
	//TCreature super_TCreature;	// IMPLICIT
	STATE State;
};

// TNPC
// =============================================================================
// TODO(fusion): Most of these are probably contained in `crnonpl.cc` because
// `TNPC` has a pointer to `TBehaviourDatabase`.
struct TNode {
	int Type;
	int Data;
	TNode *Left;
	TNode *Right;
};

struct TCondition {
	int Type;
	uint32 Text;
	TNode *Expression;
	int Property;
	int Number;
};

struct TAction {
	int Type;
	uint32 Text;
	int Number;
	TNode *Expression;
	TNode *Expression2;
	TNode *Expression3;
};

struct TBehaviour {
	vector<TCondition> Condition;
	vector<TAction> Action;
	int Conditions;
	int Actions;
};

struct TBehaviourDatabase {
	vector<TBehaviour> Behaviour;
	int Behaviours;
};

struct TNPC {
	//TNPC(void);

	// DATA
	// =================
	//TNonplayer super_TNonplayer; 	// IMPLICIT
	uint32 Interlocutor;
	int Topic;
	int Price;
	int Amount;
	int TypeID;
	uint32 Data;
	uint32 LastTalk;
	vector<uint32> QueuedPlayers;
	vector<uint32> QueuedAddresses;
	int QueueLength;
	TBehaviourDatabase *Behaviour;
};

// TMonster
// =============================================================================
struct TMonster: TNonplayer {
	//TMonster(void);

	// DATA
	//TNonplayer super_TNonplayer;	// IMPLICIT
	int Home;
	uint32 Master;
	uint32 Target;
};

// TPlayer
// =============================================================================
struct TPlayer: TCreature {
	//TPlayer(void);

	uint8 GetRealProfession(void);
	uint8 GetEffectiveProfession(void);
	uint8 GetActiveProfession(void);
	bool GetActivePromotion(void);
	void ClearProfession(void);
	void SetProfession(uint8 Profession);

	int GetQuestValue(int Number);
	void SetQuestValue(int Number, int Value);

	uint32 GetPartyLeader(bool CheckFormer);

	bool SpellKnown(int SpellNr);

	bool IsAttackJustified(uint32 Victim);
	void RecordAttack(uint32 Victim);
	void RecordMurder(uint32 Victim);

	void CheckState(void);

	// VIRTUAL FUNCTIONS
	// =================
	// TODO

	// DATA
	// =================
	//TCreature super_TCreature;	// IMPLICIT
	uint32 AccountID;
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
	vector<uint32> AttackedPlayers;
	int NumberOfAttackedPlayers;
	bool Aggressor;
	vector<uint32> FormerAttackedPlayers;
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

// Creature API
// =============================================================================
#define MAX_RACES 512

// crmain.cc
extern TRaceData RaceData[MAX_RACES];
extern int KilledCreatures[MAX_RACES];
extern int KilledPlayers[MAX_RACES];

// TODO(fusion): These probably belong elsewhere but we should come back to
// this when we're wrapping up creature files.
bool IsCreaturePlayer(uint32 CreatureID);
void AddKillStatistics(int AttackerRace, int DefenderRace);
int GetRaceByName(const char *RaceName);

#endif //TIBIA_CREATURE_HH_

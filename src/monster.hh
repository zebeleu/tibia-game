#ifndef TIBIA_MONSTER_HH_
#define TIBIA_MONSTER_HH_ 1

#include "common.hh"
#include "creature.hh"
#include "containers.hh"
#include "enums.hh"

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

#if 0
// TODO(fusion):
struct TMonster: TNonplayer {
    int Home;
    uint32 Master;
    uint32 Target;
};
#endif

//#define MAX_RACES 512
extern TRaceData RaceData[512];
extern int KilledCreatures[512];
extern int KilledPlayers[512];

#endif //TIBIA_MONSTER_HH_

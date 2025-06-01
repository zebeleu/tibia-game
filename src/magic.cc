#include "magic.hh"
#include "config.hh"
#include "creature.hh"

#include "stubs.hh"

#include <fstream>

static TSpellList SpellList[256];
static TCircle Circle[10];

static const char SpellSyllable[51][6] = {
	"",
	"al",
	"ad",
	"ex",
	"ut",
	"om",
	"para",
	"ana",
	"evo",
	"ori",
	"mort",
	"lux",
	"liber",
	"vita",
	"flam",
	"pox",
	"hur",
	"moe",
	"ani",
	"ina",
	"eta",
	"amo",
	"hora",
	"gran",
	"cogni",
	"res",
	"mas",
	"vis",
	"som",
	"aqua",
	"frigo",
	"tera",
	"ura",
	"sio",
	"grav",
	"ito",
	"pan",
	"vid",
	"isa",
	"iva",
	"con",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
};

// TImpact
// =============================================================================
void TImpact::handleField(int a, int b, int c){
	// no-op
}

void TImpact::handleCreature(TCreature *Victim){
	// no-op
}

bool TImpact::isAggressive(void){
	return true;
}

// TDamageImpact
// =============================================================================
TDamageImpact::TDamageImpact(TCreature *Actor, int DamageType, int Power, bool AllowDefense){
	if(Actor == NULL){
		error("TDamageImpact::TDamageImpact: Actor ist NULL.\n");
	}

	this->Actor = Actor;
	this->DamageType = DamageType;
	this->Power = Power;
	this->AllowDefense = AllowDefense;
}

void TDamageImpact::handleCreature(TCreature *Victim){
	if(Victim == NULL){
		error("TDamageImpact::handleCreature: Opfer existiert nicht.\n");
		return;
	}

	TCreature *Actor = this->Actor;
	if(Actor != NULL && Actor != Victim){
		if(WorldType != NON_PVP || !Actor->IsPeaceful() || !Victim->IsPeaceful()){
			int DamageType = this->DamageType;
			int Damage = this->Power;
			if(DamageType == DAMAGE_PHYSICAL && this->AllowDefense){
				// TODO(fusion): Shouldn't we clamp `Damage` to zero?
				Damage -= Victim->Combat.GetDefendDamage();
			}
			Victim->Damage(Actor, Damage, DamageType);
		}
	}
}

// Magic Related Functions
// =============================================================================
void CheckMana(TCreature *Creature, int ManaPoints, int SoulPoints, int Delay){
	if(Creature == NULL){
		error("CheckMana: Übergebene Kreatur existiert nicht.\n");
		throw ERROR;
	}

	if(Creature->Type != PLAYER || ManaPoints < 0){
		return;
	}

	TSkill *Mana = Creature->Skills[SKILL_MANA];
	if(Mana == NULL){
		error("CheckMana: Kein Skill MANA!\n");
		throw ERROR;
	}

	TSkill *Soul = Creature->Skills[SKILL_SOUL];
	if(Soul == NULL){
		error("CheckMana: Kein Skill SOULPOINTS!\n");
		throw ERROR;
	}

	if(!CheckRight(Creature->ID, UNLIMITED_MANA)){
		if(Mana->Get() < ManaPoints){
			throw NOTENOUGHMANA;
		}

		if(Soul->Get() < SoulPoints){
			throw NOTENOUGHSOULPOINTS;
		}

		Mana->Change(-ManaPoints);
		Soul->Change(-SoulPoints);
	}

	if(ManaPoints > 0){
		Creature->Skills[SKILL_MAGIC_LEVEL]->Increase(ManaPoints);
	}

	uint32 EarliestSpellTime = ServerMilliseconds + Delay;
	if(Creature->EarliestSpellTime < EarliestSpellTime){
		Creature->EarliestSpellTime = EarliestSpellTime;
	}
}

static void InitCircles(void){
	char FileName[4096];
	snprintf(FileName, sizeof(FileName), "%s/circles.dat", DATAPATH);
	std::ifstream IN(FileName, std::ios_base::in);
	if(IN.fail()){
		error("InitCircles: Kann Datei %s nicht öffnen.\n", FileName);
		throw "Cannot open \"circles.dat\"";
	}

	// TODO(fusion): This doesn't make a lot of sense because we have `Width`
	// and `Height` but not `CenterX` and `CenterY`.
	int Width, Height, Center;
	IN >> Width >> Height >> Center;

	if(Width < 0 || Width > 21 || Height < 0 || Height > 21){
		error("InitCircles: Ungültiges Dateiformat.\n");
		throw "Cannot process \"circles.dat\"";
	}

	if(Center > 0){
		for(int Y = 0; Y < Height; Y += 1)
		for(int X = 0; X < Width; X += 1){
			int Radius;
			IN >> Radius;
			if(Radius < NARRAY(Circle)){
				int PointIndex = Circle[Radius].Count;
				ASSERT(PointIndex < 32);
				Circle[Radius].x[PointIndex] = X - Center;
				Circle[Radius].y[PointIndex] = Y - Center;
				Circle[Radius].Count += 1;
			}
		}
	}

	// TODO(fusion): Probably check if we parsed the file successfully?
	//if(IN.fail()) { throw "..."; }
}

static TSpellList *CreateSpell(int SpellNr, ...){
	ASSERT(SpellNr < NARRAY(SpellList));
	int SyllableCount = 0;
	TSpellList *Spell = &SpellList[SpellNr];

	va_list ap;
	va_start(ap, SpellNr);
	while(true){
		const char *Syllable = va_arg(ap, const char*);
		if(!Syllable || Syllable[0] == 0){
			break;
		}

		for(int SyllableNr = 0;
				SyllableNr < NARRAY(SpellSyllable);
				SyllableNr += 1){
			if(strcmp(SpellSyllable[SyllableNr], Syllable) == 0){
				// TODO(fusion): I'm not sure it is a good idea to throw an
				// exception between `va_start` and `va_end`.
				if(SyllableCount > NARRAY(Spell->Syllable)){
					error("CreateSpell: Silbenzahl überschritten bei Spell %d.\n", SpellNr);
					throw "Spell has too many syllables";
				}

				Spell->Syllable[SyllableCount] = (uint8)SyllableNr;
				SyllableCount += 1;
				break;
			}
		}
	}
	va_end(ap);
	return Spell;
}

void InitSpells(void){
	TSpellList *Spell;

	Spell = CreateSpell(1, "ex", "ura", "");
	Spell->Mana = 25;
	Spell->Level = 9;
	Spell->Flags = 8;
	Spell->Comment = "Light Healing";

	Spell = CreateSpell(2, "ex", "ura", "gran", "");
	Spell->Mana = 40;
	Spell->Level = 11;
	Spell->Flags = 8;
	Spell->Comment = "Intense Healing";

	Spell = CreateSpell(3, "ex", "ura", "vita", "");
	Spell->Mana = 160;
	Spell->Level = 20;
	Spell->Flags = 8;
	Spell->Comment = "Ultimate Healing";

	Spell = CreateSpell(4, "ad", "ura", "gran", "");
	Spell->Mana = 240;
	Spell->Level = 15;
	Spell->RuneGr = 79;
	Spell->RuneNr = 5;
	Spell->Flags = 8;
	Spell->Amount = 1;
	Spell->RuneLevel = 1;
	Spell->SoulPoints = 2;
	Spell->Comment = "Intense Healing Rune";

	Spell = CreateSpell(5, "ad", "ura", "vita", "");
	Spell->Mana = 400;
	Spell->Level = 24;
	Spell->RuneGr = 79;
	Spell->RuneNr = 13;
	Spell->Flags = 8;
	Spell->Amount = 1;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 3;
	Spell->Comment = "Ultimate Healing Rune";

	Spell = CreateSpell(6, "ut", "ani", "hur", "");
	Spell->Mana = 60;
	Spell->Level = 14;
	Spell->Flags = 2;
	Spell->Comment = "Haste";

	Spell = CreateSpell(7, "ad", "ori", "");
	Spell->Mana = 120;
	Spell->Level = 15;
	Spell->RuneGr = 79;
	Spell->RuneNr = 27;
	Spell->Flags = 9;
	Spell->Amount = 5;
	Spell->RuneLevel = 0;
	Spell->SoulPoints = 1;
	Spell->Comment = "Light Magic Missile";

	Spell = CreateSpell(8, "ad", "ori", "gran", "");
	Spell->Mana = 280;
	Spell->Level = 25;
	Spell->RuneGr = 79;
	Spell->RuneNr = 51;
	Spell->Flags = 9;
	Spell->Amount = 5;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 2;
	Spell->Comment = "Heavy Magic Missile";

	Spell = CreateSpell(9, "ut", "evo", "res", "para", "");
	Spell->Mana = 0;
	Spell->Level = 25;
	Spell->Flags = 1;
	Spell->Comment = "Summon Creature";

	Spell = CreateSpell(10, "ut", "evo", "lux", "");
	Spell->Mana = 20;
	Spell->Level = 8;
	Spell->Flags = 0;
	Spell->Comment = "Light";

	Spell = CreateSpell(11, "ut", "evo", "gran", "lux", "");
	Spell->Mana = 60;
	Spell->Level = 13;
	Spell->Flags = 0;
	Spell->Comment = "Great Light";

	Spell = CreateSpell(12, "ad", "eta", "sio", "");
	Spell->Mana = 200;
	Spell->Level = 16;
	Spell->RuneGr = 79;
	Spell->RuneNr = 30;
	Spell->Flags = 1;
	Spell->Amount = 1;
	Spell->RuneLevel = 5;
	Spell->SoulPoints = 3;
	Spell->Comment = "Convince Creature";

	Spell = CreateSpell(13, "ex", "evo", "mort", "hur", "");
	Spell->Mana = 250;
	Spell->Level = 38;
	Spell->Flags = 1;
	Spell->Comment = "Energy Wave";

	Spell = CreateSpell(14, "ad", "evo", "ina", "");
	Spell->Mana = 600;
	Spell->Level = 27;
	Spell->RuneGr = 79;
	Spell->RuneNr = 31;
	Spell->Flags = 0;
	Spell->Amount = 1;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 2;
	Spell->Comment = "Chameleon";

	Spell = CreateSpell(15, "ad", "ori", "flam", "");
	Spell->Mana = 160;
	Spell->Level = 17;
	Spell->RuneGr = 79;
	Spell->RuneNr = 42;
	Spell->Flags = 9;
	Spell->Amount = 2;
	Spell->RuneLevel = 2;
	Spell->SoulPoints = 2;
	Spell->Comment = "Fireball";

	Spell = CreateSpell(16, "ad", "ori", "gran", "flam", "");
	Spell->Mana = 480;
	Spell->Level = 23;
	Spell->RuneGr = 79;
	Spell->RuneNr = 44;
	Spell->Flags = 9;
	Spell->Amount = 2;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 3;
	Spell->Comment = "Great Fireball";

	Spell = CreateSpell(17, "ad", "evo", "mas", "flam", "");
	Spell->Mana = 600;
	Spell->Level = 27;
	Spell->RuneGr = 79;
	Spell->RuneNr = 45;
	Spell->Flags = 1;
	Spell->Amount = 2;
	Spell->RuneLevel = 5;
	Spell->SoulPoints = 4;
	Spell->Comment = "Firebomb";

	Spell = CreateSpell(18, "ad", "evo", "mas", "hur", "");
	Spell->Mana = 720;
	Spell->Level = 31;
	Spell->RuneGr = 79;
	Spell->RuneNr = 53;
	Spell->Flags = 1;
	Spell->Amount = 3;
	Spell->RuneLevel = 6;
	Spell->SoulPoints = 4;
	Spell->Comment = "Explosion";

	Spell = CreateSpell(19, "ex", "evo", "flam", "hur", "");
	Spell->Mana = 80;
	Spell->Level = 18;
	Spell->Flags = 9;
	Spell->Comment = "Fire Wave";

	Spell = CreateSpell(20, "ex", "iva", "para", "");
	Spell->Mana = 20;
	Spell->Level = 8;
	Spell->Flags = 0;
	Spell->Comment = "Find Person";

	Spell = CreateSpell(21, "ad", "ori", "vita", "vis", "");
	Spell->Mana = 880;
	Spell->Level = 45;
	Spell->RuneGr = 79;
	Spell->RuneNr = 8;
	Spell->Flags = 1;
	Spell->Amount = 1;
	Spell->RuneLevel = 15;
	Spell->SoulPoints = 5;
	Spell->Comment = "Sudden Death";

	Spell = CreateSpell(22, "ex", "evo", "vis", "lux", "");
	Spell->Mana = 100;
	Spell->Level = 23;
	Spell->Flags = 1;
	Spell->Comment = "Energy Beam";

	Spell = CreateSpell(23, "ex", "evo", "gran", "vis", "lux", "");
	Spell->Mana = 200;
	Spell->Level = 29;
	Spell->Flags = 1;
	Spell->Comment = "Great Energy Beam";

	Spell = CreateSpell(24, "ex", "evo", "gran", "mas", "vis", "");
	Spell->Mana = 1200;
	Spell->Level = 60;
	Spell->Flags = 3;
	Spell->Comment = "Ultimate Explosion";

	Spell = CreateSpell(25, "ad", "evo", "grav", "flam", "");
	Spell->Mana = 240;
	Spell->Level = 15;
	Spell->RuneGr = 79;
	Spell->RuneNr = 41;
	Spell->Flags = 1;
	Spell->Amount = 3;
	Spell->RuneLevel = 1;
	Spell->SoulPoints = 1;
	Spell->Comment = "Fire Field";

	Spell = CreateSpell(26, "ad", "evo", "grav", "pox", "");
	Spell->Mana = 200;
	Spell->Level = 14;
	Spell->RuneGr = 79;
	Spell->RuneNr = 25;
	Spell->Flags = 1;
	Spell->Amount = 3;
	Spell->RuneLevel = 0;
	Spell->SoulPoints = 1;
	Spell->Comment = "Poison Field";

	Spell = CreateSpell(27, "ad", "evo", "grav", "vis", "");
	Spell->Mana = 320;
	Spell->Level = 18;
	Spell->RuneGr = 79;
	Spell->RuneNr = 17;
	Spell->Flags = 1;
	Spell->Amount = 3;
	Spell->RuneLevel = 3;
	Spell->SoulPoints = 2;
	Spell->Comment = "Energy Field";

	Spell = CreateSpell(28, "ad", "evo", "mas", "grav", "flam", "");
	Spell->Mana = 780;
	Spell->Level = 33;
	Spell->RuneGr = 79;
	Spell->RuneNr = 43;
	Spell->Flags = 1;
	Spell->Amount = 4;
	Spell->RuneLevel = 6;
	Spell->SoulPoints = 4;
	Spell->Comment = "Fire Wall";

	Spell = CreateSpell(29, "ex", "ana", "pox", "");
	Spell->Mana = 30;
	Spell->Level = 10;
	Spell->Flags = 0;
	Spell->Comment = "Antidote";

	Spell = CreateSpell(30, "ad", "ito", "grav", "");
	Spell->Mana = 120;
	Spell->Level = 17;
	Spell->RuneGr = 79;
	Spell->RuneNr = 1;
	Spell->Flags = 0;
	Spell->Amount = 3;
	Spell->RuneLevel = 3;
	Spell->SoulPoints = 2;
	Spell->Comment = "Destroy Field";

	Spell = CreateSpell(31, "ad", "ana", "pox", "");
	Spell->Mana = 200;
	Spell->Level = 15;
	Spell->RuneGr = 79;
	Spell->RuneNr = 6;
	Spell->Flags = 0;
	Spell->Amount = 1;
	Spell->RuneLevel = 0;
	Spell->SoulPoints = 1;
	Spell->Comment = "Antidote Rune";

	Spell = CreateSpell(32, "ad", "evo", "mas", "grav", "pox", "");
	Spell->Mana = 640;
	Spell->Level = 29;
	Spell->RuneGr = 79;
	Spell->RuneNr = 29;
	Spell->Flags = 1;
	Spell->Amount = 4;
	Spell->RuneLevel = 5;
	Spell->SoulPoints = 3;
	Spell->Comment = "Poison Wall";

	Spell = CreateSpell(33, "ad", "evo", "mas", "grav", "vis", "");
	Spell->Mana = 1000;
	Spell->Level = 41;
	Spell->RuneGr = 79;
	Spell->RuneNr = 19;
	Spell->Flags = 1;
	Spell->Amount = 4;
	Spell->RuneLevel = 9;
	Spell->SoulPoints = 5;
	Spell->Comment = "Energy Wall";

	Spell = CreateSpell(34, "al", "evo", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Item";

	Spell = CreateSpell(35, "al", "evo", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Item";

	Spell = CreateSpell(37, "al", "ani", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Move";

	Spell = CreateSpell(38, "ut", "evo", "res", "ina", "para", "");
	Spell->Mana = 100;
	Spell->Level = 23;
	Spell->Flags = 0;
	Spell->Comment = "Creature Illusion";

	Spell = CreateSpell(39, "ut", "ani", "gran", "hur", "");
	Spell->Mana = 100;
	Spell->Level = 20;
	Spell->Flags = 2;
	Spell->Comment = "Strong Haste";

	Spell = CreateSpell(40, "al", "evo", "cogni", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Experience";

	Spell = CreateSpell(41, "al", "eta", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Change Data";

	Spell = CreateSpell(42, "ex", "evo", "pan", "");
	Spell->Mana = 120;
	Spell->Level = 14;
	Spell->SoulPoints = 1;
	Spell->Flags = 0;
	Spell->Comment = "Food";

	Spell = CreateSpell(44, "ut", "amo", "vita", "");
	Spell->Mana = 50;
	Spell->Level = 14;
	Spell->Flags = 0;
	Spell->Comment = "Magic Shield";

	Spell = CreateSpell(45, "ut", "ana", "vid", "");
	Spell->Mana = 440;
	Spell->Level = 35;
	Spell->Flags = 0;
	Spell->Comment = "Invisible";

	Spell = CreateSpell(46, "al", "evo", "cogni", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Skill Experience";

	Spell = CreateSpell(47, "al", "ani", "sio", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Teleport to Friend";

	Spell = CreateSpell(48, "ex", "evo", "con", "pox", "");
	Spell->Mana = 130;
	Spell->Level = 16;
	Spell->SoulPoints = 2;
	Spell->Flags = 0;
	Spell->Comment = "Poisoned Arrow";

	Spell = CreateSpell(49, "ex", "evo", "con", "flam", "");
	Spell->Mana = 290;
	Spell->Level = 25;
	Spell->SoulPoints = 3;
	Spell->Flags = 0;
	Spell->Comment = "Explosive Arrow";

	Spell = CreateSpell(50, "ad", "evo", "res", "flam", "");
	Spell->Mana = 600;
	Spell->Level = 27;
	Spell->RuneGr = 79;
	Spell->RuneNr = 48;
	Spell->Flags = 3;
	Spell->Amount = 2;
	Spell->RuneLevel = 7;
	Spell->SoulPoints = 3;
	Spell->Comment = "Soulfire";

	Spell = CreateSpell(51, "ex", "evo", "con", "");
	Spell->Mana = 100;
	Spell->Level = 13;
	Spell->SoulPoints = 1;
	Spell->Flags = 0;
	Spell->Comment = "Conjure Arrow";

	Spell = CreateSpell(52, "al", "liber", "sio", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Retrieve Friend";

	Spell = CreateSpell(53, "al", "evo", "res", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Summon Wild Creature";

	Spell = CreateSpell(54, "ad", "ana", "ani", "");
	Spell->Mana = 1400;
	Spell->Level = 54;
	Spell->RuneGr = 79;
	Spell->RuneNr = 18;
	Spell->Flags = 3;
	Spell->Amount = 1;
	Spell->RuneLevel = 18;
	Spell->SoulPoints = 3;
	Spell->Comment = "Paralyze";

	Spell = CreateSpell(55, "ad", "evo", "mas", "vis", "");
	Spell->Mana = 880;
	Spell->Level = 37;
	Spell->RuneGr = 79;
	Spell->RuneNr = 2;
	Spell->Flags = 3;
	Spell->Amount = 2;
	Spell->RuneLevel = 10;
	Spell->SoulPoints = 5;
	Spell->Comment = "Energybomb";

	Spell = CreateSpell(56, "ex", "evo", "gran", "mas", "pox", "");
	Spell->Mana = 600;
	Spell->Level = 50;
	Spell->Flags = 3;
	Spell->Comment = "Poison Storm";

	Spell = CreateSpell(57, "om", "ana", "liber", "para", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Banish Account";

	Spell = CreateSpell(58, "al", "iva", "tera", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Position";

	Spell = CreateSpell(60, "om", "ani", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Temple Teleport";

	Spell = CreateSpell(61, "om", "ana", "gran", "liber", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Delete Account";

	Spell = CreateSpell(62, "om", "amo", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Set Namerule";

	Spell = CreateSpell(63, "al", "evo", "vis", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Create Gold";

	Spell = CreateSpell(64, "al", "eta", "vita", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Change Profession or Sex";

	Spell = CreateSpell(65, "om", "isa", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Entry in Criminal Record";

	Spell = CreateSpell(66, "om", "ana", "hora", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Namelock";

	Spell = CreateSpell(67, "om", "ana", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Kick Player";

	Spell = CreateSpell(68, "om", "ana", "gran", "res", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Delete Character";

	Spell = CreateSpell(69, "om", "ana", "vis", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Banish IP Address";

	Spell = CreateSpell(70, "om", "ana", "res", "para", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Banish Character";

	Spell = CreateSpell(71, "al", "eta", "sio", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Invite Guests";

	Spell = CreateSpell(72, "al", "eta", "som", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Invite Subowners";

	Spell = CreateSpell(73, "al", "ana", "sio", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Kick Guest";

	Spell = CreateSpell(74, "al", "eta", "grav", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Edit Door";

	Spell = CreateSpell(75, "ut", "evo", "vis", "lux", "");
	Spell->Mana = 140;
	Spell->Level = 26;
	Spell->Flags = 2;
	Spell->Comment = "Ultimate Light";

	Spell = CreateSpell(76, "ex", "ani", "tera", "");
	Spell->Mana = 20;
	Spell->Level = 9;
	Spell->Flags = 2;
	Spell->Comment = "Magic Rope";

	Spell = CreateSpell(77, "ad", "evo", "res", "pox", "");
	Spell->Mana = 400;
	Spell->Level = 21;
	Spell->RuneGr = 79;
	Spell->RuneNr = 32;
	Spell->Flags = 3;
	Spell->Amount = 1;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 2;
	Spell->Comment = "Envenom";

	Spell = CreateSpell(78, "ad", "ito", "tera", "");
	Spell->Mana = 200;
	Spell->Level = 21;
	Spell->RuneGr = 79;
	Spell->RuneNr = 50;
	Spell->Amount = 3;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 3;
	Spell->Flags = 3;
	Spell->Comment = "Desintegrate";

	Spell = CreateSpell(79, "ex", "evo", "con", "mort", "");
	Spell->Mana = 140;
	Spell->Level = 17;
	Spell->SoulPoints = 2;
	Spell->Flags = 2;
	Spell->Comment = "Conjure Bolt";

	Spell = CreateSpell(80, "ex", "ori", "");
	Spell->Mana = 0;
	Spell->Level = 35;
	Spell->Flags = 7;
	Spell->Comment = "Berserk";

	Spell = CreateSpell(81, "ex", "ani", "hur", "para", "");
	Spell->Mana = 50;
	Spell->Level = 12;
	Spell->Flags = 2;
	Spell->Comment = "Levitate";

	Spell = CreateSpell(82, "ex", "ura", "gran", "mas", "res", "");
	Spell->Mana = 150;
	Spell->Level = 36;
	Spell->Flags = 10;
	Spell->Comment = "Mass Healing";

	Spell = CreateSpell(83, "ad", "ana", "mort", "");
	Spell->Mana = 600;
	Spell->Level = 27;
	Spell->RuneGr = 79;
	Spell->RuneNr = 56;
	Spell->Amount = 1;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 5;
	Spell->Flags = 3;
	Spell->Comment = "Animate Dead";

	Spell = CreateSpell(84, "ex", "ura", "sio", "para", "");
	Spell->Mana = 70;
	Spell->Level = 18;
	Spell->Flags = 10;
	Spell->Comment = "Heal Friend";

	Spell = CreateSpell(85, "ex", "ana", "mas", "mort", "");
	Spell->Mana = 500;
	Spell->Level = 30;
	Spell->Flags = 3;
	Spell->Comment = "Undead Legion";

	Spell = CreateSpell(86, "ad", "evo", "grav", "tera", "");
	Spell->Mana = 750;
	Spell->Level = 32;
	Spell->RuneGr = 79;
	Spell->RuneNr = 33;
	Spell->Amount = 3;
	Spell->RuneLevel = 9;
	Spell->SoulPoints = 5;
	Spell->Flags = 3;
	Spell->Comment = "Magic Wall";

	Spell = CreateSpell(87, "ex", "ori", "mort", "");
	Spell->Mana = 20;
	Spell->Level = 11;
	Spell->Flags = 3;
	Spell->Comment = "Force Strike";

	Spell = CreateSpell(88, "ex", "ori", "vis", "");
	Spell->Mana = 20;
	Spell->Level = 12;
	Spell->Flags = 3;
	Spell->Comment = "Energy Strike";

	Spell = CreateSpell(89, "ex", "ori", "flam", "");
	Spell->Mana = 20;
	Spell->Level = 12;
	Spell->Flags = 3;
	Spell->Comment = "Flame Strike";

	Spell = CreateSpell(90, "ex", "ana", "ina", "");
	Spell->Mana = 200;
	Spell->Level = 26;
	Spell->Flags = 2;
	Spell->Comment = "Cancel Invisibility";

	Spell = CreateSpell(91, "ad", "evo", "mas", "pox", "");
	Spell->Mana = 520;
	Spell->Level = 25;
	Spell->RuneGr = 79;
	Spell->RuneNr = 26;
	Spell->Flags = 3;
	Spell->Amount = 2;
	Spell->RuneLevel = 4;
	Spell->SoulPoints = 2;
	Spell->Comment = "Poisonbomb";

	Spell = CreateSpell(92, "ex", "eta", "vis", "");
	Spell->Mana = 80;
	Spell->Level = 41;
	Spell->Flags = 2;
	Spell->Comment = "Enchant Staff";

	Spell = CreateSpell(93, "ex", "eta", "res", "");
	Spell->Mana = 30;
	Spell->Level = 20;
	Spell->Flags = 3;
	Spell->Comment = "Challenge";

	Spell = CreateSpell(94, "ex", "evo", "grav", "vita", "");
	Spell->Mana = 220;
	Spell->Level = 27;
	Spell->Flags = 3;
	Spell->Comment = "Wild Growth";

	Spell = CreateSpell(95, "ex", "evo", "con", "vis", "");
	Spell->Mana = 800;
	Spell->Level = 59;
	Spell->SoulPoints = 3;
	Spell->Flags = 2;
	Spell->Comment = "Power Bolt";

	Spell = CreateSpell(96, "al", "iva", "cogni", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Get Quest Value";

	Spell = CreateSpell(97, "al", "eta", "cogni", "para", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Set Quest Value";

	Spell = CreateSpell(98, "al", "ito", "tera", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Desintegrate Spell";

	Spell = CreateSpell(99, "al", "ani", "hur", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Levitate Gamemaster";

	Spell = CreateSpell(100, "al", "ana", "cogni", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Clear Quest Values";

	Spell = CreateSpell(101, "al", "ito", "mas", "res", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Kill All Creatures";

	Spell = CreateSpell(102, "al", "evo", "mas", "res", "para", "");
	Spell->Mana = 0;
	Spell->Level = 0;
	Spell->Flags = 0;
	Spell->Comment = "Start Monsterraid";
}

void InitMagic(void){
	InitCircles();
	InitSpells();
	InitLog("banish");
}

void ExitMagic(void){
	// no-op
}

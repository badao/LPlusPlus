#pragma once
#include "Extensions.h"
#include "Template.h"

#pragma region variables
IUnit* Player;
SArray<IUnit*> Enemies, Allies, AllHeros;
ISpell2* Q, *E, *W, *R, *Q2, *W2, *E2, *R2;
ISpell  *Flash, *Ignite, *Heal, *Smite;
float ERange = 750, RRange = 550, QRange = 625;
string summoner1, summoner2;
inline void Init()
{
	Ignite = GPluginSDK->CreateSpell(GEntityList->Player()->GetSpellSlot("summonerdot"), 600);
	Flash = GPluginSDK->CreateSpell(GEntityList->Player()->GetSpellSlot("summonerflash"), 600);
	Heal = GPluginSDK->CreateSpell(GEntityList->Player()->GetSpellSlot("summonerheal"), 600);
	summoner1 = GEntityList->Player()->GetSpellName(kSummonerSlot1);
	summoner2 = GEntityList->Player()->GetSpellName(kSummonerSlot2);
	if (Contains(summoner1, "smite"))
	{
		Smite = GPluginSDK->CreateSpell(GEntityList->Player()->GetSpellSlot(summoner1.c_str()), 550);
	}
	else if (Contains(summoner2, "smite"))
	{
		Smite = GPluginSDK->CreateSpell(GEntityList->Player()->GetSpellSlot(summoner2.c_str()), 550);
	}
	else
	{
		Smite = GPluginSDK->CreateSpell(eSpellSlot(Unknown), 550);
	}
	Player = GEntityList->Player();
	Enemies = GEntityList->GetAllHeros(false, true);
	Allies = GEntityList->GetAllHeros(true, false);
	AllHeros = GEntityList->GetAllHeros(true, true);

}
// de coi nha hihi
inline void Dutru()
{
	// lay dieu kien de cast smite nhe
	if (Smite->GetSpellSlot() != eSpellSlot(Unknown)) {}

}

// KatarinaRMis
class KatarinaDagger

{
public:
	KatarinaDagger(float creatT, IUnit* dagger)
	{
		CreationTime = creatT;
		Dagger = dagger;
	}
	KatarinaDagger()
	{}
public:
	float CreationTime = 0;
	IUnit *Dagger = nullptr;
	bool IsNull() const
	{
		return CreationTime == 0;
	}
};
// katarina bien toan bo
float LastRMis = 0;
SArray <IUnit*> RMis;
SArray <IUnit*> WMis;
SArray <IUnit*> MyBeam;
SArray <KatarinaDagger> Daggers;
inline SArray <KatarinaDagger> WDaggers()
{
	return
		Daggers.Where([&](KatarinaDagger i) -> bool
	{
		return
			WMis.Any([&](IUnit* i2) -> bool
		{
			return Distance(i.Dagger, GMissileData->GetEndPosition(i2)) <= 20;
		});

	});
}
inline SArray <KatarinaDagger> PickableDaggers()
{
	return
		Daggers.Where([&](KatarinaDagger i) -> bool
	{
		return
			GGame->TickCount() - i.CreationTime >= 1175
			&& Distance(i.Dagger, Player) <= ERange
			&& MyBeam.Any([&](IUnit* i2)
		{
			return
				Distance(i2, i.Dagger) <= 20;
		});
	});
}
//menu
IMenu* MainMenu;
IMenu* ComboMenu;
IMenu* HarassMenu;
IMenu* FarmMenu;
IMenu* AutoMenu;
IMenu* DrawingsMenu;
IMenu* FleeAndWallJump;

IMenuOption* FLeeKey;
IMenuOption* JumpKey;
//combo
IMenuOption* ComboCancelRForKS;
IMenuOption* ComboCancelRNoTarget;
IMenuOption* ComboDontAttack;
IMenuOption* ComboDontAttackRange;
//auto
IMenuOption* AutoKs;
IMenuOption* AutoQ;
//harass
IMenuOption* HarassWE;
IMenuOption* HarassW;
IMenuOption* HarassE;

//lasthit
IMenuOption* LastHitQ;
//laneclear
IMenuOption* LaneClearQ;
IMenuOption* LaneClearW;

#pragma endregion 

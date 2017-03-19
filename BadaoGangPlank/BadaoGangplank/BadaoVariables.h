#pragma once
#include "Extensions.h"
#include "Template.h"
#include "cmath"

#pragma region variables
DelayAction * pDelay = nullptr;
inline IUnit* Player() { return GEntityList->Player();}
inline SArray<IUnit*> Enemies() {return  SArray<IUnit*>(GEntityList->GetAllHeros(false, true));}
inline SArray <IUnit*>Allies() {return SArray<IUnit*>(GEntityList->GetAllHeros(true, false));}
inline SArray <IUnit*>AllHeros() {return SArray<IUnit*>(GEntityList->GetAllHeros(true, true));}
ISpell2* Q, *E, *W, *R, *Q2, *W2, *E2, *R2;
ISpell  *Flash, *Ignite, *Heal, *Smite;
float ERange = 1000, RRange = 0, QRange = 625, WRange = 0;
string summoner1, summoner2;
PLUGIN_EVENT(void) InitOnUpdate()
{
	pDelay->OnGameUpdate();
}
inline void InitOnLoad()
{
	pDelay = new DelayAction;
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
	GEventManager->AddEventHandler(kEventOnGameUpdate, InitOnUpdate);
}
inline void InitUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, InitOnUpdate);
}
// de coi nha hihi
inline void Dutru()
{
	// lay dieu kien de cast smite nhe
	if (Smite->GetSpellSlot() != eSpellSlot(Unknown)) {}

}



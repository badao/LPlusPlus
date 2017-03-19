#pragma once
#include "BadaoVariables.h"
#include <string>

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
			&& Distance(i.Dagger, Player()) <= ERange
			&& MyBeam.Any([&](IUnit* i2)
		{
			return
				Distance(i2, i.Dagger) <= 20;
		});
	});
}
PLUGIN_EVENT(void) VariablesOnCreateObject(IUnit* Source)
{
	if (Source->IsMissile() && GMissileData->GetCaster(Source) == Player())
	{
		if (Contains(GMissileData->GetName(Source), "katarinarmis"))
		{
			RMis.Add(Source);
			LastRMis = GGame->TickCount(); // in milisecond
		}
		if (Contains(GMissileData->GetName(Source), "katarinawdaggerarc"))
		{
			WMis.Add(Source);
		}
	}
	if (Contains(Source->GetObjectName(), "katarina_base_e_beam.troy"))
	{
		MyBeam.Add(Source);
	}
	if (Contains(Source->GetObjectName(), "katarina_base_w_indicator"))
	{
		Daggers.Add(KatarinaDagger(GGame->TickCount(), Source));
	}
}

PLUGIN_EVENT(void) VarialbesOnDestroyObject(IUnit* Source)
{
	if (Source->IsMissile() && GMissileData->GetCaster(Source) == Player())
	{
		if (Contains(GMissileData->GetName(Source), "katarinarmis"))
		{
			RMis.RemoveAll([&](IUnit* i) {return i == Source; });
		}
		if (Contains(GMissileData->GetName(Source), "katarinawdaggerarc"))
		{
			WMis.RemoveAll([&](IUnit* i) {return i == Source; });
		}
	}
	if (Contains(Source->GetObjectName(), "katarina_base_e_beam.troy"))
	{
		MyBeam.RemoveAll([&](IUnit* i) {return i == Source; });
	}
	if (Contains(Source->GetObjectName(), "katarina_base_w_indicator"))
	{
		Daggers.RemoveAll([&](KatarinaDagger i) {return i.Dagger == Source; });
	}
}
PLUGIN_EVENT(void) VariablesOnSpellCast(CastedSpell const& Args)
{

}
PLUGIN_EVENT(void) VariablesOnRender()
{

}

inline void VariableOnLoad()
{
	GEventManager->AddEventHandler(kEventOnCreateObject, VariablesOnCreateObject);
	GEventManager->AddEventHandler(kEventOnDestroyObject, VarialbesOnDestroyObject);
	GEventManager->AddEventHandler(kEventOnSpellCast, VariablesOnSpellCast);
	GEventManager->AddEventHandler(kEventOnRender, VariablesOnRender);
}
inline void VariableUnload()
{
	GEventManager->RemoveEventHandler(kEventOnCreateObject, VariablesOnCreateObject);
	GEventManager->RemoveEventHandler(kEventOnDestroyObject, VarialbesOnDestroyObject);
	GEventManager->RemoveEventHandler(kEventOnSpellCast, VariablesOnSpellCast);
	GEventManager->RemoveEventHandler(kEventOnRender, VariablesOnRender);
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
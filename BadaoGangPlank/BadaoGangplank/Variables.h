#pragma once
#include "BadaoVariables.h"
#include <string>

#pragma region GPvariables
//Gp variables

class Barrel
{
public:
	IUnit* Bottle = nullptr;
	int CreationTime = 0;
	Barrel() {}
	Barrel(int creatT, IUnit* bottle) { CreationTime = creatT; Bottle = bottle; }
	bool IsNull() const
	{
		return CreationTime == 0;
	}
};
SArray<Barrel> Barrels;
int LastCastE;
Vec3 LastEPos;
//int LastCondition;
inline SArray<Barrel> SingleBarrel()
{
	return Barrels.Where([&](Barrel i) {return !Barrels
		.Any([&](Barrel i2) {return i2.Bottle->GetNetworkId() != i.Bottle->GetNetworkId() && Distance(i.Bottle, i2.Bottle) <= 700; }); });
}
inline SArray<Barrel> ChainedBarrels(Barrel explodeBarrel)
{
	SArray<Barrel> level1 = Barrels.Where([&](Barrel x) {return Distance(x.Bottle, explodeBarrel.Bottle) <= 700; });
	SArray<Barrel> level2 = Barrels.Where([&](Barrel x) {return level1.Any([&](Barrel y) {return Distance(y.Bottle, x.Bottle) <= 700; }); });
	SArray<Barrel> level3 = Barrels.Where([&](Barrel x) {return level2.Any([&](Barrel y) {return Distance(y.Bottle, x.Bottle) <= 700; }); });
	return
		level3;
}
inline SArray<Barrel> AttackableBarrels(int delay = 0)
{
	int time = Player()->GetLevel() >= 13 ?
		500 :
		Player()->GetLevel() >= 7 ?
		1000 :
		2000;
	SArray<Barrel> meelebarrels = Barrels.Where([&](Barrel x)
	{
		return InAutoAttackRange(x.Bottle) &&
			(GGame->TickCount() - x.CreationTime >= 2 * time - GGame->Latency() - Player()->WindupTime() * 1000 + 50 - delay
				|| (GGame->TickCount() - x.CreationTime >= time - GGame->Latency() - Player()->WindupTime() * 1000 + 50 - delay && x.Bottle->GetHealth() == 2
					&& GGame->TickCount() - x.CreationTime <= time) ?
				true : false
				|| x.Bottle->GetHealth() == 1);
	});
	return meelebarrels;
}
inline SArray<Barrel> QableBarrels(int delay = 0)
{
	int time = Player()->GetLevel() >= 13 ?
		500 :
		Player()->GetLevel() >= 7 ?
		1000 :
		2000;
	SArray<Barrel> qbarrels = Barrels.Where([&](Barrel x)
	{
		return InSpellRange(Q, x.Bottle) &&
			(GGame->TickCount() - x.CreationTime >= 2 * time - GGame->Latency() - 350 + 50 - delay
				|| (GGame->TickCount() - x.CreationTime >= time - GGame->Latency() - 350 + 50 - delay && x.Bottle->GetHealth() == 2
					&& GGame->TickCount() - x.CreationTime <= time) ?
				true : false
				|| x.Bottle->GetHealth() == 1);
	});
	return qbarrels;
}
inline void BarrelsOnCreate(IUnit* Source)
{
	//if (Source != nullptr)
	//	GGame->PrintChat(Source->GetObjectName());
	if (Source == nullptr || !Contains(Source->GetObjectName(), "barrel"))
		return;
	//GGame->PrintChat(std::to_string(Distance(LastEPos, Source->GetPosition())).c_str());
	if (abs(LastCastE - GGame->TickCount()) < 800 && Distance(LastEPos, Source->GetPosition()) < 500)
	{
		//GGame->PrintChat("c");
		Barrels.Add(Barrel(GGame->TickCount(), Source));
		LastCastE = 0;
	}
}
inline void BarrelsOnDelete(IUnit* Source)
{
	//if (Source != nullptr)
	//	GGame->PrintChat(Source->GetObjectName());
	if (Source != nullptr && Contains(Source->GetObjectName(), "gangplank_base_e_aoe_green.troy"))
	{
		Barrels.RemoveAll([&](Barrel i) {return Distance(i.Bottle, Source->GetPosition()) <= 20; });
	}
	if (Source != nullptr && Contains(Source->GetObjectName(), "barrel"))
	{
		Barrels.RemoveAll([&](Barrel i) {return i.Bottle->GetNetworkId() == Source->GetNetworkId(); });
	}
}
inline void BarrelsOnProcess(CastedSpell const& Args)
{
	if (Args.Caster_ != Player())
		return;
	//GGame->PrintChat("a");
	if (!Contains(Args.Name_, "gangplanke"))
		return;
	//GGame->PrintChat("b");
	LastCastE = GGame->TickCount();
	LastEPos = Args.EndPosition_;
}
inline void BarrelsOnRender()
{
	for (Barrel barrel : Barrels.ToVector())
	{
		GRender->DrawOutlinedCircle(barrel.Bottle->GetPosition(), Green(), 50);
		if (ChainedBarrels(barrel).Count() >= 2)
		{
			GRender->DrawOutlinedCircle(barrel.Bottle->GetPosition(), Pink(), 100);
		}
	}
}
PLUGIN_EVENT(void) VariablesOnCreateObject(IUnit* Source)
{
	BarrelsOnCreate(Source);
}

PLUGIN_EVENT(void) VarialbesOnDestroyObject(IUnit* Source)
{
	BarrelsOnDelete(Source);
}
PLUGIN_EVENT(void) VariablesOnSpellCast(CastedSpell const& Args)
{
	BarrelsOnProcess(Args);
}
PLUGIN_EVENT(void) VariablesOnRender()
{
	BarrelsOnRender();
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
#pragma endregion 

//menu
IMenu* MainMenu;
IMenu* ComboMenu;
IMenu* HarassMenu;
IMenu* FarmMenu;
IMenu* AutoMenu;
IMenu* DrawingsMenu;
IMenu* Flee;

IMenuOption* FLeeKey;
IMenuOption* JumpKey;
//combo
IMenuOption* ComboE1;
IMenuOption* ComboQSave;
//auto
IMenuOption* AutoWLowHealth;
IMenuOption* AutoWLowHealthValue;
IMenuOption* AutoWCC;
IMenuOption* AutoUltKillable;
IMenuOption* AutoUltKillableRange;
//harass
IMenuOption* HarassQ;

//lasthit
IMenuOption* LastHitQ;
//laneclear
IMenuOption* LaneQ;
IMenuOption* JungleQ;
//Drawing
IMenuOption* DrawQ;
IMenuOption* DrawE;
IMenuOption* DrawEPlacement;
#pragma endregion 
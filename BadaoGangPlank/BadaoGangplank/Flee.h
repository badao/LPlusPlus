#pragma once
#include "Helper.h"

inline void FleeOnUpdate()
{
	if (!IsKeyDown(FLeeKey))
		return;
	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
	if (Q->IsReady())
	{
		for (Barrel barrel: QableBarrels().ToVector())
		{
			Q->CastOnTarget(barrel.Bottle);
		}
	}
	if (GOrbwalking->CanAttack())
	{
		for (Barrel barrel : AttackableBarrels().ToVector())
		{
			GOrbwalking->SetAttacksAllowed(false);
			GOrbwalking->SetMovementAllowed(false);
			pDelay->Add(100 + GGame->Latency(), [&]()
			{GOrbwalking->SetAttacksAllowed(true); GOrbwalking->SetMovementAllowed(true); });
			GGame->IssueOrder(Player(), kAttackUnit, barrel.Bottle);
		}
	}

}
inline void HarassOnUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() != kModeMixed)
		return;
	if (HarassQ->Enabled() && Q->IsReady())
	{
		IUnit* target = SelectTarget(PhysicalDamage, QRange);
		if (IsValidTarget(target))
		{
			Q->CastOnTarget(target);
		}
	}
}
PLUGIN_EVENT(void) FleeEventOnGameUpdate()
{
	FleeOnUpdate();
	HarassOnUpdate();
}
void FleeOnload()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, FleeEventOnGameUpdate);
}
void FleeOnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, FleeEventOnGameUpdate);
}
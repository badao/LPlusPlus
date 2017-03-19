#pragma once
#include "Helper.h"

inline void LastHitOnUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() != kModeLastHit)
		return;
	if (Q->IsReady() && LastHitQ->Enabled())
	{
		IUnit* minionQ = EnemyMinions(QRange).Where([](IUnit* i) {return GetQDamage(i) >= i->GetHealth(); }).FirstOrDefault();
		if (minionQ != nullptr)
		{
			Q->CastOnTarget(minionQ);
		}
	}
}

PLUGIN_EVENT(void) LastHitEventOnGameUpdate()
{
	LastHitOnUpdate();
}
void LastHitOnload()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, LastHitEventOnGameUpdate);
}
void LastHitOnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, LastHitEventOnGameUpdate);
}
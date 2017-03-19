#pragma once
#include "Helper.h"
#include <string>

inline void LaneClearOnUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() != kModeLaneClear)
		return;
	if (Q->IsReady() && LaneClearQ->Enabled())
	{
		//GGame->PrintChat(std::to_string(WardMinions().Count()).c_str());
		//int b = GEntityList->GetAllMinions(false, true, false).size();
		//GGame->PrintChat(std::to_string(b).c_str());
		IUnit* minionQ = EnemyMinions(QRange).Where([](IUnit* i) {return GetQDamage(i) >= i->GetHealth(); }).FirstOrDefault();
		if (minionQ != nullptr)
		{
			Q->CastOnTarget(minionQ);
		}
	}
	if (W->IsReady() && LaneClearW->Enabled())
	{
		//GGame->PrintChat("c");
		auto minionWs = EnemyMinions(300);
		if (minionWs.Count() >= 2)
		{
			W->CastOnPlayer();
		}
	}
};

PLUGIN_EVENT(void) LaneClearEventOnGameUpdate()
{
	LaneClearOnUpdate();
}
void LaneClearOnload()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, LaneClearEventOnGameUpdate);
}
void LaneClearOnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, LaneClearEventOnGameUpdate);
}
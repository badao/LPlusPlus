#pragma once
#include "Helper.h"

inline void JungClearOnUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() != kModeLaneClear)
		return;
	if (!JungleQ->Enabled())
		return;
	for (IUnit* minion : NeutralMinions(QRange).ToVector())
	{
		if (IsValidTarget(minion) && GDamage->GetSpellDamage(Player(),minion,Q->GetSlot()) >= minion->GetHealth())
		{
			Q->CastOnTarget(minion);
		}
	}
}
inline void LaneClearOnUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() != kModeLaneClear)
		return;
	if (!LaneQ->Enabled())
		return;
	for (IUnit* minion : EnemyMinions(QRange).ToVector())
	{
		if (IsValidTarget(minion) && GDamage->GetSpellDamage(Player(), minion, Q->GetSlot()) >= minion->GetHealth())
		{
			Q->CastOnTarget(minion);
		}
	}
}
inline void LastHitOnUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() != kModeLastHit)
		return;
	if (!LastHitQ->Enabled())
		return;
	for (IUnit* minion : EnemyMinions(QRange).ToVector())
	{
		if (IsValidTarget(minion) && GDamage->GetSpellDamage(Player(), minion, Q->GetSlot()) >= minion->GetHealth())
		{
			Q->CastOnTarget(minion);
		}
	}
}
PLUGIN_EVENT(void) FarmEventOnGameUpdate()
{
	JungClearOnUpdate();
	LaneClearOnUpdate();
	LastHitOnUpdate();
}
void FarmOnload()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, FarmEventOnGameUpdate);
}
void FarmOnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, FarmEventOnGameUpdate);
}
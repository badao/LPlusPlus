#pragma once
#include "Helper.h"

inline void HarassOnUpdate()
{
	float time;
	if (GOrbwalking->GetOrbwalkingMode() != kModeMixed  || Player()->IsCastingImportantSpell(&time))
		return;
	// Q 
	if (Q->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, QRange);
		if (target != nullptr)
		{
			Q->CastOnTarget(target);
		}
	}
	//if (W->IsReady() && E->IsReady() && HarassWE->Enabled())
	//{
	//	auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, ERange);
	//	if (target !=nullptr)
	//	{
	//		W->CastOnPlayer();
	//	}
	//}
	//if (E->IsReady() && HarassWE->Enabled())
	//{
	//	if (WDaggers().Any([](KatarinaDagger i) {return Distance(Player, i.Dagger) <= 150 && GGame->TickCount() - GGame->Latency() - i.CreationTime >= 1150; }))
	//	{
	//		for (auto  target : ValidTargets(Enemies.ToVector()).ToVector())
	//		{
	//			IUnit* vinasun = GetEVinasun().Where([&](IUnit* i) {return Distance(i, target) <= 450; })
	//				.MinOrDefault<float>([&](IUnit* i2) {return Distance(i2, target); });
	//			if (vinasun != nullptr)
	//			{
	//				E->CastOnPosition(Extend(vinasun->GetPosition(), target->GetPosition(), 150));
	//			}
	//		}
	//	}
	//}
	// W
	if (W->IsReady() && HarassW->Enabled() && !(E->IsReady() && HarassE->Enabled()))
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, 300);
		if (target != nullptr)
		{
			W->CastOnPlayer();
		}
	}
	//chieu 3 ne may thim
	if (E->IsReady() && HarassE->Enabled())
	{
		auto EdaggerTarget = GTargetSelector->FindTarget(QuickestKill, SpellDamage, ERange + 200);
		auto ETarget = GTargetSelector->FindTarget(QuickestKill, SpellDamage, ERange);
		SArray<IUnit*> EdaggerOthers = SArray<IUnit*>(GEntityList->GetAllHeros(false, true)).Where([](IUnit* i) {return Player()->IsValidTarget(i, ERange + 200) && IsDaggerFixed(i); });
		if (EdaggerTarget != nullptr && IsDaggerFixed(EdaggerTarget))
		{
			auto EdaggerTargetdagger = GetFixedDagger(EdaggerTarget);
			if (!EdaggerTargetdagger.IsNull())
			{
				CastEFixedDagger(EdaggerTargetdagger, EdaggerTarget);
			}
		}
		else if (EdaggerOthers.Any())
		{
			auto target = EdaggerOthers.MinOrDefault<float>([](IUnit* i) {return i->GetHealth(); });
			KatarinaDagger targetdagger = GetFixedDagger(target);
			if (!targetdagger.IsNull())
			{
				CastEFixedDagger(targetdagger, target);
			}
		}
		else if (ETarget != nullptr && !(Q->IsReady() && !W->IsReady()))
		{
			KatarinaDagger nearest = Daggers.MinOrDefault<float>([&](KatarinaDagger i) {return Distance(i.Dagger, ETarget); });
			if (!nearest.IsNull())
			{
				auto pos = Extend(ETarget->GetPosition(), nearest.Dagger->GetPosition(), 150);
				E->CastOnPosition(pos);
			}
			else
			{
				auto pos = Extend(ETarget->GetPosition(), Player()->GetPosition(), Q->IsReady() ? 100 : -150);
				E->CastOnPosition(pos);
			}
		}

	}
}

PLUGIN_EVENT(void) HarassEventOnGameUpdate()
{
	HarassOnUpdate();
}
void HarassOnload()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, HarassEventOnGameUpdate);
}
void HarassOnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, HarassEventOnGameUpdate);
}
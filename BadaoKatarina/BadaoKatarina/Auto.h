#pragma once
#include "Helper.h"

inline void AutoOnUpdate()
{
	float endtime;
	if (AutoKs->Enabled() && !(GOrbwalking->GetOrbwalkingMode() == kModeCombo && !ComboCancelRForKS->Enabled() && Player()->IsCastingImportantSpell(&endtime)))
	{
		IUnit* targetQ = SelectTarget(SpellDamage,QRange);
		if (IsValidTarget(targetQ) && Q->IsReady() && GetQDamage(targetQ) >= targetQ->GetHealth())
		{
			Q->CastOnTarget(targetQ);
		}
		IUnit* targetE = SelectTarget(SpellDamage, ERange);
		if (IsValidTarget(targetE) && E->IsReady() && GetEDamage(targetE) >= targetE->GetHealth())
		{
			E->CastOnPosition(targetE->GetPosition());
		}
		IUnit* targetEQ = SelectTarget(SpellDamage, ERange);
		if (IsValidTarget(targetEQ) && E->IsReady() && Q->IsReady() && GetEDamage(targetEQ) + GetQDamage(targetEQ) >= targetEQ->GetHealth())
		{
			E->CastOnPosition(targetEQ->GetPosition());
		}
		if (Q->IsReady() && E->IsReady())
		{
			for (IUnit* hero : Enemies().Where([](IUnit* i) {return IsValidTarget(i, ERange + QRange + 150) && GetQDamage(i) >= i->GetHealth(); }).ToVector())
			{
				IUnit* nearest = GetEVinasun().MinOrDefault<float>([&](IUnit* i) {return Distance(i, hero); });
				if (nearest != nullptr && Distance(nearest,hero)<= 150 + QRange)
				{
					Vec3 pos = Extend(nearest->GetPosition(), hero->GetPosition(), 150);
					E->CastOnPosition(pos);
				}
			}
		}
		if (E->IsReady())
		{
			SArray<IUnit*> EdaggerOthers = SArray<IUnit*>(GEntityList->GetAllHeros(false, true)).Where([](IUnit* i)
				{return Player()->IsValidTarget(i, ERange + 200) && IsDaggerFixed(i) && GetPassiveDamage(i) >= i->GetHealth(); });
			if (EdaggerOthers.Any())
			{
				auto target = EdaggerOthers.MinOrDefault<float>([](IUnit* i) {return i->GetHealth(); });
				KatarinaDagger targetdagger = GetFixedDagger(target);
				if (!targetdagger.IsNull())
				{
					CastEFixedDagger(targetdagger, target);
				}
			}
		}
		//ks with ignite
		if (HasSummonerSpell(Ignite))
		{
			//GGame->PrintChat(std::to_string(Ignite->GetSpellRange()).c_str());
			IUnit* target = SelectTarget(SpellDamage, Ignite->GetSpellRange());
			if (IsValidTarget(target) && 50 + 20 * Player()->GetLevel() >= target->GetHealth())
			{
				//GGame->PrintChat("ues");
				Ignite->CastOnUnit(target);
			}
		}
	}
	// auto q
	if (Q->IsReady() && AutoQ->Enabled() && GOrbwalking->GetOrbwalkingMode() != kModeCombo && !Player()->IsCastingImportantSpell(&endtime))
	{
		auto target = SelectTarget(SpellDamage, QRange);
		if (target != nullptr)
		{
			Q->CastOnTarget(target);
		}
	}
}
PLUGIN_EVENT(void) AutoEventOnGameUpdate()
{
	AutoOnUpdate();
}
void AutoOnLoad()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, AutoEventOnGameUpdate);
}
void AutoOnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, AutoEventOnGameUpdate);
}
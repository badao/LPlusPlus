#pragma once
#include "Helper.h"

inline void ComboOnGameUpdate()
{
	//chan danh thuong ne
	if (ComboDontAttack->Enabled() && GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		if (Daggers.Any([](KatarinaDagger x) {return Distance(x.Dagger, Player()) <= 150 + ComboDontAttackRange->GetInteger(); })
			&& !Daggers.Any([](KatarinaDagger x) {return Distance(x.Dagger, Player()) <= 150; }))
		{
			GOrbwalking->SetAttacksAllowed(false);
		}
		else GOrbwalking->SetAttacksAllowed(true);
	}
	else { GOrbwalking->SetAttacksAllowed(true); }
	if (GOrbwalking->GetOrbwalkingMode() != kModeCombo)
		return;
	//ks with ignite
	if (HasSummonerSpell(Ignite))
	{
		//GGame->PrintChat(std::to_string(Ignite->GetSpellRange()).c_str());
		IUnit* target = SelectTarget(SpellDamage, Ignite->GetSpellRange());
		if (IsValidTarget(target) && 50 + 20 *  Player()->GetLevel() >= target->GetHealth())
		{
			//GGame->PrintChat("ues");
			Ignite->CastOnUnit(target);
		}
	}

	// huy bo chieu 4
	float endTime = 0;
	if (ComboCancelRNoTarget->Enabled() && Player()->IsCastingImportantSpell(&endTime)
		&& CountEnemiesInRange(Player()->GetPosition(), RRange) == 0 && GGame->TickCount() >= LastRMis + 200)
	{
		GGame->IssueOrder(Player(), kMoveTo, GGame->CursorPosition());
	}
	 //an cuop ne 
	if (ComboCancelRForKS->Enabled() || !Player()->IsCastingImportantSpell(&endTime))
	{
		auto targetQ = GTargetSelector->FindTarget(QuickestKill, SpellDamage, QRange);
		if (Player()->IsValidTarget(targetQ,QRange) && Q->IsReady() && GetQDamage(targetQ) >= targetQ->GetHealth())
		{
			Q->CastOnTarget(targetQ);
		}
		auto targetE = GTargetSelector->FindTarget(QuickestKill, SpellDamage, ERange);
		if (Player()->IsValidTarget(targetQ, ERange) && E->IsReady() && GetEDamage(targetE) >= targetE->GetHealth())
		{
			E->CastOnPosition(targetE->GetPosition());
		}
		auto targetQE = GTargetSelector->FindTarget(QuickestKill, SpellDamage, ERange);
		if (Player()->IsValidTarget(targetQ, ERange) && E->IsReady() && Q->IsReady() &&
			GetQDamage(targetQE) + GetEDamage(targetQE) >= targetQE->GetHealth())
		{
			E->CastOnPosition(targetQE->GetPosition());
		}
		if (Q->IsReady() && E->IsReady())
		{
			for each
				(IUnit* hero in SArray<IUnit*>(GEntityList->GetAllHeros(false, true))
					.Where([](IUnit* x) {return Player()->IsValidTarget(x, ERange + QRange + 150) && GetQDamage(x); }).ToVector())
			{
				IUnit* nearest = GetEVinasun().MinOrDefault<float>([&](IUnit* i) {return Distance(i, hero); });
				if (nearest != nullptr && Distance(nearest, hero) <= 150 + QRange)
				{
					Vec3 pos = Extend(nearest->GetPosition(), hero->GetPosition(), 150);
					E->CastOnPosition(pos);
				}
			}
		}
	}
	// binh thuong ne cac chu em
	if (Player()->IsCastingImportantSpell(&endTime))
		return;
	// 3146 hextech, 3144 bilewater
	{
		IUnit* target = SelectTarget(SpellDamage, 700);
		CastItemOnUnit(3146, 700, target);
	}
	{
		IUnit* target = SelectTarget(SpellDamage, 550);
		CastItemOnUnit(3144, 550, target);
	}
	// chieu 2 ne may thim
	if (W->IsReady() && !E->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, 300);
		if (target != nullptr)
		{
			W->CastOnPlayer();
		}
	}
	// chieu 1 ne may thim
	if (Q->IsReady())
	{
		if (!(W->IsReady() && E->IsReady()))
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, QRange);
			if (target != nullptr)
			{
				Q->CastOnTarget(target);
			}
		}
	}
	//chieu 3 ne may thim
	if (E->IsReady())
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
		// tiem can
		else if (Q->IsReady() || W->IsReady() || R->IsReady())
		{
			KatarinaDagger vinasun = PickableDaggers().Where([](KatarinaDagger i) { return CountEnemiesInRange(Player()->GetPosition(), ERange) > 0; })
				.MaxOrDefault<int>([](KatarinaDagger i) { return CountEnemiesInRange(Player()->GetPosition(), ERange); });
			if (!vinasun.IsNull())
			{
				E->CastOnPosition(vinasun.Dagger->GetPosition());
			}
		}
		
	}
	// chieu 4 ne cac em
	if (Player()->GetSpellLevel(kSlotR) >= 1 && Player()->GetSpellBook()->GetNextCastTime(kSlotR) <= GGame->Time())
	{
		auto expires = (Player()->GetSpellBook()->GetNextCastTime(kSlotE) - GGame->Time()) * 1000;
		if (E->IsReady() || expires < 50)
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, ERange);
			if (target != nullptr)
			{
				E->CastOnPosition(target->GetPosition());
			}
		}
		else if (R->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, RRange - 200);
			if (target != nullptr)
			{
				if (W->IsReady())
					W->CastOnPlayer();
				else if (Q->IsReady())
					Q->CastOnTarget(target);
				else
					R->CastOnPlayer();
			}
		}
	}
}

PLUGIN_EVENT(void) ComboEventOnGameUpdate()
{
	ComboOnGameUpdate();
}
void ComboOnload()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, ComboEventOnGameUpdate);
}
void ComboOnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, ComboEventOnGameUpdate);
}
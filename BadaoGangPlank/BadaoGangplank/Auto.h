#pragma once
#include "Helper.h"
#include "Combo.h"

inline void AutoOnUpdate()
{
	if (GGame->TickCount() - LastCondition >= 100 + GGame->Latency())
	{
		for (IUnit* hero : ValidEnemies().ToVector())
		{
			Vec3 pred;
			GPrediction->GetFutureUnitPosition(hero, 0.5f, true, pred);
			if (Q->IsReady())
			{
				for (Barrel barrel : QableBarrels().ToVector())
				{
					SArray<Barrel> nbarrels = ChainedBarrels(barrel);
					if (nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, pred) <= 330; }))
					{
						GOrbwalking->SetAttacksAllowed(false);
						GOrbwalking->SetMovementAllowed(false);
						pDelay->Add(100 + GGame->Latency(), [&]()
						{GOrbwalking->SetAttacksAllowed(true); GOrbwalking->SetMovementAllowed(true); });
						Q->CastOnTarget(barrel.Bottle);
						LastCondition = GGame->TickCount();
						return;
					}
				}
			}
		}
		for (IUnit* hero : ValidEnemies().ToVector())
		{
			Vec3 pred;
			GPrediction->GetFutureUnitPosition(hero, 0.5f, true, pred);
			if (GOrbwalking->CanAttack())
			{
				for (Barrel barrel : AttackableBarrels().ToVector())
				{
					SArray<Barrel> nbarrels = ChainedBarrels(barrel);
					if (nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, pred) <= 330; }))
					{
						GOrbwalking->SetAttacksAllowed(false);
						GOrbwalking->SetMovementAllowed(false);
						pDelay->Add(100 + GGame->Latency(), [&]()
						{GOrbwalking->SetAttacksAllowed(true); GOrbwalking->SetMovementAllowed(true); });
						GGame->IssueOrder(Player(), kAttackUnit, barrel.Bottle);
						LastCondition = GGame->TickCount();
						return;
					}
				}
			}
		}
	}
	if (W->IsReady() && AutoWLowHealth->Enabled() && AutoWLowHealthValue->GetInteger() >= Player()->HealthPercent())
	{
		W->CastOnPlayer();
	}
	if (W->IsReady() && AutoWCC->Enabled())
	{
		vector<eBuffType> buffs = vector<eBuffType>{ BUFF_Stun, BUFF_Snare,BUFF_Suppression,BUFF_Silence,BUFF_Taunt,BUFF_Charm,BUFF_Blind,BUFF_Fear,BUFF_Polymorph };
		for (eBuffType buff : buffs)
		{
			if (Player()->HasBuffOfType(buff))
			{
				W->CastOnPlayer();
			}
		}
	}
	if (Q->IsReady())
	{
		for (IUnit* hero : ValidEnemies(QRange).Where([&](IUnit* i) {return GetSpellDamage(i, Q->GetSlot()) >= i->GetHealth(); }).ToVector())
		{
			Q->CastOnTarget(hero);
		}
	}
	if (R->IsReady() && AutoUltKillable->Enabled())
	{
		for (IUnit* hero : ValidEnemies().Where([&](IUnit* i) {return !IsValidTarget(i,AutoUltKillableRange->GetInteger())
				&& GetSpellDamage(i, R->GetSlot()) * 6 >= i->GetHealth(); }).ToVector())
		{
			R->CastOnPosition(hero->GetPosition());
		}
	}

}

PLUGIN_EVENT(void) AutoEventOnGameUpdate()
{
	AutoOnUpdate();
}
void AutoOnload()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, AutoEventOnGameUpdate);
}
void AutoOnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, AutoEventOnGameUpdate);
}
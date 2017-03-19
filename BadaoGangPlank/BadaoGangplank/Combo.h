#pragma once
#include "Helper.h"

inline int Estack() { return Player()->GetSpellBook()->GetAmmo(kSlotE); }
int LastCondition;
inline void ComboOnGameUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() != kModeCombo)
		return;
	if (GGame->TickCount() - LastCondition >= 100 + GGame->Latency())
	{
		for (IUnit* hero : ValidEnemies().ToVector())
		{
			Vec3 pred;
			GPrediction->GetFutureUnitPosition(hero, 0.5f, true, pred);
			if (Q->IsReady() && E->IsReady())
			{
				for (Barrel barrel : QableBarrels(350).ToVector())
				{
					SArray<Barrel> nbarrels = ChainedBarrels(barrel);
					if (nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, pred) <= 990; })
						&& !nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, pred) <= 330; }))
					{
						for (int i = 990; i >= 400; i -= 20)
						{
							SArray<Barrel> mbarrels = nbarrels.Where([&](Barrel x) {return Distance(x.Bottle, pred) <= i; })
							.OrderBy<float>([&](Barrel x2) {return Distance(x2.Bottle, pred); });
							for (Barrel mbarrel : mbarrels.ToVector())
							{
								Vec3 pos = Extend(mbarrel.Bottle->GetPosition(), pred, i - 330);
								if (Distance(Player(),pos) <= ERange)
								{
									GOrbwalking->SetAttacksAllowed(false);
									GOrbwalking->SetMovementAllowed(false);
									pDelay->Add(100 + GGame->Latency(),[&]()
									{GOrbwalking->SetAttacksAllowed(true); GOrbwalking->SetMovementAllowed(true); });
									E->CastOnPosition(pos);
									LastCondition = GGame->TickCount();
									return;
								}
							}
						}
					}
				}
				for (Barrel barrel : QableBarrels().ToVector())
				{
					// choi mot luc 3 thung
					SArray<Barrel> nbarrels = ChainedBarrels(barrel);
					if (Distance(barrel.Bottle, pred) <= 330 + 660 + 660 && !(Distance(barrel.Bottle, pred) <= 330 + 660) && Estack() >=2)
					{
						for (int i = 330 + 660 + 660; i >= 380 + 660; i -= 20)
						{
							if (Distance(barrel.Bottle, pred) <= i)
							{
								Vec3 pos1 = Extend(barrel.Bottle->GetPosition(), pred, 660);
								Vec3 pos2 = Extend(barrel.Bottle->GetPosition(), pred, i - 330);
								if (InSpellRange(E,pos1) && InSpellRange(E, pos2) && !GNavMesh->IsPointWall(pos1) && !GNavMesh->IsPointWall(pos2))
								{
									GOrbwalking->SetAttacksAllowed(false);
									GOrbwalking->SetMovementAllowed(false);
									pDelay->Add(100 + GGame->Latency() + 875, [&]()
									{GOrbwalking->SetAttacksAllowed(true); GOrbwalking->SetMovementAllowed(true); });
									E->CastOnPosition(pos1);
									//pDelay->Add(550, [&]() {if (barrel.Bottle != nullptr) Q->CastOnTarget(barrel.Bottle); });
									pDelay->AddIUnit(550,barrel.Bottle ,[&](IUnit* m) {if (m != nullptr) Q->CastOnTarget(m); });
									//pDelay->Add(875, [&]() {E->CastOnPosition(pos2); });
									pDelay->AddPosition(875,pos2 ,[&](Vec3 m) {E->CastOnPosition(m); });
									LastCondition = GGame->TickCount() + 875;
									return;
								}
							}
						}
					}
					for (Barrel nbarrel : nbarrels.ToVector())
					{
						if (Distance(nbarrel.Bottle, pred) <= 330 + 660 + 660 && !(Distance(nbarrel.Bottle, pred) <= 330 + 660) && Estack()>=2)
						{
							for (int i = 330 + 660 + 660; i >= 380 + 660; i -= 20)
							{
								if(Distance(nbarrel.Bottle, pred) <= i )
								{
									Vec3 pos1 = Extend(nbarrel.Bottle->GetPosition(), pred, 660);
									Vec3 pos2 = Extend(nbarrel.Bottle->GetPosition(), pred, i - 330);
									if (InSpellRange(E, pos1) && InSpellRange(E, pos2) && !GNavMesh->IsPointWall(pos1) && !GNavMesh->IsPointWall(pos2))
									{
										GOrbwalking->SetAttacksAllowed(false);
										GOrbwalking->SetMovementAllowed(false);
										pDelay->Add(100 + GGame->Latency() + 875, [&]()
										{GOrbwalking->SetAttacksAllowed(true); GOrbwalking->SetMovementAllowed(true); });
										E->CastOnPosition(pos1);
										//pDelay->Add(550, [&]() {if (barrel.Bottle != nullptr)Q->CastOnTarget(barrel.Bottle); });
										pDelay->AddIUnit(550, barrel.Bottle, [&](IUnit* m) {if (m != nullptr) Q->CastOnTarget(m); });
										//pDelay->Add(875, [&]() {E->CastOnPosition(pos2); });
										pDelay->AddPosition(875, pos2, [&](Vec3 m) {E->CastOnPosition(m); });
										LastCondition = GGame->TickCount() + 875;
										return;
									}
								}
							}
						}
					}
				}
			}
		}
		for (IUnit* hero : ValidEnemies().ToVector())
		{
			Vec3 pred;
			GPrediction->GetFutureUnitPosition(hero, 0.5f, true, pred);
			if (GOrbwalking->CanAttack() && E->IsReady())
			{
				for (Barrel barrel : AttackableBarrels(350).ToVector())
				{
					SArray<Barrel> nbarrels = ChainedBarrels(barrel);
					if (nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, pred) <= 990; })
						&& !nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, pred) <= 330; }))
					{
						for (int i = 990; i >= 400; i -= 20)
						{
							SArray<Barrel> mbarrels = nbarrels.Where([&](Barrel x) {return Distance(x.Bottle, pred) <= i; })
								.OrderBy<float>([&](Barrel x2) {return Distance(x2.Bottle, pred); });
							for (Barrel mbarrel : mbarrels.ToVector())
							{
								Vec3 pos = Extend(mbarrel.Bottle->GetPosition(), pred, i - 330);
								if (Distance(Player(), pos) <= ERange)
								{
									GOrbwalking->SetAttacksAllowed(false);
									GOrbwalking->SetMovementAllowed(false);
									pDelay->Add(100 + GGame->Latency(), [&]()
									{GOrbwalking->SetAttacksAllowed(true); GOrbwalking->SetMovementAllowed(true); });
									E->CastOnPosition(pos);
									LastCondition = GGame->TickCount();
									return;
								}
							}
						}
					}
				}
				for (Barrel barrel : AttackableBarrels().ToVector())
				{
					//// choi mot luc 3 thung
					//SArray<Barrel> nbarrels = ChainedBarrels(barrel);
					//if (Distance(barrel.Bottle, pred) <= 330 + 660 + 660 && !(Distance(barrel.Bottle, pred) <= 330 + 660) && Estack() >= 2)
					//{
					//	for (int i = 330 + 660 + 660; i >= 380 + 660; i -= 20)
					//	{
					//		if (Distance(barrel.Bottle, pred) <= i)
					//		{
					//			Vec3 pos1 = Extend(barrel.Bottle->GetPosition(), pred, 660);
					//			Vec3 pos2 = Extend(barrel.Bottle->GetPosition(), pred, i - 330);
					//			if (InSpellRange(E, pos1) && InSpellRange(E, pos2) && !GNavMesh->IsPointWall(pos1) && !GNavMesh->IsPointWall(pos2))
					//			{
					//				GOrbwalking->SetAttacksAllowed(false);
					//				GOrbwalking->SetMovementAllowed(false);
					//				pDelay->Add(100 + GGame->Latency() + 875, [&]()
					//				{GOrbwalking->SetAttacksAllowed(true); GOrbwalking->SetMovementAllowed(true); });
					//				E->CastOnPosition(pos1);
					//				//pDelay->Add(550, [&]() {if (barrel.Bottle != nullptr)GGame->IssueOrder(Player(), kAttackUnit, barrel.Bottle); });
					//				pDelay->AddIUnit(550, barrel.Bottle,[&](IUnit* m) {if (m != nullptr)GGame->IssueOrder(Player(), kAttackUnit, m); });
					//				//pDelay->Add(875, [&]() {E->CastOnPosition(pos2); });
					//				pDelay->AddPosition(775, pos2, [&](Vec3 m) {E->CastOnPosition(m); });
					//				LastCondition = GGame->TickCount() + 875;
					//				return;
					//			}
					//		}
					//	}
					//}
					//for (Barrel nbarrel : nbarrels.ToVector())
					//{
					//	if (Distance(nbarrel.Bottle, pred) <= 330 + 660 + 660 && !(Distance(nbarrel.Bottle, pred) <= 330 + 660) && Estack() >= 2)
					//	{
					//		for (int i = 330 + 660 + 660; i >= 380 + 660; i -= 20)
					//		{
					//			if (Distance(nbarrel.Bottle, pred) <= i)
					//			{
					//				Vec3 pos1 = Extend(nbarrel.Bottle->GetPosition(), pred, 660);
					//				Vec3 pos2 = Extend(nbarrel.Bottle->GetPosition(), pred, i - 330);
					//				if (InSpellRange(E, pos1) && InSpellRange(E, pos2) && !GNavMesh->IsPointWall(pos1) && !GNavMesh->IsPointWall(pos2))
					//				{
					//					GOrbwalking->SetAttacksAllowed(false);
					//					GOrbwalking->SetMovementAllowed(false);
					//					pDelay->Add(100 + GGame->Latency() + 875, [&]()
					//					{GOrbwalking->SetAttacksAllowed(true); GOrbwalking->SetMovementAllowed(true); });
					//					E->CastOnPosition(pos1);
					//					//pDelay->Add(550, [&]() {if (barrel.Bottle != nullptr) GGame->IssueOrder(Player(), kAttackUnit, barrel.Bottle); });
					//					pDelay->AddIUnit(550, barrel.Bottle, [&](IUnit* m) {if (m != nullptr)GGame->IssueOrder(Player(), kAttackUnit, m); });
					//					//pDelay->Add(875, [&]() {E->CastOnPosition(pos2); });
					//					pDelay->AddPosition(775, pos2, [&](Vec3 m) {E->CastOnPosition(m); });
					//					LastCondition = GGame->TickCount() + 875;
					//					return;
					//				}
					//			}
					//		}
					//	}
					//}
				}
			}
		}
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
	if (Estack() >= 2 && E->IsReady() && ComboE1->Enabled())
	{
		IUnit* target = SelectTarget(PhysicalDamage, ERange);
		if (IsValidTarget(target))
		{
			Vec3 pred;
			GPrediction->GetFutureUnitPosition(target, 0.5f, true, pred);
			if (!Barrels.Any([&](Barrel x) {return Distance(x.Bottle, pred) <= 660; }))
				E->CastOnPosition(pred);
		}
	}
	if (Q->IsReady())
	{
		IUnit* target = SelectTarget(PhysicalDamage, QRange);
		if (IsValidTarget(target))
		{
			bool useQ = true;
			for (Barrel barrel : QableBarrels(1500).ToVector())
			{
				SArray<Barrel> nbarrels = ChainedBarrels(barrel);
				if (E->IsReady() 
					&& nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, target->GetPosition()) <= 660 + target->BoundingRadius(); })
					&& !nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, target->GetPosition()) <= 330 + target->BoundingRadius(); }))
				{
					useQ = false;
					break;
				}
				else if (nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, target->GetPosition()) <= 330 + target->BoundingRadius(); }))
				{
					useQ = false;
					break;
				}
			}
			if (ComboQSave->Enabled())
			{
				for (Barrel barrel : QableBarrels(10000).ToVector())
				{
					SArray<Barrel> nbarrels = ChainedBarrels(barrel);
					for (IUnit* hero : ValidEnemies().ToVector())
					{
						if (nbarrels.Any([&](Barrel x) {return Distance(x.Bottle, hero->GetPosition()) <= 330 + hero->BoundingRadius(); }))
						{
							useQ = false;
							break;
						}
					}
					if (useQ == false)
						break;
				}
			}
			if (useQ)
			{
				Q->CastOnTarget(target);
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
#pragma once
#include "Variables.h"

inline bool IsDaggerFixed(IUnit* target)
{
	return

		PickableDaggers().Any([&](KatarinaDagger i)
	{ return
			Distance(i.Dagger->GetPosition(), target->GetPosition()) <= 450;
	});
}

inline KatarinaDagger GetFixedDagger(IUnit* target)
{
	return
		PickableDaggers()
		.Where([&](KatarinaDagger i)
	{return
			Distance(i.Dagger->GetPosition(), target->GetPosition()) <= 450;
	})
		.MinOrDefault<float>([&](KatarinaDagger i2)
	{return
			Distance(i2.Dagger, target);
	});
}

inline void CastEFixedDagger(KatarinaDagger dagger, IUnit* target)
{
	auto distance = Distance(dagger.Dagger->GetPosition(), target->GetPosition());
	Vec3 castpos;
	if (distance > 150)
		castpos = Extend(dagger.Dagger->GetPosition(), target->GetPosition(), 150);
	else

		castpos = Extend(dagger.Dagger->GetPosition(), target->GetPosition(), 50);

	E->CastOnPosition(castpos);
}
inline double GetPassiveDamage(IUnit* target)
{
	int nhantu = Player()->GetLevel() < 6 ? 0 :

		(Player()->GetLevel() < 11 ? 1 :

			(Player()->GetLevel() < 16 ? 2 : 3));

	auto raw = vector<double>({ 75, 80, 87, 94, 102, 111, 120, 131, 143, 155, 168, 183, 198, 214, 231, 248, 267, 287 }).at(Player()->GetLevel() - 1)

		+ Player()->PhysicalDamageMod()

		+ vector<double>({ 0.55, 0.70, 0.85, 1 }).at(nhantu) * Player()->TotalMagicDamage();

	return GDamage->CalcMagicDamage(Player(), target, raw);
}
inline double GetQDamage(IUnit* target)
{
	return GDamage->GetSpellDamage(Player(), target, kSlotQ);
}
inline double GetEDamage(IUnit* target)
{
	return GDamage->GetSpellDamage(Player(), target, kSlotE);
}
//public static double GetQDamage(Obj_AI_Base target)
//
//{
//
//	if (Q.Level == 0)
//
//		return 0;
//
//	var raw = new double[] { 75, 105, 135, 165, 195 }[Q.Level - 1] + 0.3 * Player.TotalMagicalDamage;
//
//	return Player.CalcDamage(target, Damage.DamageType.Magical, raw);
//
//}
//
//public static double GetEDamage(Obj_AI_Base target)
//
//{
//
//	if (E.Level == 0)
//
//		return 0;
//
//	var raw = new double[] { 30, 45, 60, 75, 90 }[E.Level - 1] + 0.25 * Player.TotalMagicalDamage + 0.65 * Player.TotalAttackDamage;
//
//	return Player.CalcDamage(target, Damage.DamageType.Magical, raw);
//
//}
//
//public static double GetRDamge(Obj_AI_Base target)
//
//{
//
//	if (R.Level == 0)
//
//		return 0;
//
//	var raw = new double[] {375, 562.5, 750}[R.Level - 1] + 2.85 * Player.TotalMagicalDamage + 3.30 * Player.TotalAttackDamage;
//
//	return Player.CalcDamage(target, Damage.DamageType.Magical, raw);
//
//}
inline SArray <IUnit*> GetEVinasun()
{
	SArray <IUnit*> Vinasun;
	SArray <IUnit*> Minions =
		Minions.AddRange(GEntityList->GetAllMinions(true, true, true)).Where([](IUnit* i) -> bool
	{
		return
			i != nullptr && !IsWard(i) && Distance(Player(), i) <= ERange && !i->IsDead();
	});

	SArray <IUnit*> Heroes =
		Heroes.AddRange(GEntityList->GetAllHeros(true, true)).Where([](IUnit* i) -> bool
	{
		return
			i != nullptr && i->IsTargetable() && Distance(Player(), i) <= ERange && !i->IsDead();
	});

	SArray <IUnit*> Dao =
		Dao.AddRange(Daggers
			.Select<IUnit*>([&](KatarinaDagger i)
	{
		return
			i.Dagger;
	})).Where([&](IUnit* i2)
	{
		return
			i2 != nullptr && Distance(Player(), i2) <= ERange;
	});

	Vinasun.AddRange(Minions).AddRange(Heroes).AddRange(Dao);
	return Vinasun;
}

inline SArray <IUnit*> GetQVinasun()
{
	SArray <IUnit*> Vinasun;
	SArray <IUnit*> Minions = Minions.AddRange(GEntityList->GetAllMinions(true, true, true)).Where([&](IUnit* i)
	{
		return
			!IsWard(i) && Distance(Player(), i) <= QRange && !i->IsDead();
	});

	SArray <IUnit*> Heroes = Heroes.AddRange(GEntityList->GetAllHeros(true, true)).Where([&](IUnit* i)
	{
		return
			i != nullptr && i->IsTargetable() && Distance(Player(), i) <= QRange && !i->IsDead();
	});
	Vinasun.AddRange(Minions).AddRange(Heroes);
	return Vinasun;
}
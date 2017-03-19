#pragma once
#include "PluginSDK.h"
#include "Template.h"
using namespace std;

#pragma region extension
//linqq ne`
template<class T>
vector<T> Add(vector<T> vec, T i)
{
	vector<T> newvec;
	newvec = vec;
	newvec.push_back(i);
	return newvec;
}
template<class T>
vector<T> AddRange(vector<T> vec, vector<T> vecToAdd)
{
	vector<T> newvec;
	newvec = vec;
	newvec.insert(newvec.end(), vecToAdd.begin(), vecToAdd.end());
	return newvec;
}
template<class T>
vector<T> RemoveAll(vector<T> vec, function<bool(T)> removefunc)
{
	vector<T> newvec;
	for each (T i in vec)
	{
		if (!removefunc(i))
			newvec.push_back(i);
	}
	return newvec;
}
template<class T>
vector<T> Where(vector<T> vec, function<bool(T)> wherefunc)
{
	vector<T> newvec;
	for each (T i in vec)
	{
		if (wherefunc(i))
			newvec.push_back(i);
	}
	return newvec;
}
template<class T>
bool Any(vector<T> vec, function<bool(T)> anyfunc)
{
	vector<T> newvec;
	newvec = vec;
	newvec = Where(newvec, anyfunc);
	return !newvec.empty();
}
template<class T>
bool Any(vector<T> vec)
{
	vector<T> newvec;
	newvec = vec;
	return !newvec.empty();
}
template<class T, class T2>
vector<T2> Select(vector<T> vec, function<T2(T)> selectfunc)
{
	vector<T> newvec;
	newvec = vec;
	vector<T2> returnvec;
	for each (T i in newvec)
	{
		returnvec.push_back(selectfunc(i));
	}
	return returnvec;
}
template<class T>
T FirstOrDefault(vector<T> vec, function<bool(T)> fst_function)
{
	vector<T> newvec;
	newvec = vec;

	newvec = Where(newvec, fst_function);
	if (newvec.empty())
		return T();
	return newvec.front();
}
template<class T>
T LastOrDefault(vector<T> vec, function<bool(T)> lst_function)
{
	vector<T> newvec;
	newvec = vec;

	newvec = Where(newvec, lst_function);
	if (newvec.empty())
		return T();
	return newvec.back();
}
template<class T, class T2>
T MinOrDefault(vector<T> vec, function<T2(T)> min_function)
{
	vector<T> newvec;
	newvec = vec;
	T returnelem;
	returnelem = T();
	if (newvec.empty())
		return returnelem;
	returnelem = newvec.begin();
	for each (T i in newvec)
	{
		if (min_function(i) < min_function(returnelem))
			returnelem = i;
	}
	return returnelem;
}
template<class T, class T2>
T MaxOrDefault(vector<T> vec, function<T2(T)> max_function)
{
	vector<T> newvec;
	newvec = vec;
	T returnelem;
	returnelem = T();
	if (newvec.empty())
		return returnelem;
	returnelem = newvec.begin();
	for each (T i in newvec)
	{
		if (max_function(i) < max_function(returnelem))
			returnelem = i;
	}
	return returnelem;
}
// keydown
inline bool IsKeyDown(IMenuOption* menuOption)
{
	return (GetAsyncKeyState(menuOption->GetInteger()) & 0x8000) != 0;
}
//select target
inline IUnit* SelectTarget(eDamageType type, float range)
{
	return GTargetSelector->FindTarget(QuickestKill, type, range);
}
// get selected
inline IUnit* GetSelectedTarget()
{
	return GTargetSelector->GetFocusedTarget();
}
//String viet thuong
inline string ToLower(string StringToLower)
{
	auto Lowered = StringToLower;
	for (int i = 0; i < Lowered[i]; i++)
	{
		Lowered[i] = tolower(Lowered[i]);
	}
	return Lowered;
}
// string bao gom string khac
inline bool Contains(string Container, string Contained)
{
	auto LoweredContainer = ToLower(Container);
	auto LoweredContained = ToLower(Contained);
	if (LoweredContainer.find(LoweredContained) != string::npos) return true;
	else return false;
}
//isward
inline bool IsWard(IUnit* target)
{
	Vec2 pos;
	return Contains(target->GetObjectName(), "ward") && !Contains(target->GetObjectName(), "corpse");
}
//lay khoang cach ne may ban
inline Vec2 ToVec2(Vec3 vec)
{
	return Vec2(vec.x, vec.z);
}
inline Vec3 ToVec3(Vec2 vec)
{
	return Vec3(vec.x, 0, vec.y);
}
inline float Distance(Vec3 from, Vec3 to)
{
	return (from - to).Length2D();
}
inline float Distance(IUnit* from, IUnit* to)
{
	return (from->GetPosition() - to->GetPosition()).Length2D();
}
inline float Distance(IUnit* from, Vec3 to)
{
	return (from->GetPosition() - to).Length2D();
}
inline float Distance(Vec2 from, Vec2 to)
{
	return (from - to).Length();
}
//dich chuyen ex- vector tend
inline Vec3 Extend(Vec3 from, Vec3 to, float distance)
{
	float realDistance = (from - to).Length() * distance / (from - to).Length2D();
	auto direction = (to - from).VectorNormalize();
	return from + direction * realDistance;
}
inline int CountEnemiesInRange(Vec3 Position, float Range)
{
	vector<IUnit*> a;
	a = SArray<IUnit*>(GEntityList->GetAllHeros(false, true)).Where([&](IUnit* i) { return
		i != nullptr && !i->IsDead() && Distance(i->GetPosition(), Position) <= Range; }).ToVector();
	return a.size();
}

inline SArray<IUnit*> ValidTargets(vector<IUnit*> input)
{
	SArray<IUnit*> targets = SArray<IUnit*>(input);
	return  targets.Where([](IUnit* i) {return i != nullptr && i->IsValidTarget(); });
}
inline SArray<IUnit*> ValidAllies(vector<IUnit*> input)
{
	SArray<IUnit*> targets = SArray<IUnit*>(input);
	return  targets.Where([](IUnit* i) {return i != nullptr && !i->IsDead(); });
}
inline bool IsValidTarget(IUnit* target, float range = 100000)
{
	return target != nullptr && GEntityList->Player()->IsValidTarget(target, range) && Distance(GEntityList->Player(), target) <= range;
}
inline bool IsValidAllies(IUnit* target, float range = 100000)
{
	return target != nullptr && !target->IsDead() && Distance(GEntityList->Player(), target) <= range;
}
inline bool IsValidBoth(IUnit* target, float range = 100000)
{
	return (target->IsEnemy(GEntityList->Player()) && IsValidTarget(target, range))
		|| (!target->IsEnemy(GEntityList->Player()) && IsValidAllies(target, range));
}
inline SArray<IUnit*> EnemyMinions(float range = 100000)
{
	return SArray<IUnit*>(GEntityList->GetAllMinions(false, true, false)).Where([&](IUnit* i) {return IsValidTarget(i, range) && !IsWard(i) && !Contains(i->GetObjectName(),"barrel"); });
}
inline SArray<IUnit*> AllyMinions(float range = 100000)
{
	return SArray<IUnit*>(GEntityList->GetAllMinions(true, false, false)).Where([&](IUnit* i) {return IsValidAllies(i, range) && !IsWard(i) && !Contains(i->GetObjectName(), "barrel"); });
}
inline SArray<IUnit*> NeutralMinions(float range = 100000)
{
	return SArray<IUnit*>(GEntityList->GetAllMinions(false, false, true)).Where([&](IUnit* i) {return IsValidTarget(i, range) && !IsWard(i) && !Contains(i->GetObjectName(), "barrel"); });
}
inline SArray<IUnit*> AllMinions(float range = 100000)
{
	return SArray<IUnit*>(GEntityList->GetAllMinions(true, true, true)).Where([&](IUnit* i) {return IsValidBoth(i, range) && !IsWard(i) && !Contains(i->GetObjectName(), "barrel"); });
}
inline SArray<IUnit*> WardMinions(float range = 100000, bool jungle = true, bool ally = true, bool enemy = true)
{
	SArray<IUnit*> allwards = SArray<IUnit*>(GEntityList->GetAllMinions(true, true, true))
		.Where([&](IUnit* i) {return IsValidBoth(i, range) && IsWard(i); });
	SArray<IUnit*> junglewards = allwards.Where([](IUnit* i) {return i->IsJungleCreep(); });
	SArray<IUnit*> enemywards = allwards.Where([](IUnit* i) {return GEntityList->Player()->IsEnemy(i); });
	SArray<IUnit*> allywards = allwards.Where([](IUnit* i) {return !GEntityList->Player()->IsEnemy(i); });
	SArray<IUnit*> returnwards;
	if (jungle) returnwards.AddRange(junglewards);
	if (ally) returnwards.AddRange(allywards);
	if (enemy) returnwards.AddRange(enemywards);
	return returnwards;
}
inline SArray<IUnit*> ValidEnemies(float range = 100000)
{
	return SArray<IUnit*>(GEntityList->GetAllHeros(false, true)).Where([&](IUnit* i) {return IsValidTarget(i, range); });
}
inline SArray<IUnit*> ValidAllies(float range = 100000)
{
	return SArray<IUnit*>(GEntityList->GetAllHeros(true, false)).Where([&](IUnit* i) {return IsValidAllies(i, range); });
}
inline SArray<IUnit*> ValidAllHeroes(float range = 100000)
{
	return SArray<IUnit*>(GEntityList->GetAllHeros(true, true)).Where([&](IUnit* i) {return IsValidBoth(i, range); });
}
inline bool InAutoAttackRange(IUnit* target)
{
	return Distance(GEntityList->Player(), target) <= GEntityList->Player()->BoundingRadius() + GEntityList->Player()->AttackRange() + target->BoundingRadius();
}
inline bool InSpellRange(ISpell2* spell, IUnit* target)
{
	return Distance(GEntityList->Player(), target) <= spell->Range();
}
inline bool InSpellRange(ISpell2* spell, Vec3 position)
{
	return Distance(GEntityList->Player(), position) <= spell->Range();
}
inline double GetSpellDamage(IUnit* target, int slot)
{
	return GDamage->GetSpellDamage(GEntityList->Player(), target, slot);
}
inline bool HasSummonerSpell(ISpell*  spell)
{
	return spell->GetSpellSlot() != kSlotUnknown && spell->IsReady();
}
inline void CastItemOnUnit(int itemid, float range, IUnit* target)
{
	if (GEntityList->Player()->HasItemId(itemid))
	{
		IInventoryItem* Item = GPluginSDK->CreateItemForId(itemid, range);
		if (IsValidTarget(target) && Item->IsReady())
		{
			Item->CastOnTarget(target);
		}
	}
}
inline Vec4 Red() { return Vec4(255, 0, 0, 255); }
inline Vec4 Green() { return Vec4(0, 255, 0, 255); }
inline Vec4 Pink() { return Vec4(255, 0, 255, 255); }
inline Vec4 Yellow() { return Vec4(255, 255, 0, 255); }

#pragma endregion 

#pragma  region  Delayaction
struct Action
{
	Action(int time, std::function<void()> callback)
	{
		Time = time + GGame->TickCount();
		CallbackObject = callback;
	}

	void Call()
	{
		CallbackObject();
	}

	std::function<void()> CallbackObject;
	int Time;

};
struct ActionIUnit
{
	ActionIUnit(int time, IUnit* unit, std::function<void(IUnit*)> callback)
	{
		Time = time + GGame->TickCount();
		CallbackObject = callback;
		Unit = unit;
	}

	void Call()
	{
		if (Unit != nullptr)
			CallbackObject(Unit);
	}

	std::function<void(IUnit*)> CallbackObject;
	int Time;
	IUnit* Unit;
};
struct ActionPosition
{
	ActionPosition(int time, Vec3 position, std::function<void(Vec3)> callback)
	{
		Time = time + GGame->TickCount();
		CallbackObject = callback;
		Position = position;
	}

	void Call()
	{
		CallbackObject(Position);
	}

	std::function<void(Vec3)> CallbackObject;
	int Time;
	Vec3 Position;
};

class DelayAction
{
public:
	void OnGameUpdate()
	{
		Actions.erase(std::remove_if(Actions.begin(), Actions.end(), [](Action& Args)
		{
			if (GGame->TickCount() >= Args.Time)
			{
				Args.Call();
				return true;
			}

			return false;
		}), Actions.end());
		ActionsIunit.erase(std::remove_if(ActionsIunit.begin(), ActionsIunit.end(), [](ActionIUnit& Args)
		{
			if (GGame->TickCount() >= Args.Time)
			{
				Args.Call();
				return true;
			}

			return false;
		}), ActionsIunit.end());
		ActionsPosition.erase(std::remove_if(ActionsPosition.begin(), ActionsPosition.end(), [](ActionPosition& Args)
		{
			if (GGame->TickCount() >= Args.Time)
			{
				Args.Call();
				return true;
			}

			return false;
		}), ActionsPosition.end());
	}

	void Add(int Time, std::function<void()> Callback)
	{
		Actions.emplace_back(Time, Callback);
	}
	void AddIUnit(int Time, IUnit* Unit, std::function<void(IUnit*)> Callback)
	{
		ActionsIunit.emplace_back(Time, Unit, Callback);
	}
	void AddPosition(int Time, Vec3 Position, std::function<void(Vec3)> Callback)
	{
		ActionsPosition.emplace_back(Time, Position, Callback);
	}
private:
	std::vector<Action> Actions;
	std::vector<ActionIUnit> ActionsIunit;
	std::vector<ActionPosition> ActionsPosition;
};
#pragma endregion
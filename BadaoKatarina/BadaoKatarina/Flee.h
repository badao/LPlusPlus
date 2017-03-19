#pragma once
#include "Helper.h"

inline void FleeOnUpdate()
{
	if (IsKeyDown(FLeeKey))
	{
		GGame->IssueOrder(Player(), kMoveTo, GGame->CursorPosition());
		if (E->IsReady())
		{
			IUnit* nearest = GetEVinasun().MinOrDefault<float>([](IUnit *i) {return Distance(i, GGame->CursorPosition()); });
			if (nearest != nullptr && Distance(nearest, GGame->CursorPosition()) < Distance(Player(),GGame->CursorPosition()) - 300)
			{
				Vec3 pos = Extend(nearest->GetPosition(), GGame->CursorPosition(), 150);
				E->CastOnPosition(pos);
			}
		}
		if (W->IsReady())
		{
			W->CastOnPlayer();
		}
	}
}

PLUGIN_EVENT(void) FleeEventOnGameUpdate()
{
	FleeOnUpdate();
}
void FleeOnload()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, FleeEventOnGameUpdate);
}
void FleeOnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, FleeEventOnGameUpdate);
}
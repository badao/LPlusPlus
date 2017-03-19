#pragma once
#include "Variables.h"

inline void MenuAndSpell()
{


	Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, true, kCollidesWithYasuoWall );
	Q->SetOverrideRange(QRange);
	W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, true, kCollidesWithNothing);
	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, false, kCollidesWithNothing);
	E->SetOverrideDelay(ERange);
	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, false, kCollidesWithNothing);
	R->SetOverrideRange(100000);

	MainMenu = GPluginSDK->AddMenu("BadaoGangplank");

	ComboMenu = MainMenu->AddMenu("Combo Settings");
	ComboE1 = ComboMenu->CheckBox("Plast 1st barrel", true);
	ComboQSave = ComboMenu->CheckBox("Save Q if can detonate barrels", false);

	HarassMenu = MainMenu->AddMenu("Harass Settings");
	HarassQ = HarassMenu->CheckBox("HarassQ ", true);

	FarmMenu = MainMenu->AddMenu("Farm Settings");

	LaneQ = FarmMenu->CheckBox("LaneClear Q lasthit", true);
	JungleQ = FarmMenu->CheckBox("Jungle Q lasthit", true);
	LastHitQ = FarmMenu->CheckBox("LastHit Q", true);

	AutoMenu = MainMenu->AddMenu("Auto Settings");
	AutoWLowHealth = AutoMenu->CheckBox("Auto W Low Health", true);
	AutoWLowHealthValue = AutoMenu->AddInteger("Health %",0,100,20);
	AutoWCC = AutoMenu->CheckBox("Auto W on CC-ed", true);
	AutoUltKillable = AutoMenu->CheckBox("Auto R On Killable", true);
	AutoUltKillableRange = AutoMenu->AddInteger("Min R range", 0, 100000, 1000);

	Flee = MainMenu->AddMenu("Flee");
	FLeeKey = Flee->AddKey("Flee Key", 'G');
}

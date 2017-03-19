#pragma once
#include "Variables.h"

inline void MenuAndSpell()
{


	Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, true, kCollidesWithYasuoWall);
	Q->SetOverrideRange(QRange);
	W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, true, kCollidesWithNothing);
	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, false, kCollidesWithNothing);
	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, false, kCollidesWithMinions);

	MainMenu = GPluginSDK->AddMenu("BadaoKatarina");

	ComboMenu = MainMenu->AddMenu("Combo Settings");
	ComboCancelRForKS = ComboMenu->CheckBox("Cancel R for KS", true);
	ComboCancelRNoTarget = ComboMenu->CheckBox("Cancel R if no target", true);
	ComboDontAttack = ComboMenu->CheckBox("Block AA if near a dagger", true);
	ComboDontAttackRange = ComboMenu->AddInteger("Range", 0, 200, 150);

	HarassMenu = MainMenu->AddMenu("Harass Settings");
	//HarassWE = HarassMenu->CheckBox("W-E spin", false);
	HarassE = HarassMenu->CheckBox("W", true);
	HarassW = HarassMenu->CheckBox("E", true);

	FarmMenu = MainMenu->AddMenu("Farm Settings");

	LaneClearQ = FarmMenu->CheckBox("LaneClear Q", true);
	LaneClearW = FarmMenu->CheckBox("LaneClear W", true);

	LastHitQ = FarmMenu->CheckBox("LastHit Q", true);

	AutoMenu = MainMenu->AddMenu("Auto Settings");
	AutoKs = AutoMenu->CheckBox("Auto Ks", true);
	AutoQ = AutoMenu->CheckBox("Auto Q", true);

	FleeAndWallJump = MainMenu->AddMenu("Flee & WallJump Settings");
	FLeeKey = FleeAndWallJump->AddKey("Flee Key", 'G');
	//JumpKey = FleeAndWallJump->AddKey("WallJump Key", 'H');
}

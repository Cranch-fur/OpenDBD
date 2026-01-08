#pragma once

#include "CoreMinimal.h"
#include "EOfferingEffectType.generated.h"

UENUM()
enum class EOfferingEffectType : int64
{
	SlasherPointsAll = 16640,
	PointsBrutality,
	PointsDeviouness,
	PointsSacrice,
	PointsHunting,
	CamperPointsAll,
	PointsObjective,
	PointsSurvival,
	PointsAltruism,
	PointsBoldness,
	CamperItemLostPrevention,
	KillerItemLostPrevention,
	Luck,
	IndustrialThemeSelectionModifier,
	JunkyardThemeSelectionModifier,
	FarmThemeSelectionModifier,
	SwampThemeSelectionModifier,
	MapModifier,
	ChestCountModifier,
	PortableHookCountModifier,
	LairCountModifier,
	PlayerGrouping,
	FarKiller,
	KillAllowedModifier,
	LightingModification,
	FogModification,
	LivingWorldObjectCountModifier,
	LivingWorldObjectMultModifier,
	KillLastSurvivor,
	SuburbsThemeSelectionModifier,
	AsylumThemeSelectionModifier,
	KillerSelectionModifier,
	HospitalThemeSelectionModifier,
	KillAllowedAfterHook,
	BorealThemeSelectionModifier,
	ZeroEntry = 0,
};

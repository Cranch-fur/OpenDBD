#pragma once

#include "CoreMinimal.h"
#include "EGameplayModifierSource.generated.h"

UENUM()
enum class EGameplayModifierSource : int64
{
	VE_Perk = 16896,
	VE_StatusEffect,
	VE_Item,
	VE_ItemAddon,
	VE_All,
	VE_PerkAndStatusEffect,
	VE_PerkStatusAndAddon,
	ZeroEntry = 0,
};

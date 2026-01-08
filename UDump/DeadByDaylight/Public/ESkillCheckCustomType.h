#pragma once

#include "CoreMinimal.h"
#include "ESkillCheckCustomType.generated.h"

UENUM()
enum class ESkillCheckCustomType : int64
{
	VE_None = 140694803818752,
	VE_OnPickedUp,
	VE_OnAttacked,
	VE_DecisiveStrikeWhileWiggling,
	VE_GeneratorOvercharge1,
	VE_GeneratorOvercharge2,
	VE_GeneratorOvercharge3,
	VE_Insane,
	ZeroEntry = 0,
};

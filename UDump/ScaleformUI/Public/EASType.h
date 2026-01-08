#pragma once

#include "CoreMinimal.h"
#include "EASType.generated.h"

UENUM(BlueprintType)
namespace EASType
{
	enum Type
	{
		Undefined = 4294967296,
		Null,
		Boolean,
		Int,
		Number,
		String,
		StringW,
		Object,
		DisplayObject,
		ZeroEntry = 0,
	};
}

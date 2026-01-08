#pragma once

#include "CoreMinimal.h"
#include "ENewContentType.generated.h"

UENUM()
enum class ENewContentType : int64
{
	COMING_SOON = 17153,
	NEW_CONTENT,
	PATCH_NOTES,
	DEV_MESSAGES,
	EVENTS,
	ZeroEntry = 0,
};

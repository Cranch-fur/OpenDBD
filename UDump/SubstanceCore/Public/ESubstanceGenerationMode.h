#pragma once

#include "CoreMinimal.h"
#include "ESubstanceGenerationMode.generated.h"

UENUM(BlueprintType)
enum ESubstanceGenerationMode
{
	SGM_PlatformDefault = 4294967296,
	SGM_Baked,
	SGM_OnLoadSync,
	SGM_OnLoadSyncAndCache,
	SGM_OnLoadAsync,
	SGM_OnLoadAsyncAndCache,
	ZeroEntry = 0,
};

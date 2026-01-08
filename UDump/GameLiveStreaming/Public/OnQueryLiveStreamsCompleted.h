#pragma once

#include "CoreMinimal.h"
#include "BlueprintLiveStreamInfo.h"
#include "OnQueryLiveStreamsCompleted.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQueryLiveStreamsCompleted, const TArray<FBlueprintLiveStreamInfo>&, LiveStreams, bool, bWasSuccessful);

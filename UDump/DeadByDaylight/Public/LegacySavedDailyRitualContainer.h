#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LegacySavedDailyRitualInstance.h"
#include "LegacySavedDailyRitualContainer.generated.h"

USTRUCT(BlueprintType)
struct FLegacySavedDailyRitualContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, SaveGame)
	FDateTime LastRitualReceivedDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLegacySavedDailyRitualInstance> Rituals;

public:
	DEADBYDAYLIGHT_API FLegacySavedDailyRitualContainer();
};

FORCEINLINE uint32 GetTypeHash(const FLegacySavedDailyRitualContainer) { return 0; }

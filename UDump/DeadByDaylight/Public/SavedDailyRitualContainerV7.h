#pragma once

#include "CoreMinimal.h"
#include "LegacySavedDailyRitualInstance.h"
#include "SavedDailyRitualContainerV7.generated.h"

USTRUCT(BlueprintType)
struct FSavedDailyRitualContainerV7
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString LastRitualReceivedDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FLegacySavedDailyRitualInstance> Rituals;

public:
	DEADBYDAYLIGHT_API FSavedDailyRitualContainerV7();
};

FORCEINLINE uint32 GetTypeHash(const FSavedDailyRitualContainerV7) { return 0; }

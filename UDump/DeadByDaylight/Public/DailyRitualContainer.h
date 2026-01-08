#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DailyRitualInstance.h"
#include "DailyRitualContainer.generated.h"

USTRUCT(BlueprintType)
struct FDailyRitualContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FDateTime LastRitualReceivedDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDailyRitualInstance> Rituals;

public:
	DEADBYDAYLIGHT_API FDailyRitualContainer();
};

FORCEINLINE uint32 GetTypeHash(const FDailyRitualContainer) { return 0; }

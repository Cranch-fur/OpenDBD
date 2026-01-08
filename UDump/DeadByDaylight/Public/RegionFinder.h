#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Region.h"
#include "RegionFinder.generated.h"

UCLASS(Blueprintable, Transient)
class DEADBYDAYLIGHT_API URegionFinder : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRegion> Regions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TIMEOUT;

public:
	URegionFinder();
};

FORCEINLINE uint32 GetTypeHash(const URegionFinder) { return 0; }

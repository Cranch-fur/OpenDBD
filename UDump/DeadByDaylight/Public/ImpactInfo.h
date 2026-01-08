#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ImpactInfo.generated.h"

class AActor;
class UPrimitiveComponent;

USTRUCT(BlueprintType)
struct FImpactInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UPrimitiveComponent* Component;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SurfaceName;

public:
	DEADBYDAYLIGHT_API FImpactInfo();
};

FORCEINLINE uint32 GetTypeHash(const FImpactInfo) { return 0; }

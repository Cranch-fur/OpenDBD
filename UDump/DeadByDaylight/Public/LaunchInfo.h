#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LaunchInfo.generated.h"

USTRUCT(BlueprintType)
struct FLaunchInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

public:
	DEADBYDAYLIGHT_API FLaunchInfo();
};

FORCEINLINE uint32 GetTypeHash(const FLaunchInfo) { return 0; }

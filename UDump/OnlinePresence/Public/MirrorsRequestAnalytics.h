#pragma once

#include "CoreMinimal.h"
#include "MirrorsRequestAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FMirrorsRequestAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CallSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResultCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Version;

public:
	ONLINEPRESENCE_API FMirrorsRequestAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FMirrorsRequestAnalytics) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "BloodWebRingPersistentData.h"
#include "BloodWebPersistentData.generated.h"

USTRUCT(BlueprintType)
struct FBloodWebPersistentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 VersionNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBloodWebRingPersistentData> RingData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Paths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EntityCurrentNode;

public:
	DEADBYDAYLIGHT_API FBloodWebPersistentData();
};

FORCEINLINE uint32 GetTypeHash(const FBloodWebPersistentData) { return 0; }

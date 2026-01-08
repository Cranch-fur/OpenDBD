#pragma once

#include "CoreMinimal.h"
#include "PlayerLoadoutData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerLoadoutData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ItemAddOns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> CamperPerks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> CamperPerkLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CamperFavor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> PowerAddOns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> SlasherPerks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SlasherPerkLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SlasherFavor;

public:
	DEADBYDAYLIGHT_API FPlayerLoadoutData();
};

FORCEINLINE uint32 GetTypeHash(const FPlayerLoadoutData) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "EPlayerRole.h"
#include "LoadOutAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FLoadOutAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LoadoutItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LoadoutItemAddOn1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LoadoutItemAddOn2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LoadoutPerk1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LoadoutPerk2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LoadoutPerk3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LoadoutPerk4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LoadoutOffering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FLoadOutAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FLoadOutAnalytics) { return 0; }

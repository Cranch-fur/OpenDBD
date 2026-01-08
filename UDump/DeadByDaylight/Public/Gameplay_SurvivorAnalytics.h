#pragma once

#include "CoreMinimal.h"
#include "Gameplay_SurvivorAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FGameplay_SurvivorAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InjuredSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InjuredDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthyDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KODuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PalletSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PalletProcedural;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PalletGeneric;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PalletDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PalletStun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UnhookCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealCountSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ClosetEnter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Hatch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillCheckCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillCheckCountGood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillCheckCountGreat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FGameplay_SurvivorAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FGameplay_SurvivorAnalytics) { return 0; }

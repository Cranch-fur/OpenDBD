#pragma once

#include "CoreMinimal.h"
#include "ESurvivorGrouping.h"
#include "Dependency.h"
#include "GeneratedLevelData.generated.h"

class USceneComponent;

USTRUCT(BlueprintType)
struct FGeneratedLevelData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UsedPaperTileMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AvailableEscapeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AvailableSurvivorItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESurvivorGrouping GroupingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	USceneComponent* KillerSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USceneComponent*> SurvivorSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USceneComponent*> InteractableElementsSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USceneComponent*> MeatHookSpawners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USceneComponent*> SearchableSpawners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USceneComponent*> HexSpawners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USceneComponent*> KillerLairSpawners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDependency> LevelDependencies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseForcedMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GenerationLogs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GenerationErrors;

public:
	DEADBYDAYLIGHT_API FGeneratedLevelData();
};

FORCEINLINE uint32 GetTypeHash(const FGeneratedLevelData) { return 0; }

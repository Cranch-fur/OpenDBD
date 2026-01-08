#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParadiseData.h"
#include "LevelParadise.generated.h"

class USceneComponent;
class ADBDPlayer;

UCLASS(Blueprintable)
class ALevelParadise : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoopStartDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoopDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	USceneComponent* MainPlayerSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USceneComponent*> OtherSurvivorSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USceneComponent*> ParadiseTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* ParadiseViewTarget;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ADBDPlayer* _spawnedMainPawn;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	USceneComponent* _runnerComponent;

public:
	UFUNCTION(BlueprintCallable)
	void ParadiseEntered(const FParadiseData& PlayerData);

private:
	UFUNCTION(BlueprintCallable)
	void OnInterpolationDone();

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EnterParadise(const FParadiseData& PlayerData);

public:
	ALevelParadise();
};

FORCEINLINE uint32 GetTypeHash(const ALevelParadise) { return 0; }

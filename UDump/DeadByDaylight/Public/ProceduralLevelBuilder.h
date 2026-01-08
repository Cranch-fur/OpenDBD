#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"
#include "GeneratedLevelData.h"
#include "Dependency.h"
#include "UObject/NoExportTypes.h"
#include "FILE_NOT_FOUND"
#include "SpawnPointPopulation.h"
#include "ActorSpawnerPopulation.h"
#include "ETileSpawnPointType.h"
#include "GenerationParams.h"
#include "ProceduralLevelBuilder.generated.h"

class UPaperTileMap;
class ATile;
class UProceduralGenerationData;
class UMapData;
class UObjectLibrary;
class UTileMatrix;
class UTileBank;
class UActorSpawner;
class AProceduralLevelData;
class ADBDClientSyncer;

UCLASS(Blueprintable)
class AProceduralLevelBuilder : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PathToMapDirectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PathToTilesDirectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KillerMinProximityFromCamper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SurvivorMinProximityFromOtherCamper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinProximityBetweenGenerators;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SurvivorMinProximityFromGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SurvivorMinProximityFromTotem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPaperTileMap> DebugMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ATile>> DebugTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SmallMeatLockerMinProximityFromKillerLair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SmallMeatLockerMinProximityFromEachOther;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralGenerationData* ProceduralGenerationData;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FGeneratedLevelData _generatedData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<FDependency> _escapeDependencies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UClass* _tileClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FRandomStream _proceduralRandomizationStream;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UMapData* _mapData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UObjectLibrary* _availableMapsLibrary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UObjectLibrary* _availableTilesLibrary;

	UPROPERTY(EditAnywhere)
	FStreamableManager _streamable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UObjectLibrary* _availableDerivedTileLibrary;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UTileMatrix* _tileMatrix;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UTileBank* _tileBank;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<uint8, FSpawnPointPopulation> _availableSpawnPoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<uint8, FActorSpawnerPopulation> _availableActorSpawners;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<UActorSpawner*> _edgeObjects;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UActorSpawner* _killerLairSpawner;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FStringAssetReference> _itemAssetReferences;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<ATile*> _tilesThatHaveBeenSpawned;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	AProceduralLevelData* _debugProceduralDatas;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	ADBDClientSyncer* _syncer;

private:
	UFUNCTION(BlueprintCallable)
	void SyncSeeds();

public:
	UFUNCTION(BlueprintCallable)
	AActor* SpawnActor(UClass* ActorClass, ETileSpawnPointType spawnPointType);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OfferingAdjustFog(float Modifier);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_DebugShareGenerationData(FGenerationParams usedParams);

	UFUNCTION(BlueprintCallable)
	void BuildFromSeed();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AProceduralLevelBuilder();
};

FORCEINLINE uint32 GetTypeHash(const AProceduralLevelBuilder) { return 0; }

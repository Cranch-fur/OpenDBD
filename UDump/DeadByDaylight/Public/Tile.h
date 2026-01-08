#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "EscapeStrategyType.h"
#include "ETileType.h"
#include "EDensity.h"
#include "EPathType.h"
#include "Tile.generated.h"

class UActorComponent;
class UTileSpawnPoint;

UCLASS(Blueprintable)
class ATile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinimumSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaximumSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaximumCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Dimension;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EscapeStrategyType> AvailableEscapeTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDensity Density;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPathType Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PlaceHolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TestTile;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	TArray<UActorComponent*> SpawnPointsCache;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	TArray<UActorComponent*> ObjectRandomizersCache;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	TArray<UActorComponent*> ActorSpawnersCache;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	TArray<UActorComponent*> ActorVariationSpawnersCache;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_Initialized, Transient)
	bool _initialized;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TypeSpacing;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSetSpawnObject(UTileSpawnPoint* TileSpawnPoint, AActor* spawnedObject);

protected:
	UFUNCTION(BlueprintCallable)
	void OnRep_Initialized(bool OldValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InitOnSpawned();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ATile();
};

FORCEINLINE uint32 GetTypeHash(const ATile) { return 0; }

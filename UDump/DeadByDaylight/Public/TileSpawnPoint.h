#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ETileSpawnPointType.h"
#include "Templates/SubclassOf.h"
#include "TileSpawnPoint.generated.h"

class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UTileSpawnPoint : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileSpawnPointType TileSpawnPointType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> ObjectSpawnModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> Visualization;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_SpawnObject)
	AActor* _spawnedObject;

protected:
	UFUNCTION(BlueprintCallable)
	void OnRep_SpawnObject();

public:
	UFUNCTION(BlueprintPure)
	AActor* GetSpawnedObject() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UTileSpawnPoint();
};

FORCEINLINE uint32 GetTypeHash(const UTileSpawnPoint) { return 0; }

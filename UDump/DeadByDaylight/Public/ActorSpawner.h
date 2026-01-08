#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EGameplayElementType.h"
#include "ActorSpawnerProperties.h"
#include "ActorSpawnedDelegate.h"
#include "ActorSpawner.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UActorSpawner : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameplayElementType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> Visualization;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ActivatedByDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FActorSpawnerProperties> ActivatedSceneElement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FActorSpawnerProperties> DeactivatedSceneElement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCountMax;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FActorSpawnedDelegate OnAsyncActorSpawned;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UClass* _toSpawn;

public:
	UFUNCTION(BlueprintCallable)
	void EditorForceSpawnVisualization();

	UFUNCTION(BlueprintCallable)
	void Authority_SpawnActorAsyncEvent();

public:
	UActorSpawner();
};

FORCEINLINE uint32 GetTypeHash(const UActorSpawner) { return 0; }

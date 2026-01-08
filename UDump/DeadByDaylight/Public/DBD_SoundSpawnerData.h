#pragma once

#include "CoreMinimal.h"
#include "DBD_SoundSpawnerData.generated.h"

USTRUCT(BlueprintType)
struct FDBD_SoundSpawnerData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TAssetSubclassOf<UObject>> TriggerObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TriggerVisualForCamper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TriggerVisualForSlasher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TAssetSubclassOf<AActor>> ActorToSpawnVisual;

public:
	DEADBYDAYLIGHT_API FDBD_SoundSpawnerData();
};

FORCEINLINE uint32 GetTypeHash(const FDBD_SoundSpawnerData) { return 0; }

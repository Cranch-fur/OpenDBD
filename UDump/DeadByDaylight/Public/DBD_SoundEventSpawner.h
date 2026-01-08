#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DBD_SoundSpawnerData.h"
#include "Templates/SubclassOf.h"
#include "UObject/NoExportTypes.h"
#include "DBD_SoundEventSpawner.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ADBD_SoundEventSpawner : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDBD_SoundSpawnerData> SoundDescriptor;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<TSubclassOf<AActor>> _loadedSoundAssets;

protected:
	UFUNCTION(BlueprintCallable)
	void OnSoundTriggered(AActor* originator, FVector Location, bool shouldTrack, float audibleRange);

	UFUNCTION(BlueprintCallable)
	void AssetsLoaded();

public:
	ADBD_SoundEventSpawner();
};

FORCEINLINE uint32 GetTypeHash(const ADBD_SoundEventSpawner) { return 0; }

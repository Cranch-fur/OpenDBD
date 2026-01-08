#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapMeshToAkAudioEvent.h"
#include "MapSoundsToAvoid.h"
#include "Engine/HitResult.h"
#include "DBD_SoundClipUtil.generated.h"

class UStaticMesh;
class UAkAudioEvent;
class UStaticMeshComponent;
class UPrimitiveComponent;
class AActor;

UCLASS(Blueprintable, Abstract, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBD_SoundClipUtil : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapMeshToAkAudioEvent> AudioMap;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TMap<UStaticMesh*, FMapMeshToAkAudioEvent> _audioEventMap;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<FMapSoundsToAvoid> _objectsWhoHavePlayedASound;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TMap<UAkAudioEvent*, float> _nextSoundPlayTime;

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnGotMeshToTriggerSound(UStaticMeshComponent* usm);

	UFUNCTION()
	void OnCharacterEnterSoundRegion(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	UAkAudioEvent* GetAudioEventForMesh(UStaticMeshComponent* usm);

public:
	UDBD_SoundClipUtil();
};

FORCEINLINE uint32 GetTypeHash(const UDBD_SoundClipUtil) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AkAmbientSound.generated.h"

class UAkAudioEvent;
class UAkComponent;

UCLASS(Blueprintable)
class AKAUDIO_API AAkAmbientSound : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UAkAudioEvent* AkAudioEvent_DEPRECATED;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UAkComponent* AkComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopWhenOwnerIsDestroyed;

public:
	UFUNCTION(BlueprintCallable)
	void StopAmbientSound();

	UFUNCTION(BlueprintCallable)
	void StartAmbientSound();

public:
	AAkAmbientSound();
};

FORCEINLINE uint32 GetTypeHash(const AAkAmbientSound) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AkComponent.generated.h"

class UAkAudioEvent;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class AKAUDIO_API UAkComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StopWhenOwnerDestroyed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttenuationScalingFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OcclusionRefreshInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldTransmitRelativeAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OcclusionAffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAkAudioEvent* AkAudioEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventName;

public:
	UFUNCTION(BlueprintCallable)
	void UseReverbVolumes(bool inUseReverbVolumes);

	UFUNCTION(BlueprintCallable)
	void Stop();

	UFUNCTION(BlueprintCallable)
	void SetSwitch(const FString& SwitchGroup, const FString& SwitchState);

	UFUNCTION(BlueprintCallable)
	void SetStopWhenOwnerDestroyed(bool bStopWhenOwnerDestroyed);

	UFUNCTION(BlueprintCallable)
	void SetRTPCValue(const FString& RTPC, float Value, int32 InterpolationTimeMs);

	UFUNCTION(BlueprintCallable)
	void SetOutputBusVolume(float BusVolume);

	UFUNCTION(BlueprintCallable)
	void SetAttenuationScalingFactor(float Value);

	UFUNCTION(BlueprintCallable)
	void SetActiveListeners(int32 in_uListenerMask);

	UFUNCTION(BlueprintCallable)
	void PostTrigger(const FString& Trigger);

	UFUNCTION(BlueprintCallable)
	int32 PostAssociatedAkEvent();

	UFUNCTION(BlueprintCallable)
	int32 PostAkEventByName(const FString& in_EventName);

	UFUNCTION(BlueprintCallable)
	int32 PostAkEvent(UAkAudioEvent* AkEvent, const FString& in_EventName);

	UFUNCTION(BlueprintPure)
	float GetAttenuationRadius() const;

public:
	UAkComponent();
};

FORCEINLINE uint32 GetTypeHash(const UAkComponent) { return 0; }

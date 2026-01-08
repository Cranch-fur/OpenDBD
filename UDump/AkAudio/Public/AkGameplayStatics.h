#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "AkGameplayStatics.generated.h"

class AActor;
class UAkAudioBank;
class UObject;
class UAkAudioEvent;
class UAkComponent;
class USceneComponent;

UCLASS(Blueprintable)
class AKAUDIO_API UAkGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void UseReverbVolumes(bool inUseReverbVolumes, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static void UnloadBankByName(const FString& BankName);

	UFUNCTION(BlueprintCallable)
	static void UnloadBank(UAkAudioBank* bank, const FString& BankName);

	UFUNCTION(BlueprintCallable)
	static void StopProfilerCapture();

	UFUNCTION(BlueprintCallable)
	static void StopOutputCapture();

	UFUNCTION(BlueprintCallable)
	static void StopAllAmbientSounds(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void StopAll();

	UFUNCTION(BlueprintCallable)
	static void StopActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static void StartProfilerCapture(const FString& FileName);

	UFUNCTION(BlueprintCallable)
	static void StartOutputCapture(const FString& FileName);

	UFUNCTION(BlueprintCallable)
	static void StartAllAmbientSounds(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static UAkComponent* SpawnAkComponentAtLocation(UObject* WorldContextObject, UAkAudioEvent* AkEvent, FVector Location, FRotator Orientation, bool AutoPost, const FString& EventName, bool AutoDestroy);

	UFUNCTION(BlueprintCallable)
	static void SetSwitch(FName SwitchGroup, FName SwitchState, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static void SetState(FName StateGroup, FName State);

	UFUNCTION(BlueprintCallable)
	static void SetRTPCValue(FName RTPC, float Value, int32 InterpolationTimeMs, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static void SetOutputBusVolume(float BusVolume, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static void SetOcclusionRefreshInterval(float RefreshInterval, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static void PostTrigger(FName Trigger, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static void PostEventByName(const FString& EventName, AActor* Actor, bool bStopWhenAttachedToDestroyed);

	UFUNCTION(BlueprintCallable)
	static int32 PostEventAttached(UAkAudioEvent* AkEvent, AActor* Actor, FName attachPointName, bool bStopWhenAttachedToDestroyed, const FString& EventName);

	UFUNCTION(BlueprintCallable)
	static void PostEventAtLocationByName(const FString& EventName, FVector Location, FRotator Orientation, UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static int32 PostEventAtLocation(UAkAudioEvent* AkEvent, FVector Location, FRotator Orientation, const FString& EventName, UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static int32 PostEvent(UAkAudioEvent* AkEvent, AActor* Actor, bool bStopWhenAttachedToDestroyed, const FString& EventName);

	UFUNCTION(BlueprintCallable)
	static void LoadInitBank();

	UFUNCTION(BlueprintCallable)
	static void LoadBanks(const TArray<UAkAudioBank*>& SoundBanks, bool SynchronizeSoundBanks);

	UFUNCTION(BlueprintCallable)
	static void LoadBankByName(const FString& BankName);

	UFUNCTION(BlueprintCallable)
	static void LoadBank(UAkAudioBank* bank, const FString& BankName);

	UFUNCTION(BlueprintCallable)
	static UAkComponent* GetAkComponent(USceneComponent* AttachToComponent, FName attachPointName, FVector Location, EAttachLocation::Type LocationType);

	UFUNCTION(BlueprintCallable)
	static void ClearBanks();

	UFUNCTION(BlueprintCallable)
	static void AddOutputCaptureMarker(const FString& MarkerText);

public:
	UAkGameplayStatics();
};

FORCEINLINE uint32 GetTypeHash(const UAkGameplayStatics) { return 0; }

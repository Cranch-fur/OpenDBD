#include "AkGameplayStatics.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"

class AActor;
class UAkAudioBank;
class UObject;
class UAkComponent;
class UAkAudioEvent;
class USceneComponent;

void UAkGameplayStatics::UseReverbVolumes(bool inUseReverbVolumes, AActor* Actor)
{

}

void UAkGameplayStatics::UnloadBankByName(const FString& BankName)
{

}

void UAkGameplayStatics::UnloadBank(UAkAudioBank* bank, const FString& BankName)
{

}

void UAkGameplayStatics::StopProfilerCapture()
{

}

void UAkGameplayStatics::StopOutputCapture()
{

}

void UAkGameplayStatics::StopAllAmbientSounds(UObject* WorldContextObject)
{

}

void UAkGameplayStatics::StopAll()
{

}

void UAkGameplayStatics::StopActor(AActor* Actor)
{

}

void UAkGameplayStatics::StartProfilerCapture(const FString& FileName)
{

}

void UAkGameplayStatics::StartOutputCapture(const FString& FileName)
{

}

void UAkGameplayStatics::StartAllAmbientSounds(UObject* WorldContextObject)
{

}

UAkComponent* UAkGameplayStatics::SpawnAkComponentAtLocation(UObject* WorldContextObject, UAkAudioEvent* AkEvent, FVector Location, FRotator Orientation, bool AutoPost, const FString& EventName, bool AutoDestroy)
{
	return NULL;
}

void UAkGameplayStatics::SetSwitch(FName SwitchGroup, FName SwitchState, AActor* Actor)
{

}

void UAkGameplayStatics::SetState(FName StateGroup, FName State)
{

}

void UAkGameplayStatics::SetRTPCValue(FName RTPC, float Value, int32 InterpolationTimeMs, AActor* Actor)
{

}

void UAkGameplayStatics::SetOutputBusVolume(float BusVolume, AActor* Actor)
{

}

void UAkGameplayStatics::SetOcclusionRefreshInterval(float RefreshInterval, AActor* Actor)
{

}

void UAkGameplayStatics::PostTrigger(FName Trigger, AActor* Actor)
{

}

void UAkGameplayStatics::PostEventByName(const FString& EventName, AActor* Actor, bool bStopWhenAttachedToDestroyed)
{

}

int32 UAkGameplayStatics::PostEventAttached(UAkAudioEvent* AkEvent, AActor* Actor, FName attachPointName, bool bStopWhenAttachedToDestroyed, const FString& EventName)
{
	return 0;
}

void UAkGameplayStatics::PostEventAtLocationByName(const FString& EventName, FVector Location, FRotator Orientation, UObject* WorldContextObject)
{

}

int32 UAkGameplayStatics::PostEventAtLocation(UAkAudioEvent* AkEvent, FVector Location, FRotator Orientation, const FString& EventName, UObject* WorldContextObject)
{
	return 0;
}

int32 UAkGameplayStatics::PostEvent(UAkAudioEvent* AkEvent, AActor* Actor, bool bStopWhenAttachedToDestroyed, const FString& EventName)
{
	return 0;
}

void UAkGameplayStatics::LoadInitBank()
{

}

void UAkGameplayStatics::LoadBanks(const TArray<UAkAudioBank*>& SoundBanks, bool SynchronizeSoundBanks)
{

}

void UAkGameplayStatics::LoadBankByName(const FString& BankName)
{

}

void UAkGameplayStatics::LoadBank(UAkAudioBank* bank, const FString& BankName)
{

}

UAkComponent* UAkGameplayStatics::GetAkComponent(USceneComponent* AttachToComponent, FName attachPointName, FVector Location, EAttachLocation::Type LocationType)
{
	return NULL;
}

void UAkGameplayStatics::ClearBanks()
{

}

void UAkGameplayStatics::AddOutputCaptureMarker(const FString& MarkerText)
{

}

UAkGameplayStatics::UAkGameplayStatics()
{

}

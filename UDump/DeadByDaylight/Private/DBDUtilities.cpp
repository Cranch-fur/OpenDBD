#include "DBDUtilities.h"
#include "UObject/NoExportTypes.h"
#include "Engine/HitResult.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"

class USphereComponent;
class AActor;
class UObject;
class UMeshComponent;
class UMaterialInterface;
class UAnimMontage;
class UWorld;
class ACamperPlayer;
class ADBDPlayer;
class ADBDPlayerControllerBase;
class ADBDPlayerCameraManager;
class UPrimitiveComponent;
class ADBDGameState;
class ADBDGameMode;
class UDBDGameInstance;
class UActorComponent;
class UAnimSequence;
class UAkAudioBank;

bool UDBDUtilities::WithinRangeExclusive(float Min, float Max, float Value)
{
	return false;
}

bool UDBDUtilities::WithinRange(float Min, float Max, float Value)
{
	return false;
}

bool UDBDUtilities::SphereTraceSingle(const FVector Start, const FVector End, const USphereComponent* Sphere, FHitResult& outHitResult)
{
	return false;
}

bool UDBDUtilities::SphereTraceAgainstActor(const FVector Start, const FVector End, const USphereComponent* Sphere, const AActor* Actor, FHitResult& outHitResult)
{
	return false;
}

int32 UDBDUtilities::PointerToInt(UObject* Obj)
{
	return 0;
}

bool UDBDUtilities::NormalizeXY(FVector& Vector, float Tolerance)
{
	return false;
}

void UDBDUtilities::LogMeshMaterialInfo(const FString& inStr, const UMeshComponent* Root)
{

}

void UDBDUtilities::LogMaterialInfo(const FString& inStr, const UMaterialInterface* Root)
{

}

bool UDBDUtilities::IsLocallyObservingASurvivor(UObject* WorldContextObject)
{
	return false;
}

bool UDBDUtilities::IsLocallyObservingAKiller(UObject* WorldContextObject)
{
	return false;
}

bool UDBDUtilities::HasWalkableSurfaceForward(UObject* WorldContextObject, FVector Location, FVector fowardVector, float Distance, float raycastStartHeight, float raycastEndHeight, TArray<FHitResult>& OutHits, FVector& raycastStart)
{
	return false;
}

bool UDBDUtilities::HasLineOfSightToTarget(UObject* WorldContextObject, FVector StartPosition, FVector targetPosition, AActor* ignoredActor)
{
	return false;
}

bool UDBDUtilities::HasFloorForward(UObject* WorldContextObject, FVector Location, FVector fowardVector, float Distance, float raycastStartHeight, float raycastEndHeight, TArray<FHitResult>& OutHits, FVector& raycastStart)
{
	return false;
}

FVector UDBDUtilities::GetXYVector(const FVector& Vector)
{
	return FVector{};
}

float UDBDUtilities::GetVelocityXYAngleDegrees(const AActor* Actor)
{
	return 0.0f;
}

float UDBDUtilities::GetTunableValue(UObject* WorldContextObject, FName valueName)
{
	return 0.0f;
}

float UDBDUtilities::GetSlasherTunableValue(UObject* WorldContextObject, FName valueName)
{
	return 0.0f;
}

FVector UDBDUtilities::GetSimilarVector(FVector Vector, float maximumDeviation)
{
	return FVector{};
}

float UDBDUtilities::GetPlayRateForMontage(UAnimMontage* Montage, float Time)
{
	return 0.0f;
}

float UDBDUtilities::GetPercentOfRange(const float Val, const float Min, const float Max)
{
	return 0.0f;
}

void UDBDUtilities::GetNearbyCampers(UWorld* World, FVector Center, float Radius, TArray<ACamperPlayer*>& outNearbyCampers, const TArray<AActor*>& ignoredCampers)
{

}

float UDBDUtilities::GetMontageLength(UAnimMontage* Montage)
{
	return 0.0f;
}

ADBDPlayer* UDBDUtilities::GetLocallyObservedCharacter(UObject* WorldContextObject)
{
	return NULL;
}

ADBDPlayer* UDBDUtilities::GetLocallyControlledCharacter(UObject* WorldContextObject)
{
	return NULL;
}

ADBDPlayerControllerBase* UDBDUtilities::GetLocalDBDPlayerController(UObject* WorldContextObject)
{
	return NULL;
}

ADBDPlayerCameraManager* UDBDUtilities::GetLocalDBDCameraManager(UObject* WorldContextObject)
{
	return NULL;
}

FCollisionResponseContainer UDBDUtilities::GetImpactCollisionResponseContainer(const UPrimitiveComponent* Primitive)
{
	return FCollisionResponseContainer{};
}

ECollisionChannel UDBDUtilities::GetImpactCollisionObjectType(const UPrimitiveComponent* Primitive)
{
	return ECC_WorldStatic;
}

FText UDBDUtilities::GetFormattedNumberText(float Value, int32 maxDecimals, int32 minDecimals, bool leadingZero)
{
	return FText::GetEmpty();
}

FString UDBDUtilities::GetFormattedNumberString(float Value, int32 maxDecimals, int32 minDecimals, bool leadingZero)
{
	return TEXT("");
}

FVector UDBDUtilities::GetFloorLocationAt(UObject* WorldContextObject, FVector Location, float raycastStartHeight, float raycastEndHeight)
{
	return FVector{};
}

AActor* UDBDUtilities::GetFirstActorWithTag(FName Tag, TArray<AActor*> actorsArray)
{
	return NULL;
}

ADBDGameState* UDBDUtilities::GetDBDGameState(UObject* WorldContextObject)
{
	return NULL;
}

ADBDGameMode* UDBDUtilities::GetDBDGameMode(UObject* WorldContextObject)
{
	return NULL;
}

UDBDGameInstance* UDBDUtilities::GetDBDGameInstance(UObject* WorldContextObject)
{
	return NULL;
}

FString UDBDUtilities::GetComponentOwnerName(UActorComponent* ActorComponent)
{
	return TEXT("");
}

FTransform UDBDUtilities::GetClosestSnapTransformOnOrbit(const FVector& centerPoint, const FVector& orbitterPosition, const float Distance)
{
	return FTransform{};
}

FVector UDBDUtilities::GetClosestPointOnOrbit(const FVector& centerPoint, const FVector& orbitterPosition, const float Distance)
{
	return FVector{};
}

float UDBDUtilities::GetBadgeTunableValue(UObject* WorldContextObject, FName valueName)
{
	return 0.0f;
}

float UDBDUtilities::GetAnimSequenceLength(UAnimSequence* Sequence)
{
	return 0.0f;
}

FString UDBDUtilities::GetActorOwnerName(AActor* Actor)
{
	return TEXT("");
}

AActor* UDBDUtilities::FindParentOfClass(AActor* InChild, UClass* InClass)
{
	return NULL;
}

void UDBDUtilities::DBDLoadAudioBankAsync(UAkAudioBank* bank, AActor* caller)
{

}

void UDBDUtilities::DBDLoadAudioBank(UAkAudioBank* bank, AActor* caller)
{

}

bool UDBDUtilities::DBDCapsuleTraceSingle(UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, ECollisionChannel TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf)
{
	return false;
}

void UDBDUtilities::CrashGameCausingInterrupt()
{

}

UActorComponent* UDBDUtilities::AddComponentToActor(UClass* compClass, FName compName, AActor* Actor)
{
	return NULL;
}

UDBDUtilities::UDBDUtilities()
{

}

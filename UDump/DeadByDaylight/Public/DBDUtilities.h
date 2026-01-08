#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/NoExportTypes.h"
#include "Engine/HitResult.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DBDUtilities.generated.h"

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

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UDBDUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static bool WithinRangeExclusive(float Min, float Max, float Value);

	UFUNCTION(BlueprintPure)
	static bool WithinRange(float Min, float Max, float Value);

	UFUNCTION(BlueprintPure)
	static bool SphereTraceSingle(const FVector Start, const FVector End, const USphereComponent* Sphere, FHitResult& outHitResult);

	UFUNCTION(BlueprintPure)
	static bool SphereTraceAgainstActor(const FVector Start, const FVector End, const USphereComponent* Sphere, const AActor* Actor, FHitResult& outHitResult);

	UFUNCTION(BlueprintPure)
	static int32 PointerToInt(UObject* Obj);

	UFUNCTION(BlueprintCallable)
	static bool NormalizeXY(FVector& Vector, float Tolerance);

	UFUNCTION(BlueprintCallable)
	static void LogMeshMaterialInfo(const FString& inStr, const UMeshComponent* Root);

	UFUNCTION(BlueprintCallable)
	static void LogMaterialInfo(const FString& inStr, const UMaterialInterface* Root);

	UFUNCTION(BlueprintPure)
	static bool IsLocallyObservingASurvivor(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static bool IsLocallyObservingAKiller(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static bool HasWalkableSurfaceForward(UObject* WorldContextObject, FVector Location, FVector fowardVector, float Distance, float raycastStartHeight, float raycastEndHeight, TArray<FHitResult>& OutHits, FVector& raycastStart);

	UFUNCTION(BlueprintPure)
	static bool HasLineOfSightToTarget(UObject* WorldContextObject, FVector StartPosition, FVector targetPosition, AActor* ignoredActor);

	UFUNCTION(BlueprintCallable)
	static bool HasFloorForward(UObject* WorldContextObject, FVector Location, FVector fowardVector, float Distance, float raycastStartHeight, float raycastEndHeight, TArray<FHitResult>& OutHits, FVector& raycastStart);

	UFUNCTION(BlueprintCallable)
	static FVector GetXYVector(const FVector& Vector);

	UFUNCTION(BlueprintCallable)
	static float GetVelocityXYAngleDegrees(const AActor* Actor);

	UFUNCTION(BlueprintPure)
	static float GetTunableValue(UObject* WorldContextObject, FName valueName);

	UFUNCTION(BlueprintPure)
	static float GetSlasherTunableValue(UObject* WorldContextObject, FName valueName);

	UFUNCTION(BlueprintPure)
	static FVector GetSimilarVector(FVector Vector, float maximumDeviation);

	UFUNCTION(BlueprintPure)
	static float GetPlayRateForMontage(UAnimMontage* Montage, float Time);

	UFUNCTION(BlueprintCallable)
	static float GetPercentOfRange(const float Val, const float Min, const float Max);

	UFUNCTION(BlueprintCallable)
	static void GetNearbyCampers(UWorld* World, FVector Center, float Radius, TArray<ACamperPlayer*>& outNearbyCampers, const TArray<AActor*>& ignoredCampers);

	UFUNCTION(BlueprintPure)
	static float GetMontageLength(UAnimMontage* Montage);

	UFUNCTION(BlueprintPure)
	static ADBDPlayer* GetLocallyObservedCharacter(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static ADBDPlayer* GetLocallyControlledCharacter(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static ADBDPlayerControllerBase* GetLocalDBDPlayerController(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static ADBDPlayerCameraManager* GetLocalDBDCameraManager(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static FCollisionResponseContainer GetImpactCollisionResponseContainer(const UPrimitiveComponent* Primitive);

	UFUNCTION(BlueprintPure)
	static ECollisionChannel GetImpactCollisionObjectType(const UPrimitiveComponent* Primitive);

	UFUNCTION(BlueprintCallable)
	static FText GetFormattedNumberText(float Value, int32 maxDecimals, int32 minDecimals, bool leadingZero);

	UFUNCTION(BlueprintCallable)
	static FString GetFormattedNumberString(float Value, int32 maxDecimals, int32 minDecimals, bool leadingZero);

	UFUNCTION(BlueprintPure)
	static FVector GetFloorLocationAt(UObject* WorldContextObject, FVector Location, float raycastStartHeight, float raycastEndHeight);

	UFUNCTION(BlueprintCallable)
	static AActor* GetFirstActorWithTag(FName Tag, TArray<AActor*> actorsArray);

	UFUNCTION(BlueprintPure)
	static ADBDGameState* GetDBDGameState(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static ADBDGameMode* GetDBDGameMode(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static UDBDGameInstance* GetDBDGameInstance(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static FString GetComponentOwnerName(UActorComponent* ActorComponent);

	UFUNCTION(BlueprintCallable)
	static FTransform GetClosestSnapTransformOnOrbit(const FVector& centerPoint, const FVector& orbitterPosition, const float Distance);

	UFUNCTION(BlueprintCallable)
	static FVector GetClosestPointOnOrbit(const FVector& centerPoint, const FVector& orbitterPosition, const float Distance);

	UFUNCTION(BlueprintPure)
	static float GetBadgeTunableValue(UObject* WorldContextObject, FName valueName);

	UFUNCTION(BlueprintPure)
	static float GetAnimSequenceLength(UAnimSequence* Sequence);

	UFUNCTION(BlueprintCallable)
	static FString GetActorOwnerName(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static AActor* FindParentOfClass(AActor* InChild, UClass* InClass);

	UFUNCTION(BlueprintCallable)
	static void DBDLoadAudioBankAsync(UAkAudioBank* bank, AActor* caller);

	UFUNCTION(BlueprintCallable)
	static void DBDLoadAudioBank(UAkAudioBank* bank, AActor* caller);

	UFUNCTION(BlueprintCallable)
	static bool DBDCapsuleTraceSingle(UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, ECollisionChannel TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf);

	UFUNCTION(BlueprintCallable)
	static void CrashGameCausingInterrupt();

	UFUNCTION(BlueprintCallable)
	static UActorComponent* AddComponentToActor(UClass* compClass, FName compName, AActor* Actor);

public:
	UDBDUtilities();
};

FORCEINLINE uint32 GetTypeHash(const UDBDUtilities) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "Engine/HitResult.h"
#include "LaunchInfo.h"
#include "ImpactInfo.h"
#include "Engine/EngineTypes.h"
#include "BaseProjectile.generated.h"

class UBaseProjectileLauncher;
class USphereComponent;
class UPrimitiveComponent;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ProjectileLauncher, Export, meta=(AllowPrivateAccess=true))
	TWeakObjectPtr<UBaseProjectileLauncher> _projectileLauncher;

public:
	UFUNCTION(BlueprintPure)
	bool SphereTraceSingle(FVector Start, FVector End, USphereComponent* Sphere, FHitResult& outHitResult) const;

	UFUNCTION(BlueprintCallable)
	void SetActive(bool Active);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSetActive(bool Active);

private:
	UFUNCTION(BlueprintCallable)
	void OnRep_ProjectileLauncher();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLaunch(FLaunchInfo LaunchInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInitialize(UBaseProjectileLauncher* projectileLauncher);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDetectPlayer(FImpactInfo ImpactInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDetectCollision(FImpactInfo ImpactInfo);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_Launch(FLaunchInfo LaunchInfo);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_DetectPlayer(FImpactInfo ImpactInfo);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Multicast_DetectImpact(FImpactInfo ImpactInfo);

public:
	UFUNCTION(BlueprintPure)
	bool LineTraceSingle(FVector Start, FVector End, FHitResult& outHitResult) const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsValidPlayerDetection(FImpactInfo ImpactInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsValidImpactDetection(FImpactInfo ImpactInfo);

public:
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool IsAvailable() const;

	UFUNCTION(BlueprintPure)
	bool IsActive() const;

	UFUNCTION(BlueprintPure)
	UBaseProjectileLauncher* GetProjectileLauncher() const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UPrimitiveComponent* GetImpactPrimitiveComponent() const;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FCollisionResponseContainer GetImpactCollisionResponseContainer() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ECollisionChannel GetImpactCollisionObjectType() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool Authority_TryDetectPlayer(FImpactInfo ImpactInfo);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool Authority_TryDetectCollision(FImpactInfo ImpactInfo, bool Force);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_Launch(FLaunchInfo LaunchInfo);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ABaseProjectile();
};

FORCEINLINE uint32 GetTypeHash(const ABaseProjectile) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Templates/SubclassOf.h"
#include "LaunchInfo.h"
#include "UObject/NoExportTypes.h"
#include "BaseProjectileLauncher.generated.h"

class ABaseProjectile;
class ADBDPlayer;
class AActor;

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UBaseProjectileLauncher : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PoolSize;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	TArray<ABaseProjectile*> _projectiles;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ABaseProjectile* _debugProjectile;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetProjectileVar(FName VarName, float Value);

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnReload(int32 ammoNew);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLaunch(FLaunchInfo LaunchInfo);

public:
	UFUNCTION(BlueprintCallable)
	void Local_SetMaxAmmo(int32 newMaxAmmo);

	UFUNCTION(BlueprintCallable)
	void Local_Reload(int32 ammoToAdd);

	UFUNCTION(BlueprintCallable)
	void Local_Launch();

	UFUNCTION(BlueprintPure)
	bool IsLocallyControlled() const;

	UFUNCTION(BlueprintPure)
	bool HasProjectile() const;

protected:
	UFUNCTION(BlueprintPure)
	bool HasAuthority() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ABaseProjectile* GetProjectileToLaunch() const;

public:
	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetPlayerOwner() const;

protected:
	UFUNCTION(BlueprintPure)
	ABaseProjectile* GetOldestProjectile() const;

public:
	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmo() const;

protected:
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetLaunchSpeed() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FVector GetLaunchPosition() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FVector GetLaunchDirection() const;

	UFUNCTION(BlueprintPure)
	ABaseProjectile* GetFirstAvailableProjectile() const;

public:
	UFUNCTION(BlueprintPure)
	int32 GetAmmo() const;

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Broadcast_SetMaxAmmo_Server(int8 newMaxAmmo);

	UFUNCTION(NetMulticast, Reliable)
	void Broadcast_SetMaxAmmo_Multicast(int8 newMaxAmmo);

	UFUNCTION(Server, Reliable, WithValidation)
	void Broadcast_Reload_Server(int8 ammoToAdd);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Broadcast_Reload_Multicast(int8 ammoToAdd);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Broadcast_Launch_Server(FLaunchInfo LaunchInfo);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void Broadcast_Launch_Multicast(FLaunchInfo LaunchInfo);

	UFUNCTION(BlueprintCallable)
	void Authority_OnProjectileDestroyed(AActor* DestroyedActor);

	UFUNCTION(BlueprintCallable)
	void Authority_CleanProjectilePool(AActor* DestroyedActor);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UBaseProjectileLauncher();
};

FORCEINLINE uint32 GetTypeHash(const UBaseProjectileLauncher) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraSocketChanged.h"
#include "EDBDCameraSocketID.h"
#include "CameraHandlerComponent.generated.h"

class AActor;
class UParticleSystemComponent;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UCameraHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FCameraSocketChanged OnSocketChanged;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<EDBDCameraSocketID, AActor*> _cameraMap;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	EDBDCameraSocketID _delayedSocketID;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	EDBDCameraSocketID _currentSocketID;

public:
	UFUNCTION(BlueprintCallable)
	void SetSocketID(EDBDCameraSocketID mode);

	UFUNCTION(BlueprintCallable)
	void PlayVFXOnCurrentCamera(UParticleSystemComponent* Component);

	UFUNCTION(BlueprintCallable)
	EDBDCameraSocketID GetSocketID();

	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentCamera();

	UFUNCTION(BlueprintCallable)
	void AddCameraToSocket(EDBDCameraSocketID mode, AActor* Camera);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UCameraHandlerComponent();
};

FORCEINLINE uint32 GetTypeHash(const UCameraHandlerComponent) { return 0; }

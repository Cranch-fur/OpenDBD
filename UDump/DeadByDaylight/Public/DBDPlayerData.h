#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "YawAndPitchRotator_NetQuantize32.h"
#include "UObject/NoExportTypes.h"
#include "DBDPlayerData.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UDBDPlayerData : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool Blinded;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, meta=(AllowPrivateAccess=true))
	FYawAndPitchRotator_NetQuantize32 _controlRotation;

public:
	UFUNCTION(BlueprintCallable)
	void SetLoadoutSpawned();

	UFUNCTION(BlueprintCallable)
	void SetLightSpawned();

	UFUNCTION(BlueprintCallable)
	void SetIntroCompleted();

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetIntroCompleted();

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetControlRotation(FRotator Rotation);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_LoadoutSpawned();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_LightSpawned();

public:
	UFUNCTION(BlueprintCallable)
	bool IsLoadoutSpawned() const;

	UFUNCTION(BlueprintCallable)
	bool IsLightSpawned() const;

	UFUNCTION(BlueprintCallable)
	bool IsIntroCompleted();

	UFUNCTION(BlueprintPure)
	bool GetUseInputPressed() const;

	UFUNCTION(BlueprintPure)
	bool GetLeftRightInputMashed() const;

	UFUNCTION(BlueprintPure)
	bool GetInteractionInputMashed() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UDBDPlayerData();
};

FORCEINLINE uint32 GetTypeHash(const UDBDPlayerData) { return 0; }

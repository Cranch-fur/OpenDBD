#pragma once

#include "CoreMinimal.h"
#include "DBDPlayerControllerBase.h"
#include "OnPawnLeavingGameDelegate.h"
#include "OnLocallyObservedChangedDelegate.h"
#include "DBDTimer.h"
#include "DBDPlayerController.generated.h"

class ANetworkFenceActor;
class ADBDPlayerState;
class ADBDPlayer;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ADBDPlayerController : public ADBDPlayerControllerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnPawnLeavingGameDelegate OnPawnLeavingGame;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnLocallyObservedChangedDelegate OnLocallyObservedChanged;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_Fence, Transient, meta=(AllowPrivateAccess=true))
	ANetworkFenceActor* _theFence;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FDBDTimer _yawInputScalingTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FDBDTimer _pitchInputScalingTimer;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetSpectatorModeInEditor(bool On, ADBDPlayerState* spectatingPlayerState);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetSpectatorMode(bool On);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetReadyToTravel();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_LeaveGame();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_EndGame(bool serverHasLeftTheGame);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_CompleteEscapeRequirements();

private:
	UFUNCTION(BlueprintCallable)
	void OnRep_Fence();

public:
	UFUNCTION(BlueprintPure)
	bool IsSpectating() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayerState* GetSpectatedPlayerState() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetSpectatedPlayer() const;

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetControlledPlayer() const;

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SetSpectatorModeInEditor(bool On, ADBDPlayerState* spectatingPlayerState);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SetSpectatorMode(bool On);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ADBDPlayerController();
};

FORCEINLINE uint32 GetTypeHash(const ADBDPlayerController) { return 0; }

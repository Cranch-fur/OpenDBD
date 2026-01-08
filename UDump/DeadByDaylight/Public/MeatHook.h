#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "MeatHook.generated.h"

class UDBDClipRegionComponent;
class UPollableEventListener;
class ACamperPlayer;
class UMontagePlayer;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AMeatHook : public AInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSurvivorStruggling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WasSurvivorStruggleCancelled;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	TArray<UDBDClipRegionComponent*> ClipRegions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInBasement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBreakable;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UPollableEventListener* _eventListener;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ACamperPlayer* _hookedSurvivor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isSacrificed;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isSabotaged;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _survivorUnhookable;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _canSurvivorAttemptEscape;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _survivorCanStruggle;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _canBeSabotaged;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	float _drainStartTimer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _isIdle;

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnTickStruggle(float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnStruggleExit();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnStruggleEnter();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSacrificeOut();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnSacrificeIn();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnReactionIn();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnReaction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHookingExit();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHookingEnter();

	UFUNCTION(BlueprintCallable)
	void OnHookedSurvivorAttacked();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHookedIdle();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHookedExit();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnHookedEnter();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnEscapeAttemptSuccessful(ACamperPlayer* survivor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnAutoRepair();

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetIsSacrificed(bool Value);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetIsSabotaged(bool Value);

protected:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetHookedSurvivor(ACamperPlayer* survivor);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_EscapeAttemptResult(bool result);

public:
	UFUNCTION(BlueprintPure)
	UMontagePlayer* GetMontagePlayer() const;

	UFUNCTION(BlueprintPure)
	bool GetIsSacrificed() const;

	UFUNCTION(BlueprintPure)
	bool GetIsSabotaged() const;

	UFUNCTION(BlueprintPure)
	bool GetIsBroken() const;

	UFUNCTION(BlueprintPure)
	ACamperPlayer* GetHookedSurvivor() const;

	UFUNCTION(BlueprintPure)
	bool CanUnhookSurvivor(ACamperPlayer* survivor) const;

	UFUNCTION(BlueprintPure)
	bool CanSurvivorStruggle(ACamperPlayer* survivor) const;

	UFUNCTION(BlueprintPure)
	bool CanSurvivorAttemptEscape(ACamperPlayer* survivor) const;

	UFUNCTION(BlueprintPure)
	bool CanHookSurvivor() const;

	UFUNCTION(BlueprintPure)
	bool CanBeSabotaged() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SignalUnhookingEnter();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SignalUnhookingCharged();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SignalUnhookingAborted();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SignalSurvivorAttemptingEscapeCharged();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SignalSurvivorAttemptingEscapeAborted();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SignalSurvivorAttemptingEscape();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SetIsSacrificed(bool Value);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SetIsSabotaged(bool Value);

	UFUNCTION(BlueprintCallable)
	void Authority_SetHookedSurvivor(ACamperPlayer* survivor);

public:
	AMeatHook();
};

FORCEINLINE uint32 GetTypeHash(const AMeatHook) { return 0; }

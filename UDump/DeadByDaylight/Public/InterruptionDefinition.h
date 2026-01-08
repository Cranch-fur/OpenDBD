#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UObject/NoExportTypes.h"
#include "AnimationMontageDescriptor.h"
#include "InterruptionDefinition.generated.h"

class ADBDPlayer;
class UInteractionDefinition;
class AInteractable;

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UInterruptionDefinition : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName InterruptionID;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _usesWorldCoordinates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _snapInterruptor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FTransform _interruptionSnapPointForInterruptor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FAnimationMontageDescriptor _animationMontageToPlayOnInterruptionForInterruptorNew;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _snapInterruptee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FTransform _interruptionSnapPointForInterruptee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FAnimationMontageDescriptor _animationMontageToPlayOnInterruptionForInterrupteeNew;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _pickUpOnInterrupt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _canInterruptWhileCarrying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _useMontageFollower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float _maxHeightDeltaBetweenCharacters;

public:
	UFUNCTION(BlueprintCallable)
	void SetInterruptorSnapPoint(const FTransform& Point);

	UFUNCTION(BlueprintCallable)
	void SetInterrupteeSnapPoint(const FTransform& Point);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInterruptionUpdateStart(ADBDPlayer* interruptor, ADBDPlayer* interruptee);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInterruptionExitEnd(ADBDPlayer* interruptor, ADBDPlayer* interruptee);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInterruptionEnterStart(ADBDPlayer* interruptor, ADBDPlayer* interruptee);

	UFUNCTION(BlueprintPure)
	bool IsUsingMontageFollower() const;

	UFUNCTION(BlueprintPure)
	bool IsUsingAttack() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsInterruptionPossible(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee) const;

	UFUNCTION(BlueprintPure)
	bool InteractionCheckHeightDelta(const ADBDPlayer* interruptor, const ADBDPlayer* interruptee) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FRotator GetInterruptionSnapPointRotationForInterruptor(ADBDPlayer* interruptor, ADBDPlayer* interruptee) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FRotator GetInterruptionSnapPointRotationForInterruptee(ADBDPlayer* interruptor, ADBDPlayer* interruptee) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FVector GetInterruptionSnapPointPositionForInterruptor(ADBDPlayer* interruptor, ADBDPlayer* interruptee) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FVector GetInterruptionSnapPointPositionForInterruptee(ADBDPlayer* interruptor, ADBDPlayer* interruptee) const;

	UFUNCTION(BlueprintPure)
	UInteractionDefinition* GetInteractionDefinition() const;

	UFUNCTION(BlueprintPure)
	AInteractable* GetInteractable() const;

	UFUNCTION(BlueprintPure)
	bool CanInterruptWhileCarrying() const;

public:
	UInterruptionDefinition();
};

FORCEINLINE uint32 GetTypeHash(const UInterruptionDefinition) { return 0; }

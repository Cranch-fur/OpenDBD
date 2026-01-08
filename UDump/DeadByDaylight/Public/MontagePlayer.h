#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MontageInstanceInfo.h"
#include "AnimationMontageDescriptor.h"
#include "UObject/NoExportTypes.h"
#include "MontagePlayer.generated.h"

class UDataTable;
class UAnimMontage;
class UAnimInstance;

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class UMontagePlayer : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* _animationMappingsTable;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UAnimMontage* _currentlyPlayingMontage;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FMontageInstanceInfo> _activeMontagesInfo;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FName _outSectionName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	float _timeBeforeOut;

public:
	UFUNCTION(BlueprintCallable)
	void Stop(float blendOutTime);

	UFUNCTION(BlueprintCallable)
	void SetPlayRate(float PlayRate);

	UFUNCTION(BlueprintCallable)
	void SetAnimInstance(UAnimInstance* AnimInstance);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* PlayScaledForDuration(FAnimationMontageDescriptor animMontageID, float Duration);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* PlayLoopForDuration(FAnimationMontageDescriptor animMontageID, float Duration, float PlayRate, FName Out);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* Play(FAnimationMontageDescriptor animMontageID, float PlayRate, bool isFollower);

private:
	UFUNCTION()
	void OnMontageEndedInternal(FGuid Guid, UAnimMontage* Montage, bool interrupted);

	UFUNCTION()
	void OnMontageBlendingOutInternal(FGuid Guid, UAnimMontage* Montage, bool interrupted);

public:
	UFUNCTION(BlueprintCallable)
	void JumpToSectionEnd(FName sectionName);

	UFUNCTION(BlueprintCallable)
	void JumpToSection(FName sectionName);

	UFUNCTION(BlueprintPure)
	bool IsPlaying(FAnimationMontageDescriptor montageDescriptor) const;

	UFUNCTION(BlueprintCallable)
	UAnimInstance* GetAnimInstance();

public:
	UMontagePlayer();
};

FORCEINLINE uint32 GetTypeHash(const UMontagePlayer) { return 0; }

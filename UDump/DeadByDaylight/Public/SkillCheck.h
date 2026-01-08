#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ESkillCheckCustomType.h"
#include "SkillCheck.generated.h"

class ADBDPlayer;
class UChargeableInteractionDefinition;
class UInteractionDefinition;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class USkillCheck : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProgressRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SuccessZoneStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SuccessZoneEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BonusZoneLength;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	int32 SkillCheckCount;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	int32 xOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float xOffset_max;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	int32 yOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float yOffset_max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InsaneSkillCheckJitterAmplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float kSuccessZoneMinimum;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ADBDPlayer* _owner;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UChargeableInteractionDefinition* _currentInteraction;

public:
	UFUNCTION(BlueprintCallable)
	bool ShouldDeactivateSkillCheckOnInteractionEnd() const;

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetInteraction(UChargeableInteractionDefinition* Interaction);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_OnSkillCheckSuccess(bool Bonus, ESkillCheckCustomType Type);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_OnSkillCheckFailure(bool hadInput, ESkillCheckCustomType Type);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SkillCheckResponse(bool hadInput, bool success, bool Bonus, ESkillCheckCustomType Type);

public:
	UFUNCTION(BlueprintCallable)
	void ActivateSkillCheck(UInteractionDefinition* Interaction, float Delay);

	UFUNCTION(BlueprintCallable)
	void ActivateCustomSkillCheck(ESkillCheckCustomType Type, float Delay);

public:
	USkillCheck();
};

FORCEINLINE uint32 GetTypeHash(const USkillCheck) { return 0; }

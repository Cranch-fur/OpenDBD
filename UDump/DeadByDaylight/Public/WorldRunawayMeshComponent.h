#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "WorldRunawayMeshComponent.generated.h"

UCLASS(Blueprintable, EditInlineNew, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UWorldRunawayMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResetCooldown;

public:
	UFUNCTION(BlueprintCallable)
	void SetShouldRunAwayWithRandomDelay(bool should, float delayRange);

	UFUNCTION(BlueprintCallable)
	void SetShouldRunAwayWithDelay(bool should, float Delay);

	UFUNCTION(BlueprintCallable)
	void SetShouldRunAway(bool should);

	UFUNCTION(BlueprintPure)
	bool GetShouldRunAway() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetRunawayMustTickDuration() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetReturnMustTickDuration() const;

public:
	UWorldRunawayMeshComponent();
};

FORCEINLINE uint32 GetTypeHash(const UWorldRunawayMeshComponent) { return 0; }

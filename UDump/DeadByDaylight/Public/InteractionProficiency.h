#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EStatusEffectType.h"
#include "InteractionProficiency.generated.h"

class UChargeableInteractionDefinition;

UCLASS(Blueprintable)
class UInteractionProficiency : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> InteractionIDs;

protected:
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetValue(const UChargeableInteractionDefinition* chargeableInteraction) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	EStatusEffectType GetType(const UChargeableInteractionDefinition* chargeableInteraction) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	int32 GetLevel(const UChargeableInteractionDefinition* chargeableInteraction) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool GetIsActive(const UChargeableInteractionDefinition* chargeableInteraction) const;

public:
	UInteractionProficiency();
};

FORCEINLINE uint32 GetTypeHash(const UInteractionProficiency) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KeyDisplayInfo.h"
#include "EInputInteractionType.h"
#include "InputCoreTypes.h"
#include "EPlayerRole.h"
#include "DBDKeyDisplayInfo.generated.h"

class UPlayerInput;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UDBDKeyDisplayInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PendingResetKeyBindingsData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PendingResetMustBeGamepadKey;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TMap<FName, FKeyDisplayInfo> _keyDisplayInfoMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TMap<EInputInteractionType, FName> _interactionTypeToActionMap_Camper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TMap<EInputInteractionType, FName> _interactionTypeToActionMap_Slasher;

public:
	UFUNCTION()
	bool IsExcludedKey(FKey Key);

	UFUNCTION(BlueprintCallable)
	FKeyDisplayInfo GetKeyDisplayInfoForInteractionType(UPlayerInput* PlayerInput, EPlayerRole playerRole, EInputInteractionType interactionType, bool useGamePad);

	UFUNCTION()
	FKeyDisplayInfo GetKeyDisplayInfo(FKey Key);

	UFUNCTION()
	FKey GetFirstKeyForAction(UPlayerInput* PlayerInput, FName ActionName, bool useGamePad);

	UFUNCTION(BlueprintCallable)
	FName GetActionForInteractionType(EPlayerRole playerRole, EInputInteractionType inputInteractionType, bool useGamePad, bool ignoreInteractMash);

public:
	UDBDKeyDisplayInfo();
};

FORCEINLINE uint32 GetTypeHash(const UDBDKeyDisplayInfo) { return 0; }

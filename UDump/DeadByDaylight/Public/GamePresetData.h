#pragma once

#include "CoreMinimal.h"
#include "GamePresetData.generated.h"

USTRUCT(BlueprintType)
struct FGamePresetData
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<uint8> _mapAvailabilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<uint8> _perkAvailabilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<uint8> _offeringAvailabilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<uint8> _itemAvailabilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<uint8> _itemAddonAvailabilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<uint8> _customizationItemAvailabilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<uint8> _characterAvailabilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _allowDlcContent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _privateMatch;

public:
	DEADBYDAYLIGHT_API FGamePresetData();
};

FORCEINLINE uint32 GetTypeHash(const FGamePresetData) { return 0; }

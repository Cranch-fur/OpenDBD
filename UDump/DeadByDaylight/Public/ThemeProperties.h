#pragma once

#include "CoreMinimal.h"
#include "EThemeColorId.h"
#include "EOfferingEffectType.h"
#include "GameplayElementSubsitutions.h"
#include "ThemeProperties.generated.h"

USTRUCT(BlueprintType)
struct FThemeProperties
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Weather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TilePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ThemeNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EThemeColorId ThemeColorId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EOfferingEffectType ThemeSelectionOfferingEffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayElementSubsitutions> SubstitutionElements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DlcIDString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DlcIDStringPS4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DlcIDStringXB1;

public:
	DEADBYDAYLIGHT_API FThemeProperties();
};

FORCEINLINE uint32 GetTypeHash(const FThemeProperties) { return 0; }

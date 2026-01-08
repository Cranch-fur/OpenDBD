#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EPlayerRole.h"
#include "CharacterDescription.generated.h"

USTRUCT(BlueprintType)
struct FCharacterDescription : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharacterIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Subtitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText BackStory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Biography;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName IconFilePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RequiredDlcIDString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RequiredDlcIDStringPS4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RequiredDlcIDStringXB1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName IdName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<APawn> GamePawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<APawn> MenuPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DefaultItem;

public:
	DEADBYDAYLIGHT_API FCharacterDescription();
};

FORCEINLINE uint32 GetTypeHash(const FCharacterDescription) { return 0; }

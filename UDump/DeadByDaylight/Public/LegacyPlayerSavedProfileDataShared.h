#pragma once

#include "CoreMinimal.h"
#include "LegacyPlayerSavedProfileDataShared.generated.h"

USTRUCT(BlueprintType)
struct FLegacyPlayerSavedProfileDataShared
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 SelectedCamperIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 SelectedSlasherIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FName> SelectedCharacterCustomization;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 SlasherSkulls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 CamperSkulls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 CamperStreak;

public:
	DEADBYDAYLIGHT_API FLegacyPlayerSavedProfileDataShared();
};

FORCEINLINE uint32 GetTypeHash(const FLegacyPlayerSavedProfileDataShared) { return 0; }

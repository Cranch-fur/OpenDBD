#pragma once

#include "CoreMinimal.h"
#include "PlayerSavedProfileDataShared.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSavedProfileDataShared
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SteamId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelectedCamperIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelectedSlasherIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> SelectedCharacterCustomization;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlasherSkulls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CamperSkulls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CamperStreak;

public:
	DEADBYDAYLIGHT_API FPlayerSavedProfileDataShared();
};

FORCEINLINE uint32 GetTypeHash(const FPlayerSavedProfileDataShared) { return 0; }

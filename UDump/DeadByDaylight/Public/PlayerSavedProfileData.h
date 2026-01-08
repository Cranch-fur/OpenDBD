#pragma once

#include "CoreMinimal.h"
#include "PlayerSavedProfileDataShared.h"
#include "PlayerSavedProfileDataLocal.h"
#include "ConsoleUserSettings.h"
#include "PlayerSavedProfileData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSavedProfileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerUID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerSavedProfileDataShared SharedData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerSavedProfileDataLocal LocalData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FConsoleUserSettings ConsoleUserSettings;

public:
	DEADBYDAYLIGHT_API FPlayerSavedProfileData();
};

FORCEINLINE uint32 GetTypeHash(const FPlayerSavedProfileData) { return 0; }

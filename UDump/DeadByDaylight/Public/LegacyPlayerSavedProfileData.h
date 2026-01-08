#pragma once

#include "CoreMinimal.h"
#include "SaveDataBase.h"
#include "LegacyPlayerSavedProfileDataShared.h"
#include "LegacyPlayerSavedProfileDataLocal.h"
#include "LegacyPlayerSavedProfileData.generated.h"

USTRUCT(BlueprintType)
struct FLegacyPlayerSavedProfileData : public FSaveDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString PlayerUID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacyPlayerSavedProfileDataShared SharedData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLegacyPlayerSavedProfileDataLocal LocalData;

public:
	DEADBYDAYLIGHT_API FLegacyPlayerSavedProfileData();
};

FORCEINLINE uint32 GetTypeHash(const FLegacyPlayerSavedProfileData) { return 0; }

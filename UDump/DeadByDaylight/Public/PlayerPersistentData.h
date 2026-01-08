#pragma once

#include "CoreMinimal.h"
#include "EPlayerRole.h"
#include "PlayerLoadoutData.h"
#include "DailyRitualInstance.h"
#include "PlayerSavedProfileData.h"
#include "PlayerPersistentData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerPersistentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole GameRole;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FPlayerLoadoutData StartingLoadout;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FPlayerLoadoutData CurrentLoadout;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	FDailyRitualInstance CachedAvailableRitual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerSavedProfileData SavedData;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 PreviousSlasherSkulls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 PreviousCamperSkulls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 PreviousExperience;

public:
	DEADBYDAYLIGHT_API FPlayerPersistentData();
};

FORCEINLINE uint32 GetTypeHash(const FPlayerPersistentData) { return 0; }

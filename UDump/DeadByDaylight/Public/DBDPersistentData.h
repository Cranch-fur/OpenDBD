#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GamePersistentData.h"
#include "PlayerPersistentData.h"
#include "CachedPlayerScoreData.h"
#include "CloudInventoryItem.h"
#include "DBDPersistentData.generated.h"

UCLASS(Blueprintable)
class UDBDPersistentData : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FGamePersistentData _gamePersistentData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FString> _keepDataIDs;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<FString, FPlayerPersistentData> _playerIdToPersistentDataMap;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	FPlayerPersistentData _localPlayerPersistentData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<FString, FCachedPlayerScoreData> _cachedPlayerScoreMap;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FCloudInventoryItem> _cloudInventory;

public:
	UDBDPersistentData();
};

FORCEINLINE uint32 GetTypeHash(const UDBDPersistentData) { return 0; }

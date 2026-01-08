#pragma once

#include "CoreMinimal.h"
#include "DBDBadge.h"
#include "EDBDScoreTypes.h"
#include "EBadgeValueStrategy.h"
#include "DBDBadge_AllCamperComparator.generated.h"

class ADBDPlayer;

UCLASS(Blueprintable)
class UDBDBadge_AllCamperComparator : public UDBDBadge
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EDBDScoreTypes> ComparatorScoreTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumValueForReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBadgeValueStrategy ValueStrategy;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	float _eventValueForOwner;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TMap<ADBDPlayer*, float> _eventValueForOtherPlayers;

public:
	UDBDBadge_AllCamperComparator();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_AllCamperComparator) { return 0; }

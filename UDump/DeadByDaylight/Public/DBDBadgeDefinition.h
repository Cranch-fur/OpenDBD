#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EPlayerRole.h"
#include "EBadgeGroup.h"
#include "DBDBadgeRewardConditionGroup.h"
#include "EBadgeEvaluation.h"
#include "DBDBadgeDefinition.generated.h"

USTRUCT(BlueprintType)
struct FDBDBadgeDefinition : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EPlayerRole> Roles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SpecificCharacterIndexes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBadgeGroup BadgeGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PipsToAward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDBDBadgeRewardConditionGroup> RewardConditionGroups;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<UDBDBadge> CustomBadge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> IconFilePaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBadgeEvaluation BadgeEvaluation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Enabled;

public:
	DEADBYDAYLIGHT_API FDBDBadgeDefinition();
};

FORCEINLINE uint32 GetTypeHash(const FDBDBadgeDefinition) { return 0; }

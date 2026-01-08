#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EPlayerRole.h"
#include "EDBDScoreTypes.h"
#include "DailyRitualLevel.h"
#include "DailyRitualDefinition.generated.h"

USTRUCT(BlueprintType)
struct FDailyRitualDefinition : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> PossibleCharacterIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EPlayerRole> PossibleRoles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EDBDScoreTypes> TrackedEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDailyRitualLevel> DifficultyLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<URitualEvaluatorBase> Evaluator;

public:
	DEADBYDAYLIGHT_API FDailyRitualDefinition();
};

FORCEINLINE uint32 GetTypeHash(const FDailyRitualDefinition) { return 0; }

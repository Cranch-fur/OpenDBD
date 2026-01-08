#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RankDefinition.h"
#include "PerkLevelDefinition.h"
#include "LobbiesSearchMessageDefinition.h"
#include "DBDDesignTunables.generated.h"

UCLASS(Blueprintable, Transient)
class DEADBYDAYLIGHT_API UDBDDesignTunables : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WinStreakThresholdLarge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WinStreakThresholdSmall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxFearTokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSkulls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRankDefinition> RankDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> PipThresholds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPerkLevelDefinition> PerkLevelDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLobbiesSearchMessageDefinition> LobbiesSearchMessageDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CreditsHtml;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LegalTextTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LegalText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SystemMessage_ChatDisconnected;

public:
	UFUNCTION(BlueprintPure)
	float GetTunableValue(FName ID, bool warnIfRowMissing) const;

public:
	UDBDDesignTunables();
};

FORCEINLINE uint32 GetTypeHash(const UDBDDesignTunables) { return 0; }

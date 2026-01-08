#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/SoftObjectPtr.h"
#include "EKillerMoodInfluence.h"
#include "GeneratedLevelData.h"
#include "ProceduralLevelData.generated.h"

class UPaperTileMap;

UCLASS(Blueprintable)
class AProceduralLevelData : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperTileMap> SelectedPaperMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GenerationSeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKillerMoodInfluence LevelMood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGeneratedLevelData GeneratedData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Offerings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PerksLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> perks;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	AActor* _builder;

public:
	AProceduralLevelData();
};

FORCEINLINE uint32 GetTypeHash(const AProceduralLevelData) { return 0; }

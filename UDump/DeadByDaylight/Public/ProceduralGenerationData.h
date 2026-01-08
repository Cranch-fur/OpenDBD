#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuadrantSpawnTypeProperties.h"
#include "QuadrantSpawnCategories.h"
#include "ThemeProperties.h"
#include "KillerItemDependencies.h"
#include "SurvivorItemProperties.h"
#include "EscapeStrategyProperty.h"
#include "EscapeTypeObjects.h"
#include "GameplayElementsPopulation.h"
#include "ManagedGameplayElementsPopulation.h"
#include "SurvivorGroupingProbability.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralGenerationData.generated.h"

class UCurveFloat;

UCLASS(Blueprintable)
class UProceduralGenerationData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuadrantSpawnTypeProperties> QuadrantSpawnTypeProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuadrantSpawnCategories> QuadrantSpawnCategories;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FThemeProperties> ThemeProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSurvivorCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FKillerItemDependencies> KillersItemDependencies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* SurvivorItemPopulationProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSurvivorItemProperties> SurvivorItemProperty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* EscapeStrategyPopulationProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEscapeStrategyProperty> EscapeStrategyProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEscapeTypeObjects> EscapeTypeActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayElementsPopulation> ElementsPopulation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FManagedGameplayElementsPopulation> ManagedElementsPopulation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSurvivorGroupingProbability> SurvivorGroupingProbabilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ProceduralMapOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProceduralTileWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProceduralTileHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DifficultyModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CharacterCollisionRadius;

public:
	UProceduralGenerationData();
};

FORCEINLINE uint32 GetTypeHash(const UProceduralGenerationData) { return 0; }

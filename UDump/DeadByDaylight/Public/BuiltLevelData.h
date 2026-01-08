#pragma once

#include "CoreMinimal.h"
#include "Dependency.h"
#include "BuiltLevelData.generated.h"

USTRUCT(BlueprintType)
struct FBuiltLevelData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ThemeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ThemeWeather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TileCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDependency> Dependencies;

public:
	DEADBYDAYLIGHT_API FBuiltLevelData();
};

FORCEINLINE uint32 GetTypeHash(const FBuiltLevelData) { return 0; }

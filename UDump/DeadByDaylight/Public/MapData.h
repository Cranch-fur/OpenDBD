#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileProperties.h"
#include "MapData.generated.h"

class UProceduralGenerationData;

UCLASS(Blueprintable)
class UMapData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ThemeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ThemeWeather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MapWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MapHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTileProperties> MapTileProperties;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UProceduralGenerationData* _proceduralGenerationData;

public:
	UMapData();
};

FORCEINLINE uint32 GetTypeHash(const UMapData) { return 0; }

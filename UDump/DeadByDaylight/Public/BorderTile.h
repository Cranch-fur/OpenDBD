#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "BorderElements.h"
#include "BorderTile.generated.h"

UCLASS(Blueprintable)
class ABorderTile : public ATile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBorderElements> Borders;

public:
	ABorderTile();
};

FORCEINLINE uint32 GetTypeHash(const ABorderTile) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArrayOfTiles.h"
#include "TileMatrix.generated.h"

class ATile;

UCLASS(Blueprintable)
class UTileMatrix : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FArrayOfTiles> _matrix;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<ATile*> _extraTiles;

public:
	UTileMatrix();
};

FORCEINLINE uint32 GetTypeHash(const UTileMatrix) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConstArrayOfTiles.h"
#include "TileBank.generated.h"

UCLASS(Blueprintable)
class UTileBank : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<uint8, FConstArrayOfTiles> _tileBlueprints;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<uint8, FConstArrayOfTiles> _escapeTileBlueprints;

public:
	UTileBank();
};

FORCEINLINE uint32 GetTypeHash(const UTileBank) { return 0; }

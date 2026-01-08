#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MaterialMapForClip.generated.h"

class UMaterialInterface;

USTRUCT(BlueprintType)
struct FMaterialMapForClip : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* SrcMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DstMaterial;

public:
	DEADBYDAYLIGHT_API FMaterialMapForClip();
};

FORCEINLINE uint32 GetTypeHash(const FMaterialMapForClip) { return 0; }

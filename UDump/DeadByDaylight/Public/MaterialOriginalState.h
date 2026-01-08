#pragma once

#include "CoreMinimal.h"
#include "MaterialOriginalState.generated.h"

class UMeshComponent;
class UMaterialInterface;

USTRUCT(BlueprintType)
struct FMaterialOriginalState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced)
	UMeshComponent* MeshWithChangedMaterials;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<UMaterialInterface*> OriginalMaterial;

public:
	DEADBYDAYLIGHT_API FMaterialOriginalState();
};

FORCEINLINE uint32 GetTypeHash(const FMaterialOriginalState) { return 0; }

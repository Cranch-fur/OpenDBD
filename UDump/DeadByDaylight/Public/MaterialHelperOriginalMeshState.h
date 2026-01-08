#pragma once

#include "CoreMinimal.h"
#include "MaterialHelperOriginalMeshState.generated.h"

class UMeshComponent;
class UMaterialInterface;

USTRUCT(BlueprintType)
struct FMaterialHelperOriginalMeshState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UMeshComponent* _targetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> _originalMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> _originalDynamicMaterials;

public:
	DEADBYDAYLIGHT_API FMaterialHelperOriginalMeshState();
};

FORCEINLINE uint32 GetTypeHash(const FMaterialHelperOriginalMeshState) { return 0; }

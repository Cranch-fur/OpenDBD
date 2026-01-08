#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/TextureDefines.h"
#include "SubstanceTexture2D.generated.h"

class USubstanceGraphInstance;

UCLASS(Blueprintable, MinimalAPI)
class USubstanceTexture2D : public UTexture2DDynamic
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USubstanceGraphInstance* ParentInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AssetRegistrySearchable)
	TEnumAsByte<TextureAddress> AddressX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AssetRegistrySearchable)
	TEnumAsByte<TextureAddress> AddressY;

public:
	USubstanceTexture2D();
};

FORCEINLINE uint32 GetTypeHash(const USubstanceTexture2D) { return 0; }

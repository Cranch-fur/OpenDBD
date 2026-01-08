#pragma once

#include "CoreMinimal.h"
#include "EItemRarity.h"
#include "FILE_NOT_FOUND"
#include "RarityTexture.generated.h"

USTRUCT(BlueprintType)
struct FRarityTexture
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStringAssetReference Texture;

public:
	DEADBYDAYLIGHT_API FRarityTexture();
};

FORCEINLINE uint32 GetTypeHash(const FRarityTexture) { return 0; }

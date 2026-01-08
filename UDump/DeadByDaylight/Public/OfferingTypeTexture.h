#pragma once

#include "CoreMinimal.h"
#include "FILE_NOT_FOUND"
#include "OfferingTypeTexture.generated.h"

USTRUCT(BlueprintType)
struct FOfferingTypeTexture
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStringAssetReference TextureBack;

public:
	DEADBYDAYLIGHT_API FOfferingTypeTexture();
};

FORCEINLINE uint32 GetTypeHash(const FOfferingTypeTexture) { return 0; }

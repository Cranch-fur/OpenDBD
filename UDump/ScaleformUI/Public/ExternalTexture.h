#pragma once

#include "CoreMinimal.h"
#include "ExternalTexture.generated.h"

class UTexture;

USTRUCT(BlueprintType)
struct FExternalTexture
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Texture;

public:
	SCALEFORMUI_API FExternalTexture();
};

FORCEINLINE uint32 GetTypeHash(const FExternalTexture) { return 0; }

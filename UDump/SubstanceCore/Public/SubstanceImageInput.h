#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SubstanceImageInput.generated.h"

class USubstanceGraphInstance;

UCLASS(Blueprintable, MinimalAPI)
class USubstanceImageInput : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CompressionRGB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CompressionAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SizeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SourceFilePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SourceFileTimestamp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<USubstanceGraphInstance*> Consumers;

public:
	USubstanceImageInput();
};

FORCEINLINE uint32 GetTypeHash(const USubstanceImageInput) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "EASType.h"
#include "ASValue.generated.h"

class UGFxObject;

USTRUCT(BlueprintType)
struct SCALEFORMUI_API FASValue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EASType::Type> ASType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ASBoolean;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ASInt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ASNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ASString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGFxObject* ASObject;

public:
	FASValue();
};

FORCEINLINE uint32 GetTypeHash(const FASValue) { return 0; }

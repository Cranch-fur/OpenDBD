#pragma once

#include "CoreMinimal.h"
#include "GFxWidgetBinding.generated.h"

USTRUCT(BlueprintType)
struct FGFxWidgetBinding
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WidgetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* WidgetClass;

public:
	SCALEFORMUI_API FGFxWidgetBinding();
};

FORCEINLINE uint32 GetTypeHash(const FGFxWidgetBinding) { return 0; }

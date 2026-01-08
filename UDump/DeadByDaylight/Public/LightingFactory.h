#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LightingFactory.generated.h"

class ULightingData;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ULightingFactory : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULightingData* _data;

public:
	ULightingFactory();
};

FORCEINLINE uint32 GetTypeHash(const ULightingFactory) { return 0; }

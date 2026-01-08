#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BloodwebEntity.generated.h"

UCLASS(Blueprintable)
class UBloodwebEntity : public UObject
{
	GENERATED_BODY()

public:
	UBloodwebEntity();
};

FORCEINLINE uint32 GetTypeHash(const UBloodwebEntity) { return 0; }

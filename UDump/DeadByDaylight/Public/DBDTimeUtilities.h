#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DBDTimeUtilities.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UDBDTimeUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void GetNetDateTime();

public:
	UDBDTimeUtilities();
};

FORCEINLINE uint32 GetTypeHash(const UDBDTimeUtilities) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FILE_NOT_FOUND"
#include "DBDSurfaceTypeName.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UDBDSurfaceTypeName : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FName Convert(EPhysicalSurface surfaceType);

public:
	UDBDSurfaceTypeName();
};

FORCEINLINE uint32 GetTypeHash(const UDBDSurfaceTypeName) { return 0; }

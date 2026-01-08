#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SurfaceTypeName.generated.h"

USTRUCT(BlueprintType)
struct FSurfaceTypeName : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

public:
	DEADBYDAYLIGHT_API FSurfaceTypeName();
};

FORCEINLINE uint32 GetTypeHash(const FSurfaceTypeName) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "KillerItemProperty.generated.h"

class UCurveFloat;

USTRUCT(BlueprintType)
struct FKillerItemProperty
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TAssetSubclassOf<AActor> Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UCurveFloat* Population;

public:
	DEADBYDAYLIGHT_API FKillerItemProperty();
};

FORCEINLINE uint32 GetTypeHash(const FKillerItemProperty) { return 0; }

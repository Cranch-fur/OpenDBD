#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OfferingHandler.generated.h"

class UDBDGameInstance;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UOfferingHandler : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDGameInstance* _gameInstance;

public:
	UOfferingHandler();
};

FORCEINLINE uint32 GetTypeHash(const UOfferingHandler) { return 0; }

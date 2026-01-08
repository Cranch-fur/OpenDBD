#pragma once

#include "CoreMinimal.h"
#include "UObject/SoftObjectPtr.h"
#include "DataTableProxy.generated.h"

class UDataTable;

USTRUCT(BlueprintType)
struct FDataTableProxy
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UDataTable> AssetPtr;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDataTable* _dataTable;

public:
	DEADBYDAYLIGHT_API FDataTableProxy();
};

FORCEINLINE uint32 GetTypeHash(const FDataTableProxy) { return 0; }

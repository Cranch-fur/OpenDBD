#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FearMarketDataReceivedDelegate.h"
#include "FearMarketItemInstance.h"
#include "FearMarketOfferingInstance.h"
#include "FearMarketManager.generated.h"

class UDBDGameInstance;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UFearMarketManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFearMarketDataReceivedDelegate OnFearMarketDataReceived;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFearMarketItemInstance PendingPurchaseFearMarketItem;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FFearMarketOfferingInstance _currentFearMarketOffering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<FFearMarketOfferingInstance> _availableOfferings;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TWeakObjectPtr<UDBDGameInstance> _gameInstance;

public:
	UFearMarketManager();
};

FORCEINLINE uint32 GetTypeHash(const UFearMarketManager) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBDHud.generated.h"

class AStatusView;
class AActor;

UCLASS(Blueprintable)
class UDBDHud : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TMap<FName, AStatusView*> _statusViews;

private:
	UFUNCTION(BlueprintCallable)
	void RemoveStatusViewOnDestroy(AActor* Actor);

public:
	UDBDHud();
};

FORCEINLINE uint32 GetTypeHash(const UDBDHud) { return 0; }

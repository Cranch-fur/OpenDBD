#pragma once

#include "CoreMinimal.h"
#include "DBDBadge.h"
#include "DBDBadge_KillerLazy.generated.h"

class UStillnessTrackerComponent;

UCLASS(Blueprintable)
class UDBDBadge_KillerLazy : public UDBDBadge
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	UStillnessTrackerComponent* _stillnessComponent;

public:
	UDBDBadge_KillerLazy();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_KillerLazy) { return 0; }

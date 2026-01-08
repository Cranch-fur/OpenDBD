#pragma once

#include "CoreMinimal.h"
#include "DBDBadge.h"
#include "DBDBadge_KillerGoals.generated.h"

UCLASS(Blueprintable)
class UDBDBadge_KillerGoals : public UDBDBadge
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	int32 GetKillerGoalCount() const;

public:
	UDBDBadge_KillerGoals();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge_KillerGoals) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EMapActorCategory.h"
#include "MapActorDB.generated.h"

class AActor;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UMapActorDB : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetUniqueRandom(const TArray<EMapActorCategory>& Categories, int32 Count) const;

	UFUNCTION(BlueprintPure)
	AActor* GetRandom(EMapActorCategory Category) const;

public:
	UMapActorDB();
};

FORCEINLINE uint32 GetTypeHash(const UMapActorDB) { return 0; }

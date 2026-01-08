#pragma once

#include "CoreMinimal.h"
#include "ETileSpawnPointType.h"
#include "FILE_NOT_FOUND"
#include "Templates/SubclassOf.h"
#include "Dependency.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct FDependency
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileSpawnPointType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStringAssetReference AssetReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Unique;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;

public:
	DEADBYDAYLIGHT_API FDependency();
};

FORCEINLINE uint32 GetTypeHash(const FDependency) { return 0; }

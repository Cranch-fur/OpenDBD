#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dependency.h"
#include "ActorDependency.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UActorDependency : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDependency> Dependencies;

public:
	UActorDependency();
};

FORCEINLINE uint32 GetTypeHash(const UActorDependency) { return 0; }

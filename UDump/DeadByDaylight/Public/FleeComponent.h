#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FleeComponent.generated.h"

class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UFleeComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	AActor* _threat;

public:
	UFleeComponent();
};

FORCEINLINE uint32 GetTypeHash(const UFleeComponent) { return 0; }

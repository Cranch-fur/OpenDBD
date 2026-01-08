#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParticleSystemDistanceFaderComponent.generated.h"

class UParticleSystemComponent;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UParticleSystemDistanceFaderComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName OpacityParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartFadeDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndFadeDistance;

public:
	UParticleSystemDistanceFaderComponent();
};

FORCEINLINE uint32 GetTypeHash(const UParticleSystemDistanceFaderComponent) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlashLightReceiverComponent.generated.h"

class UFlashLightComponent;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UFlashLightReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RemoveFlashLight(UFlashLightComponent* flashLight);

	UFUNCTION(BlueprintPure)
	bool IsInAnyFlashLightZone() const;

	UFUNCTION(BlueprintPure)
	static bool IsActorInAnyFlashLightZone(const AActor* target);

	UFUNCTION(BlueprintCallable)
	void AddFlashLight(UFlashLightComponent* flashLight);

public:
	UFlashLightReceiverComponent();
};

FORCEINLINE uint32 GetTypeHash(const UFlashLightReceiverComponent) { return 0; }

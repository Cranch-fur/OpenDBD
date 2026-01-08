#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OfferingRevealDelays.h"
#include "OfferingSequenceAssets.generated.h"

class AOfferingCard;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AOfferingSequenceAssets : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FOfferingRevealDelays> OfferingRevealDelays;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AOfferingCard*> Cards;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlaySmoke();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayFadeOut();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayFadeIn();

public:
	AOfferingSequenceAssets();
};

FORCEINLINE uint32 GetTypeHash(const AOfferingSequenceAssets) { return 0; }

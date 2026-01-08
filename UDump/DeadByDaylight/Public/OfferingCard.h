#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RarityTexture.h"
#include "OfferingTypeTexture.h"
#include "EOfferingAnimState.h"
#include "OfferingCard.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AOfferingCard : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRarityTexture> RarityTextures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FOfferingTypeTexture> OfferingTypeTextures;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayAnim(EOfferingAnimState State);

public:
	AOfferingCard();
};

FORCEINLINE uint32 GetTypeHash(const AOfferingCard) { return 0; }

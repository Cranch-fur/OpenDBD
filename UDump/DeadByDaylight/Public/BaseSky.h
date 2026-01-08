#pragma once

#include "CoreMinimal.h"
#include "Fadeable.h"
#include "GameFramework/Actor.h"
#include "BaseSky.generated.h"

class UPostProcessComponent;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ABaseSky : public AActor, public IFadeable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AdjustCullDistance;

protected:
	UFUNCTION(BlueprintCallable)
	void SetCullDistance(float newCullDistance);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFadeEnd(AActor* Other);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFadeBegin(AActor* Other);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnAlphaChanged(float newAlpha, AActor* Other);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UPostProcessComponent* GetPostprocessComponent();

public:
	ABaseSky();
};

FORCEINLINE uint32 GetTypeHash(const ABaseSky) { return 0; }

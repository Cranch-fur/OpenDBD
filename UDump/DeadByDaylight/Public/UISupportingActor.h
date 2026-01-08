#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UISupportingActor.generated.h"

UCLASS(Blueprintable)
class AUISupportingActor : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PlayFadeToBloodweb(bool FadeIn);

	UFUNCTION(BlueprintCallable)
	void PlayFade(bool FadeIn, bool fromRoleSwitch);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsFading();

	UFUNCTION(BlueprintCallable)
	bool IsFadedIn();

	UFUNCTION(BlueprintCallable)
	void FadeOutDone();

	UFUNCTION(BlueprintCallable)
	void FadeInDone();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ExecuteFadeToBloodweb(bool FadeIn);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ExecuteFade(bool FadeIn, bool fromRoleSwitch);

public:
	AUISupportingActor();
};

FORCEINLINE uint32 GetTypeHash(const AUISupportingActor) { return 0; }

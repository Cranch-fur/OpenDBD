#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "HintData.h"
#include "LoadingScreen.generated.h"

UCLASS(Blueprintable)
class ULoadingScreen : public UScreenBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowLoadingTip(FHintData tipData) const;

	UFUNCTION(BlueprintCallable)
	void ShowLoadingProgressBar(bool show) const;

	UFUNCTION(BlueprintCallable)
	void ShowLoadingImageTips(bool isKiller) const;

	UFUNCTION(BlueprintCallable)
	void SetLoadingProgressValue(float Value) const;

public:
	ULoadingScreen();
};

FORCEINLINE uint32 GetTypeHash(const ULoadingScreen) { return 0; }

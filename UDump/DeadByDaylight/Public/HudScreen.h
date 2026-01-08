#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "HudScreen.generated.h"

UCLASS(Blueprintable)
class UHudScreen : public UScreenBase
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable)
	void OnQuitGameButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnPreviousButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnNextButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnLeaveButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnCancelQuitGameButtonClick();

public:
	UHudScreen();
};

FORCEINLINE uint32 GetTypeHash(const UHudScreen) { return 0; }

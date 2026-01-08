#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "HelpScreen.generated.h"

UCLASS(Blueprintable)
class UHelpScreen : public UScreenBase
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable)
	void OnVideoButtonClick(int32 Index);

	UFUNCTION(BlueprintCallable)
	void OnSkipTutorialVideo();

	UFUNCTION(BlueprintCallable)
	void OnManualButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnBackButtonClick();

public:
	UHelpScreen();
};

FORCEINLINE uint32 GetTypeHash(const UHelpScreen) { return 0; }

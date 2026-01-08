#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "SplashScreen.generated.h"

UCLASS(Blueprintable)
class USplashScreen : public UScreenBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowPressKeyState();

	UFUNCTION(BlueprintCallable)
	void ShowLoadingState();

	UFUNCTION(BlueprintCallable)
	void SetVersionText(const FString& versionText);

private:
	UFUNCTION(BlueprintCallable)
	void OnKeyPressed();

public:
	UFUNCTION(BlueprintCallable)
	void HidePressKeyState();

public:
	USplashScreen();
};

FORCEINLINE uint32 GetTypeHash(const USplashScreen) { return 0; }

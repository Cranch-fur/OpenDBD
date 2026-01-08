#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "OverlayMenuScreen.generated.h"

UCLASS(Blueprintable)
class UOverlayMenuScreen : public UScreenBase
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable)
	void OnSurvivorMouseSensitivityChanged(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnSurvivorControllerSensitivityChanged(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnResolutionClick(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnResetDefaults(const FString& keyBindingsData, int32 mustBeGamepadKey);

	UFUNCTION(BlueprintCallable)
	void OnRequestKeyInput(const FString& Name, bool isAxis, float Scale, const FString& Key);

	UFUNCTION(BlueprintCallable)
	void OnQuitGameButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnQuitButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnQualitySetIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void OnPlayerInfoClick(const FString& PlayerId);

	UFUNCTION(BlueprintCallable)
	void OnMuteOnFousLostClick();

	UFUNCTION(BlueprintCallable)
	void OnMenuMusicVolumeClick(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnMainVolumeClick(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnLanguageChange(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnKillerMouseSensitivityChanged(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnKillerControllerSensitivityChanged(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnInvertYClick();

	UFUNCTION(BlueprintCallable)
	void OnHelpButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnHeadphonesClick();

	UFUNCTION(BlueprintCallable)
	void OnFullScreenClick();

	UFUNCTION(BlueprintCallable)
	void OnBackButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnAutoAdjustClick();

public:
	UOverlayMenuScreen();
};

FORCEINLINE uint32 GetTypeHash(const UOverlayMenuScreen) { return 0; }

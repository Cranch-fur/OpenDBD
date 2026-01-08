#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "ETestBuildType.h"
#include "RoleSelectionScreen.generated.h"

UCLASS(Blueprintable)
class URoleSelectionScreen : public UScreenBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowTestBuildContainer(ETestBuildType testBuildType);

	UFUNCTION(BlueprintCallable)
	void ShowSurvivorGroupButton(bool show);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerName(const FString& PlayerName);

	UFUNCTION(BlueprintCallable)
	void ShowDailyRitualsNotification(bool show);

	UFUNCTION(BlueprintCallable)
	void SetLockedOutTimer(int32 seconds);

	UFUNCTION(BlueprintCallable)
	void SetExistingLobbies(const FString& SurvivorMessage, const FString& KillerMessage);

	UFUNCTION(BlueprintCallable)
	void RestoreButtonsState();

private:
	UFUNCTION(BlueprintCallable)
	void OnSurvivorGroupButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnSurvivorButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnSocialButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnSettingsButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnQuitGameButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnQuickMatchButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnPartyModeButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnNewContentButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnKillerButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnHelpButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnDailyRitualsButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnCreditsButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnChangeUserButtonClick();

public:
	URoleSelectionScreen();
};

FORCEINLINE uint32 GetTypeHash(const URoleSelectionScreen) { return 0; }

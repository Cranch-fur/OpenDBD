#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "SystemPromptsScreen.generated.h"

UCLASS(Blueprintable)
class USystemPromptsScreen : public UScreenBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowSeasonEndPrompt(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void ShowReportPlayerPrompt(const FString& PlayerName);

	UFUNCTION(BlueprintCallable)
	void ShowOfflineWarningPrompt();

	UFUNCTION(BlueprintCallable)
	void ShowNewContentPrompt(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void ShowMarketConfirmationPrompt(const FString& ItemData, int32 availableTokens);

	UFUNCTION(BlueprintCallable)
	void ShowLegalPrompt(const FString& Title, const FString& LegalText);

	UFUNCTION(BlueprintCallable)
	void ShowGetLatestPrompt();

	UFUNCTION(BlueprintCallable)
	void ShowFTUEPrompt();

	UFUNCTION(BlueprintCallable)
	void ShowErrorPrompt(const FString& Title, const FString& message);

	UFUNCTION(BlueprintCallable)
	void ShowErrorForceQuitPrompt(const FString& Title, const FString& message);

	UFUNCTION(BlueprintCallable)
	void ShowDisconnectWarningPrompt();

	UFUNCTION(BlueprintCallable)
	void ShowDailyRitualsPrompt(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void ShowConfirmResetKeyBindings(bool showPS4SpecialStrings);

	UFUNCTION(BlueprintCallable)
	void ShowConfirmQuitGame();

	UFUNCTION(BlueprintCallable)
	void ShowConfirmPrestigeUpPrompt();

	UFUNCTION(BlueprintCallable)
	void ShowConfirmLeaveMatch();

	UFUNCTION(BlueprintCallable)
	void ShowConfirmLeaveLobby();

	UFUNCTION(BlueprintCallable)
	void ShowCheatingForceQuitPrompt(const FString& message);

	UFUNCTION(BlueprintCallable)
	void ShowCheatingDisconnectPrompt(const FString& Title, const FString& message);

private:
	UFUNCTION(BlueprintCallable)
	void OnTextInputMouseOver(int32 isMouseOver);

	UFUNCTION(BlueprintCallable)
	void OnSystemPromptsEnd();

	UFUNCTION(BlueprintCallable)
	void OnSurvivorRoleButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnReplyLegalTerms(int32 accepted);

	UFUNCTION(BlueprintCallable)
	void OnRemoveDailyRitual(const FString& ID);

	UFUNCTION(BlueprintCallable)
	void OnKillerRoleButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnConfirmResetKeyBindings();

	UFUNCTION(BlueprintCallable)
	void OnConfirmReportPlayer(const FString& Category, const FString& Comment);

	UFUNCTION(BlueprintCallable)
	void OnConfirmQuitGame();

	UFUNCTION(BlueprintCallable)
	void OnConfirmPrestigeUp();

	UFUNCTION(BlueprintCallable)
	void OnConfirmLeaveMatch();

	UFUNCTION(BlueprintCallable)
	void OnConfirmLeaveLobby();

	UFUNCTION(BlueprintCallable)
	void OnConfirmBuyMarketItem();

public:
	USystemPromptsScreen();
};

FORCEINLINE uint32 GetTypeHash(const USystemPromptsScreen) { return 0; }

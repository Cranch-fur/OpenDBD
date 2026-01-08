#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "MatchManagementScreen.generated.h"

UCLASS(Blueprintable)
class UMatchManagementScreen : public UScreenBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetSelectedMapName(const FString& MapName);

	UFUNCTION(BlueprintCallable)
	void SetPrivateMatchSettings(bool isPrivate);

	UFUNCTION(BlueprintCallable)
	void SetPerksSettingsValueStrings(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void SetPerksSettings(int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetOfferingsSettingsValueStrings(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void SetOfferingsSettings(int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetMapsData(const FString& Data, int32 selectedIndex);

	UFUNCTION(BlueprintCallable)
	void SetItemsAndAddonsSettingsValueStrings(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void SetItemsAndAddonsSettings(int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetAllowDlcSettings(bool allow);

private:
	UFUNCTION(BlueprintCallable)
	void OnToggleMatchAccessibility();

	UFUNCTION(BlueprintCallable)
	void OnRemoveMap(const FString& MapId);

	UFUNCTION(BlueprintCallable)
	void OnPerksSettingsChange(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnOfferingsSettingsChange(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnItemsAndAddonsSettingsChange(int32 Value);

	UFUNCTION(BlueprintCallable)
	void OnChooseMapButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnBackToOverviewButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnBackButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnApplyChangesClicked();

	UFUNCTION(BlueprintCallable)
	void OnAllowDlcSettingsClick();

	UFUNCTION(BlueprintCallable)
	void OnAddMap(const FString& MapId);

public:
	UMatchManagementScreen();
};

FORCEINLINE uint32 GetTypeHash(const UMatchManagementScreen) { return 0; }

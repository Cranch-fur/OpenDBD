#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "EPlayerRole.h"
#include "PanelScreen.generated.h"

UCLASS(Blueprintable)
class UPanelScreen : public UScreenBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowTabNotification(int32 tabIndex, bool show);

	UFUNCTION(BlueprintCallable)
	void ShowPanel(int32 Index);

	UFUNCTION(BlueprintCallable)
	void SetRole(EPlayerRole currentPlayerRole, EPlayerRole nextPlayerRole);

	UFUNCTION(BlueprintCallable)
	void SetProgressionTabsEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable)
	void SetFadeInDelay(float Delay);

	UFUNCTION(BlueprintCallable)
	void SetAllowRoleChange(bool allowRoleChange, bool hideButton);

	UFUNCTION(BlueprintCallable)
	void SetAllowFadeOut(bool allow);

protected:
	UFUNCTION(BlueprintCallable)
	void OnWebNodeSelected(int32 ID, int32 nodeDepth);

	UFUNCTION(BlueprintCallable)
	void OnSideTabButtonClick(int32 tabIndex);

	UFUNCTION(BlueprintCallable)
	void OnRoleButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnReservedBloodPoints(int32 BloodPoints);

	UFUNCTION(BlueprintCallable)
	void OnRegenerateButtonClick(int32 levelUp);

	UFUNCTION(BlueprintCallable)
	void OnPrestigeUp();

	UFUNCTION(BlueprintCallable)
	void OnMarketSlotButtonClick(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void OnLoadoutSlotButtonClick(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void OnItemUnselected(int32 ItemIndex);

	UFUNCTION(BlueprintCallable)
	void OnItemSelected(int32 ItemIndex);

	UFUNCTION(BlueprintCallable)
	void OnConfirmCollectSelectedNode();

	UFUNCTION(BlueprintCallable)
	void OnCharInfoClick();

	UFUNCTION(BlueprintCallable)
	void OnCharacterSlotButtonClick(int32 CharacterIndex);

	UFUNCTION(BlueprintCallable)
	void OnCategoryIndexSelected(int32 categoryIndex);

	UFUNCTION(BlueprintCallable)
	void OnAvailableItemSelected(int32 ItemIndex);

public:
	UFUNCTION(BlueprintCallable)
	void MarketSetSlotAsPurchased(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void MarketSetData(const FString& slotsData, int32 minutesLeft);

	UFUNCTION(BlueprintCallable)
	void MarketSetAvailableFearTokens(int32 availableFearTokens);

	UFUNCTION(BlueprintCallable)
	void LoadoutSetData(const FString& Data, bool usingMatchRules, bool perkSlotsLockedByAdmin, int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void HideTabs(bool hide) const;

	UFUNCTION(BlueprintCallable)
	void HideAllPanels() const;

public:
	UPanelScreen();
};

FORCEINLINE uint32 GetTypeHash(const UPanelScreen) { return 0; }

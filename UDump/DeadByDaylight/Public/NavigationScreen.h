#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "ETestBuildType.h"
#include "EGameType.h"
#include "EConnectionMessage.h"
#include "NavigationScreen.generated.h"

UCLASS(Blueprintable)
class UNavigationScreen : public UScreenBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerInfo(int32 PlayerId, bool inFrontOfCamera, float xPosition, float yPosition, float Scale);

	UFUNCTION(BlueprintCallable)
	void UpdateCardsScreenPositions(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void ShowTestBuildContainer(ETestBuildType testBuildType);

	UFUNCTION(BlueprintCallable)
	void ShowReservedBloodPoints(int32 bankBloodPoints, int32 reservedBloodPoints);

	UFUNCTION(BlueprintCallable)
	void ShowRemovedFearTokens(int32 bankFearTokens, int32 removedFearTokens);

	UFUNCTION(BlueprintCallable)
	void ShowPlayersInfo(bool show);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerRolesWarning(bool show, const FString& message, int32 Count);

	UFUNCTION(BlueprintCallable)
	void ShowPartyInfo(EGameType GameType);

	UFUNCTION(BlueprintCallable)
	void ShowMatchRulesNotification(bool perksEnabled, bool itemsAndAddonsEnabled, bool offeringsEnabled, const FString& MapName);

	UFUNCTION(BlueprintCallable)
	void ShowMatchManagementButton(bool show);

	UFUNCTION(BlueprintCallable)
	void ShowInviteButton(bool show);

	UFUNCTION(BlueprintCallable)
	void ShowDailyRitualsNotification(bool show);

	UFUNCTION(BlueprintCallable)
	void ShowCancelButton(bool show);

	UFUNCTION(BlueprintCallable)
	void SetSpectatorView();

	UFUNCTION(BlueprintCallable)
	void SetReadyStatus(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void SetPlayersJoinedCount(int32 Count, int32 requiredPlayers);

	UFUNCTION(BlueprintCallable)
	void SetPartyModeData(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void SetOfflineView(EConnectionMessage connectionMessageId, bool isSlasher);

	UFUNCTION(BlueprintCallable)
	void SetOfferingsData(const FString& Data);

	UFUNCTION(BlueprintCallable)
	void SetNotificationsOnlyView();

	UFUNCTION(BlueprintCallable)
	void SetLockedOutTimer(int32 seconds);

	UFUNCTION(BlueprintCallable)
	void SetLocalPlayerInfo(int32 BloodPoints, int32 FearTokens, const FString& CharacterName, const FString& PlayerName, int32 playerLevel, int32 PrestigeLevel, int32 PlayerRank, int32 Skulls, int32 skullsNeeded, bool isSlasher);

	UFUNCTION(BlueprintCallable)
	void SetLobbyTimer(int32 seconds);

	UFUNCTION(BlueprintCallable)
	void SetFogView();

	UFUNCTION(BlueprintCallable)
	void SetExistingLobbies(const FString& message);

	UFUNCTION(BlueprintCallable)
	void SetConnectingView();

	UFUNCTION(BlueprintCallable)
	void SetConnectedView(EConnectionMessage connectionMessageId, bool IsPlayerReady, bool disableNavigationButtons);

	UFUNCTION(BlueprintCallable)
	void SetChatComponentVisible(bool Visible);

	UFUNCTION(BlueprintCallable)
	void SetChatComponentEnabled(bool Enabled, bool defaultVisible);

	UFUNCTION(BlueprintCallable)
	void SetBackButton(bool Visible);

	UFUNCTION(BlueprintCallable)
	void RemovePlayersInfo(int32 PlayerId);

private:
	UFUNCTION(BlueprintCallable)
	void OnToggleChatVisibility();

	UFUNCTION(BlueprintCallable)
	void OnStartButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnSettingsClick();

	UFUNCTION(BlueprintCallable)
	void OnSendChatMsgButtonClick(const FString& message);

	UFUNCTION(BlueprintCallable)
	void OnReadyButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnQuitGameButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnPlayerInfoClick(int32 PlayerId);

	UFUNCTION(BlueprintCallable)
	void OnMatchManagementButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnInviteFriendClick();

	UFUNCTION(BlueprintCallable)
	void OnDailyRitualsClick();

	UFUNCTION(BlueprintCallable)
	void OnChatInputMouseOver(int32 isMouseOver);

	UFUNCTION(BlueprintCallable)
	void OnCancelButtonClick();

public:
	UFUNCTION(BlueprintCallable)
	void ClearPlayersInfo();

	UFUNCTION(BlueprintCallable)
	void AddSystemMessage(const FString& message);

	UFUNCTION(BlueprintCallable)
	void AddPlayerInfo(int32 PlayerId, const FString& CharacterName, const FString& PlayerName, int32 playerLevel, int32 PrestigeLevel, int32 PlayerRank, int32 Skulls, int32 skullsNeeded, bool isSlasher, bool IsPlayerReady);

	UFUNCTION(BlueprintCallable)
	void AddChatMessage(int32 playerIndex, const FString& PlayerName, const FString& message);

public:
	UNavigationScreen();
};

FORCEINLINE uint32 GetTypeHash(const UNavigationScreen) { return 0; }

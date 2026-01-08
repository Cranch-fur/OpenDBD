#include "NavigationScreen.h"
#include "ETestBuildType.h"
#include "EGameType.h"
#include "EConnectionMessage.h"

void UNavigationScreen::UpdatePlayerInfo(int32 PlayerId, bool inFrontOfCamera, float xPosition, float yPosition, float Scale)
{

}

void UNavigationScreen::UpdateCardsScreenPositions(const FString& Data)
{

}

void UNavigationScreen::ShowTestBuildContainer(ETestBuildType testBuildType)
{

}

void UNavigationScreen::ShowReservedBloodPoints(int32 bankBloodPoints, int32 reservedBloodPoints)
{

}

void UNavigationScreen::ShowRemovedFearTokens(int32 bankFearTokens, int32 removedFearTokens)
{

}

void UNavigationScreen::ShowPlayersInfo(bool show)
{

}

void UNavigationScreen::ShowPlayerRolesWarning(bool show, const FString& message, int32 Count)
{

}

void UNavigationScreen::ShowPartyInfo(EGameType GameType)
{

}

void UNavigationScreen::ShowMatchRulesNotification(bool perksEnabled, bool itemsAndAddonsEnabled, bool offeringsEnabled, const FString& MapName)
{

}

void UNavigationScreen::ShowMatchManagementButton(bool show)
{

}

void UNavigationScreen::ShowInviteButton(bool show)
{

}

void UNavigationScreen::ShowDailyRitualsNotification(bool show)
{

}

void UNavigationScreen::ShowCancelButton(bool show)
{

}

void UNavigationScreen::SetSpectatorView()
{

}

void UNavigationScreen::SetReadyStatus(const FString& Data)
{

}

void UNavigationScreen::SetPlayersJoinedCount(int32 Count, int32 requiredPlayers)
{

}

void UNavigationScreen::SetPartyModeData(const FString& Data)
{

}

void UNavigationScreen::SetOfflineView(EConnectionMessage connectionMessageId, bool isSlasher)
{

}

void UNavigationScreen::SetOfferingsData(const FString& Data)
{

}

void UNavigationScreen::SetNotificationsOnlyView()
{

}

void UNavigationScreen::SetLockedOutTimer(int32 seconds)
{

}

void UNavigationScreen::SetLocalPlayerInfo(int32 BloodPoints, int32 FearTokens, const FString& CharacterName, const FString& PlayerName, int32 playerLevel, int32 PrestigeLevel, int32 PlayerRank, int32 Skulls, int32 skullsNeeded, bool isSlasher)
{

}

void UNavigationScreen::SetLobbyTimer(int32 seconds)
{

}

void UNavigationScreen::SetFogView()
{

}

void UNavigationScreen::SetExistingLobbies(const FString& message)
{

}

void UNavigationScreen::SetConnectingView()
{

}

void UNavigationScreen::SetConnectedView(EConnectionMessage connectionMessageId, bool IsPlayerReady, bool disableNavigationButtons)
{

}

void UNavigationScreen::SetChatComponentVisible(bool Visible)
{

}

void UNavigationScreen::SetChatComponentEnabled(bool Enabled, bool defaultVisible)
{

}

void UNavigationScreen::SetBackButton(bool Visible)
{

}

void UNavigationScreen::RemovePlayersInfo(int32 PlayerId)
{

}

void UNavigationScreen::OnToggleChatVisibility()
{

}

void UNavigationScreen::OnStartButtonClick()
{

}

void UNavigationScreen::OnSettingsClick()
{

}

void UNavigationScreen::OnSendChatMsgButtonClick(const FString& message)
{

}

void UNavigationScreen::OnReadyButtonClick()
{

}

void UNavigationScreen::OnQuitGameButtonClick()
{

}

void UNavigationScreen::OnPlayerInfoClick(int32 PlayerId)
{

}

void UNavigationScreen::OnMatchManagementButtonClick()
{

}

void UNavigationScreen::OnInviteFriendClick()
{

}

void UNavigationScreen::OnDailyRitualsClick()
{

}

void UNavigationScreen::OnChatInputMouseOver(int32 isMouseOver)
{

}

void UNavigationScreen::OnCancelButtonClick()
{

}

void UNavigationScreen::ClearPlayersInfo()
{

}

void UNavigationScreen::AddSystemMessage(const FString& message)
{

}

void UNavigationScreen::AddPlayerInfo(int32 PlayerId, const FString& CharacterName, const FString& PlayerName, int32 playerLevel, int32 PrestigeLevel, int32 PlayerRank, int32 Skulls, int32 skullsNeeded, bool isSlasher, bool IsPlayerReady)
{

}

void UNavigationScreen::AddChatMessage(int32 playerIndex, const FString& PlayerName, const FString& message)
{

}

UNavigationScreen::UNavigationScreen()
{

}

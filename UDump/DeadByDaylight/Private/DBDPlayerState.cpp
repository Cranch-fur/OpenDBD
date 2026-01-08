#include "DBDPlayerState.h"
#include "SkillCheckInteractionData.h"
#include "EGameState.h"
#include "EKillerJoiningState.h"
#include "EPlayerRole.h"
#include "ScoreEvent.h"
#include "EDBDScoreTypes.h"
#include "StatusViewSource.h"
#include "Net/UnrealNetwork.h"
#include "RitualHandlerComponent.h"
#include "AchievementHandlerComponent.h"
#include "ActivityTrackerComponent.h"
#include "DBDBadgeEvaluatorComponent.h"
#include "GameplayNotificationManager.h"

class AActor;

void ADBDPlayerState::UpdateSkillCheckInteractionData_Implementation(float Progress)
{

}

bool ADBDPlayerState::UpdateSkillCheckInteractionData_Validate(float Progress)
{
	return true;
}

void ADBDPlayerState::UpdateOngoingScores()
{

}

void ADBDPlayerState::SetSkillCheckInteractionData_Implementation(FSkillCheckInteractionData skillCheckData)
{

}

bool ADBDPlayerState::SetSkillCheckInteractionData_Validate(FSkillCheckInteractionData skillCheckData)
{
	return true;
}

void ADBDPlayerState::SetPlayerGameState(EGameState newGameState)
{

}

void ADBDPlayerState::SetKillerJoiningState(EKillerJoiningState State)
{

}

void ADBDPlayerState::Server_SetSelectedCharacterId_Implementation(EPlayerRole forRole, int32 ID, bool updateDisplayData)
{

}

bool ADBDPlayerState::Server_SetSelectedCharacterId_Validate(EPlayerRole forRole, int32 ID, bool updateDisplayData)
{
	return true;
}

void ADBDPlayerState::Server_SetSelectedCharacterCustomization_Implementation(EPlayerRole forRole, int32 ID, const TArray<FName>& customizationItemIds, bool updateDisplayData)
{

}

bool ADBDPlayerState::Server_SetSelectedCharacterCustomization_Validate(EPlayerRole forRole, int32 ID, const TArray<FName>& customizationItemIds, bool updateDisplayData)
{
	return true;
}

void ADBDPlayerState::Server_SetPlayerGameState_Implementation(EGameState newGameState)
{

}

bool ADBDPlayerState::Server_SetPlayerGameState_Validate(EGameState newGameState)
{
	return true;
}

void ADBDPlayerState::Server_SetGameRole_Implementation(EPlayerRole newPlayerRole)
{

}

bool ADBDPlayerState::Server_SetGameRole_Validate(EPlayerRole newPlayerRole)
{
	return true;
}

void ADBDPlayerState::Server_KillerServerJoined_Implementation(bool succeeded)
{

}

bool ADBDPlayerState::Server_KillerServerJoined_Validate(bool succeeded)
{
	return true;
}

void ADBDPlayerState::OnRep_DisplayData()
{

}

void ADBDPlayerState::Multicast_UpdateSkillCheckInteractionData_Implementation(float Progress)
{

}

bool ADBDPlayerState::Multicast_UpdateSkillCheckInteractionData_Validate(float Progress)
{
	return true;
}

void ADBDPlayerState::Multicast_SetSkillCheckInteractionData_Implementation(FSkillCheckInteractionData skillCheckInteraction)
{

}

bool ADBDPlayerState::Multicast_SetSkillCheckInteractionData_Validate(FSkillCheckInteractionData skillCheckInteraction)
{
	return true;
}

void ADBDPlayerState::Multicast_SetSelectedCharacterId_Implementation(EPlayerRole forRole, int32 ID, bool updateDisplayData)
{

}

void ADBDPlayerState::Multicast_SetSelectedCharacterCustomization_Implementation(EPlayerRole forRole, int32 ID, const TArray<FName>& customizationItemIds, bool updateDisplayData)
{

}

void ADBDPlayerState::Multicast_SetPlayerGameState_Implementation(EGameState newGameState)
{

}

void ADBDPlayerState::Multicast_SetInParadise_Implementation(bool KILLED)
{

}

bool ADBDPlayerState::Multicast_SetInParadise_Validate(bool KILLED)
{
	return true;
}

void ADBDPlayerState::Multicast_SetEquipment_Implementation(int32 SlotIndex, FName ItemId, bool callOnRep)
{

}

void ADBDPlayerState::Multicast_SetAsLeftMatch_Implementation()
{

}

void ADBDPlayerState::Multicast_ScoreEvent_Implementation(FScoreEvent ScoreEvent)
{

}

bool ADBDPlayerState::Multicast_ScoreEvent_Validate(FScoreEvent ScoreEvent)
{
	return true;
}

void ADBDPlayerState::Multicast_FireGameplayEvent_Implementation(EDBDScoreTypes playerGameplayEventType, float amount, AActor* effector, AActor* target)
{

}

bool ADBDPlayerState::Multicast_FireGameplayEvent_Validate(EDBDScoreTypes playerGameplayEventType, float amount, AActor* effector, AActor* target)
{
	return true;
}

void ADBDPlayerState::LogCurrentLoadout()
{

}

EKillerJoiningState ADBDPlayerState::KillerJoiningState() const
{
	return EKillerJoiningState::Disconnected;
}

bool ADBDPlayerState::IsEscaped()
{
	return false;
}

bool ADBDPlayerState::HasHappened(EDBDScoreTypes gameplayEventType, float maxSecondsAgo)
{
	return false;
}

UGameplayNotificationManager* ADBDPlayerState::GetGameplayNotificationManager() const
{
	return NULL;
}

int32 ADBDPlayerState::GetCurrentCharacterID() const
{
	return 0;
}

void ADBDPlayerState::FireScoreEvent(EDBDScoreTypes ScoreType, float percentToAward, bool redirectToGameEventSystem)
{

}

void ADBDPlayerState::FireActiveStatusViewEvent(FName StatusViewID, FName uniqueSourceID, const FStatusViewSource StatusViewSource)
{

}

void ADBDPlayerState::FireActiveStatusEffectEvent(FName statusEffectID, float percentage, int32 iconFilePathIndex, int32 LevelToDisplay)
{

}

void ADBDPlayerState::FireActivePerkEvent(FName perkID, int32 LevelToDisplay, int32 iconFilePathIndex, float percentage, int32 chargeCount)
{

}

void ADBDPlayerState::Client_SetInParadise_Implementation(bool KILLED)
{

}

void ADBDPlayerState::Client_SetGameRole_Implementation(EPlayerRole newRole)
{

}

bool ADBDPlayerState::Client_SetGameRole_Validate(EPlayerRole newRole)
{
	return true;
}

void ADBDPlayerState::Client_RemoveItemFromInventory_Implementation(FName toRemove, bool updateLoadout)
{

}

void ADBDPlayerState::AuthorityFireGameplayEvent(EDBDScoreTypes playerGameplayEventType, float amount, AActor* effector, AActor* target)
{

}

void ADBDPlayerState::Authority_ForceEndOngoingScoreEvent(EDBDScoreTypes ScoreType)
{

}

void ADBDPlayerState::Authority_ForceCancelOngoingScoreEvent(EDBDScoreTypes ScoreType)
{

}

void ADBDPlayerState::Authority_ApplyScoreEvent(EDBDScoreTypes ScoreType, float percentToAward, bool redirectToGameEventSystem)
{

}

void ADBDPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADBDPlayerState, GameLevelLoaded);
	DOREPLIFETIME(ADBDPlayerState, MirrorsId);
	DOREPLIFETIME(ADBDPlayerState, SteamId);
	DOREPLIFETIME(ADBDPlayerState, IsPlayerReady);
	DOREPLIFETIME(ADBDPlayerState, SelectedCamperIndex);
	DOREPLIFETIME(ADBDPlayerState, GameRole);
	DOREPLIFETIME(ADBDPlayerState, SelectedSlasherIndex);
	DOREPLIFETIME(ADBDPlayerState, CamperData);
	DOREPLIFETIME(ADBDPlayerState, SlasherData);
	DOREPLIFETIME(ADBDPlayerState, PlayerData);
	DOREPLIFETIME(ADBDPlayerState, SkillCheckInteractionData);
	DOREPLIFETIME(ADBDPlayerState, _inParadise);
}

ADBDPlayerState::ADBDPlayerState()
{
	this->GameLevelLoaded = false;
	this->MirrorsId = TEXT("");
	this->SteamId = TEXT("");
	this->IsPlayerReady = false;
	this->SelectedCamperIndex = -1;
	this->GameRole = EPlayerRole::VE_None;
	this->SelectedSlasherIndex = -1;
	this->RitualHandler = CreateDefaultSubobject<URitualHandlerComponent>(TEXT("RitualHandler"));
	this->AchievementHandler = CreateDefaultSubobject<UAchievementHandlerComponent>(TEXT("AchievementHandler"));
	this->ActivityTracker = CreateDefaultSubobject<UActivityTrackerComponent>(TEXT("ActivityTracker"));
	this->BadgeEvaluator = CreateDefaultSubobject<UDBDBadgeEvaluatorComponent>(TEXT("BadgeHandlerComponent"));
	this->IsReadyToTravel = false;
	this->_inParadise = false;
	this->_gameplayNotificationManager = CreateDefaultSubobject<UGameplayNotificationManager>(TEXT("GameplayNotificationManager"));
}

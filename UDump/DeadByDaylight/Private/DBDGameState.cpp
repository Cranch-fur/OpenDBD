#include "DBDGameState.h"
#include "SelectedOffering.h"
#include "Dependency.h"
#include "UObject/NoExportTypes.h"
#include "GamePresetData.h"
#include "GameFramework/OnlineReplStructs.h"
#include "ServerJoiningData.h"
#include "BuiltLevelData.h"
#include "Net/UnrealNetwork.h"

class AInteractable;
class AActor;
class ADBDPlayerState_Menu;
class ADBDPlayerState;
class ACamperPlayer;
class ADBDPlayer;
class APawn;

void ADBDGameState::TravelToKillerServer()
{

}

void ADBDGameState::SortOfferings()
{

}

bool ADBDGameState::ShouldDisplayMapName() const
{
	return false;
}

void ADBDGameState::SetupGameLoaded()
{

}

void ADBDGameState::SetKillerIntroCompletedNormalized(float completedAmount)
{

}

void ADBDGameState::SetGameSelectedOffering(const TArray<FSelectedOffering>& Offerings)
{

}

void ADBDGameState::SetGameLevelLoaded()
{

}

void ADBDGameState::SetDisplayMapName(bool Display)
{

}

void ADBDGameState::SetBuiltLevelData(const FName& ThemeName, const FName& ThemeWeather, const FString& MapName, int32 TileCount, const TArray<FDependency>& LevelDependencies)
{

}

void ADBDGameState::Server_UpdateGameRole()
{

}

void ADBDGameState::ResetGameLevelLoaded()
{

}

void ADBDGameState::RemoveTrap(AInteractable* toRemove)
{

}

void ADBDGameState::RemoveHeartbeatEmitter(AActor* toRemove)
{

}

void ADBDGameState::OnRep_SurvivorLeft(int32 OldValue)
{

}

void ADBDGameState::OnRep_SessionId(FGuid OldValue)
{

}

void ADBDGameState::OnRep_ServerJoiningData()
{

}

void ADBDGameState::OnRep_OnLevelReadyToPlay(bool OldValue)
{

}

void ADBDGameState::OnRep_OnLevelLoaded(bool OldValue)
{

}

void ADBDGameState::OnRep_HatchOpened(bool OldValue)
{

}

void ADBDGameState::OnRep_GamePresetData()
{

}

void ADBDGameState::OnRep_EscapeDoorActivated(bool OldValue)
{

}

void ADBDGameState::OnRep_ActivatedGeneratorCount(int32 OldValue)
{

}

void ADBDGameState::Multicast_UpdateCharacterFromGamePreset_Implementation(ADBDPlayerState_Menu* playerState_Menu, FGamePresetData GamePresetData)
{

}

void ADBDGameState::Multicast_SetServerLeftGame_Implementation(bool hasServerLeftGame)
{

}

void ADBDGameState::Multicast_SetKillerGoalPoints_Implementation(FUniqueNetIdRepl PlayerId, int32 killerGoals)
{

}

void ADBDGameState::Multicast_SetGameLevelLoaded_Implementation()
{

}

void ADBDGameState::Multicast_SetGameEnded_Implementation(bool hasServerLeftGame)
{

}

void ADBDGameState::Multicast_SetBuiltLevelData_Implementation(const FName& ThemeName, const FName& ThemeWeather, const FString& MapName, int32 TileCount, const TArray<FDependency>& LevelDependencies)
{

}

void ADBDGameState::Multicast_KillerServerSearchFailed_Implementation()
{

}

void ADBDGameState::Multicast_KillerSearchStarted_Implementation()
{

}

void ADBDGameState::Multicast_JoinKillerServerFailed_Implementation()
{

}

void ADBDGameState::KillerServerFound(const FServerJoiningData& joinParams)
{

}

bool ADBDGameState::IsPlayerDistributionReady() const
{
	return false;
}

bool ADBDGameState::IsOnePlayerLeft()
{
	return false;
}

bool ADBDGameState::IsOfferingReceived() const
{
	return false;
}

bool ADBDGameState::IsObsessionTargetAliveInLevel()
{
	return false;
}

bool ADBDGameState::IsObsessionTargetAlive()
{
	return false;
}

bool ADBDGameState::IsLightingGenerated() const
{
	return false;
}

bool ADBDGameState::IsLevelSetupDone() const
{
	return false;
}

bool ADBDGameState::IsLevelReadyToPlay() const
{
	return false;
}

bool ADBDGameState::IsIntroCompleted() const
{
	return false;
}

bool ADBDGameState::IsHatchVisible() const
{
	return false;
}

bool ADBDGameState::IsEscapeDoorOpen() const
{
	return false;
}

bool ADBDGameState::IsEscapeDoorActivated() const
{
	return false;
}

void ADBDGameState::IntroCompleted()
{

}

int32 ADBDGameState::GetSurvivorLeft() const
{
	return 0;
}

TArray<FSelectedOffering> ADBDGameState::GetSelectedOfferings() const
{
	return TArray<FSelectedOffering>();
}

bool ADBDGameState::GetSelectedOffering(int32 Index, FSelectedOffering& SelectedOffering) const
{
	return false;
}

int32 ADBDGameState::GetRemainingGeneratorsNeeded()
{
	return 0;
}

ADBDPlayerState* ADBDGameState::GetPlayerStateByIDString(const FString& ID) const
{
	return NULL;
}

void ADBDGameState::GetPlayerRoleCounts(int32& survivorCount, int32& killerCount, int32& SpectatorCount) const
{

}

int32 ADBDGameState::GetNumberOfOtherActiveSurvivors(ACamperPlayer* exception) const
{
	return 0;
}

int32 ADBDGameState::GetNumberOfActiveSurvivors() const
{
	return 0;
}

ADBDPlayerState* ADBDGameState::GetLocalPlayerState() const
{
	return NULL;
}

ADBDPlayer* ADBDGameState::GetLocalPlayerPawn() const
{
	return NULL;
}

APawn* ADBDGameState::GetLocalPlayerBasePawn() const
{
	return NULL;
}

FBuiltLevelData ADBDGameState::GetLevelData() const
{
	return FBuiltLevelData{};
}

float ADBDGameState::GetKillerIntroCompletedNormalized()
{
	return 0.0f;
}

int32 ADBDGameState::GetHeartbeatEmittersCount() const
{
	return 0;
}

bool ADBDGameState::GetGameLevelLoaded() const
{
	return false;
}

void ADBDGameState::Authority_SetSurvivorLeft(int32 survivorRemaining)
{

}

void ADBDGameState::Authority_SetServerJoiningData(const FServerJoiningData& joiningData)
{

}

void ADBDGameState::Authority_SetLevelReadyToPlay()
{

}

void ADBDGameState::Authority_SetHatchOpen(bool OPENED)
{

}

void ADBDGameState::Authority_SetAllPlayerLoaded()
{

}

void ADBDGameState::Authority_EvaluateObsessionTarget(ADBDPlayer* potentialTarget)
{

}

void ADBDGameState::Authority_EscapeThroughHatch()
{

}

void ADBDGameState::Authority_EnableObsession()
{

}

void ADBDGameState::AddTrap(AInteractable* toAdd)
{

}

void ADBDGameState::AddHeartbeatEmitter(AActor* toAdd)
{

}

void ADBDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADBDGameState, Slasher);
	DOREPLIFETIME(ADBDGameState, GeneratorNeeded);
	DOREPLIFETIME(ADBDGameState, CamperDeadCount);
	DOREPLIFETIME(ADBDGameState, CamperInMeatLockerCount);
	DOREPLIFETIME(ADBDGameState, CamperEscaped);
	DOREPLIFETIME(ADBDGameState, WaitingForEscape);
	DOREPLIFETIME(ADBDGameState, CamperCount);
	DOREPLIFETIME(ADBDGameState, IsGameEnded);
	DOREPLIFETIME(ADBDGameState, PlayersReadyToStart);
	DOREPLIFETIME(ADBDGameState, SecondsLeftInLobby);
	DOREPLIFETIME(ADBDGameState, _gameLevelLoaded);
	DOREPLIFETIME(ADBDGameState, _builtLevelData);
	DOREPLIFETIME(ADBDGameState, _levelOfferings);
	DOREPLIFETIME(ADBDGameState, _serverJoiningData);
	DOREPLIFETIME(ADBDGameState, _sessionId);
	DOREPLIFETIME(ADBDGameState, _activatedGeneratorCount);
	DOREPLIFETIME(ADBDGameState, _escapeDoorActivated);
	DOREPLIFETIME(ADBDGameState, _isHatchOpen);
	DOREPLIFETIME(ADBDGameState, _levelReadyToPlay);
	DOREPLIFETIME(ADBDGameState, _playerDistributionReady);
	DOREPLIFETIME(ADBDGameState, _usingWeakenedMechanic);
	DOREPLIFETIME(ADBDGameState, _survivorLeft);
	DOREPLIFETIME(ADBDGameState, _obsessionTarget);
	DOREPLIFETIME(ADBDGameState, _gamePresetData);
}

ADBDGameState::ADBDGameState()
{
	this->GeneratorNeeded = 0;
	this->CamperDeadCount = 0;
	this->CamperInMeatLockerCount = 0;
	this->CamperEscaped = 0;
	this->WaitingForEscape = 0;
	this->CamperCount = 0;
	this->LeaveSpectateRequested = false;
	this->IsGameEnded = false;
	this->PlayersReadyToStart = false;
	this->SecondsLeftInLobby = -1;
	this->HasServerLeftTheGame = false;
	this->_gameLevelLoaded = false;
	this->_activatedGeneratorCount = 0;
	this->_escapeDoorActivated = false;
	this->_isHatchOpen = false;
	this->_levelReadyToPlay = false;
	this->_playerDistributionReady = false;
	this->_usingWeakenedMechanic = false;
	this->_survivorLeft = 0;
	this->_camperEscapedThroughHatch = 0;
	this->_cachedObsessionEscaped = false;
}

#include "DBDGameInstance.h"
#include "UObject/NoExportTypes.h"

class UActorKnowledgeCollection;
class ADBDPlayer;
class AActor;
class USequencer;
class UMapActorDB;
class APlayerController;
class ULocalPlayer;
class ULightingInterpolator;
class ULightingHelper;
class ABaseSky;
class ACamperPlayer;
class ASlasherPlayer;
class ADBDPlayerState;

void UDBDGameInstance::StartOfferingSequence()
{

}

void UDBDGameInstance::SimulatePerk(TArray<FName> perks, int32 Level)
{

}

void UDBDGameInstance::SimulateOfferingSequence(const TArray<FName>& Offerings, bool playSequence)
{

}

void UDBDGameInstance::RemoveActorKnowledgeCollection(UActorKnowledgeCollection* collection)
{

}

bool UDBDGameInstance::IsActorKnown(ADBDPlayer* knowledgePossessor, AActor* possiblyKnownActor) const
{
	return false;
}

USequencer* UDBDGameInstance::GetSacrificeSequencer()
{
	return NULL;
}

int32 UDBDGameInstance::GetMaxSurvivorCount() const
{
	return 0;
}

UMapActorDB* UDBDGameInstance::GetMapActorDB() const
{
	return NULL;
}

APlayerController* UDBDGameInstance::GetLocalPlayerController() const
{
	return NULL;
}

ULocalPlayer* UDBDGameInstance::GetLocalPlayer() const
{
	return NULL;
}

ULightingInterpolator* UDBDGameInstance::GetLightInterpolator()
{
	return NULL;
}

ULightingHelper* UDBDGameInstance::GetLightingHelper()
{
	return NULL;
}

ABaseSky* UDBDGameInstance::GetLevelLighting()
{
	return NULL;
}

TArray<ACamperPlayer*> UDBDGameInstance::GetInGameSurvivors() const
{
	return TArray<ACamperPlayer*>();
}

TArray<ADBDPlayer*> UDBDGameInstance::GetInGamePlayers() const
{
	return TArray<ADBDPlayer*>();
}

TArray<ASlasherPlayer*> UDBDGameInstance::GetInGameKillers() const
{
	return TArray<ASlasherPlayer*>();
}

int32 UDBDGameInstance::GetDeadOrDisconnectedCampersCount(ADBDPlayerState* exception) const
{
	return 0;
}

void UDBDGameInstance::GenerateBloodWebForCurrentCharacter()
{

}

void UDBDGameInstance::ForceCloseGame()
{

}

void UDBDGameInstance::FireSoundEvent(AActor* Instigator, const FVector& Location, bool shouldTrack, float audibleRange, ADBDPlayer* instigatingPlayer, bool isQuickAction)
{

}

void UDBDGameInstance::EndLoadingScreen()
{

}

void UDBDGameInstance::DBDToggleLightingLoaded()
{

}

void UDBDGameInstance::DBDLogFindSessions(int32 searchType, int32 Rank, bool ignoreVersion)
{

}

void UDBDGameInstance::DBDForceSave()
{

}

void UDBDGameInstance::DBDForceLoad()
{

}

void UDBDGameInstance::DBDDeleteLocalSaveFile()
{

}

void UDBDGameInstance::DBDApplyOfferingModification(float lightModifier, float fogModifier)
{

}

void UDBDGameInstance::DBD_UpdateSteamInventory()
{

}

void UDBDGameInstance::DBD_ToggleOnlineRole()
{

}

void UDBDGameInstance::DBD_TestTickedDiceRoll(float BaseProbability, float ticks, float MultiplicativeModifier, float AdditiveModifier)
{

}

void UDBDGameInstance::DBD_TestSaveFile(const FString& Name)
{

}

void UDBDGameInstance::DBD_TestRegion()
{

}

void UDBDGameInstance::DBD_TestDiceRoll(float BaseProbability, float MultiplicativeModifier, float AdditiveModifier)
{

}

void UDBDGameInstance::DBD_StartQuickPlay()
{

}

void UDBDGameInstance::DBD_SetStatsUploadEnabled(bool Enabled)
{

}

void UDBDGameInstance::DBD_SetCursorStickySpeed(float Value)
{

}

void UDBDGameInstance::DBD_SetCursorStickIgnore(float Value)
{

}

void UDBDGameInstance::DBD_SetCursorDefaultSpeed(float Value)
{

}

void UDBDGameInstance::DBD_SetBonusEventsDate(const FString& dateString)
{

}

void UDBDGameInstance::DBD_SendFriendInvite()
{

}

void UDBDGameInstance::DBD_ResetInventory()
{

}

void UDBDGameInstance::DBD_ResetCurrentUserStats(bool andUpload, bool andUpdateData)
{

}

void UDBDGameInstance::DBD_ResetCurrentUserStat(FName StatName, bool andUpload)
{

}

void UDBDGameInstance::DBD_ResetAllRituals()
{

}

void UDBDGameInstance::DBD_RemoveRitual(FName toRemove)
{

}

void UDBDGameInstance::DBD_PrintAudioConfig() const
{

}

void UDBDGameInstance::DBD_LogBloodWebDefinition()
{

}

void UDBDGameInstance::DBD_LeaveMatch()
{

}

void UDBDGameInstance::DBD_GotoSplashScreen(bool showDefaultDialogueOnSplashScreen)
{

}

void UDBDGameInstance::DBD_GetRegion()
{

}

void UDBDGameInstance::DBD_GenerateNewRitual(FName Name)
{

}

void UDBDGameInstance::DBD_GenerateNewRandomRitual()
{

}

void UDBDGameInstance::DBD_GenerateBloodWeb(int32 Level)
{

}

void UDBDGameInstance::DBD_DumpSessions()
{

}

void UDBDGameInstance::DBD_DisplayCurrentUserStat(FName StatName)
{

}

void UDBDGameInstance::DBD_DestroySteamInventory()
{

}

void UDBDGameInstance::DBD_ClearInventory()
{

}

void UDBDGameInstance::DBD_CheckForNewContent() const
{

}

void UDBDGameInstance::DBD_ChangeSteamOverlayPosition(int32 Position, int32 VerticalOffset, int32 horizontalOffset)
{

}

void UDBDGameInstance::DBD_AssignNewRitual()
{

}

void UDBDGameInstance::DBD_AnalyticsTest()
{

}

void UDBDGameInstance::DBD_AnalyticsReset()
{

}

void UDBDGameInstance::DBD_AddToStat(FName StatName, float amount)
{

}

void UDBDGameInstance::DBD_AddToRitual(int32 Index, float Value)
{

}

void UDBDGameInstance::DBD_AddSlasherSkulls(int32 Count)
{

}

void UDBDGameInstance::DBD_AddSkulls(int32 camperSkullsCount, int32 slasherSkullsCount)
{

}

void UDBDGameInstance::DBD_AddPerkToInventory(FName perkID)
{

}

void UDBDGameInstance::DBD_AddOfferingToInventory(FName ItemId)
{

}

void UDBDGameInstance::DBD_AddItemToInventory(FName ItemId)
{

}

void UDBDGameInstance::DBD_AddItemAddonToInventory(FName ItemAddonID)
{

}

void UDBDGameInstance::DBD_AddFearTokens(int32 Count)
{

}

void UDBDGameInstance::DBD_AddEverythingToInventory()
{

}

void UDBDGameInstance::DBD_AddCamperSkulls(int32 Count)
{

}

void UDBDGameInstance::DBD_AddBloodPoints(int32 Count)
{

}

void UDBDGameInstance::DBD_AddAllPerksToInventory(int32 amount)
{

}

void UDBDGameInstance::DBD_AddAllItemsAddonsOfferingsToInventory(int32 amount)
{

}

void UDBDGameInstance::BeginLoadingScreen(const FString& MapName)
{

}

void UDBDGameInstance::AddActorKnowledgeCollection(UActorKnowledgeCollection* collection)
{

}

UDBDGameInstance::UDBDGameInstance()
{
	this->DesignTunableClass = NULL;
	this->RegionClass = NULL;
	this->LeaveGameRequested = false;
	this->LockOutTimer = 0.000000;
	this->GenerationSeed = 0;
	this->PaperTileAssetName = TEXT("");
	this->PlayerProfileLoadFailed = false;
	this->SeasonEnded = false;
	this->PendingReportSteamId = TEXT("");
	this->PendingReportPlayerName = TEXT("");
	this->PendingReportPlayerRole = EPlayerRole::VE_None;
	this->MatchRateValue = 0;
	this->ShouldRequestBonusPointsEvents = true;
	this->DisconnectError = EDisconnectErrors::None;
	this->LastAuthOrEACErrorCode = 0;
	this->AuthOrEACErrorAdditionalInfo = TEXT("");
	this->HasServerLeftMatch = false;
	this->HasWrittenGameEndStats = false;
	this->_networkFailureDetected = false;
	this->_networkConnectionMonitoringOnly = false;
	this->_achievementStatsUploadEnabled = true;
	this->_isDisconnected = false;
	this->_isMirrorsSessionKilled = false;
	this->_isGamepadReconnected = false;
	this->_isProfileOffline = false;
	this->_isAppSuspended = false;
	this->_isAppResumed = false;
	this->_isInvitationRejected = false;
	this->_isAdditionalContentInstalled = false;
	this->_legalTermsStatus = ELegalTermsStatus::None;
}

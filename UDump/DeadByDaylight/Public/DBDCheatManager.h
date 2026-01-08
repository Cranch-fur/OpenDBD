#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "DBDCheatManager.generated.h"

UCLASS(Blueprintable)
class UDBDCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_VisualizeHookRenderRegions();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ToggleProceduralDebugMode();

	UFUNCTION(Exec)
	void DBD_TeleportToTile(uint32 X, uint32 Y);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_Teleport(float X, float Y, float Z);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SpectateSurvivor();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SpectateKiller();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SpawnStatusEffect(FName statusEffectID);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SpawnPerk(FName perkID, int32 perkLevel);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SpawnItemAddon(FName addonID);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SpawnItem(FName ItemId);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ShowKillerPowerDebugInfo(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetWindowFreeCam(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetUserExperience(int32 Exp);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetUserBonusExperience(int32 Exp);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetSurvivorLeftCount(int32 Count);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetSlasherAutoAttackOnCamperInteractFrameDelay(int32 frames);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetRunVaultEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetReverseTraverseEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetRandomInputAmplitude(float Amplitude);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetPerchRadiusThreshold(float Threshold);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetOnHookDrainToNextStage(bool onHookDrainToNextStage);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetMaxWalkSpeed(float MaxWalkSpeed);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetLungeType(int32 lungeType);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetKillerHeadVisibility(bool Visible);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetItemNormalizedEnergyLevel(float normalizedLevel);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetItemEnergyLevel(float Level);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetHatchetVar(FName VarName, float Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetEthereal(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetEnableHookSlashableZone(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetDebugSnapPoint(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetDebugFailAllInteractions(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetDebugCarry(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetCustomizationMesh(FName ItemName);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetContinuousPrintState(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetContinuousPrintDebug(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetAutoItemInteractEnabled(bool Enabled, float Duration, float Delay);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetAutoInteractEnabled(bool Enabled, float Duration, float Delay);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetAuthoritativeMovement(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetActiveGeneratorCount(int32 Count);

	UFUNCTION(Exec)
	void DBD_SelectSlasher(uint32 slasherIndex);

	UFUNCTION(Exec)
	void DBD_SelectCamper(uint32 camperIndex);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ResistentCamper();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ResetSeason();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ResetMeshRelativeOffSet();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ResetExperience();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ResetCamera();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_RemoveHP();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_RemoveAllPerks();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_Ragdoll(FName RootBone, float Blend);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ProcessOffering();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_PrintSteamBuildIds();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_PrintCurrentStateOnce();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_PlayMontage(FName MontageID);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OptionToggleUseHeadphones();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OptionToggleMuteOnFocusLost();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OptionToggleMuteMusic();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OptionToggleInvertY();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OptionToggleFullScreen();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OptionToggleAutoAdjust();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OptionSetResolution(int32 Resolution);

	UFUNCTION(Exec)
	void DBD_OptionSetQualityLevel(uint32 qualityLevel);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OptionSetMainVolume(int32 Volume);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OptionSetLanguage(const FString& lang);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_OpenLogFolder();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_NewAuthToken();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_Mute(bool mute);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListStatusEffects();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListPerks();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListItemAddons();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListInventory();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListInteractionInZone();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListDBStatusEffects();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListDBPerks();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListDBOfferings();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListDBItems();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListDBItemAddons();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListDBCustomizations();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListAvailableInteractions();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListAllStatusEffects();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListAllPerks();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ListAllItemAddons();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_LevelUp();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_Kill(bool sacrificed);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ItsAlive(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_IsServerPositionUpdatesEnabled();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_HookEscapeAutoSuccess(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_GenerateAuthToken();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ForceSkillChecks(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ForceFlee(bool toward);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ForceDisableSkillChecks(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ForceDisableFootIK(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_EvaluateBadges();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_EnsureFail();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_EndGame();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_EnableStealth(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_EnableServerPositionUpdates(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_EnableDebugUnavailableInteractions(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_EnableCapsuleDynamicResize(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DispatchOnPostItemAddonsCreation();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DebugSetCustomizationMesh(FName ItemName);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DebugPrintInteractionsInZone();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DebugIsInterruptable(bool interruptable);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DebugInteractionPriority(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_CrashGameWithNullPtr();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_CrashGameWithCheck();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ConsumeBloodwebs(int32 initialLevel, int32 Count);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_CompleteEscapeRequirements();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_CheatChainsaw(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AutoWiggle();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AlwaysPounce(bool Enabled);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddOffering(FName Name);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_AddHP();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ActivateWeakenedMechanic(bool On);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_ActivateEscapeDoor();

public:
	UDBDCheatManager();
};

FORCEINLINE uint32 GetTypeHash(const UDBDCheatManager) { return 0; }

class __base(ADBDPlayerControllerBase, 0) ADBDPlayerController_Menu
{
/*0x0000*/  struct ADBDPlayerController_Menu::VTable* vtable;
/*0x0008*/  __inherited enum EObjectFlags UObjectBase::ObjectFlags;
/*0x000c*/  __inherited int32_t UObjectBase::InternalIndex;
/*0x0010*/  __inherited class UClass* UObjectBase::ClassPrivate;
/*0x0018*/  __inherited class FName UObjectBase::NamePrivate;
/*0x0020*/  __inherited class UObject* UObjectBase::OuterPrivate;
/*0x0028*/  __inherited struct FActorTickFunction AActor::PrimaryActorTick;
/*0x0078*/  __inherited float AActor::CustomTimeDilation;
/*0x007c*/  __inherited union
            {
              uint8_t bHidden;
              uint8_t bNetTemporary;
              uint8_t bNetStartup;
              uint8_t bOnlyRelevantToOwner;
              uint8_t bAlwaysRelevant;
              uint8_t bReplicateMovement;
              uint8_t bTearOff;
              uint8_t bExchangedRoles;
            } __bitfield7c;
/*0x007d*/  __inherited union
            {
              uint8_t bPendingNetUpdate;
              uint8_t bNetLoadOnClient;
              uint8_t bNetUseOwnerRelevancy;
              uint8_t bBlockInput;
              uint8_t bRunningUserConstructionScript;
              uint8_t bAllowTickBeforeBeginPlay;
              uint8_t bHasFinishedSpawning;
              uint8_t bTickFunctionsRegistered;
            } __bitfield7d;
/*0x007e*/  __inherited union
            {
              uint8_t bActorEnableCollision;
              uint8_t bNetCheckedInitialPhysicsState;
              uint8_t bReplicates;
            } __bitfield7e;
            __padding char _7f[1];
/*0x0080*/  __inherited class FName AActor::NetDriverName;
/*0x0088*/  __inherited class TEnumAsByte<enum ENetRole> AActor::RemoteRole;
/*0x0090*/  __inherited class AActor* AActor::Owner;
/*0x0098*/  __inherited struct FRepMovement AActor::ReplicatedMovement;
            __padding char _cc[4];
/*0x00d0*/  __inherited struct FRepAttachment AActor::AttachmentReplication;
/*0x0110*/  __inherited class TEnumAsByte<enum ENetRole> AActor::Role;
/*0x0111*/  __inherited class TEnumAsByte<enum ENetDormancy> AActor::NetDormancy;
/*0x0112*/  __inherited class TEnumAsByte<enum EAutoReceiveInput::Type> AActor::AutoReceiveInput;
/*0x0114*/  __inherited int32_t AActor::InputPriority;
/*0x0118*/  __inherited class UInputComponent* AActor::InputComponent;
/*0x0120*/  __inherited class TEnumAsByte<enum EInputConsumeOptions> AActor::InputConsumeOption_DEPRECATED;
/*0x0124*/  __inherited float AActor::NetCullDistanceSquared;
/*0x0128*/  __inherited int32_t AActor::NetTag;
/*0x012c*/  __inherited float AActor::NetUpdateTime;
/*0x0130*/  __inherited float AActor::NetUpdateFrequency;
/*0x0134*/  __inherited float AActor::MinNetUpdateFrequency;
/*0x0138*/  __inherited float AActor::NetPriority;
/*0x013c*/  __inherited float AActor::LastNetUpdateTime;
/*0x0140*/  __inherited union
            {
              uint8_t bAutoDestroyWhenFinished;
              uint8_t bCanBeDamaged;
              uint8_t bActorIsBeingDestroyed;
              uint8_t bCollideWhenPlacing;
              uint8_t bFindCameraComponentWhenViewTarget;
              uint8_t bRelevantForNetworkReplays;
              uint8_t bGenerateOverlapEventsDuringLevelStreaming;
            } __bitfield140;
/*0x0141*/  __inherited enum ESpawnActorCollisionHandlingMethod AActor::SpawnCollisionHandlingMethod;
/*0x0144*/  __inherited float AActor::CreationTime;
/*0x0148*/  __inherited class APawn* AActor::Instigator;
/*0x0150*/  __inherited class TArray<AActor *,FDefaultAllocator> AActor::Children;
/*0x0160*/  __inherited class USceneComponent* AActor::RootComponent;
/*0x0168*/  __inherited class TArray<AMatineeActor*, FDefaultAllocator> AActor::ControllingMatineeActors;
/*0x0178*/  __inherited float AActor::InitialLifeSpan;
/*0x017c*/  __padding char _17c[4];
/*0x0180*/  __inherited struct FTimerHandle AActor::TimerHandle_LifeSpanExpired;
/*0x0188*/  __inherited class TArray<FName, FDefaultAllocator> AActor::Layers;
/*0x0198*/  __inherited struct TWeakObjectPtr<AActor, FWeakObjectPtr> AActor::ParentComponentActor_DEPRECATED;
/*0x01a0*/  __inherited struct TWeakObjectPtr<UChildActorComponent, FWeakObjectPtr> AActor::ParentComponent;
/*0x01a8*/  __inherited union
            {
              uint8_t bAllowReceiveTickEventOnDedicatedServer;
              uint8_t bActorInitialized;
              enum AActor::EActorBeginPlayState ActorHasBegunPlay;
              uint8_t bActorSeamlessTraveled;
              uint8_t bIgnoresOriginShifting;
              uint8_t bEnableAutoLODGeneration;
            } __bitfield1a8;
            __padding char _1a9[7];
/*0x01b0*/  __inherited class TArray<FName, FDefaultAllocator> AActor::Tags;
/*0x01c0*/  __inherited uint64_t AActor::HiddenEditorViews;
/*0x01c8*/  __inherited class FTakeAnyDamageSignature AActor::OnTakeAnyDamage;
/*0x01d8*/  __inherited class FTakePointDamageSignature AActor::OnTakePointDamage;
/*0x01e8*/  __inherited class FActorBeginOverlapSignature AActor::OnActorBeginOverlap;
/*0x01f8*/  __inherited class FActorEndOverlapSignature AActor::OnActorEndOverlap;
/*0x0208*/  __inherited class FActorBeginCursorOverSignature AActor::OnBeginCursorOver;
/*0x0218*/  __inherited class FActorEndCursorOverSignature AActor::OnEndCursorOver;
/*0x0228*/  __inherited class FActorOnClickedSignature AActor::OnClicked;
/*0x0238*/  __inherited class FActorOnReleasedSignature AActor::OnReleased;
/*0x0248*/  __inherited class FActorOnInputTouchBeginSignature AActor::OnInputTouchBegin;
/*0x0258*/  __inherited class FActorOnInputTouchEndSignature AActor::OnInputTouchEnd;
/*0x0268*/  __inherited class FActorBeginTouchOverSignature AActor::OnInputTouchEnter;
/*0x0278*/  __inherited class FActorEndTouchOverSignature AActor::OnInputTouchLeave;
/*0x0288*/  __inherited class FActorHitSignature AActor::OnActorHit;
/*0x0298*/  __inherited class FActorDestroyedSignature AActor::OnDestroyed;
/*0x02a8*/  __inherited class FActorEndPlaySignature AActor::OnEndPlay;
/*0x02b8*/  __inherited class TSet<UActorComponent*, DefaultKeyFuncs<UActorComponent*, 0>,FDefaultSetAllocator> AActor::OwnedComponents;
/*0x0308*/  __inherited class TSet<UActorComponent*, DefaultKeyFuncs<UActorComponent*, 0>,FDefaultSetAllocator> AActor::ReplicatedComponents;
/*0x0358*/  __inherited class TArray<UActorComponent*, FDefaultAllocator> AActor::BlueprintCreatedComponents;
/*0x0368*/  __inherited class TArray<UActorComponent*, FDefaultAllocator> AActor::InstanceComponents;
/*0x0378*/  __inherited class FRenderCommandFence AActor::DetachFence;
/*0x0380*/  __inherited struct ADebugCameraController::APlayerController::AController::INavAgentInterface::VTable* INavAgentInterface::vtable;
/*0x0388*/  __inherited class APawn* AController::Pawn;
/*0x0390*/  __inherited struct TWeakObjectPtr<APawn,FWeakObjectPtr> AController::OldPawn;
/*0x0398*/  __inherited class ACharacter* AController::Character;
/*0x03a0*/  __inherited class APlayerState* AController::PlayerState;
/*0x03a8*/  __inherited class USceneComponent* AController::TransformComponent;
/*0x03b0*/  __inherited struct FRotator AController::ControlRotation;
/*0x03bc*/  __inherited union
            {
                uint32_t bAttachToPawn;
                uint32_t bIsPlayerController;
            } __bitfield3bc;
/*0x03c0*/  __inherited bool AController::IgnoreMoveInput;
/*0x03c1*/  __inherited bool AController::IgnoreLookInput;
            __padding char _3c2[2];
/*0x03c4*/  __inherited struct TWeakObjectPtr<AActor,FWeakObjectPtr> AController::StartSpot;
            __padding char _3cc[4];
/*0x03d0*/  __inherited class FName AController::StateName;
/*0x03d8*/  __inherited class FInstigatedAnyDamageSignature AController::OnInstigatedAnyDamage;
/*0x03e8*/  __inherited class UPlayer* APlayerController::Player;
/*0x03f0*/  __inherited union
            {
                uint32_t bShortConnectTimeOut;
            } __bitfield3f0;
/*0x03f8*/  __inherited class APawn* APlayerController::AcknowledgedPawn;
/*0x0400*/  __inherited class UInterpTrackInstDirector* APlayerController::ControllingDirTrackInst;
/*0x0408*/  __inherited float APlayerController::LocalPlayerCachedLODDistanceFactor;
/*0x0410*/  __inherited class AHUD* APlayerController::MyHUD;
/*0x0418*/  __inherited class APlayerCameraManager* APlayerController::PlayerCameraManager;
/*0x0420*/  __inherited class TSubclassOf<APlayerCameraManager> APlayerController::PlayerCameraManagerClass;
/*0x0428*/  __inherited bool APlayerController::bAutoManageActiveCameraTarget;
            __padding char _429[3];
/*0x042c*/  __inherited struct FRotator APlayerController::TargetViewRotation;
/*0x0438*/  __inherited struct FRotator APlayerController::BlendedTargetViewRotation;
/*0x0444*/  __inherited float APlayerController::SmoothTargetViewRotationSpeed;
/*0x0448*/  __inherited class TArray<AActor *,FDefaultAllocator> APlayerController::HiddenActors;
/*0x0458*/  __inherited float APlayerController::LastSpectatorStateSynchTime;
/*0x045c*/  __inherited struct FVector APlayerController::LastSpectatorSyncLocation;
/*0x0468*/  __inherited struct FRotator APlayerController::LastSpectatorSyncRotation;
/*0x0474*/  __inherited int32_t APlayerController::ClientCap;
/*0x0478*/  __inherited class UCheatManager* APlayerController::CheatManager;
/*0x0480*/  __inherited class TSubclassOf<UCheatManager> APlayerController::CheatClass;
/*0x0488*/  __inherited class UPlayerInput* APlayerController::PlayerInput;
/*0x0490*/  __inherited class TArray<FActiveForceFeedbackEffect,FDefaultAllocator> APlayerController::ActiveForceFeedbackEffects;
/*0x04a0*/  __inherited class TMap<int,FDynamicForceFeedbackDetails,FDefaultSetAllocator,TDefaultMapKeyFuncs<int,FDynamicForceFeedbackDetails,0> > APlayerController::DynamicForceFeedbacks;
/*0x04f0*/  __inherited class TSharedPtr<FActiveHapticFeedbackEffect,0> APlayerController::ActiveHapticEffect_Left;
/*0x0500*/  __inherited class TSharedPtr<FActiveHapticFeedbackEffect,0> APlayerController::ActiveHapticEffect_Right;
/*0x0510*/  __inherited class TArray<FName,FDefaultAllocator> APlayerController::PendingMapChangeLevelNames;
/*0x0520*/  __inherited union
            {
                uint32_t bCinematicMode;
                uint32_t bHidePawnInCinematicMode;
                uint32_t bIsUsingStreamingVolumes;
                uint32_t bPlayerIsWaiting;
            } __bitfield520;
/*0x0524*/  __inherited uint8_t APlayerController::NetPlayerIndex;
            __padding char _525[3];
/*0x0528*/  __inherited struct FPlayerMuteList APlayerController::MuteList;
/*0x0560*/  __inherited class UNetConnection* APlayerController::PendingSwapConnection;
/*0x0568*/  __inherited class UNetConnection* APlayerController::NetConnection;
/*0x0570*/  __inherited struct FRotator APlayerController::RotationInput;
/*0x057c*/  __inherited float APlayerController::InputYawScale;
/*0x0580*/  __inherited float APlayerController::InputPitchScale;
/*0x0584*/  __inherited float APlayerController::InputRollScale;
/*0x0588*/  __inherited union
            {
                uint32_t bShowMouseCursor;
                uint32_t bEnableClickEvents;
                uint32_t bEnableTouchEvents;
                uint32_t bEnableMouseOverEvents;
                uint32_t bEnableTouchOverEvents;
                uint32_t bForceFeedbackEnabled;
            } __bitfield588;
            __padding char _58c[4];
/*0x0590*/  __inherited class TArray<FKey,FDefaultAllocator> APlayerController::ClickEventKeys;
/*0x05a0*/  __inherited class TEnumAsByte<enum EMouseCursor::Type> APlayerController::DefaultMouseCursor;
/*0x05a1*/  __inherited class TEnumAsByte<enum EMouseCursor::Type> APlayerController::CurrentMouseCursor;
/*0x05a2*/  __inherited class TEnumAsByte<enum ECollisionChannel> APlayerController::DefaultClickTraceChannel;
/*0x05a3*/  __inherited class TEnumAsByte<enum ECollisionChannel> APlayerController::CurrentClickTraceChannel;
/*0x05a4*/  __inherited float APlayerController::HitResultTraceDistance;
/*0x05a8*/  __inherited struct FForceFeedbackValues APlayerController::ForceFeedbackValues;
/*0x05b8*/  __inherited struct TWeakObjectPtr<UPrimitiveComponent,FWeakObjectPtr> APlayerController::CurrentClickablePrimitive;
/*0x05c0*/  __inherited struct TWeakObjectPtr<UPrimitiveComponent,FWeakObjectPtr> APlayerController::CurrentTouchablePrimitives[0xb];
/*0x0618*/  __inherited class TArray<TWeakObjectPtr<UInputComponent,FWeakObjectPtr>,FDefaultAllocator> APlayerController::CurrentInputStack;
/*0x0628*/  __inherited class UInputComponent* APlayerController::InactiveStateInputComponent;
/*0x0630*/  __inherited union
            {
                uint32_t bCinemaDisableInputMove;
                uint32_t bCinemaDisableInputLook;
                uint32_t bInputEnabled;
                uint32_t bShouldPerformFullTickWhenPaused;
            } __bitfield630;
            __padding char _634[4];
/*0x0638*/  __inherited class TSharedPtr<SVirtualJoystick,0> APlayerController::VirtualJoystick;
/*0x0648*/  __inherited class UTouchInterface* APlayerController::CurrentTouchInterface;
/*0x0650*/  __inherited struct FTimerHandle APlayerController::TimerHandle_UnFreeze;
/*0x0658*/  __inherited struct FTimerHandle APlayerController::TimerHandle_DelayedPrepareMapChange;
/*0x0660*/  __inherited struct FTimerHandle APlayerController::TimerHandle_ClientCommitMapChange;
/*0x0668*/  __inherited union
            {
                uint32_t bOverrideAudioListener;
            } __bitfield668;
/*0x066c*/  __inherited struct TWeakObjectPtr<USceneComponent,FWeakObjectPtr> APlayerController::AudioListenerComponent;
/*0x0674*/  __inherited struct FVector APlayerController::AudioListenerLocationOverride;
/*0x0680*/  __inherited struct FRotator APlayerController::AudioListenerRotationOverride;
/*0x0690*/  __inherited class ASpectatorPawn* APlayerController::SpectatorPawn;
/*0x0698*/  __inherited struct FVector APlayerController::SpawnLocation;
/*0x06a4*/  __inherited float APlayerController::LastRetryPlayerTime;
/*0x06a8*/  __inherited bool APlayerController::bIsLocalPlayerController;
/*0x06aa*/  __inherited uint16_t APlayerController::SeamlessTravelCount;
/*0x06ac*/  __inherited uint16_t APlayerController::LastCompletedSeamlessTravelCount;
/*0x06b0*/  __inherited class UFearMarketManager* ADBDPlayerControllerBase::FearMarketManager;
            __padding char _6b8[8];
/*0x06c0*/  __inherited class TMulticastDelegate<void, bool> ADBDPlayerControllerBase::OnFearMerketDataRecievedUI;
/*0x0730*/  __inherited class TMulticastDelegate<void, bool> ADBDPlayerControllerBase::OnCloudLoadStatsComplete;
/*0x07a0*/  __inherited class TMulticastDelegate<void, bool> ADBDPlayerControllerBase::OnMirrorCurrenciesLoadComplete;
/*0x0810*/  __inherited bool ADBDPlayerControllerBase::_playerDataRequested;
            __padding char _811[7];
/*0x0818*/  __inherited class FDelegateHandle ADBDPlayerControllerBase::_getTimeHandle;
/*0x0820*/  __inherited class FDelegateHandle ADBDPlayerControllerBase::_statInitHandle;
/*0x0828*/  __inherited class FDelegateHandle ADBDPlayerControllerBase::_statReadHandle;
/*0x0830*/  __inherited class FDelegateHandle ADBDPlayerControllerBase::_statUploadHandle;
/*0x0838*/  __inherited class FDelegateHandle ADBDPlayerControllerBase::_getCurrencyHandle;
/*0x0840*/  __inherited class ADBDPlayerControllerBase::FOnKeyPressedEvent ADBDPlayerControllerBase::_onKeyPressedEvent;
/*0x08b0*/  __inherited bool ADBDPlayerControllerBase::_hasFinishedPlaying;
/*0x08b4*/  __inherited float ADBDPlayerControllerBase::_slasherGamepadScale;
/*0x08b8*/  __inherited float ADBDPlayerControllerBase::_slasherMouseScale;
/*0x08bc*/  __inherited float ADBDPlayerControllerBase::_camperGamepadScale;
/*0x08c0*/  __inherited float ADBDPlayerControllerBase::_camperMouseScale;
/*0x08c4*/  __inherited bool ADBDPlayerControllerBase::_uploadStatsNeeded;
};

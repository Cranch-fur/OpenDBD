class __base(AGameMode, 0) ADBDParadiseGameMode
{
    struct ADBDParadiseGameMode::VTable* vtable;
    __inherited enum EObjectFlags UObjectBase::ObjectFlags;
    __inherited int32_t UObjectBase::InternalIndex;
    __inherited class UClass* UObjectBase::ClassPrivate;
    __inherited class FName UObjectBase::NamePrivate;
    __inherited class UObject* UObjectBase::OuterPrivate;
    __inherited struct FActorTickFunction AActor::PrimaryActorTick;
    __inherited float AActor::CustomTimeDilation;
    __inherited union
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
    __inherited union
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
    __inherited union
    {
        uint8_t bActorEnableCollision;
        uint8_t bNetCheckedInitialPhysicsState;
        uint8_t bReplicates;
    } __bitfield7e;
    __padding char _7f[1];
    __inherited class FName AActor::NetDriverName;
    __inherited class TEnumAsByte<enum ENetRole> AActor::RemoteRole;
    __inherited class AActor* AActor::Owner;
    __inherited struct FRepMovement AActor::ReplicatedMovement;
    __padding char _cc[4];
    __inherited struct FRepAttachment AActor::AttachmentReplication;
    __inherited class TEnumAsByte<enum ENetRole> AActor::Role;
    __inherited class TEnumAsByte<enum ENetDormancy> AActor::NetDormancy;
    __inherited class TEnumAsByte<enum EAutoReceiveInput::Type> AActor::AutoReceiveInput;
    __inherited int32_t AActor::InputPriority;
    __inherited class UInputComponent* AActor::InputComponent;
    __inherited class TEnumAsByte<enum EInputConsumeOptions> AActor::InputConsumeOption_DEPRECATED;
    __inherited float AActor::NetCullDistanceSquared;
    __inherited int32_t AActor::NetTag;
    __inherited float AActor::NetUpdateTime;
    __inherited float AActor::NetUpdateFrequency;
    __inherited float AActor::MinNetUpdateFrequency;
    __inherited float AActor::NetPriority;
    __inherited float AActor::LastNetUpdateTime;
    __inherited union
    {
        uint8_t bAutoDestroyWhenFinished;
        uint8_t bCanBeDamaged;
        uint8_t bActorIsBeingDestroyed;
        uint8_t bCollideWhenPlacing;
        uint8_t bFindCameraComponentWhenViewTarget;
        uint8_t bRelevantForNetworkReplays;
        uint8_t bGenerateOverlapEventsDuringLevelStreaming;
    } __bitfield140;
    __inherited enum ESpawnActorCollisionHandlingMethod AActor::SpawnCollisionHandlingMethod;
    __inherited float AActor::CreationTime;
    __inherited class APawn* AActor::Instigator;
    __inherited class TArray<AActor*, FDefaultAllocator> AActor::Children;
    __inherited class USceneComponent* AActor::RootComponent;
    __inherited class TArray<AMatineeActor*, FDefaultAllocator> AActor::ControllingMatineeActors;
    __inherited float AActor::InitialLifeSpan;
    __padding char _17c[4];
    __inherited struct FTimerHandle AActor::TimerHandle_LifeSpanExpired;
    __inherited class TArray<FName, FDefaultAllocator> AActor::Layers;
    __inherited struct TWeakObjectPtr<AActor, FWeakObjectPtr> AActor::ParentComponentActor_DEPRECATED;
    __inherited struct TWeakObjectPtr<UChildActorComponent, FWeakObjectPtr> AActor::ParentComponent;
    __inherited union
    {
        uint8_t bAllowReceiveTickEventOnDedicatedServer;
        uint8_t bActorInitialized;
        enum AActor::EActorBeginPlayState ActorHasBegunPlay;
        uint8_t bActorSeamlessTraveled;
        uint8_t bIgnoresOriginShifting;
        uint8_t bEnableAutoLODGeneration;
    } __bitfield1a8;
    __padding char _1a9[7];
    __inherited class TArray<FName, FDefaultAllocator> AActor::Tags;
    __inherited uint64_t AActor::HiddenEditorViews;
    __inherited class FTakeAnyDamageSignature AActor::OnTakeAnyDamage;
    __inherited class FTakePointDamageSignature AActor::OnTakePointDamage;
    __inherited class FActorBeginOverlapSignature AActor::OnActorBeginOverlap;
    __inherited class FActorEndOverlapSignature AActor::OnActorEndOverlap;
    __inherited class FActorBeginCursorOverSignature AActor::OnBeginCursorOver;
    __inherited class FActorEndCursorOverSignature AActor::OnEndCursorOver;
    __inherited class FActorOnClickedSignature AActor::OnClicked;
    __inherited class FActorOnReleasedSignature AActor::OnReleased;
    __inherited class FActorOnInputTouchBeginSignature AActor::OnInputTouchBegin;
    __inherited class FActorOnInputTouchEndSignature AActor::OnInputTouchEnd;
    __inherited class FActorBeginTouchOverSignature AActor::OnInputTouchEnter;
    __inherited class FActorEndTouchOverSignature AActor::OnInputTouchLeave;
    __inherited class FActorHitSignature AActor::OnActorHit;
    __inherited class FActorDestroyedSignature AActor::OnDestroyed;
    __inherited class FActorEndPlaySignature AActor::OnEndPlay;
    __inherited class TSet<UActorComponent*, DefaultKeyFuncs<UActorComponent*, 0>, FDefaultSetAllocator> AActor::OwnedComponents;
    __inherited class TSet<UActorComponent*, DefaultKeyFuncs<UActorComponent*, 0>, FDefaultSetAllocator> AActor::ReplicatedComponents;
    __inherited class TArray<UActorComponent*, FDefaultAllocator> AActor::BlueprintCreatedComponents;
    __inherited class TArray<UActorComponent*, FDefaultAllocator> AActor::InstanceComponents;
    __inherited class FRenderCommandFence AActor::DetachFence;
    __inherited class FName AGameMode::MatchState;
    __inherited union
    {
        uint32_t bUseSeamlessTravel;
        uint32_t bPauseable;
        uint32_t bStartPlayersAsSpectators;
        uint32_t bDelayedStart;
    } __bitfield388;
    __padding char _38c[4];
    __inherited class FString AGameMode::OptionsString;
    __inherited class TSubclassOf<APawn> AGameMode::DefaultPawnClass;
    __inherited class TSubclassOf<AHUD> AGameMode::HUDClass;
    __inherited int32_t AGameMode::NumSpectators;
    __inherited int32_t AGameMode::NumPlayers;
    __inherited int32_t AGameMode::NumBots;
    __inherited float AGameMode::MinRespawnDelay;
    __inherited class AGameSession* AGameMode::GameSession;
    __inherited int32_t AGameMode::NumTravellingPlayers;
    __inherited int32_t AGameMode::CurrentID;
    __inherited class FText AGameMode::DefaultPlayerName;
    __inherited class TSubclassOf<ULocalMessage> AGameMode::EngineMessageClass;
    __inherited class TSubclassOf<APlayerController> AGameMode::PlayerControllerClass;
    __inherited class TSubclassOf<ASpectatorPawn> AGameMode::SpectatorClass;
    __inherited class TSubclassOf<APlayerController> AGameMode::ReplaySpectatorPlayerControllerClass;
    __inherited class TSubclassOf<APlayerState> AGameMode::PlayerStateClass;
    __inherited class TSubclassOf<AGameState> AGameMode::GameStateClass;
    __inherited class AGameState* AGameMode::GameState;
    __inherited class TArray<APlayerState*, FDefaultAllocator> AGameMode::InactivePlayerArray;
    __inherited class TArray<TBaseDelegate<bool>, FDefaultAllocator> AGameMode::Pausers;
    __inherited class TArray<FGameClassShortName, FDefaultAllocator> AGameMode::GameModeClassAliases;
    __inherited float AGameMode::InactivePlayerStateLifeSpan;
    __inherited uint8_t AGameMode::bHandleDedicatedServerReplays;
    __padding char _455[3];
};

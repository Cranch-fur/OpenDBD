class __base(ASpectatorPawn, 0) ADBDSpectator
{
/*0x0000*/  struct ADBDPlayer::VTable* vtable;
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
/*0x0388*/  __inherited union
            {
              uint32_t bUseControllerRotationPitch;
              uint32_t bUseControllerRotationYaw;
              uint32_t bUseControllerRotationRoll;
              uint32_t bCanAffectNavigationGeneration;
              uint32_t bInputEnabled;
            } __bitfield388;
/*0x038c*/  __inherited float APawn::BaseEyeHeight;
/*0x0390*/  __inherited class TEnumAsByte<enum EAutoReceiveInput::Type> APawn::AutoPossessPlayer;
/*0x0391*/  __inherited enum EAutoPossessAI APawn::AutoPossessAI;
            __padding char _392[6];
/*0x0398*/  __inherited class TSubclassOf<AController> APawn::AIControllerClass;
/*0x03a0*/  __inherited class APlayerState* APawn::PlayerState;
/*0x03a8*/  __inherited uint8_t APawn::RemoteViewPitch;
/*0x03b0*/  __inherited class AController* APawn::LastHitBy;
/*0x03b8*/  __inherited class AController* APawn::Controller;
/*0x03c0*/  __inherited float APawn::AllowedYawError;
/*0x03c4*/  __inherited union
            {
              uint32_t bProcessingOutsideWorldBounds;
            } __bitfield3c4;
/*0x03c8*/  __inherited struct FVector APawn::ControlInputVector;
/*0x03d4*/  __inherited struct FVector APawn::LastControlInputVector;
/*0x03e0*/  __inherited float ADefaultPawn::BaseTurnRate;
/*0x03e4*/  __inherited float ADefaultPawn::BaseLookUpRate;
/*0x03e8*/  __inherited class UPawnMovementComponent* ADefaultPawn::MovementComponent;
/*0x03f0*/  __inherited class USphereComponent* ADefaultPawn::CollisionComponent;
/*0x03f8*/  __inherited class UStaticMeshComponent* ADefaultPawn::MeshComponent;
/*0x0400*/  __inherited union
            {
                uint32_t bAddDefaultMovementBindings;
            } __bitfield400;
};

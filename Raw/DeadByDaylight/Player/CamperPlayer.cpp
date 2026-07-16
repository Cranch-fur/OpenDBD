// starts at line 122
ACamperPlayer::ACamperPlayer(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCamperMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Primitive and struct initializations
    this->InputMashCount = 1.0f;
    this->InputMashDecayTime = 1.0f;
    this->CameraResetToleranceYaw = 2.0f;
    this->CameraResetTolerancePitch = 2.0f;
    this->CameraRecenterOffsetYaw = 90.0f;
    this->CameraRecenterOffsetPitch = 30.0f;
    this->Gender = EGender::VE_Male;
    this->TimeforDeathWhileCrawling = 60.0f;
    this->TimeforDeathWhileHooked = 60.0f;
    this->SecondsUntilFootprintTrigger = 0.1f;
    this->DisconnectScoringDone = false;
    
    this->_secondsUntilNextFootprint = 0.0f;
    this->_dropStaggerTimer = FDBDTimer(0.0f);
    this->_dropStaggerReductionCooldownTimer = FDBDTimer(0.0f);
    this->_wasStrafingLastFrame = false;
    
    this->_defaultCameraAttachment.Parent = nullptr;
    this->_defaultCameraAttachment.SocketName = NAME_None;
    this->_currentCameraAttachment.Parent = nullptr;
    this->_currentCameraAttachment.SocketName = NAME_None;
    
    this->_drainTimer = FDBDTimer(120.0f);
    this->_dyingTimer = FDBDTimer(120.0f);
    this->_injuredBleedoutTimer = FDBDTimer(30.0f);
    this->_dyingStartTime = 0.0f;
    this->_isInInjuredBleedout = false;
    this->_numHealthyHeals = 0;
    this->_lastEscaped = false;
    this->_onHookDrainToNextStage = true;
    this->_runInputPressed = false;
    this->_hookedCount = 0;
    this->_hasFiredStrugglePhaseEvent = false;

    this->GlobalTunableDB.Reset(TEXT("/Game/Data/SurvivorTunableDB.SurvivorTunableDB"));
    this->bCanBeDamaged = true;
    this->Camera = nullptr;

    // Subobject creation
    this->_SM = CreateDefaultSubobject<UCamperStateMachine>(TEXT("SlasherStateMachine"));
    this->_playerData = CreateDefaultSubobject<UDBDCamperData>(TEXT("PlayerData"));
    this->_hpSlot01 = CreateDefaultSubobject<UChargeableComponent>(TEXT("HPSlot01"));
    this->_hpSlot02 = CreateDefaultSubobject<UChargeableComponent>(TEXT("HPSlot02"));

    this->SlashableZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Slashable"));
    this->SlashableZone->SetupAttachment(this->GetRootComponent());

    this->HookSlashableZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HookSlashable"));
    this->HookSlashableZone->SetupAttachment(this->GetRootComponent());
    this->HookSlashableZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    this->HookSlashableZone->bGenerateOverlapEvents = false;

    this->_hpSlot01->OnInteractionCompletionStateChanged.AddUObject(this, &ACamperPlayer::OnHealthBarCharged);
    this->_hpSlot02->OnInteractionCompletionStateChanged.AddUObject(this, &ACamperPlayer::OnHealthBarCharged);

    this->_carryEscapeProgress = CreateDefaultSubobject<UChargeableComponent>(TEXT("CarryEscape"));
    this->_wiggleAxisFlickMasher = CreateDefaultSubobject<UAxisFlickMasher>(TEXT("WiggleAxisFlickMasher"));
    this->_interactInputMasher = CreateDefaultSubobject<UInputMasher>(TEXT("InteractInputMasher"));

    this->_pixelCounter = CreateDefaultSubobject<UBoxOcclusionQueryComponent>(TEXT("PixelCounter"));
    this->_pixelCounter->SetBoxExtent(FVector(35.0f, 35.0f, 70.0f), false);

    this->StalkedComponent = CreateDefaultSubobject<UStalkedComponent>(TEXT("StalkedComponent"));
    this->_stillnessTracker = CreateDefaultSubobject<UCamperStillnessTrackerComponent>(TEXT("StillnessTracker"));

    this->ProximityZone = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityZone"));
    this->ProximityZone->SetupAttachment(this->GetRootComponent());

    this->_camperSensor = CreateDefaultSubobject<UDBDPawnSensingComponent>(TEXT("CamperSensor"));
    this->_camperSensor->OnSeePawn.AddDynamic(this, &ACamperPlayer::OnPawnSensed);

    this->StruggleMashDelay = 0.4f;
    this->WiggleMashDelay = 0.4f;
}

// starts at line 176
void ACamperPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty, FDefaultAllocator>& OutLifetimeProps) const
{
    // Call the base class implementation first to gather its replicated properties
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACamperPlayer, _drainTimer);
    DOREPLIFETIME(ACamperPlayer, _dyingTimer);
    DOREPLIFETIME(ACamperPlayer, _injuredBleedoutTimer);
    DOREPLIFETIME(ACamperPlayer, _isInInjuredBleedout);
    DOREPLIFETIME(ACamperPlayer, _numHealthyHeals);
    DOREPLIFETIME(ACamperPlayer, _isBeingChased);
}

// starts at line 187
void ACamperPlayer::PostInitializeComponents()
{
    // Execute base class initialization first
    Super::PostInitializeComponents();

    // 1. Pixel Counter Attachment
    // 0x1000000 in FAttachmentTransformRules memory layout aligns with KeepRelativeTransform flags
    FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::KeepRelativeTransform;
    
    bool bIsPixelCounterNull = (this->_pixelCounter == nullptr);
    if (bIsPixelCounterNull == false)
    {
        this->_pixelCounter->AttachToComponent(this->RootComponent, AttachmentRules, NAME_None);
    }

    // 2. Camera Anchor Setup
    // The disassembly checks GUObjectArray flags here, which is standard UE4 IsValid() behavior
    bool bIsCameraAnchorValid = IsValid(this->CameraAnchor);
    if (bIsCameraAnchorValid == false)
    {
        // Fallback to mesh if the anchor is missing or marked for garbage collection
        this->_cameraAnchorOriginalRelativeCameraLocation = FVector::ZeroVector;
        this->_defaultCameraAttachment.Parent = this->Mesh;
        this->_defaultCameraAttachment.SocketName = NAME_None;
    }
    else
    {
        // Extract translation offsets and hierarchy data from the valid anchor
        FTransform RelativeTransform = this->CameraAnchor->GetRelativeTransform();
        this->_cameraAnchorOriginalRelativeCameraLocation = RelativeTransform.GetTranslation();
        this->_defaultCameraAttachment.Parent = this->CameraAnchor->GetAttachParent();
        this->_defaultCameraAttachment.SocketName = this->CameraAnchor->GetAttachSocketName();
    }

    // Assign the evaluated default attachment to the current attachment state
    this->_currentCameraAttachment = this->_defaultCameraAttachment;

    // 3. Proximity Zone Configuration
    bool bIsProximityZoneValid = IsValid(this->ProximityZone);
    if (bIsProximityZoneValid == true)
    {
        // Disable physical collision but keep overlap tracking
        this->ProximityZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
        // The disassembly reveals the compiler fully expanding the AddDynamic macro.
        // We restore the standard macro here for readability and standard compliance.
        this->ProximityZone->OnComponentBeginOverlap.AddDynamic(this, &ACamperPlayer::OnProximityEntered);
    }

    // 4. Stance Metrics
    bool bIsCapsuleComponentNull = (this->CapsuleComponent == nullptr);
    if (bIsCapsuleComponentNull == false)
    {
        // Cache the default standing height
        this->_standingCapsuleHalfHeight = this->CapsuleComponent->CapsuleHalfHeight;
    }

    // 5. Hook Slashable Zone Setup
    bool bIsHookSlashableZoneNull = (this->HookSlashableZone == nullptr);
    if (bIsHookSlashableZoneNull == false)
    {
        this->HookSlashableZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
        // Disassembly: *((_DWORD *)&this->HookSlashableZone->UPrimitiveComponent + 170) &= ~2u;
        // This bitwise operation clears the bGenerateOverlapEvents flag inside UPrimitiveComponent.
        this->HookSlashableZone->bGenerateOverlapEvents = false; 

        // Force the engine to re-evaluate overlap states after modifying the flags
        bool bDoNotifies = true;
        this->HookSlashableZone->UpdateOverlaps(nullptr, bDoNotifies, nullptr);
    }

    // 6. Player Data Binding
    UDBDCamperData* CamperData = Cast<UDBDCamperData>(this->_playerData);
    bool bIsCamperDataNull = (CamperData == nullptr);
    
    if (bIsCamperDataNull == false)
    {
        // Bind to the generic engine delegate system
        CamperData->DamageStateChangedEvent.BindUObject(this, &ACamperPlayer::DamageStateChanged);
    }
}

// starts at line 222
void ACamperPlayer::InitializeTunableValues()
{
    // Call base class implementation first
    Super::InitializeTunableValues();

    UGameInstance* GameInstance = this->GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);

    bool bIsGameInstanceNull = (DBDGameInstance == nullptr);
    if (bIsGameInstanceNull == true)
    {
        // Assembly handles a fallback check for PIE (Play In Editor = 2) when the instance is invalid
        UWorld* World = this->GetWorld();
        bool bIsWorldNull = (World == nullptr);
        if (bIsWorldNull == false)
        {
            bool bIsPIE = (World->WorldType == EWorldType::PIE);
            if (bIsPIE == false)
            {
                this->GetWorld();
            }
        }
        return;
    }

    bool bIsPendingKill = (IsValid(DBDGameInstance) == false);
    if (bIsPendingKill == true)
    {
        return;
    }

    UDBDDesignTunables* DesignTunables = DBDGameInstance->DesignTunables;
    bool bIsTunablesNull = (DesignTunables == nullptr);
    if (bIsTunablesNull == true)
    {
        return;
    }

    // 1. Basic Chase Tunables
    this->_chaseNoiseDistance = DesignTunables->GetTunableValue(TunableValues::CHASE_MAXIMUM_DISTANCE, false);
    this->_timeToHidden = DesignTunables->GetTunableValue(TunableValues::CHASE_CAMPER_HIDE_COOLDOWN, false);
    this->_postChaseTime = DesignTunables->GetTunableValue(TunableValues::POST_CHASE_CAMPER_COOLDOWN, false);

    // 2. Movement Component Tunables
    UCamperMovementComponent* CamperMovement = Cast<UCamperMovementComponent>(this->CharacterMovement);
    bool bIsMovementNull = (CamperMovement == nullptr);
    
    if (bIsMovementNull == false)
    {
        bool bIsMovementValid = (IsValid(CamperMovement) == true);
        if (bIsMovementValid == true)
        {
            CamperMovement->MaxWalkSpeed = DesignTunables->GetTunableValue(TunableValues::CAMPER_MAX_WALK_SPEED, false);
            CamperMovement->MaxWalkSpeedInjured = DesignTunables->GetTunableValue(TunableValues::CAMPER_MAX_WALK_SPEED_INJURED, false);
            CamperMovement->MaxRunSpeed = DesignTunables->GetTunableValue(TunableValues::CAMPER_MAX_RUN_SPEED, false);
            CamperMovement->MaxRunSpeedFatigued = DesignTunables->GetTunableValue(TunableValues::CAMPER_MAX_RUN_SPEED_FATIGUED, false);
            CamperMovement->MaxRunSpeedInjured = DesignTunables->GetTunableValue(TunableValues::CAMPER_MAX_RUN_SPEED_INJURED, false);
            CamperMovement->DashAcceleration = DesignTunables->GetTunableValue(TunableValues::CAMPER_DASH_ACCELERATION, false);
        }
    }

    // 3. Stillness Tracker Tunables
    UCamperStillnessTrackerComponent* StillnessTracker = this->_stillnessTracker;
    bool bIsStillnessTrackerNull = (StillnessTracker == nullptr);
    
    if (bIsStillnessTrackerNull == false)
    {
        bool bIsStillnessTrackerValid = (IsValid(StillnessTracker) == true);
        if (bIsStillnessTrackerValid == true)
        {
            StillnessTracker->StillnessThreshold = DesignTunables->GetTunableValue(TunableValues::CAMPER_STILLNESS_THRESHOLD, false);
            StillnessTracker->SpeedThreshold = DesignTunables->GetTunableValue(TunableValues::CAMPER_STILLNESS_SPEED_THRESHOLD, false);
            StillnessTracker->DistanceThreshold = DesignTunables->GetTunableValue(TunableValues::CAMPER_STILLNESS_DISTANCE_THRESHOLD, false);
            StillnessTracker->DistanceDecay = DesignTunables->GetTunableValue(TunableValues::CAMPER_STILLNESS_DISTANCE_DECAY, false);
            StillnessTracker->ChickenShitModifier = DesignTunables->GetTunableValue(TunableValues::CAMPER_EXPOSER_CHICKEN_SHIT_MODIFIER, false);
            StillnessTracker->LoudNoiseTime = DesignTunables->GetTunableValue(TunableValues::CAMPER_EXPOSER_LOUD_NOISE_TIME, false);

            StillnessTracker->ExposerSpawnTimes.Empty();
            StillnessTracker->ExposerDespawnTimes.Empty();

            int32 ExposerIndex = 1;
            bool bKeepParsing = true;

            // Dynamically parse incrementing exposure tunables (e.g. CAMPER_EXPOSER_SPAWN_TIME_1)
            while (bKeepParsing == true)
            {
                FString SpawnStr = FString::Printf(TEXT("CAMPER_EXPOSER_SPAWN_TIME_%d"), ExposerIndex);
                FName SpawnName = FName(*SpawnStr);

                FString DespawnStr = FString::Printf(TEXT("CAMPER_EXPOSER_DESPAWN_TIME_%d"), ExposerIndex);
                FName DespawnName = FName(*DespawnStr);

                float ExposerSpawnValue = 0.0f;
                float ExposerDespawnValue = 0.0f;

                bool bHasSpawn = DesignTunables->GetTunableValue(ExposerSpawnValue, SpawnName, false);
                bool bHasDespawn = DesignTunables->GetTunableValue(ExposerDespawnValue, DespawnName, false);

                bool bMissingSpawn = (bHasSpawn == false);
                if (bMissingSpawn == true)
                {
                    bKeepParsing = false;
                }
                else
                {
                    bool bMissingDespawn = (bHasDespawn == false);
                    if (bMissingDespawn == true)
                    {
                        bKeepParsing = false;
                    }
                }

                if (bKeepParsing == true)
                {
                    StillnessTracker->ExposerSpawnTimes.Add(ExposerSpawnValue);
                    StillnessTracker->ExposerDespawnTimes.Add(ExposerDespawnValue);
                    ExposerIndex = ExposerIndex + 1;
                }
            }

            StillnessTracker->ExposerSpawnTimes.Sort();
            StillnessTracker->ExposerDespawnTimes.Sort();
        }
    }

    this->PartiallyHiddenStillnessThreshold = DesignTunables->GetTunableValue(TunableValues::CHASE_CAMPER_PARTIALLY_HIDDEN_STILLNESS, false);

    // 4. Proximity Zone (Sphere Radius)
    USphereComponent* ProximityZone = this->ProximityZone;
    bool bIsProximityZoneNull = (ProximityZone == nullptr);
    if (bIsProximityZoneNull == false)
    {
        bool bIsProximityZoneValid = (IsValid(ProximityZone) == true);
        if (bIsProximityZoneValid == true)
        {
            float CamperNearRange = DesignTunables->GetTunableValue(TunableValues::CAMPER_NEAR_RANGE, false);
            ProximityZone->SetSphereRadius(CamperNearRange, true);
        }
    }

    // 5. Camper AI Sensing Component
    UDBDPawnSensingComponent* CamperSensor = this->_camperSensor;
    bool bIsCamperSensorNull = (CamperSensor == nullptr);
    
    if (bIsCamperSensorNull == false)
    {
        bool bIsCamperSensorValid = (IsValid(CamperSensor) == true);
        if (bIsCamperSensorValid == true)
        {
            float CamperNearRange = DesignTunables->GetTunableValue(TunableValues::CAMPER_NEAR_RANGE, false);
            CamperSensor->SightRadius = CamperNearRange;
            CamperSensor->PartiallyHiddenSensingDistance = CamperNearRange * 0.5f;

            FMinimalViewInfo ViewInfo;
            ViewInfo.Location = FVector::ZeroVector;
            ViewInfo.bConstrainAspectRatio = true;
            ViewInfo.OrthoFarClipPlane = 2097152.0f;
            ViewInfo.AspectRatio = 1.3333334f;
            ViewInfo.Rotation = FRotator::ZeroRotator;
            ViewInfo.OrthoNearClipPlane = 0.0f;
            ViewInfo.ProjectionMode = ECameraProjectionMode::Perspective;
            ViewInfo.PostProcessBlendWeight = 0.0f;

            // Generate an updated view angle 
            this->CalcCamera(0.0f, ViewInfo);
            
            CamperSensor->SetPeripheralVisionAngle(ViewInfo.FOV * 0.5f);
        }
    }

    // 6. Carry Escape Progress
    UChargeableComponent* CarryEscapeProgress = this->_carryEscapeProgress;
    bool bIsCarryEscapeNull = (CarryEscapeProgress == nullptr);
    
    if (bIsCarryEscapeNull == false)
    {
        bool bIsCarryEscapeValid = (IsValid(CarryEscapeProgress) == true);
        if (bIsCarryEscapeValid == true)
        {
            CarryEscapeProgress->DechargeRate = DesignTunables->GetTunableValue(TunableValues::CAMPER_WIGGLE_FREE_DECHARGE_RATE, false);
        }
    }

    // 7. Timers & Stamina Configurations
    float SacrificeTime = DesignTunables->GetTunableValue(TunableValues::CAMPER_SACRIFICE_TIME, false);
    this->_drainTimer.Reset(SacrificeTime);

    float DyingTime = DesignTunables->GetTunableValue(TunableValues::CAMPER_DYING_TIME, false);
    this->_dyingTimer.Reset(DyingTime);

    this->_injuredBleedoutTimer.SetReplicateTimeLeft(true);

    float RechargeTime = DesignTunables->GetTunableValue(TunableValues::CAMPER_RUN_STAMINA_RECHARGE_TIME, false);
    float RechargeRate = 1.0f / RechargeTime;
    float RechargeDelay = DesignTunables->GetTunableValue(TunableValues::CAMPER_RUN_STAMINA_RECHARGE_DELAY, false);
    
    this->SetupStamina(RechargeRate, RechargeDelay);

    this->_runStaminaTotalTime = DesignTunables->GetTunableValue(TunableValues::CAMPER_RUN_STAMINA_TOTAL_TIME, false);
}

// starts at line 331
void ACamperPlayer::PostNetReceiveLocationAndRotation()
{
    // Check if the camper is not currently attached to a guiding player (e.g., being carried by the killer)
    if (this->_attachedToGuidingPlayer == false)
    {
        // Allow the base character class to handle the network location and rotation update
        ACharacter::PostNetReceiveLocationAndRotation();
    }
}

// starts at line 341
void ACamperPlayer::PostNetReceiveVelocity(const FVector& NewVelocity)
{
    // Check if the camper is not currently attached to a guiding player
    if (this->_attachedToGuidingPlayer == false)
    {
        // Allow the base pawn class to handle the replicated velocity update
        APawn::PostNetReceiveVelocity(NewVelocity);
    }
}

// starts at line 422
void ACamperPlayer::Tick(float DeltaSeconds)
{
    // 1. Guided Action Check
    UDBDCamperData* CamperData = Cast<UDBDCamperData>(this->_playerData);
    bool bIsCamperDataNull = (CamperData == nullptr);
    if (bIsCamperDataNull == false)
    {
        // 8 correlates to the VE_BeingCarried (or similar carry state) in ECamperGuidedAction enum
        bool bIsBeingCarried = (CamperData->CurrentGuidedAction == 8);
        if (bIsBeingCarried == true)
        {
            this->MoveToOwnerCarryJoint();
        }
    }

    // 2. Base Tick and Event Updates
    Super::Tick(DeltaSeconds);
    this->UpdateSlasherProximityEvents(DeltaSeconds);

    // 3. Basement Scoring Logic
    bool bIsInBasement = this->IsInBasement();
    if (bIsInBasement == true)
    {
        bool bIsInNeed = this->IsInNeed();
        if (bIsInNeed == false)
        {
            // Re-fetch CamperData in case state changed during previous ticks/events
            UDBDCamperData* CurrentCamperData = Cast<UDBDCamperData>(this->_playerData);
            bool bIsCurrentDataNull = (CurrentCamperData == nullptr);
            
            bool bCanReceiveScore = true;
            if (bIsCurrentDataNull == false)
            {
                bool bIsCurrentlyCarried = (CurrentCamperData->CurrentGuidedAction == 8);
                if (bIsCurrentlyCarried == true)
                {
                    bCanReceiveScore = false;
                }
            }

            if (bCanReceiveScore == true)
            {
                // 0x37 (55 in decimal) corresponds to DBDCamperScore_BasementChillingPerSecond
                this->TryFireScoreEvent(EDBDScoreTypes::DBDCamperScore_BasementChillingPerSecond, DeltaSeconds, true);
            }
        }
    }

    // 4. General State Updates
    this->TickSoundEvents(DeltaSeconds);
    this->UpdateSurvivorAnalytics(DeltaSeconds);

    // 5. Input Mash Counter Decay
    float MashedInputDecay = DeltaSeconds / this->InputMashDecayTime;
    float NewMashCounter = this->_currentInputMashCounter - MashedInputDecay;
    
    bool bIsCounterPositive = (NewMashCounter >= 0.0f);
    if (bIsCounterPositive == true)
    {
        this->_currentInputMashCounter = FMath::Min(NewMashCounter, 1.0f);
    }
    else
    {
        this->_currentInputMashCounter = 0.0f;
    }

    this->OnTickGameTimers(DeltaSeconds);
    this->UpdateHealthBars();

    // 6. Local Player Input Processing
    bool bIsLocallyControlled = this->IsLocallyControlled();
    if (bIsLocallyControlled == true)
    {
        UDBDPlayerData* PlayerData = this->_playerData;
        bool bIsPlayerDataNull = (PlayerData == nullptr);
        if (bIsPlayerDataNull == false)
        {
            bool bIsPlayerDataValid = (IsValid(PlayerData) == true);
            if (bIsPlayerDataValid == true)
            {
                // Note: The IDA pseudo-code mistakenly tags interaction 7 as VE_ManuallyLeftMatch.
                // In DBD, 7 is typically EInputInteractionType::VE_Interact and 8 is VE_FastInteract/Struggle
                bool bHasInteractionInput = (PlayerData->_interactionInputMashed == true) || (PlayerData->_autoInteractionMash == true);
                if (bHasInteractionInput == true)
                {
                    this->TryInteractionType(EInputInteractionType::VE_Interact, this); // Enum Value 7
                }

                bool bHasLeftRightInput = (PlayerData->_leftRightInputMashed == true) || (PlayerData->_autoLeftRightMash == true);
                if (bHasLeftRightInput == true)
                {
                    this->TryInteractionType(EInputInteractionType::VE_FastInteract, this); // Enum Value 8
                }
            }
        }
    }

    // 7. Stillness and Audio RTPC Logic
    FVector CurrentVelocity = this->GetVelocity();
    bool bIsStationaryX = (CurrentVelocity.X == FVector::ZeroVector.X);
    bool bIsStationaryY = (CurrentVelocity.Y == FVector::ZeroVector.Y);
    bool bIsStationaryZ = (CurrentVelocity.Z == FVector::ZeroVector.Z);

    bool bIsCompletelyStationary = (bIsStationaryX == true) && (bIsStationaryY == true) && (bIsStationaryZ == true);

    if (bIsCompletelyStationary == true)
    {
        bool bWasAlreadyIdle = (this->_idleStart.GetTicks() != 0);
        if (bWasAlreadyIdle == false)
        {
            FDateTime CurrentTime = FDateTime::UtcNow();
            this->_idleStart = CurrentTime;
            this->_lastUpdate = CurrentTime;
        }
    }
    else
    {
        bool bWasIdle = (this->_idleStart.GetTicks() != 0);
        if (bWasIdle == true)
        {
            this->_idleStart = FDateTime(0);
            
            FAkAudioDevice* AkAudioDevice = FAkAudioDevice::Get();
            bool bIsAudioDeviceNull = (AkAudioDevice == nullptr);
            if (bIsAudioDeviceNull == false)
            {
                AkAudioDevice->SetRTPCValue(TEXT("AudioRTPC_Survivor_Time_Stop_Moving"), 0.0f, 0, nullptr);
            }
        }
    }
}

// starts at line 484
void ACamperPlayer::UpdateCameraReset(float DeltaTime)
{
    // The compiler has highly optimized the math routines here, operating directly on SIMD registers.
    // This decompiled block is effectively an optimized version of taking the difference
    // between two rotators, normalizing the angles to a -180 to 180 degree range, and determining 
    // if the absolute delta exceeds specific tolerance limits to auto-recenter the camera.

    USkeletalMeshComponent* CamperMesh = this->Mesh;
    bool bIsMeshNull = (CamperMesh == nullptr);
    
    // Safety check - though not explicitly in assembly, it's unsafe to operate if mesh is invalid.
    // If you need exact assembly reproduction, remove this outer null check block.
    if (bIsMeshNull == false)
    {
        // 1. Get the current World Rotation of the Character Mesh
        FQuat CurrentRotationQuat = CamperMesh->GetComponentTransform().GetRotation();
        
        // The assembly manually checks if the cached rotator is dirty against the active quaternion
        // However, Unreal Engine abstracts this behind standard getter functions for safety.
        // The equivalent high-level engine logic is to fetch the world rotator.
        FRotator CharacterRotation = CurrentRotationQuat.Rotator();

        // 2. Apply recenter offsets to character's current facing direction
        CharacterRotation.Yaw = CharacterRotation.Yaw + this->CameraRecenterOffsetYaw;
        CharacterRotation.Pitch = CharacterRotation.Pitch - this->CameraRecenterOffsetPitch;

        // 3. Retrieve current active control/camera rotation
        FRotator CameraRotation = this->GetControlRotation();

        // 4. Calculate delta rotators (Target - Current)
        // The extensive _mm_ unpacking and logic in assembly is just FRotator::Normalize() applied to the delta.
        FRotator DeltaRotation = CharacterRotation - CameraRotation;
        DeltaRotation.Normalize();

        // Default state
        this->_cameraResetting = false;

        // 5. Check if the absolute Yaw difference is greater than the allowed tolerance
        float AbsoluteYawDelta = FMath::Abs(DeltaRotation.Yaw);
        bool bYawNeedsReset = (AbsoluteYawDelta > this->CameraResetToleranceYaw);

        if (bYawNeedsReset == true)
        {
            // Apply the delta mapped against time and speed to create a smooth reset
            float YawInput = DeltaRotation.Yaw * DeltaTime * this->CameraResetSpeed;
            this->AddControllerYawInput(YawInput);
            this->_cameraResetting = true;
        }

        // 6. Check if the absolute Pitch difference is greater than the allowed tolerance
        float AbsolutePitchDelta = FMath::Abs(DeltaRotation.Pitch);
        bool bPitchNeedsReset = (AbsolutePitchDelta > this->CameraResetTolerancePitch);

        if (bPitchNeedsReset == true)
        {
            // Note: The assembly has `xorps xmm1, cs:__xmm@80000000800000008000000080000000` here.
            // This is flipping the sign bit. Pitch input usually needs to be inverted depending on camera axis setup.
            float PitchInput = -(DeltaRotation.Pitch * DeltaTime * this->CameraResetSpeed);
            this->AddControllerPitchInput(PitchInput);
            this->_cameraResetting = true;
        }
    }
}

// starts at line 509
void ACamperPlayer::ResetCamera()
{
    this->_cameraResetting = 1;
}

// starts at line 520
void ACamperPlayer::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
    // Call base class implementation first
    Super::SetupPlayerInputComponent(InputComponent);

    // Bind Interaction Inputs
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Interact_Camper")), IE_Pressed, this, &Super::InteractionInputPressed);
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Interact_Camper")), IE_Released, this, &Super::InteractionInputReleased);

    // Bind Fast Interaction Inputs (Vaulting, throwing pallets, etc.)
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("FastInteract_Camper")), IE_Pressed, this, &Super::FastInteractionInputPressed);
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("FastInteract_Camper")), IE_Released, this, &Super::FastInteractionInputReleased);

    // Bind Running Inputs
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Run_Camper")), IE_Pressed, this, &ACamperPlayer::RunInputPressed);
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Run_Camper")), IE_Released, this, &ACamperPlayer::RunInputReleased);

    // Bind Secondary Action
    // Note: The assembly calls a virtual thunk (`vcall'{3424,{flat}}), which maps to SecondaryActionInputPressed
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("SecondaryAction_Camper")), IE_Pressed, this, &Super::SecondaryActionInputPressed);

    // Bind Item Usage
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("ItemUse_Camper")), IE_Pressed, this, &Super::ItemUsePressed);
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("ItemUse_Camper")), IE_Released, this, &Super::ItemUseReleased);

    // Bind Item Dropping
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("ItemDrop_Camper")), IE_Pressed, this, &Super::ItemDropPressed);
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("ItemDrop_Camper")), IE_Released, this, &Super::ItemDropReleased);

    // Bind Gestures
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Gesture01")), IE_Pressed, this, &ACamperPlayer::Gesture01Pressed);
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Gesture02")), IE_Pressed, this, &ACamperPlayer::Gesture02Pressed);
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Gesture03")), IE_Pressed, this, &ACamperPlayer::Gesture03Pressed);
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Gesture04")), IE_Pressed, this, &ACamperPlayer::Gesture04Pressed);

    // Bind Primary Action Inputs
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Action_Camper")), IE_Pressed, this, &ACamperPlayer::ActionInputPressed);
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Action_Camper")), IE_Released, this, &ACamperPlayer::ActionInputReleased);

    // Bind Struggle / Mash Inputs
    InputComponent->BindAction<ADBDPlayer>(FName(TEXT("Mash_Camper")), IE_Pressed, this, &ACamperPlayer::StruggleInputPressed);

    // Initialize Interaction Masher
    UInputMasher* InteractInputMasher = this->_interactInputMasher;
    InteractInputMasher->_actionName = FName(TEXT("Mash_Camper"));
    InteractInputMasher->_delay = this->StruggleMashDelay;
    InteractInputMasher->_inputComponent = InputComponent;
    InteractInputMasher->BindInput();
    InteractInputMasher->_onMashingChanged.BindUObject(this, &ACamperPlayer::MashInputChanged);

    // Initialize Wiggle/Axis Masher (Left/Right Flicking)
    UAxisFlickMasher* WiggleAxisFlickMasher = this->_wiggleAxisFlickMasher;

    // Create Left Flick Object
    UClass* AxisFlickClass = UAxisFlick::StaticClass();
    UAxisFlick* LeftFlick = Cast<UAxisFlick>(StaticConstructObject_Internal(AxisFlickClass, WiggleAxisFlickMasher, NAME_None, RF_NoFlags, EInternalObjectFlags::None, nullptr, false, nullptr));
    
    // Setting up the left flick ranges (-1.0 to -0.2 for flicked, -0.05 to +MAX for released)
    FRange<float> LeftFlickedRange;
    LeftFlickedRange._min = -3.4028235e38f; // -FLT_MAX
    LeftFlickedRange._max = -0.2f;
    
    FRange<float> LeftReleasedRange;
    LeftReleasedRange._min = -0.050000001f;
    LeftReleasedRange._max = 3.4028235e38f; // FLT_MAX
    
    LeftFlick->Initialize(FName(TEXT("MoveRight")), LeftFlickedRange, LeftReleasedRange);

    // Create Right Flick Object
    UAxisFlick* RightFlick = Cast<UAxisFlick>(StaticConstructObject_Internal(AxisFlickClass, WiggleAxisFlickMasher, NAME_None, RF_NoFlags, EInternalObjectFlags::None, nullptr, false, nullptr));
    
    // Setting up the right flick ranges (0.2 to +MAX for flicked, -MAX to 0.05 for released)
    FRange<float> RightFlickedRange;
    RightFlickedRange._min = 0.2f;
    RightFlickedRange._max = 3.4028235e38f; // FLT_MAX
    
    FRange<float> RightReleasedRange;
    RightReleasedRange._min = -3.4028235e38f; // -FLT_MAX
    RightReleasedRange._max = 0.050000001f;
    
    RightFlick->Initialize(FName(TEXT("MoveRight")), RightFlickedRange, RightReleasedRange);

    // Assign flicks and configure the Wiggle Masher
    WiggleAxisFlickMasher->_inputComponent = InputComponent;
    WiggleAxisFlickMasher->_axisFlick1 = LeftFlick;
    WiggleAxisFlickMasher->_axisFlick2 = RightFlick;
    WiggleAxisFlickMasher->BindInput();
    WiggleAxisFlickMasher->_delay = this->WiggleMashDelay;
    WiggleAxisFlickMasher->_onMashingChanged.BindUObject(this, &ACamperPlayer::LeftRightMashedInputChanged);
}

// starts at line 566
void ACamperPlayer::Destroyed()
{
    Super::Destroyed();

    UDBDCamperData* CamperData = Cast<UDBDCamperData>(this->_playerData);
    bool bIsCamperDataNull = (CamperData == nullptr);
    if (bIsCamperDataNull == false)
    {
        CamperData->DamageStateChangedEvent.Unbind();
    }

    bool bIsLocallyControlled = this->IsLocallyControlled();
    if (bIsLocallyControlled == true)
    {
        UInputMasher* InteractInputMasher = this->_interactInputMasher;
        bool bIsInteractMasherNull = (InteractInputMasher == nullptr);
        if (bIsInteractMasherNull == false)
        {
            bool bIsInteractMasherValid = (IsValid(InteractInputMasher) == true);
            if (bIsInteractMasherValid == true)
            {
                InteractInputMasher->Unbind();
            }
        }

        UAxisFlickMasher* WiggleAxisFlickMasher = this->_wiggleAxisFlickMasher;
        bool bIsWiggleMasherNull = (WiggleAxisFlickMasher == nullptr);
        if (bIsWiggleMasherNull == false)
        {
            bool bIsWiggleMasherValid = (IsValid(WiggleAxisFlickMasher) == true);
            if (bIsWiggleMasherValid == true)
            {
                WiggleAxisFlickMasher->Unbind();
            }
        }
    }
}

// starts at line 594
void ACamperPlayer::PossessedBy(AController* NewController)
{
    // Call the base class implementation first
    Super::PossessedBy(NewController);

    // Reset the run input state upon possession
    this->_runInputPressed = false;

    // The assembly checks if NewController inherits from APlayerController via ClassTreeIndex.
    // This is equivalent to a standard Unreal Engine Cast.
    APlayerController* PlayerController = Cast<APlayerController>(NewController);
    bool bIsPlayerControllerNull = (PlayerController == nullptr);

    if (bIsPlayerControllerNull == false)
    {
        // Cache the valid player controller
        this->_storedPlayerController = PlayerController;
    }
    else
    {
        // Clear the stored controller if it is not a valid player controller (e.g., an AIController)
        this->_storedPlayerController = nullptr;
    }
}

// starts at line 601
void ACamperPlayer::AuthorityHandleLoadoutOnSpawn()
{
    // Execute the base class logic first
    Super::AuthorityHandleLoadoutOnSpawn();

    // Fetch the Game Instance and ensure it's a valid DBD game instance
    UGameInstance* BaseGameInstance = this->GetGameInstance();
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(BaseGameInstance);
    
    bool bIsGameInstanceNull = (DBDGameInstance == nullptr);
    if (bIsGameInstanceNull == true)
    {
        return;
    }

    bool bIsGameInstanceValid = IsValid(DBDGameInstance);
    if (bIsGameInstanceValid == false)
    {
        return;
    }

    // Role == 3 correlates to ENetRole::ROLE_Authority
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    if (bIsAuthority == false)
    {
        return;
    }

    FName ItemToSpawnName = NAME_None;
    
    // Resolve the player's persistent data using their Unique ID
    APlayerState* LocalPlayerState = this->PlayerState;
    FUniqueNetIdRepl PlayerNetId = LocalPlayerState->UniqueId;
    
    // Fetch persistent data structure 
    FPlayerPersistentData* PlayerData = UDBDCamperData::GetPlayerPersistentData(DBDGameInstance->_persistentData, PlayerNetId);
    
    bool bIsPlayerDataNull = (PlayerData == nullptr);
    if (bIsPlayerDataNull == false)
    {
        ItemToSpawnName = PlayerData->CurrentLoadout.Item;
    }

    // Check for Console Variable overrides that force a specific start item
    // The CVar data fetching in assembly is highly expanded memory copying logic. 
    // In UE4, this translates to accessing a string console variable.
    static IConsoleVariable* CVarForcedSurvivorStartItem = IConsoleManager::Get().FindConsoleVariable(TEXT("DBD.ForcedSurvivorStartItem"));
    FString ForcedItemString = CVarForcedSurvivorStartItem->GetString();
    int32 StringLength = ForcedItemString.Len();
    
    bool bHasForcedItem = (StringLength > 1);
    if (bHasForcedItem == true)
    {
        ItemToSpawnName = FName(*ForcedItemString);
    }

    // Spawn the resolved item
    ACollectable* SpawnedItem = this->SpawnCollectedItem(ItemToSpawnName);
    bool bIsItemNull = (SpawnedItem == nullptr);
    if (bIsItemNull == false)
    {
        SpawnedItem->FromPlayerSpawn = true;
    }

    // Determine if inventory modifications should be saved based on CVar and GameType
    bool bIsItemNotForced = (StringLength <= 1);
    if (bIsItemNotForced == true)
    {
        bool bAffectsProgression = DBDGameInstance->GetCurrentGametypeAffectsProgression();
        if (bAffectsProgression == true)
        {
            ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);
            bool bIsDBDPlayerStateNull = (DBDPlayerState == nullptr);
            if (bIsDBDPlayerStateNull == false)
            {
                // Remove the consumed item from the player's inventory
                DBDPlayerState->Client_RemoveItemFromInventory(ItemToSpawnName, false);
                
                // Trigger a profile save to commit the inventory change
                UPlayerDataStorageFacade* StorageFacade = DBDGameInstance->_playerDataFacade;
                bool bIsFacadeNull = (StorageFacade == nullptr);
                if (bIsFacadeNull == false)
                {
                    StorageFacade->SaveFullProfile(false);
                }
            }
        }
    }
}

// starts at line 645
UCamperStillnessTrackerComponent* ACamperPlayer::GetStillnessComponent()
{
    return this->_stillnessTracker;
}

// starts at line 650
bool ACamperPlayer::IsRunning() const
{
    // Check if the player is actively pressing the run input or is currently in a dashing state
    bool bIsAttemptingToRun = (this->_runInputPressed == true) || (this->IsDashing() == true);

    // Verify that the currently equipped or active item does not prevent the player from running
    bool bIsAllowedToRun = (this->_itemPreventRunning == false);

    // The player is considered running only if they are trying to run and are not restricted by an item
    if (bIsAttemptingToRun == true)
    {
        if (bIsAllowedToRun == true)
        {
            return true;
        }
    }

    return false;
}

// starts at line 660
void ACamperPlayer::UpdateStance()
{
    // Check if the camper is currently in the crawling state
    bool bIsCrawling = this->IsCrawling();

    if (bIsCrawling == true)
    {
        // Update the animation data stance to Crawl
        this->GetAnimData()->Stance = ECharacterStance::VE_Crawl;
    }
    else
    {
        // Check if the player's movement speed exceeds the crouch walking speed threshold
        bool bIsBeyondCrouchWalkSpeed = this->GetIsBeyondCrouchWalkSpeed();

        // The compiler optimized a boolean flip into an implicit enum cast.
        // If the speed is NOT beyond crouch speed (true/1), the stance evaluates to crouching (VE_Crouch).
        // If the speed IS beyond crouch speed (false/0), the stance evaluates to standing (VE_Stand).
        ECharacterStance ResolvedStance = (ECharacterStance)(bIsBeyondCrouchWalkSpeed == false);

        // Apply the resolved stance
        this->GetAnimData()->Stance = ResolvedStance;
    }
}

// starts at line 673
TArray<UItemAddon*> ACamperPlayer::GetItemAddons() const
{
    // The assembly reveals an offset calculation `(char *)this - 1952`
    // This implies that this function is part of an interface or multiple inheritance structure,
    // and the `this` pointer passed in points to the interface vtable, requiring adjustment
    // to reach the base `ADBDPlayer` object. 
    // In standard C++, we simply cast or call the base class function directly.
    ACollectable* Item = this->GetItem();

    bool bIsItemNull = (Item == nullptr);
    if (bIsItemNull == true)
    {
        return TArray<UItemAddon*>();
    }

    bool bIsItemValid = IsValid(Item);
    if (bIsItemValid == false)
    {
        return TArray<UItemAddon*>();
    }

    // Assembly checks `(*((_BYTE *)&v5->AActor + 320) & 4) != 0`
    // This is equivalent to checking the `bActorIsBeingDestroyed` flag within the AActor bitfield.
    bool bIsItemBeingDestroyed = Item->IsActorBeingDestroyed();
    if (bIsItemBeingDestroyed == true)
    {
        return TArray<UItemAddon*>();
    }

    // Retrieve the addons from the valid, non-destroyed item
    TArray<UItemAddon*> ItemAddons = Item->GetItemAddons();

    // The assembly manually performs an array copy by allocating memory and using memcpy.
    // In high-level UE4 C++, returning the array by value leverages the compiler's return value optimization 
    // or the TArray copy constructor, achieving the exact same result safely.
    return ItemAddons;
}

// starts at line 684
void ACamperPlayer::Authority_AddItemAddonToPlayer(UItemAddon* itemAddon)
{
    // Check if the current instance has server authority (Role == 3 correlates to ROLE_Authority)
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    if (bIsAuthority == true)
    {
        // Retrieve the currently equipped item from the base class
        ACollectable* Item = this->GetItem();
        
        bool bIsItemNull = (Item == nullptr);
        if (bIsItemNull == false)
        {
            // The assembly checks GUObjectArray flags, which translates to a standard Unreal Engine IsValid check
            bool bIsItemValid = IsValid(Item);
            if (bIsItemValid == true)
            {
                // The assembly specifically checks bit 4 of the AActor bitfield at offset 320, 
                // which safely translates to checking if the actor is pending destruction
                bool bIsItemBeingDestroyed = Item->IsActorBeingDestroyed();
                if (bIsItemBeingDestroyed == false)
                {
                    // If the item is completely valid and active, forward the addon application
                    Item->Authority_AddItemAddon(itemAddon);
                }
            }
        }
    }
}

// starts at line 692
void ACamperPlayer::UpdateSlasherProximityEvents(float DeltaSeconds)
{
    UGameInstance* BaseGameInstance = this->GetGameInstance();
    UDBDGameInstance* GameInstance = Cast<UDBDGameInstance>(BaseGameInstance);
    
    bool bIsGameInstanceNull = (GameInstance == nullptr);
    if (bIsGameInstanceNull == true)
    {
        GameInstance = nullptr;
    }

    UWorld* World = this->GetWorld();
    bool bIsWorldNull = (World == nullptr);
    if (bIsWorldNull == true)
    {
        return;
    }

    ADBDGameState* GameState = Cast<ADBDGameState>(World->GetGameState());
    bool bIsGameStateNull = (GameState == nullptr);
    if (bIsGameStateNull == true)
    {
        return;
    }

    bool bIsGameStateValid = IsValid(GameState);
    if (bIsGameStateValid == false)
    {
        return;
    }

    bool bIsGameInstanceValid = IsValid(GameInstance);
    if (bIsGameInstanceValid == false)
    {
        return;
    }

    UDBDDesignTunables* Tunables = GameInstance->DesignTunables;
    
    float ClosestSlasherDistance = 3.4028235e38f; // FLT_MAX
    float ClosestAudioProximity = 3.4028235e38f;  // FLT_MAX
    
    // 1. Calculate proximity to all active Killers (_heartbeatEmitters array at offset 0x730)
    TArray<ASlasherPlayer*> HeartbeatEmitters = GameState->_heartbeatEmitters;
    
    for (int32 i = 0; i < HeartbeatEmitters.Num(); i = i + 1)
    {
        ASlasherPlayer* Slasher = HeartbeatEmitters[i];
        bool bIsSlasherValid = IsValid(Slasher);
        if (bIsSlasherValid == true)
        {
            USceneComponent* SlasherRoot = Slasher->GetRootComponent();
            USceneComponent* CamperRoot = this->GetRootComponent();
            
            bool bRootsValid = (SlasherRoot != nullptr) && (CamperRoot != nullptr);
            if (bRootsValid == true)
            {
                float Distance = FVector::Dist(SlasherRoot->GetComponentLocation(), CamperRoot->GetComponentLocation());
                
                bool bIsCloser = (Distance < ClosestSlasherDistance);
                if (bIsCloser == true)
                {
                    ClosestSlasherDistance = Distance;
                }
                
                float TerrorRadius = GameState->GetHeartbeatEmitterTerrorRadius(Slasher);
                float HeartbeatTunable = Tunables->GetTunableValue(FName(TEXT("AUDIO_HEARTBEAT_RADIUS")), false);
                
                float NormalizedDistance = (HeartbeatTunable * Distance) / FMath::Max(TerrorRadius, 0.0001f);
                
                bool bIsCloserAudio = (NormalizedDistance < ClosestAudioProximity);
                if (bIsCloserAudio == true)
                {
                    ClosestAudioProximity = NormalizedDistance;
                }
            }
        }
    }

    // 2. Calculate proximity to secondary standalone Terror Radius Emitters (at offset 0x5F0)
    AActor* TerrorRadiusEmitter = GameState->Slasher;
    bool bIsEmitterValid = IsValid(TerrorRadiusEmitter);
    if (bIsEmitterValid == true)
    {
        USceneComponent* EmitterRoot = TerrorRadiusEmitter->GetRootComponent();
        USceneComponent* CamperRoot = this->GetRootComponent();
        
        bool bRootsValid = (EmitterRoot != nullptr) && (CamperRoot != nullptr);
        if (bRootsValid == true)
        {
            float Distance = FVector::Dist(EmitterRoot->GetComponentLocation(), CamperRoot->GetComponentLocation());
            float HeartbeatTunable = Tunables->GetTunableValue(FName(TEXT("AUDIO_HEARTBEAT_RADIUS")), false);
            
            // Offset 0x1488 (5256) maps to the actor's custom TerrorRadius property
            float EmitterRadius = TerrorRadiusEmitter->GetTerrorRadius();
            
            float NormalizedDistance = (HeartbeatTunable * Distance) / FMath::Max(EmitterRadius, 0.0001f);
            
            bool bIsCloserAudio = (NormalizedDistance < ClosestAudioProximity);
            if (bIsCloserAudio == true)
            {
                ClosestAudioProximity = NormalizedDistance;
            }
        }
    }

    // 3. Process localized audio RTPC events and Perk Modifiers
    bool bIsLocallyObserved = this->IsLocallyObserved();
    if (bIsLocallyObserved == true)
    {
        UPerkManager* PerkManager = this->GetPerkManager();
        bool bIsPerkManagerValid = IsValid(PerkManager);
        if (bIsPerkManagerValid == true)
        {
            // 0xA1 (161) maps to the EGameplayModifierType governing Fake Killer Terror Radius
            EGameplayModifierType FakeKillerMod = EGameplayModifierType::VE_OverrideTerrorRadiusPerception;
            bool bHasFakeKillerDistance = PerkManager->HasPerkModifierOfType(FakeKillerMod);
            
            if (bHasFakeKillerDistance == true)
            {
                float MinFakeDistance = PerkManager->GetPerkModifierMinValue(FakeKillerMod, 1.0f);
                float HeartbeatTunable = Tunables->GetTunableValue(TunableValues::AUDIO_HEARTBEAT_RADIUS, false);
                
                float TargetFakeDistance = MinFakeDistance * HeartbeatTunable;
                this->_fakeKillerDistance = FMath::FInterpTo(this->_fakeKillerDistance, TargetFakeDistance, DeltaSeconds, 1.0f);
                
                bool bIsEngineValid = (GEngine != nullptr);
                if (bIsEngineValid == true)
                {
                    // The decompiler mislabeled the AddOnScreenDebugMessage parameters as PostSerialize. 
                    // This perfectly matches the GEngine->AddOnScreenDebugMessage arguments in the assembly.
                    FString DebugMsg = FString::Printf(TEXT("Fake Killer Distance: %.2f"), this->_fakeKillerDistance);
                    GEngine->AddOnScreenDebugMessage(999, 5.0f, FColor::White, DebugMsg, true, FVector2D::UnitVector);
                }
                
                // Override the physical distance with the simulated perk distance
                ClosestAudioProximity = this->_fakeKillerDistance;
                ClosestSlasherDistance = this->_fakeKillerDistance;
            }
            else
            {
                this->_fakeKillerDistance = ClosestAudioProximity;
            }

            FAkAudioDevice* AudioDevice = FAkAudioDevice::Get();
            bool bIsAudioDeviceValid = (AudioDevice != nullptr);
            if (bIsAudioDeviceValid == true)
            {
                // Send standard proximity to Wwise
                AudioDevice->SetRTPCValue(TEXT("AudioRTPC_Proximity_Survivor_Killer"), ClosestAudioProximity, 0, nullptr);
                
                float ChaseNoiseDistance = this->_chaseNoiseDistance;
                
                bool bIsWithinChaseDistance = (ClosestSlasherDistance <= ChaseNoiseDistance);
                bool bIsChasedAndWithinDistance = (this->_isBeingChased == true) && (ClosestSlasherDistance <= ChaseNoiseDistance);
                
                // 0x3A (58) maps to EGameplayModifierFlag governing the Fake Killer Stain/Chase state
                bool bHasFakeStain = PerkManager->HasPerkFlag(EGameplayModifierFlag::VE_ShowFakeKillerStain, EGameplayModifierSource::VE_All);
                
                int32 bIsChaseActive = 0;
                
                // Evaluate if any condition forces the Chase audio sequence to start/continue
                bool bShouldTriggerChaseAudio = (bIsWithinChaseDistance == true) || (bIsChasedAndWithinDistance == true) || (bHasFakeStain == true);
                if (bShouldTriggerChaseAudio == true)
                {
                    bIsChaseActive = 1;
                }
                
                // Send chase threshold trigger to Wwise
                AudioDevice->SetRTPCValue(TEXT("AudioRTPC_Chase_Survivor"), (float)bIsChaseActive, 0, nullptr);
            }
        }
    }
}

// starts at line 756
void ACamperPlayer::ProcessHit(const FHitResult *in_hit, int in_hitNumber, bool causedKO)
{
    if (in_hitNumber == false)
    {
        this->OnDamageHit(in_hit, causedKO);
    }
}

// starts at line 764
bool ACamperPlayer::HasHitEvents()
{
    return this->_hitEvents.ArrayNum > 0;
}

// starts at line 769
void ACamperPlayer::OnTickGameTimers(float deltaTime)
{
    this->UpdateDropStagger(deltaTime);

    APlayerState* CurrentPlayerState = this->PlayerState;
    bool bIsPlayerStateNull = (CurrentPlayerState == nullptr);
    if (bIsPlayerStateNull == false)
    {
        bool bIsPlayerStateValid = IsValid(CurrentPlayerState);
        bool bIsPlayerStateBeingDestroyed = CurrentPlayerState->IsActorBeingDestroyed();
        
        bool bCanProcessTimers = (bIsPlayerStateValid == true) && (bIsPlayerStateBeingDestroyed == false);
        if (bCanProcessTimers == true)
        {
            UDBDCamperData* CamperData = Cast<UDBDCamperData>(this->_playerData);
            bool bIsCamperDataNull = (CamperData == nullptr);
            
            bool bShouldUpdateHideTimer = false;
            
            if (bIsCamperDataNull == false)
            {
                // Verify the camper isn't fully immobilized, dead, or escaped
                bool bIsActive = (CamperData->CurrentGuidedAction == ECamperGuidedAction::VE_Active);
                bool bIsDead = (CamperData->CurrentGuidedAction == ECamperGuidedAction::VE_Dead);
                bool bIsEscaped = (CamperData->ImmobilizeState == ECamperImmobilizeState::VE_Escaped);
                
                bool bIsPlayableState = (bIsActive == true) || (bIsDead == true);
                bool bIsNotEscaped = (bIsEscaped == false);
                
                if (bIsPlayableState == true)
                {
                    if (bIsNotEscaped == true)
                    {
                        bShouldUpdateHideTimer = true;
                    }
                }
            }
            else
            {
                // If CamperData is missing, fallback logic still allows standard timer ticks
                bShouldUpdateHideTimer = true;
            }
            
            if (bShouldUpdateHideTimer == true)
            {
                bool bIsCrawling = this->IsCrawling();
                if (bIsCrawling == true)
                {
                    this->UpdateDyingTimer(deltaTime);
                }

                // Check if the camper escaped a chase while injured to trigger hidden event logic
                bool bIsEscapedInjured = (this->GetImmobilizedState() == ECamperImmobilizeState::VE_EscapedInjured);
                bool bShouldTickHide = (this->_isBeingChased == true) && (bIsEscapedInjured == true);
                
                if (bShouldTickHide == true)
                {
                    this->_hideTimer.Update(deltaTime);
                }
            }

            // Server-only authoritative logic
            bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
            if (bIsAuthority == true)
            {
                // Injured bleedout logic (Deep Wound / Mend timer)
                bool bIsInBleedout = (this->_isInInjuredBleedout == true);
                if (bIsInBleedout == true)
                {
                    UChargeableComponent* HPSlot = this->GetCurrentHPSlot();
                    bool bIsHPSlotNull = (HPSlot == nullptr);
                    
                    if (bIsHPSlotNull == false)
                    {
                        bool bIsHPSlotValid = IsValid(HPSlot);
                        bool bIsCharging = HPSlot->_isCharging;
                        
                        bool bShouldUpdateBleedout = (bIsHPSlotValid == true) && (bIsCharging == false);
                        if (bShouldUpdateBleedout == true)
                        {
                            this->_injuredBleedoutTimer.Update(deltaTime);
                        }
                    }

                    bool bIsBleedoutDone = this->_injuredBleedoutTimer.IsDone();
                    if (bIsBleedoutDone == true)
                    {
                        this->Authority_ApplyKO();
                        
                        AInteractable* Interactable = Super::GetInteractable();
                        bool bIsInteractableNull = (Interactable == nullptr);
                        
                        if (bIsInteractableNull == false)
                        {
                            bool bIsInteractableValid = IsValid(Interactable);
                            bool bIsInteractableBeingDestroyed = Interactable->IsActorBeingDestroyed();
                            
                            bool bCanCancelInteraction = (bIsInteractableValid == true) && (bIsInteractableBeingDestroyed == false);
                            if (bCanCancelInteraction == true)
                            {
                                Interactable->AuthorityRequestCancelOngoingInteractions(false);
                            }
                        }
                        
                        // Fire the event noting the player went down
                        this->Authority_OnSlashed(nullptr);
                    }
                    else
                    {
                        bool bIsInjured = this->IsInjured();
                        bool bIsHealed = (bIsInjured == false);
                        
                        // If they are no longer injured (were fully healed), cancel the bleedout state
                        if (bIsHealed == true)
                        {
                            this->_isInInjuredBleedout = false;
                            this->ResetHealthBars();
                        }
                    }
                }

                // Sacrifice and Dying State Resolution
                bool bIsDyingTimerDone = this->_dyingTimer.IsDone();
                bool bIsSacrificed = (this->_sacrificed == true);
                
                bool bHasExpired = (bIsDyingTimerDone == true) || (bIsSacrificed == true);
                
                if (bHasExpired == true)
                {
                    bool bIsDead = this->IsDead();
                    bool bIsAlive = (bIsDead == false);
                    
                    bool bIsActive = (this->GetGuidedAction() == ECamperGuidedAction::VE_Active);
                    
                    bool bShouldProcessDeath = (bIsAlive == true) && (bIsActive == true);
                    if (bShouldProcessDeath == true)
                    {
                        this->Kill(this->_sacrificed);
                        
                        UGameInstance* BaseGameInstance = this->GetGameInstance();
                        UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(BaseGameInstance);
                        
                        bool bIsDBDGameInstanceValid = IsValid(DBDGameInstance);
                        
                        UWorld* World = this->GetWorld();
                        bool bIsWorldNull = (World == nullptr);
                        if (bIsWorldNull == false)
                        {
                            ADBDGameState* GameState = World->GetGameState<ADBDGameState>();
                            bool bIsGameStateValid = IsValid(GameState);
                            bool bIsGameStateBeingDestroyed = false;
                            
                            if (bIsGameStateValid == true)
                            {
                                bIsGameStateBeingDestroyed = GameState->IsActorBeingDestroyed();
                            }
                            
                            bool bCanScoreDeath = (bIsDBDGameInstanceValid == true) && (bIsGameStateValid == true) && (bIsGameStateBeingDestroyed == false);
                            if (bCanScoreDeath == true)
                            {
                                ASlasherPlayer* Slasher = GameState->Slasher;
                                bool bIsSlasherValid = IsValid(Slasher);
                                bool bIsSlasherBeingDestroyed = false;
                                
                                if (bIsSlasherValid == true)
                                {
                                    bIsSlasherBeingDestroyed = Slasher->IsActorBeingDestroyed();
                                }
                                
                                bool bCanScoreSlasher = (bIsSlasherValid == true) && (bIsSlasherBeingDestroyed == false);
                                if (bCanScoreSlasher == true)
                                {
                                    bool bIsSlasherActive = Slasher->IsValidImpl();
                                    
                                    APlayerState* SlasherPlayerState = Slasher->PlayerState;
                                    ADBDPlayerState* DBDSlasherPlayerState = Cast<ADBDPlayerState>(SlasherPlayerState);
                                    
                                    bool bIsDBDSlasherStateValid = IsValid(DBDSlasherPlayerState);
                                    bool bIsDBDSlasherStateBeingDestroyed = false;
                                    
                                    if (bIsDBDSlasherStateValid == true)
                                    {
                                         bIsDBDSlasherStateBeingDestroyed = DBDSlasherPlayerState->IsActorBeingDestroyed();
                                    }
                                    
                                    bool bScoreReady = (bIsSlasherActive == true) && (bIsDBDSlasherStateValid == true) && (bIsDBDSlasherStateBeingDestroyed == false);
                                    if (bScoreReady == true)
                                    {
                                        bool bWasSacrificed = (this->_sacrificed == true);
                                        if (bWasSacrificed == true)
                                        {
                                            this->TryFireScoreEvent(EDBDScoreTypes::DBDCamperScore_DieSacrificed, 1.0f, true);
                                            
                                            UGameEventTracker* EventTracker = DBDGameInstance->_gameEventTracker;
                                            EventTracker->FireGameEvent(EDBDScoreTypes::DBDSlasherScore_SacrificeSuccess, 1.0f, Slasher, this);
                                        }
                                        else
                                        {
                                            bool bDiedFromBleedout = this->_dyingTimer.IsDone();
                                            if (bDiedFromBleedout == true)
                                            {
                                                float PercentElapsed = this->_dyingTimer.GetPercentTimeElapsed();
                                                float BleedOutAmount = PercentElapsed - this->_dyingStartTime;
                                                
                                                DBDSlasherPlayerState->FireScoreEvent(EDBDScoreTypes::DBDSlasherScore_BleedOutPercent, BleedOutAmount, true);
                                                
                                                UGameEventTracker* EventTracker = DBDGameInstance->_gameEventTracker;
                                                EventTracker->FireGameEvent(EDBDScoreTypes::DBDSlasherScore_BleedOut, 1.0f, Slasher, this);
                                                
                                                this->TryFireScoreEvent(EDBDScoreTypes::DBDCamperScore_DieBleedOut, 1.0f, true);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // Temporary Authoritative Movement State Tracking
                bool bWasSlashedDone = this->_slashedAuthoritativeMovementTimer.IsDone();
                this->_slashedAuthoritativeMovementTimer.Update(deltaTime);
                bool bIsSlashedDoneNow = this->_slashedAuthoritativeMovementTimer.IsDone();
                
                bool bSlashedTimerJustFinished = (bWasSlashedDone == false) && (bIsSlashedDoneNow == true);
                if (bSlashedTimerJustFinished == true)
                {
                    this->Multicast_SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag::SLASHED, false);
                }

                bool bWasDroppedDone = this->_droppedAuthoritativeMovementTimer.IsDone();
                this->_droppedAuthoritativeMovementTimer.Update(deltaTime);
                bool bIsDroppedDoneNow = this->_droppedAuthoritativeMovementTimer.IsDone();
                
                bool bDroppedTimerJustFinished = (bWasDroppedDone == false) && (bIsDroppedDoneNow == true);
                if (bDroppedTimerJustFinished == true)
                {
                    this->Multicast_SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag::DROPPED, false);
                }

                // Stamina Consumption
                bool bIsRunning = this->IsRunning();
                if (bIsRunning == true)
                {
                    float ConsumedStamina = deltaTime / this->_runStaminaTotalTime;
                    this->ConsumeStamina(ConsumedStamina);
                }
            }
        }
    }
}

// starts at line 883
void ACamperPlayer::TriggerAfflictionHUDFeedback()
{
    this->TriggerAfflictionHUDIntro = 1;
}

// starts at line 888
bool ACamperPlayer::IsHealingTimerExpired()
{
    return this->_healingTimer->IsDone();
}

// starts at line 893
float ACamperPlayer::GetPercentHealingTimer()
{
    return this->_healingTimer->GetPercentTimeLeft();
}

// starts at line 898
void ACamperPlayer::StartHealingTimer(float time)
{
    this->_healingTimer->Reset(time);
}

// starts at line 903
void ACamperPlayer::StopHealingTimer()
{
    this->_healingTimer->Stop();
}

// starts at line 908
void ACamperPlayer::UpdateHealingTimer(float deltaTime)
{
    this->_healingTimer->Update(deltaTime);
}

// starts at line 913
void ACamperPlayer::AuthoritySetHealth(ECamperDamageState health)
{
    // Verify that this instance has server authority (Role == 3 correlates to ROLE_Authority)
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    if (bIsAuthority == true)
    {
        // Cast the player data to ensure it is valid camper data
        UDBDCamperData* CamperData = Cast<UDBDCamperData>(this->_playerData);
        bool bIsCamperDataNull = (CamperData == nullptr);
        
        if (bIsCamperDataNull == false)
        {
            // Apply the new health state, passing true (1) to force the update or notify observers
            this->SetDamageState(health, true);
            
            // Retrieve the current world to access the game mode
            UWorld* World = this->GetWorld();
            bool bIsWorldNull = (World == nullptr);
            
            if (bIsWorldNull == false)
            {
                // Fetch the authoritative game mode and safely cast it to the DBD-specific game mode
                // The assembly checks the ClassTreeIndex, which is standard Unreal Engine Cast<> behavior
                AGameModeBase* AuthGameMode = World->GetAuthGameMode();
                ADBDGameMode* DBDGameMode = Cast<ADBDGameMode>(AuthGameMode);
                
                bool bIsDBDGameModeNull = (DBDGameMode == nullptr);
                if (bIsDBDGameModeNull == false)
                {
                    // Notify the game mode about the state change affecting guided actions
                    DBDGameMode->GuidedActionModified();
                }
            }
        }
    }
}

// starts at line 927
void ACamperPlayer::Authority_ApplyKO()
{
    // Verify that this instance has server authority (Role == 3 correlates to ROLE_Authority)
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    if (bIsAuthority == true)
    {
        UDBDCamperData* CamperData = Cast<UDBDCamperData>(this->_playerData);
        
        // Safety check before setting the knowledge sharing type
        bool bIsCamperDataNull = (CamperData == nullptr);
        if (bIsCamperDataNull == false)
        {
            CamperData->SetDamageState(ECamperDamageState::VE_KO);
        }

        // Notify that the damage state has changed (1 translates to true)
        this->DamageStateChanged(true);

        // The assembly features redundant SafeCasts, likely the result of an inlined function or macro expanding
        UDBDCamperData* CurrentCamperData = Cast<UDBDCamperData>(this->_playerData);
        bool bIsCurrentCamperDataNull = (CurrentCamperData == nullptr);
        
        if (bIsCurrentCamperDataNull == false)
        {
            UDBDCamperData* FinalCamperData = Cast<UDBDCamperData>(this->_playerData);
            bool bIsFinalCamperDataNull = (FinalCamperData == nullptr);
            
            if (bIsFinalCamperDataNull == false)
            {
                // Again, VE_Escaped is an IDA artifact for integer 2, which likely corresponds to ECamperDamageState::VE_KO
                bool bIsDamageStateEscaped = (FinalCamperData->GetDamageState() == ECamperDamageState::VE_KO);
                if (bIsDamageStateEscaped == true)
                {
                    this->Multicast_OnKO();
                }
            }
        }

        // Snapshot the current dying timer progress
        float PercentTimeElapsed = this->_dyingTimer.GetPercentTimeElapsed();
        
        // Re-evaluating authority per the assembly's explicit boolean check
        bool bIsStillAuthority = (this->Role == ENetRole::ROLE_Authority);
        this->_dyingStartTime = PercentTimeElapsed;
        
        bool bIsInBleedout = (this->_isInInjuredBleedout == true);
        
        // If the player was in the Deep Wound / Injured Bleedout state, clear it upon KO
        bool bShouldResetBleedout = (bIsStillAuthority == true) && (bIsInBleedout == true);
        if (bShouldResetBleedout == true)
        {
            this->_isInInjuredBleedout = false;
            this->ResetHealthBars();
        }
    }
}

// starts at line 937
void ACamperPlayer::Authority_EndInjuredBleedout()
{
    // Verify that this instance has server authority (Role == 3 correlates to ENetRole::ROLE_Authority)
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    
    // Check if the player is currently in the injured bleedout state (Deep Wound)
    bool bIsInBleedout = (this->_isInInjuredBleedout == true);

    bool bShouldEndBleedout = (bIsAuthority == true) && (bIsInBleedout == true);
    if (bShouldEndBleedout == true)
    {
        // Clear the bleedout state
        this->_isInInjuredBleedout = false;

        // Reset the health bars to update the UI and clear the bleedout timer visual
        this->ResetHealthBars();
    }
}

// starts at line 948
void ACamperPlayer::Authority_StartInjuredBleedout()
{
    // Verify that this instance has server authority (Role == 3 correlates to ENetRole::ROLE_Authority)
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    
    // Ensure the player is not already in the bleedout state and is currently injured
    bool bIsNotInBleedout = (this->_isInInjuredBleedout == false);
    bool bIsInjured = this->IsInjured();

    bool bShouldStartBleedout = (bIsAuthority == true) && (bIsNotInBleedout == true) && (bIsInjured == true);
    if (bShouldStartBleedout == true)
    {
        UPerkManager* PerkManager = this->GetPerkManager();
        
        // IDA incorrectly maps the integer to VE_ApplyGrislyChainsDebuffOnChainsawAttack combined with a 0x80 bitwise flag.
        // This is a common decompiler artifact for enums. It represents the specific modifier for the Bleedout Timer.
        EGameplayModifierType BleedoutModifierType = EGameplayModifierType::VE_SetInjuredBleedoutTime; 
        
        float MaxBleedoutTime = 0.0f;
        bool bIsPerkManagerNull = (PerkManager == nullptr);
        if (bIsPerkManagerNull == false)
        {
            MaxBleedoutTime = PerkManager->GetPerkModifierMaxValue(BleedoutModifierType);
        }

        // Reset the timer with the calculated max value (factoring in perks) and set the state
        this->_injuredBleedoutTimer.Reset(MaxBleedoutTime);
        this->_isInInjuredBleedout = true;
        
        // Update health bar UI to reflect the Deep Wound state
        this->ResetHealthBars();

        UDBDGameInstance* DBDGameInstance = this->GetDBDGameInstance();
        bool bIsGameInstanceNull = (DBDGameInstance == nullptr);
        
        if (bIsGameInstanceNull == false)
        {
            UGameEventTracker* EventTracker = DBDGameInstance->GameEventTracker;
            bool bIsEventTrackerNull = (EventTracker == nullptr);
            
            if (bIsEventTrackerNull == false)
            {
                // Fire the scoring/analytics event for starting the injured bleedout
                EventTracker->FireGameEvent(EDBDScoreTypes::DBDCamperScore_StartInjuredBleedout, 0.0f, this, this);
            }
        }
    }
}

// starts at line 959
void ACamperPlayer::OnFinishedPlaying()
{
    // Call the base class implementation first
    Super::OnFinishedPlaying();

    // Verify that this instance has server authority (Role == 3 correlates to ENetRole::ROLE_Authority)
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    
    // Check if the player is currently in the injured bleedout state (Deep Wound)
    bool bIsInBleedout = (this->_isInInjuredBleedout == true);

    // If the player has finished playing (e.g., escaped, died, or disconnected), clear active bleedout states
    bool bShouldEndBleedout = (bIsAuthority == true) && (bIsInBleedout == true);
    if (bShouldEndBleedout == true)
    {
        // Clear the bleedout state
        this->_isInInjuredBleedout = false;

        // Reset the health bars to clean up the UI
        this->ResetHealthBars();
    }
}

// starts at line 965
void ACamperPlayer::Authority_PostApplyDamage(bool didDamage, bool causedKO)
{
    // Verify that this instance has server authority (Role == 3 correlates to ROLE_Authority)
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    if (bIsAuthority == true)
    {
        if (didDamage == true)
        {
            // Retrieve the current world to access the game mode
            UWorld* World = this->GetWorld();
            bool bIsWorldNull = (World == nullptr);
            
            if (bIsWorldNull == false)
            {
                // Fetch the authoritative game mode and safely cast it to the DBD-specific game mode
                AGameModeBase* AuthGameMode = World->GetAuthGameMode();
                ADBDGameMode* DBDGameMode = Cast<ADBDGameMode>(AuthGameMode);
                
                bool bIsDBDGameModeNull = (DBDGameMode == nullptr);
                if (bIsDBDGameModeNull == false)
                {
                    // Notify the game mode about the state change affecting guided actions
                    DBDGameMode->GuidedActionModified();
                }
            }
        }

        AInteractable* Interactable = this->GetInteractable();
        bool bIsInteractableNull = (Interactable == nullptr);
        
        if (bIsInteractableNull == false)
        {
            bool bIsInteractableValid = IsValid(Interactable);
            bool bIsInteractableBeingDestroyed = Interactable->IsActorBeingDestroyed();
            
            bool bCanCancelInteraction = (bIsInteractableValid == true) && (bIsInteractableBeingDestroyed == false);
            if (bCanCancelInteraction == true)
            {
                Interactable->AuthorityRequestCancelOngoingInteractions(true);
            }
        }

        // Process any scoring events specifically tied to being hit
        this->Authority_HandleOnHitScoringEvents();

        // Broadcast the OnAttackedDelegate to notify listeners that this camper was attacked
        this->OnAttackedDelegate.Broadcast();
    }
}

// starts at line 988
void ACamperPlayer::Authority_ApplyDamageToState(bool& didDamage, bool& causedKO, ECamperDamageState targetState)
{
    // Initialize out parameters
    didDamage = false;
    causedKO = false;

    // Verify that this instance has server authority
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    if (bIsAuthority == true)
    {
        UDBDCamperData* CamperData = Cast<UDBDCamperData>(this->_playerData);
        bool bIsCamperDataNull = (CamperData == nullptr);
        
        if (bIsCamperDataNull == false)
        {
            // The assembly checks offset 10Bh which maps to ShouldTakeDamage
            bool bShouldTakeDamage = (CamperData->ShouldTakeDamage == true);
            if (bShouldTakeDamage == true)
            {
                bool bIsInBleedout = (this->_isInInjuredBleedout == true);
                
                // The compiler redundantly re-checks authority here due to inlining of Authority_EndInjuredBleedout
                bool bIsStillAuthority = (this->Role == ENetRole::ROLE_Authority);
                bool bShouldEndBleedout = (bIsInBleedout == true) && (bIsStillAuthority == true);
                
                if (bShouldEndBleedout == true)
                {
                    this->_isInInjuredBleedout = false;
                    this->ResetHealthBars();
                }

                // Note: IDA pseudo-code frequently misinterprets numeric enum values as the first alphabetical match in the database.
                // Value 1 is VE_Injured (not VE_Dead). Value 2 is VE_KO (not VE_Escaped).
                if (targetState == ECamperDamageState::VE_Injured)
                {
                    // Apply Injured state directly (true/1 forces score event/broadcast)
                    this->SetDamageState(ECamperDamageState::VE_Injured, true);
                    this->ResetHealthBars();
                    didDamage = true;
                }
                else if (targetState == ECamperDamageState::VE_KO)
                {
                    bool bCanBeImmediatelyKO = this->CanBeImmediatelyKO();
                    if (bCanBeImmediatelyKO == true)
                    {
                        didDamage = true;
                        this->Authority_ApplyKO();
                    }
                    else
                    {
                        // If they have Deep Wound protection (e.g. Borrowed Time/Endurance), apply injured and bleedout instead
                        this->SetDamageState(ECamperDamageState::VE_Injured, true);
                        this->ResetHealthBars();
                        this->Authority_StartInjuredBleedout();
                    }
                }

                bool bIsNowKO = this->IsKO();
                if (bIsNowKO == true)
                {
                    causedKO = true;
                }
            }
        }

        // Trigger post-damage effects, scoring, and UI updates
        this->Authority_PostApplyDamage(didDamage, causedKO);
    }
}

// starts at line 1034
void ACamperPlayer::DBDApplyDamage(bool& didDamage, bool& causedKO)
{
    // Verify that this instance has server authority (Role == 3 correlates to ROLE_Authority)
    bool bIsAuthority = (this->Role == ENetRole::ROLE_Authority);
    if (bIsAuthority == true)
    {
        UDBDCamperData* CamperData = Cast<UDBDCamperData>(this->_playerData);
        bool bIsCamperDataNull = (CamperData == nullptr);
        
        if (bIsCamperDataNull == false)
        {
            // Note: The assembly executes multiple redundant SafeCasts due to macro/inline expansion.
            // We resolve the single active damage state here.
            ECamperDamageState CurrentDamageState = CamperData->GetDamageState();
            
            // Note: IDA frequently misinterprets numeric enum values. 
            // Based on standard Dead By Daylight logic:
            // 0 is typically VE_Healthy
            // 1 is typically VE_Injured
            // 2 is typically VE_KO
            
            ECamperDamageState TargetDamageState;
            
            // If the camper is currently healthy (0)
            if (CurrentDamageState == ECamperDamageState::VE_Healthy)
            {
                // Move them to the Injured state (1)
                TargetDamageState = ECamperDamageState::VE_Injured;
            }
            else
            {
                // If the camper is Injured (1) or already KO (2), attempt to push them to the KO state (2)
                bool bIsInjuredOrKO = (CurrentDamageState == ECamperDamageState::VE_Injured) || (CurrentDamageState == ECamperDamageState::VE_KO);
                if (bIsInjuredOrKO == false)
                {
                    // If they are in any other state (e.g., Dead, Escaped), they cannot take normal hit damage
                    return;
                }
                
                TargetDamageState = ECamperDamageState::VE_KO;
            }
            
            // Apply the computed damage state
            this->Authority_ApplyDamageToState(didDamage, causedKO, TargetDamageState);
        }
    }
}

// starts at line 1055
void ACamperPlayer::Authority_HandleOnHitScoringEvents()
{
    // The method handles various scoring events triggered when a camper is hit by the slasher.
    // Logic includes checking recent unhooks and proximity to other campers in need.

    ADBDPlayerState* DBDPlayerState = Cast<ADBDPlayerState>(this->PlayerState);
    bool bIsPlayerStateNull = (DBDPlayerState == nullptr);

    if (bIsPlayerStateNull == false)
    {
        UWorld* World = this->GetWorld();
        bool bIsWorldNull = (World == nullptr);

        if (bIsWorldNull == false)
        {
            // 1. Check for Hit After Hook Save event
            float Timeout = FDBDRecentGameplayEvents::GetEventTimeoutTunable(World, TunableValues::SAVE_FROM_HOOK_HIT_AFTER_HOOK_SAVE_TIMEOUT);
            
            bool bHasBeenSavedFromHook = DBDPlayerState->HasHappened(EDBDScoreTypes::DBDCamperScore_SaveFromHook, Timeout);
            bool bHasBeenSavedFromHookPostExit = false;

            if (bHasBeenSavedFromHook == false)
            {
                bHasBeenSavedFromHookPostExit = DBDPlayerState->HasHappened(EDBDScoreTypes::DBDCamperScore_SaveFromHook_PostExit, Timeout);
            }

            bool bTriggerHitAfterSave = (bHasBeenSavedFromHook == true) || (bHasBeenSavedFromHookPostExit == true);
            if (bTriggerHitAfterSave == true)
            {
                UDBDGameInstance* GameInstance = ALevelParadise::GetDBDGameInstance(this);
                bool bIsGameInstanceValid = (GameInstance != nullptr);
                if (bIsGameInstanceValid == true)
                {
                    GameInstance->GameEventTracker->FireGameEvent(EDBDScoreTypes::DBDCamperScore_HitAfterHookSave, 1.0f, this, this);
                }
            }

            // 2. Proximity Scoring: Near Friends In Need
            TArray<AActor*> IgnoredCampers;
            IgnoredCampers.Add(this);

            TArray<ACamperPlayer*> NearbyCampers;
            FVector CurrentLocation = FVector::ZeroVector;
            
            bool bIsRootValid = (this->RootComponent != nullptr);
            if (bIsRootValid == true)
            {
                CurrentLocation = this->RootComponent->GetComponentLocation();
            }

            // Fetch campers within 200 units (2 meters)
            UDBDUtilities::GetNearbyCampers(World, CurrentLocation, 200.0f, NearbyCampers, IgnoredCampers);

            bool bFriendInNeedNearby = false;
            bool bUnhookedFriendInNeedNearby = false;

            for (int32 i = 0; i < NearbyCampers.Num(); i = i + 1)
            {
                ACamperPlayer* Friend = NearbyCampers[i];
                bool bIsFriendValid = (Friend != nullptr);

                if (bIsFriendValid == true)
                {
                    bool bIsFriendInNeed = Friend->IsInNeed();
                    if (bIsFriendInNeed == true)
                    {
                        bFriendInNeedNearby = true;

                        float UnhookTimeout = FDBDRecentGameplayEvents::GetEventTimeoutTunable(World, TunableValues::WAS_UNHOOKED_TIMEOUT);
                        ADBDPlayerState* FriendState = Friend->GetDBDPlayerState();
                        bool bIsFriendStateValid = (FriendState != nullptr);

                        if (bIsFriendStateValid == true)
                        {
                            bool bFriendWasRecentlyUnhooked = FriendState->HasHappened(EDBDScoreTypes::DBDCamperScore_WasUnhooked, UnhookTimeout);
                            if (bFriendWasRecentlyUnhooked == true)
                            {
                                bUnhookedFriendInNeedNearby = true;
                            }
                        }
                    }
                }
            }

            // Award "Hit Near Friend In Need" if not already awarded recently
            float HitNearFriendTimeout = FDBDRecentGameplayEvents::GetEventTimeoutTunable(World, TunableValues::HIT_NEAR_FRIEND_IN_NEED_TIMEOUT);
            bool bAlreadyAwardedNearFriend = DBDPlayerState->HasHappened(EDBDScoreTypes::DBDCamperScore_HitNearFriendInNeed, HitNearFriendTimeout);
            
            if (bAlreadyAwardedNearFriend == false)
            {
                if (bFriendInNeedNearby == true)
                {
                    UDBDGameInstance* GameInstance = ALevelParadise::GetDBDGameInstance(this);
                    bool bIsGameInstanceValid = (GameInstance != nullptr);
                    if (bIsGameInstanceValid == true)
                    {
                        GameInstance->GameEventTracker->FireGameEvent(EDBDScoreTypes::DBDCamperScore_HitNearFriendInNeed, 1.0f, this, this);
                    }
                }
            }

            // Award "Hit Near Unhooked Friend In Need" if not already awarded recently
            float HitNearUnhookedTimeout = FDBDRecentGameplayEvents::GetEventTimeoutTunable(World, TunableValues::HIT_NEAR_UNHOOKED_FRIEND_IN_NEED_TIMEOUT);
            bool bAlreadyAwardedNearUnhooked = DBDPlayerState->HasHappened(EDBDScoreTypes::DBDCamperScore_HitNearUnhookedFriendInNeed, HitNearUnhookedTimeout);

            if (bAlreadyAwardedNearUnhooked == false)
            {
                if (bUnhookedFriendInNeedNearby == true)
                {
                    UDBDGameInstance* GameInstance = ALevelParadise::GetDBDGameInstance(this);
                    bool bIsGameInstanceValid = (GameInstance != nullptr);
                    if (bIsGameInstanceValid == true)
                    {
                        GameInstance->GameEventTracker->FireGameEvent(EDBDScoreTypes::DBDCamperScore_HitNearUnhookedFriendInNeed, 1.0f, this, this);
                    }
                }
            }
        }
    }
}

// starts at line 1098
void ACamperPlayer::ApplyHeal()
{
    // Check if the current actor has network authority (typically server)
    if (this->Role != ROLE_Authority)
    {
        return;
    }

    // Safely cast the player data to UDBDCamperData
    UDBDCamperData* camperData = SafeCast<UDBDCamperData>(this->_playerData);
    if (camperData == nullptr)
    {
        return;
    }

    // Handle the case where the camper is in an injured bleedout state
    if (this->_isInInjuredBleedout != false)
    {
        // Additional authority check (often generated by compiler or macros)
        if (this->Role == ROLE_Authority)
        {
            if (this->_isInInjuredBleedout != false)
            {
                this->_isInInjuredBleedout = false;
                this->ResetHealthBars();
            }
        }
    }
    else
    {
        // Retrieve the current damage state
        ECamperDamageState damageState = camperData->GetDamageState();

        // State 1 implies VE_Injured
        if (damageState == ECamperDamageState::VE_Injured)
        {
            if (this->CanBecomeHealthy() != false)
            {
                this->_numHealthyHeals++;
                // State 0 implies VE_Healthy
                this->SetDamageState(ECamperDamageState::VE_Healthy, true);
            }
        }
        // State 2 implies VE_KOD (Dying / Knocked Out)
        else if (damageState == ECamperDamageState::VE_KO)
        {
            // Set damage state to VE_Injured
            this->SetDamageState(ECamperDamageState::VE_Injured, true);
        }
    }

    // Retrieve the current world
    UWorld* world = this->GetWorld();
    if (world != nullptr)
    {
        // Access GameState to reward the killer
        ADBDGameState* gameState = Cast<ADBDGameState>(world->GameState);
        if (gameState != nullptr)
        {
            // Access the Slasher (Killer) reference from GameState
            ADBDPlayer* slasher = gameState->Slasher;
            if (slasher != nullptr)
            {
                // IsValidImpl is resolved dynamically from VTable index 0x1070
                if (slasher->IsValidImpl() != false)
                {
                    ADBDPlayerState* slasherPlayerState = Cast<ADBDPlayerState>(slasher->PlayerState);
                    if (slasherPlayerState != nullptr)
                    {
                        // Calculate bleedout completion percentage to award points
                        float percentTimeElapsed = this->_dyingTimer.GetPercentTimeElapsed();
                        float percentToAward = percentTimeElapsed - this->_dyingStartTime;
                        
                        // 0x4E (78) represents DBDSlasherScore_BleedOutPercent
                        slasherPlayerState->FireScoreEvent(EDBDScoreTypes::DBDSlasherScore_BleedOutPercent, percentToAward, true);
                    }
                }
            }
        }

        // Notify the GameMode about the modified guided action
        ADBDGameMode* authorityGameMode = Cast<ADBDGameMode>(world->AuthorityGameMode);
        if (authorityGameMode != nullptr)
        {
            authorityGameMode->GuidedActionModified();
        }
    }

    // Finally, trigger the heal effects on all clients via Remote Procedure Call (RPC)
    this->Multicast_OnHealed();
}

// starts at line 1152
void ACamperPlayer::ApplyFullHeal()
{
    // Check if the current actor has network authority (typically server)
    // 3 corresponds to ROLE_Authority
    if (this->Role == ROLE_Authority)
    {
        // Safely cast the player data to UDBDCamperData
        UDBDCamperData* camperData = SafeCast<UDBDCamperData>(this->_playerData);
        
        if (camperData == nullptr)
        {
            // Do nothing if the cast fails
        }
        else
        {
            // Check if the camper is currently allowed to enter the healthy state
            if (this->CanBecomeHealthy() == false)
            {
                // Fallback to a standard heal if a full heal is not permitted
                this->ApplyHeal();
            }
            else
            {
                // Handle the case where the camper is in an injured bleedout state
                if (this->_isInInjuredBleedout == true) 
                {
                    this->_isInInjuredBleedout = false;
                    this->ResetHealthBars();
                }

                // Check the damage state to verify if it requires resetting
                UDBDCamperData* secondCamperDataCheck = SafeCast<UDBDCamperData>(this->_playerData);
                bool shouldSetDamageState = false;

                if (secondCamperDataCheck == nullptr)
                {
                    shouldSetDamageState = true;
                }
                else if (secondCamperDataCheck->GetDamageState() == ECamperDamageState::VE_Dead)
                {
                    // State 3 corresponds to VE_Dead; we do not overwrite this state
                    shouldSetDamageState = false; 
                }
                else
                {
                    // For all other valid damage states, we process the state change
                    shouldSetDamageState = true;
                }

                if (shouldSetDamageState == true)
                {
                    // State 0 corresponds to VE_Active / VE_Healthy
                    this->SetDamageState(ECamperDamageState::VE_Healthy, true);
                }

                // Retrieve the current world
                UWorld* world = this->GetWorld();
                if (world == nullptr)
                {
                    // Do nothing if world is missing
                }
                else
                {
                    // Notify the GameMode about the modified guided action
                    ADBDGameMode* authorityGameMode = Cast<ADBDGameMode>(world->AuthorityGameMode);
                    if (authorityGameMode == nullptr)
                    {
                        // Do nothing if the GameMode is not of type ADBDGameMode
                    }
                    else
                    {
                        authorityGameMode->GuidedActionModified();
                    }
                }

                // Trigger the heal effects on all clients via Remote Procedure Call (RPC)
                this->Multicast_OnHealed();
            }
        }
    }
}

// starts at line 1187
void ACamperPlayer::Multicast_OnHealed_Implementation()
{
    // Broadcast the healed event to all bound listeners across the network
    this->OnHealedDelegate.Broadcast();
}

// starts at line 1192
void ACamperPlayer::DBD_CheatEscaped()
{
    this->Server_CheatEscaped();
}

// starts at line 1203
void ACamperPlayer::Server_CheatEscaped_Implementation()
{
    this->Escape(0);
}

// starts at line 1208
void ACamperPlayer::SetOverlappingEscape(AActor* escape)
{
    this->_overlappingEscape = escape;
}

// starts at line 1213
void ACamperPlayer::Escape(AActor* escape)
{
    // Record analytics for the camper escaping. 
    // The ASM's string conversion boilerplate maps to a standard TEXT macro in UE.
    UDBDAnalytics::RecordGameProgress(TEXT("CamperEscaped"));

    // Store the escape actor reference
    this->_escape = escape;

    // Retrieve the current world
    UWorld* world = this->GetWorld();
    if (world == nullptr)
    {
        // Do nothing if the world is invalid
    }
    else
    {
        // Safely cast the GameState to ADBDGameState
        ADBDGameState* gameState = Cast<ADBDGameState>(world->GameState);
        if (gameState == nullptr)
        {
            // Do nothing if the cast fails
        }
        else
        {
            // If this player is the last remaining active survivor, set the flag
            if (gameState->GetNumberOfOtherActiveSurvivors(this) == 0)
            {
                this->_lastEscaped = true;
            }
        }
    }

    // Set immobilized state to 0 (typically VE_None or VE_Active)
    this->Server_SetImmobilized(ECamperImmobilizeState::VE_None);

    // Trigger end-of-play logic for this camper
    this->OnFinishedPlaying();

    // Access the PlayerState to update the player's outcome
    ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->PlayerState);
    if (playerState == nullptr)
    {
        // Do nothing if the player state is invalid
    }
    else
    {
        // The ASM checks offset 0x7F9 to verify if the player is already in the target state.
        // We abstract this raw memory access into a getter function call.
        // Value 2 corresponds to EGameState::Escaped.
        if (playerState->PlayerData._playerGameState != EGameState::VE_Escaped)
        {
            // Set the game state to Escaped
            playerState->SetPlayerGameState(EGameState::VE_Escaped);

            // 3 corresponds to ROLE_Authority
            if (this->Role == ROLE_Authority)
            {
                // Value 0 corresponds to ECamperDamageState::VE_Healthy
                this->SetDamageState(ECamperDamageState::VE_Healthy, true);
            }
        }
    }
}

// starts at line 1241
void ACamperPlayer::DBD_LeaveParadise()
{
    // Retrieve the GameInstance and cast it to the game-specific custom class
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // NOTE: The original assembly lacks this null check, resulting in a direct write 
    // to memory address 0x118 (a guaranteed crash) if the cast returned nullptr.
    // A safety check is added here to ensure the code is stable and functional.
    if (gameInstance != nullptr)
    {
        // Set the byte flag at offset 0x118 to true.
        // Utilizing the naming convention generated by IDA Pseudo due to missing struct definitions.
        gameInstance->LeaveGameRequested = true;
    }
}

// starts at line 1248
void ACamperPlayer::DBD_ForceRun(bool forced)
{
    // Check if the run action is being forced to start
    if (forced == true)
    {
        this->RunInputPressed();
    }
    else
    {
        // Safely cast the player data to UDBDCamperData
        UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

        // Ensure the player data is valid and successfully cast
        if (camperData != nullptr)
        {
            // Verify that this pawn is controlled by the local player (VTable 0x630)
            if (this->IsLocallyControlled() != false)
            {
                // Reset the run input flag
                this->_runInputPressed = false;
                
                // Notify the server that the run input has been stopped (VTable 0x1248)
                this->Broadcast_SendRunInput_Server(false);
            }
        }
    }
}

// starts at line 1261
void ACamperPlayer::DBD_AutoLeftRightMash(bool enabled)
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Apply the requested toggle state to the auto left-right mash property
        camperData->_autoLeftRightMash = enabled;
    }
}

// starts at line 1269
void ACamperPlayer::DBD_AutoInteractionMash(bool enabled)
{
    // Safely cast the base player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Apply the requested toggle state to the auto interaction mash property
        camperData->_autoInteractionMash = enabled;
    }
}

// starts at line 1277
void ACamperPlayer::SetGuidedAction(ECamperGuidedAction action)
{
    // Safely cast the base player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the current actor has network authority 
        // 3 corresponds to ENetRole::ROLE_Authority in Unreal Engine
        if (this->Role == ROLE_Authority)
        {
            // Trigger the guided action update on all clients via Remote Procedure Call (RPC)
            this->Multicast_SetGuidedAction(action);
        }
    }
}

// starts at line 1285
void ACamperPlayer::SetGuidedActionLocal(ECamperGuidedAction action)
{
    // Safely cast the base player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Apply the guided action to the camper data object
        // 0x10A offset maps to CurrentGuidedAction
        camperData->CurrentGuidedAction = action;

        // Log the action state change
        // Verbosity level 5 maps to ELogVerbosity::Verbose in Unreal Engine
        // Enum::ToString is a custom helper to stringify the enum value
        UE_LOG(LogInteraction, Verbose, TEXT("[%s] SetGuidedAction [%s]"), *this->GetName(), *Enum::ToString(action));
    }
}

// starts at line 1294
void ACamperPlayer::Multicast_SetGuidedAction_Implementation(ECamperGuidedAction action)
{
    // First, apply the guided action locally for this client/server
    this->SetGuidedActionLocal(action);

    // Retrieve the current world to access the GameMode
    UWorld* world = this->GetWorld();

    // Check explicitly if the world context is valid
    if (world != nullptr)
    {
        // Fetch the AuthorityGameMode and safely cast it to the custom ADBDGameMode class
        ADBDGameMode* authorityGameMode = Cast<ADBDGameMode>(world->AuthorityGameMode);

        // Explicitly verify the cast was successful before interacting
        if (authorityGameMode != nullptr)
        {
            // Notify the game mode that a guided action has been modified
            authorityGameMode->GuidedActionModified();
        }
    }
}

// starts at line 1308
ECamperGuidedAction ACamperPlayer::GetGuidedAction()
{
    // Safely cast the base player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Return the current guided action enum value
        return camperData->CurrentGuidedAction;
    }
    else
    {
        // If the camper data is invalid, the assembly returns 0 (null pointer).
        // We safely cast 0 back to the expected enum type (typically representing 'None' or default).
        return ECamperGuidedAction::VE_None;
    }
}

// starts at line 1317
bool ACamperPlayer::HasGuidedAction()
{
    // Safely cast the base player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the current guided action is set to something other than the default VE_Active
        if (camperData->CurrentGuidedAction != VE_None)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    // Return false if the camper data pointer is invalid
    return false;
}

// starts at line 1322
bool ACamperPlayer::IsGuidedBySlasher() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Retrieve the current guided action
        ECamperGuidedAction currentAction = camperData->CurrentGuidedAction;

        // The binary representation of 0xBC6 is 1011 1100 0110.
        // This evaluates to true if the bit index (actionValue) is 1, 2, 6, 7, 8, 9, or 11.
        if (currentAction == VE_PrepBeingKilled || currentAction == VE_BeingKilled || currentAction == VE_BeingPickedUp || 
            currentAction == VE_BeingPutDown || currentAction == VE_BeingCarried || currentAction == VE_BeingPutOnHook ||
            currentAction == VE_BeingPulledFromCloset)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // Return false if the player data pointer is invalid
    return false;
}

// starts at line 1335
void ACamperPlayer::SetBeingCarried(bool isBeingCarried, ADBDPlayer* carrier)
{
    // Check if the camper is actively being carried
    if (isBeingCarried == true)
    {
        // Assign the carrier as the player guiding the camper's position and state
        this->SetGuidingPlayer(carrier);
    }
    else
    {
        // If not being carried, clear the guiding player by passing a null pointer
        this->SetGuidingPlayer(nullptr);
    }
}

// starts at line 1348
bool ACamperPlayer::IsMovementGuided()
{
    return this->_attachedToGuidingPlayer;
}

// starts at line 1353
UDBDCamperData* ACamperPlayer::GetCamperData()
{
    // Safely cast the base player data object to the specific camper data type and return it
    return Cast<UDBDCamperData>(this->_playerData);
}

// starts at line 1358
void ACamperPlayer::BeingCarriedMovement()
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the current guided action corresponds to the carried state (value 8)
        if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_BeingCarried)
        {
            // Update the camper's position to snap to the carrier's designated joint
            this->MoveToOwnerCarryJoint();
        }
    }
}

// starts at line 1366
void ACamperPlayer::MoveToOwnerCarryJoint()
{
    // Retrieve the player that is currently carrying/guiding this camper
    ADBDPlayer* guidingPlayer = this->GetGuidingPlayer();

    // Explicitly verify the guiding player pointer
    if (guidingPlayer != nullptr)
    {
        // Check if the guiding player is a valid, active object
        // IsValidImpl corresponds to the VTable check 0x1070
        if (guidingPlayer->IsValidImpl() != false)
        {
            // CVarAllowGuidance dictates whether this snapping behavior is permitted
            if (CVarAllowGuidance.GetValueOnGameThread() != 0)
            {
                // Access the skeletal mesh of the guiding player
                USkeletalMeshComponent* guidingMesh = guidingPlayer->Mesh;
                if (guidingMesh != nullptr)
                {
                    // Access the root component to determine the guiding player's rotation
                    USceneComponent* guidingRoot = guidingPlayer->RootComponent;
                    FRotator capsuleRotation = FRotator::ZeroRotator;

                    if (guidingRoot != nullptr)
                    {
                        capsuleRotation = guidingRoot->GetComponentRotation();
                    }

                    // Flip the camper 180 degrees so they face the correct direction when carried
                    capsuleRotation.Yaw += -180.0f;

                    // Determine the feet position of the guiding player to align the base
                    FVector feetPosition = guidingPlayer->GetFeetPosition();

                    // Snap the camper's capsule to the feet position of the carrier without sweeping
                    this->SetActorLocationByCapsuleBottom(feetPosition, false, nullptr);

                    // Apply the adjusted rotation to the camper's actor
                    this->SetActorRotation(capsuleRotation, ETeleportType::None);

                    // Retrieve the specific socket/joint transform on the carrier where the camper should be attached
                    FName carrySocketName = guidingPlayer->CarryJointName;
                    FTransform socketTransform = guidingMesh->GetSocketTransform(carrySocketName, ERelativeTransformSpace::RTS_World);

                    // Decompose the socket transform
                    FRotator targetRotation = socketTransform.GetRotation().Rotator();
                    FVector targetLocation = socketTransform.GetLocation();

                    // Specifically move the camper's mesh to align with the carrier's joint in world space
                    if (this->Mesh != nullptr)
                    {
                        this->Mesh->SetWorldRotation(targetRotation, false, nullptr, ETeleportType::None);
                        this->Mesh->SetWorldLocation(targetLocation, false, nullptr, ETeleportType::None);
                    }
                }
            }
        }
    }
}

// starts at line 1390
float ACamperPlayer::GetWiggleFreeChargePercent()
{
    return this->_carryEscapeProgress->GetPercentComplete();
}

// starts at line 1395
void ACamperPlayer::Authority_AddWiggleFreeChargePercentNoModifiers(float chargePercent)
{
    // Ensure the chargeable component exists before attempting to modify it
    if (this->_carryEscapeProgress == nullptr)
    {
        // Do nothing if the component is invalid
    }
    else
    {
        // Add the specified charge percentage to the wiggle/escape progress.
        // The 0 passed in the pseudo-code represents 'false', indicating no modifiers should be applied.
        this->_carryEscapeProgress->AddChargePercent(chargePercent, false);
    }
}

// starts at line 1400
void ACamperPlayer::Authority_AddWiggleFreeCharge(float charge)
{
    // Default modifier is 1.0 (no modification)
    float multiplicativeModifierValue = 1.0f;

    // Retrieve the player that is currently carrying the camper
    ADBDPlayer* guidingPlayer = this->GetGuidingPlayer();

    if (guidingPlayer != nullptr)
    {
        // Safely cast the guiding player to ASlasherPlayer to ensure they are the Killer
        ASlasherPlayer* slasher = Cast<ASlasherPlayer>(guidingPlayer);

        if (slasher != nullptr)
        {
            // Verify that the slasher is a valid and active object
            if (slasher->IsValidImpl() != false)
            {
                // Retrieve the Perk Manager via the IPerkManagerOwnerInterface
                UPerkManager* perkManager = slasher->GetPerkManager();

                if (perkManager != nullptr)
                {
                    // 0x52 (82 in decimal) represents the specific EGameplayModifierType for wiggle progression
                    multiplicativeModifierValue = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyWiggleFreeTimer);
                }
            }
        }
    }

    // Explicitly ensure the chargeable component is valid before attempting to modify it
    if (this->_carryEscapeProgress != nullptr)
    {
        // Calculate the final charge by applying the killer's modifier
        float finalCharge = charge * multiplicativeModifierValue;

        // Apply the charge to the escape progress. 
        // The nullptr represents the instigator (AActor*), which is absent here.
        this->_carryEscapeProgress->AddCharge(finalCharge, nullptr);
    }
}

// starts at line 1411
void ACamperPlayer::Authority_ResetWiggleFreeCharge()
{
    this->_carryEscapeProgress->Reset();
}

// starts at line 1418
void ACamperPlayer::AttachToGuidingPlayer()
{
    this->_attachedToGuidingPlayer = 1;
}

// starts at line 1423
void ACamperPlayer::DetachFromGuidingPlayer()
{
    this->_attachedToGuidingPlayer = 0;
}

// starts at line 1428
void ACamperPlayer::Server_Kill_Implementation(bool sacrificed)
{
    this->Kill(sacrificed);
}

// starts at line 1438
void ACamperPlayer::Kill(bool sacrificed)
{
    // Record analytics for the camper dying. 
    // The ASM's string conversion boilerplate maps to a standard TEXT macro in UE.
    UDBDAnalytics::RecordGameProgress(TEXT("CamperDied"));

    // Trigger end-of-play logic for this camper (VTable 0xEF8)
    this->OnFinishedPlaying();

    // Check if the current actor has network authority (typically server)
    // 3 corresponds to ENetRole::ROLE_Authority
    if (this->Role == ROLE_Authority)
    {
        // Access the PlayerState to update the player's outcome
        ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->PlayerState);

        // Check if the cast was successful and the pointer is valid
        if (playerState != nullptr)
        {
            // Safely cast the player data object to the specific camper data type
            UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

            if (camperData != nullptr)
            {
                // Value 3 corresponds to ECamperDamageState::VE_Dead
                // Passing true sets the 'fireScoreEvent' parameter
                this->SetDamageState(ECamperDamageState::VE_Dead, true);

                // Determine the correct final state
                if (sacrificed == true)
                {
                    // 4 corresponds to EGameState::VE_Sacrificed
                    playerState->SetPlayerGameState(EGameState::VE_Sacrificed);
                }
                else
                {
                    // 1 corresponds to EGameState::VE_Dead
                    playerState->SetPlayerGameState(EGameState::VE_Dead);
                }
            }
        }
    }
}

// starts at line 1463
void ACamperPlayer::OnPickedupStart(ADBDPlayer* picker)
{
    // Designate the picker as the guiding player for this camper
    this->SetGuidingPlayer(picker);

    // Ensure the picker is valid before proceeding with score and collision logic
    if (picker != nullptr)
    {
        // Verify the picker is a valid active entity (VTable 0x1070)
        if (picker->IsValidImpl() != false)
        {
            // Award the killer score for picking up a camper
            // 0xA4 maps to DBDSlasherScore_PickupCamper
            picker->TryFireScoreEvent(EDBDScoreTypes::DBDSlasherScore_PickupCamper, 1.0f, true);

            // Safely cast the player data object to the specific camper data type
            UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

            if (camperData != nullptr)
            {
                // Check if the camper was previously trapped (Value 2 corresponds to VE_Escaped/Trapped state in this context)
                if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Trapped)
                {
                    // Award the killer score for picking up a camper out of a trap
                    // 0x5C maps to DBDSlasherScore_TrapPickup
                    picker->TryFireScoreEvent(EDBDScoreTypes::DBDSlasherScore_TrapPickup, 1.0f, true);/
                }
            }

            // Synchronize the camper's animation with the killer's animation
            UAnimationMontageSlave* montageSlave = Cast<UAnimationMontageSlave>(this->GetComponentByClass(UAnimationMontageSlave::StaticClass()));
            if (montageSlave != nullptr)
            {
                UMontagePlayer* pickerMontagePlayer = picker->GetMontagePlayer();
                montageSlave->SetLeader(pickerMontagePlayer);
            }

            // Instruct the picker to ignore collision with this camper
            picker->SetIgnoreActorCollision(this, true);
        }
    }

    // Instruct the camper's camera boom to ignore the picker so the camera doesn't clip into them
    UDBDSpringArmComponent* springArm = this->GetSpringArmComponent();
    if (springArm != nullptr)
    {
        springArm->IgnoreActor(picker, true);
    }

    // Adjust world collision to overlap so the camper doesn't block other entities while carried
    this->SetWorldCollisionResponse(ECollisionResponse::ECR_Overlap);

    // Disable standard movement updates for the camper
    if (this->CharacterMovement != nullptr)
    {
        // 5 corresponds to MOVE_Flying
        this->CharacterMovement->SetMovementMode(EMovementMode::MOVE_Flying);
        this->CharacterMovement->StopMovementImmediately();
        
        // Consume any lingering input vectors to prevent sudden movement if dropped
        this->CharacterMovement->ConsumeInputVector();
    }

    // Trigger the OnPickedUpDelegate to notify listeners (e.g., UI, GameState)
    // Offset 0x1298 maps directly to OnPickedUpDelegate
    this->OnPickedUpDelegate.Broadcast();

    // Set the local guided action to VE_BeingPickedUp (Value 6) to formalize the carried state
    this->SetGuidedActionLocal(ECamperGuidedAction::VE_BeingPickedUp);
}

// starts at line 1500
void ACamperPlayer::OnPickedupEnd(ADBDPlayer* picker)
{
    // Mark the camper as officially attached to the guiding player (the killer)
    this->_attachedToGuidingPlayer = true;

    // Check if the current player character is locally controlled (e.g., this is the victim's client)
    // VTable 0x630 maps to APawn::IsLocallyControlled
    if (this->IsLocallyControlled() != false)
    {
        // Retrieve the Perk Manager via the IPerkManagerOwnerInterface
        UPerkManager* perkManager = this->GetPerkManager();

        // Check if the player has a perk that triggers a skill check upon being picked up (e.g., Decisive Strike)
        // 0x26 maps to EGameplayModifierFlag for SkillCheckOnPickup
        // 0x4 maps to EGameplayModifierSource for Perks (or All)
        if (perkManager->HasPerkFlag(EGameplayModifierFlag::VE_SkillCheckOnPickup, EGameplayModifierSource::VE_All) != false)
        {
            // Access the interaction handler to trigger the skill check
            UPlayerInteractionHandler* interactionHandler = this->GetPlayerInteractionHandler();
            
            // Trigger a custom skill check with a 1.0 second delay. 
            // 1 maps to ESkillCheckCustomType::DecisiveStrike (or similar mechanic)
            interactionHandler->StartCustomSkillCheck(ESkillCheckCustomType::VE_OnPickedUp, 1.0f);
        }
    }

    // Set the local guided action to 8, which signifies the camper is now fully "Carried"
    this->SetGuidedActionLocal(ECamperGuidedAction::VE_BeingCarried);
}

// starts at line 1511
void ACamperPlayer::OnUnhooked()
{
    // Detach the camper from the guiding player (the killer who hooked them)
    this->_attachedToGuidingPlayer = false;

    // Reset world collision response to block, allowing normal physical interactions
    // 2 corresponds to ECollisionResponse::ECR_Block
    this->SetWorldCollisionResponse(ECollisionResponse::ECR_Block);

    // Re-enable server position updates so the client syncs movement correctly
    this->SetEnableServerPositionUpdates(true);

    // Retrieve the killer that previously guided this camper to the hook
    ADBDPlayer* guidingPlayer = this->GetGuidingPlayer();

    if (guidingPlayer != nullptr)
    {
        // Verify the guiding player is a valid active entity (VTable 0x1070)
        if (guidingPlayer->IsValidImpl() != false)
        {
            // Instruct the killer to stop ignoring collision with this camper
            guidingPlayer->SetIgnoreActorCollision(this, false);
        }
    }

    // Clear the guiding player reference entirely
    this->SetGuidingPlayer(nullptr);

    // Reset the local guided action state to normal/active (Value 0 maps to VE_Active)
    this->SetGuidedActionLocal(ECamperGuidedAction::VE_None);

    // Force a physical collision update now that responses and ignore lists are modified
    this->UpdateCharacterCollisions();

    // Restore camera behavior by clearing ignored actors from the spring arm
    UDBDSpringArmComponent* springArm = this->GetSpringArmComponent();
    if (springArm != nullptr)
    {
        springArm->ClearIgnoreActors();
    }

    // Reset movement component state after being immobilized on the hook
    if (this->CharacterMovement != nullptr)
    {
        // 1 corresponds to EMovementMode::MOVE_Walking
        this->CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
        this->CharacterMovement->StopMovementImmediately();
        this->CharacterMovement->ConsumeInputVector();
    }

    // Check if the current actor has network authority (3 corresponds to ROLE_Authority)
    if (this->Role == ROLE_Authority)
    {
        // Safely cast the player data object to the specific camper data type
        UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

        if (camperData != nullptr)
        {
            // Note: The decompiler hallucinated `ASlasherPlayer::Multicast_SetGuidedAction` here
            // due to matching binary signatures. This is actually a method on ACamperPlayer.
            this->Multicast_SetGuidedAction(ECamperGuidedAction::VE_None);
        }
    }

    // Flatten the camper's rotation so they are upright upon being unhooked
    FRotator newRotation = FRotator::ZeroRotator;
    
    if (this->RootComponent != nullptr)
    {
        // Extract current rotation, neutralizing pitch and roll
        newRotation = this->RootComponent->GetComponentRotation();
        newRotation.Pitch = 0.0f;
        newRotation.Roll = 0.0f;
    }

    // Apply the flattened rotation to the character actor
    this->SetActorRotation(newRotation, ETeleportType::None);
}

// starts at line 1548
void ACamperPlayer::OnMadeLoudNoise()
{
    // Check if the camper is currently outside of the killer's terror radius
    if (this->IsInTerrorRadius() == false)
    {
        // Award the player with a score event for creating a noise distraction away from the killer
        // 1.0f is the percentToAward, and true signifies the score event should be broadcasted/fired
        this->TryFireScoreEvent(DBDCamperScore_NoiseDistraction, 1.0f, true);
    }
}

// starts at line 1556
void ACamperPlayer::OnDroppedStart()
{
    // Check if the camper currently has an active skill check
    if (this->HasActiveSkillCheck() != false)
    {
        // Explicitly check if the interaction handler and skill check objects are valid
        if (this->_interactionHandler != nullptr)
        {
            if (this->_interactionHandler->_skillCheck != nullptr)
            {
                // Deactivate the ongoing skill check. The 0.25f likely represents a penalty or specific evaluation threshold.
                this->_interactionHandler->_skillCheck->DeactivateSkillCheck(0.25f);
            }
        }
    }

    // Detach the camper from the guiding player (the killer dropping them)
    this->_attachedToGuidingPlayer = false;

    // Reset the world collision response to block, allowing normal physical interactions in the world
    this->SetWorldCollisionResponse(ECollisionResponse::ECR_Block);

    // Reset the relative position and rotation of the camper's skeletal mesh to its default state
    this->ResetMeshRelativePosition();
    this->ResetMeshRelativeRotation();

    // Set the local guided action state. 
    // Note: The decompiler often hallucinates 'VE_ManuallyLeftMatch' if it shares an integer value 
    // with the actual expected ECamperGuidedAction enum value (e.g., Dropped).
    this->SetGuidedActionLocal(ECamperGuidedAction::VE_BeingPutDown);
}

// starts at line 1571
void ACamperPlayer::OnDroppedEnd(bool clearLeader)
{
    // Retrieve the killer that is currently guiding/carrying this camper
    ADBDPlayer* guidingPlayer = this->GetGuidingPlayer();

    if (guidingPlayer != nullptr)
    {
        // Verify the guiding player is a valid active entity (VTable 0x1070)
        if (guidingPlayer->IsValidImpl() != false)
        {
            // If the killer dropping the camper is locally controlled, record analytics
            // VTable 0x630 maps to APawn::IsLocallyControlled
            if (guidingPlayer->IsLocallyControlled() != false)
            {
                UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
                if (gameInstance != nullptr)
                {
                    // Access the killer analytics controller and increment the drop count.
                    // The inline offset 0x7C8 in the pseudo-code points to this specific component within UDBDGameInstance
                    gameInstance->KillerAnalyticsController.IncrementDropCount();
                }
            }

            // Instruct the killer to stop ignoring collision with this camper
            guidingPlayer->SetIgnoreActorCollision(this, false);
        }
    }

    // Clear the guiding player reference entirely since the camper is now dropped
    this->SetGuidingPlayer(nullptr);

    // Force a physical collision update now that ignore lists are modified
    this->UpdateCharacterCollisions();

    // Restore camera behavior by clearing ignored actors from the spring arm
    UDBDSpringArmComponent* springArm = this->GetSpringArmComponent();
    if (springArm != nullptr)
    {
        springArm->ClearIgnoreActors();
    }

    // Reset movement component state after being dropped
    if (this->CharacterMovement != nullptr)
    {
        // 1 corresponds to EMovementMode::MOVE_Walking
        this->CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
        this->CharacterMovement->StopMovementImmediately();
        this->CharacterMovement->ConsumeInputVector();
    }

    // If requested, sever the animation synchronization link with the killer
    if (clearLeader == true)
    {
        UAnimationMontageSlave* montageSlave = Cast<UAnimationMontageSlave>(this->GetComponentByClass(UAnimationMontageSlave::StaticClass()));
        if (montageSlave != nullptr)
        {
            // Passing nullptr clears the current leader
            montageSlave->SetLeader(nullptr);
        }
    }

    // Flatten the camper's rotation so they are upright upon hitting the ground
    FRotator newRotation = FRotator::ZeroRotator;
    
    if (this->RootComponent != nullptr)
    {
        // Extract current rotation, neutralizing pitch and roll
        newRotation = this->RootComponent->GetComponentRotation();
        newRotation.Pitch = 0.0f;
        newRotation.Roll = 0.0f;
    }

    // Apply the flattened rotation to the character actor
    this->SetActorRotation(newRotation, ETeleportType::None);

    // Apply a brief immunity window to traps (e.g., Bear Traps placed exactly where the killer drops them)
    float trapImmunityDuration = UDBDUtilities::GetTunableValue(this, TunableValues::CAMPER_DROPPED_TRAP_IMMUNITY_DURATION);
    this->TriggerTrapImmunity(trapImmunityDuration);

    // Reset the local guided action state to normal/active (Value 0 maps to VE_None)
    this->SetGuidedActionLocal(ECamperGuidedAction::VE_None);
}

// starts at line 1618
void ACamperPlayer::OnDeath()
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Verify if the current damage state is Dead (Value 3 corresponds to VE_Dead)
        if (camperData->GetDamageState() == ECamperDamageState::VE_Dead)
        {
            // Check if the current player character is locally controlled (VTable 0x630 maps to APawn::IsLocallyControlled)
            if (this->IsLocallyControlled() == true)
            {
                // Force the release of any item currently being used
                this->ItemUseReleased();
            }

            // Check if this camper is currently the killer's obsession
            if (this->IsObsessionTarget() == true)
            {
                // Retrieve the global Wwise audio device manager
                FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
                
                if (audioDevice != nullptr)
                {
                    // Post the global audio event indicating the obsession has died.
                    // The dynamic string/memory boilerplate from the assembly is replaced with the UE TEXT() macro.
                    audioDevice->PostEvent(TEXT("AudioEvent_Obsession_Killed"), nullptr, 0, nullptr, nullptr, false);
                }
            }

            // Locate the animation montage slave component to detach from the killer's synchronized animations
            // VTable 0x590 corresponds to AActor::FindComponentByClass
            UAnimationMontageSlave* montageSlave = Cast<UAnimationMontageSlave>(this->FindComponentByClass(UAnimationMontageSlave::StaticClass()));
            
            if (montageSlave != nullptr)
            {
                // Passing a null pointer clears the current animation leader
                montageSlave->SetLeader(nullptr);
            }

            // Retrieve the player that is currently guiding/carrying this camper
            ADBDPlayer* guidingPlayer = this->GetGuidingPlayer();

            if (guidingPlayer != nullptr)
            {
                // Verify the guiding player is a valid active entity (VTable 0x1070 maps to IsValidImpl)
                if (guidingPlayer->IsValidImpl() != false)
                {
                    // Safely cast the guiding player to ASlasherPlayer (the Killer class)
                    ASlasherPlayer* slasher = Cast<ASlasherPlayer>(guidingPlayer);

                    if (slasher != nullptr)
                    {
                        // Verify the slasher player is a valid active entity
                        if (slasher->IsValidImpl() != false)
                        {
                            // Instruct the killer to clear their guided camper reference
                            slasher->SetGuidedCamper(nullptr);
                        }
                    }
                }
            }

            // Clear the camper's own guiding player reference entirely
            this->SetGuidingPlayer(nullptr);
        }
    }
}

// starts at line 1648
void ACamperPlayer::DamageStateChanged(bool fireScoreEvent)
{
    // Trigger blood trail effect for the camper
    this->TriggerBloodTrail();

    // Check if the camper is in need of help (e.g., injured/dying/hooked)
    bool inNeed = this->IsInNeed();

    if (this->ProximityZone != nullptr)
    {
        if (inNeed == false)
        {
            // Disable collision for the proximity zone when not in need
            // VTable offset 0x528 maps to SetCollisionEnabled. 0 corresponds to ECollisionEnabled::NoCollision.
            this->ProximityZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            
            // Clear the potential saviors array completely.
            // The assembly ArrayNum = 0 and ResizeTo(0) is standard unrolled UE4 TArray::Empty() logic.
            this->_potentialSaviors.Empty();
        }
        else
        {
            // Enable query collision for the proximity zone to detect saviors
            // 1 corresponds to ECollisionEnabled::QueryOnly.
            this->ProximityZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }
    }

    // Force a character collision update
    this->UpdateCharacterCollisions();

    // Evaluate the player's damage state
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);
    ECamperDamageState damageState = ECamperDamageState::VE_Healthy; // Default to VE_Healthy (0)

    if (camperData != nullptr)
    {
        damageState = camperData->GetDamageState();

        // 3 corresponds to ECamperDamageState::VE_Dead
        if (damageState == ECamperDamageState::VE_Dead)
        {
            this->OnDeath();
        }
    }

    // Notify blueprints and sub-systems that the damage state has changed
    this->DamageStateChangedEvent(damageState);

    // If requested, award points for the state change
    if (fireScoreEvent == true)
    {
        // 9 corresponds to DBDCamperScore_DamageStateChanged
        // 1.0f is percentToAward, true redirects to the Game Event System
        this->TryFireScoreEvent(EDBDScoreTypes::DBDCamperScore_DamageStateChanged, 1.0f, true);
    }

    // Check if the current actor has network authority
    // 3 corresponds to ENetRole::ROLE_Authority
    if (this->Role == ROLE_Authority)
    {
        bool shouldResetEscapeProgress = false;

        if (camperData == nullptr)
        {
            shouldResetEscapeProgress = true;
        }
        else if (damageState == ECamperDamageState::VE_Healthy) // 0 corresponds to VE_Healthy
        {
            shouldResetEscapeProgress = true;
        }
        else if (damageState == ECamperDamageState::VE_Injured) // 1 corresponds to VE_Injured
        {
            shouldResetEscapeProgress = true;
        }

        if (shouldResetEscapeProgress == true)
        {
            if (this->_carryEscapeProgress != nullptr)
            {
                this->_carryEscapeProgress->Reset();
            }
        }
    }

    // Retrieve the GameInstance to update global survivor analytics
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
    
    if (gameInstance != nullptr)
    {
        // The inline offset 0x898 maps to the FGameplay_SurvivorAnalyticsController component
        gameInstance->SurvivorAnalyticsController.UpdateDamageState(damageState);
    }
}

// starts at line 1685
ECollisionResponse ACamperPlayer::GetPawnCollisionChannel() const
{
    // Check if the camper is currently crawling (dying state)
    if (this->IsCrawling() == true)
    {
        // 1 corresponds to ECollisionResponse::ECR_Overlap
        return ECollisionResponse::ECR_Overlap;
    }

    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Check if the camper is dead.
        // The assembly compares the damage state against 3, which corresponds to VE_Dead.
        if (camperData->GetDamageState() == ECamperDamageState::VE_Dead)
        {
            return ECollisionResponse::ECR_Overlap;
        }

        // Check if the camper's guided action reflects being picked up or carried.
        // The assembly compares against 6, which corresponds to VE_Disconnected.
        if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_BeingPickedUp)
        {
            return ECollisionResponse::ECR_Overlap;
        }
    }

    // Check if the camper is actively being carried by the killer
    if (this->IsBeingCarried() == true)
    {
        return ECollisionResponse::ECR_Overlap;
    }

    // Check if the camper is in the process of being put down
    if (this->IsBeingPutDown() == true)
    {
        return ECollisionResponse::ECR_Overlap;
    }

    // If none of the specific overlapping conditions are met, fall back to the base class implementation.
    // Note: We specifically do NOT use 'this->' here to prevent an infinite recursive loop.
    return Super::GetPawnCollisionChannel();
}

// starts at line 1695
bool ACamperPlayer::ComputeHookEscapeResult()
{
    // Check if the camper has a status effect or perk that guarantees a successful hook escape (e.g., Deliverance)
    if (this->_hookEscapeAutoSucess == true)
    {
        return true;
    }

    // Otherwise, rely on the standard RNG system to determine if the escape attempt succeeds
    bool escapeResult = UDBDDiceRoller::GetRollResultByDiceRollType(this, VE_EscapeHook);

    if (escapeResult == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// starts at line 1700
bool ACamperPlayer::ComputeBeartrapEscapeResult()
{
    // Check if the camper has a status effect or perk that guarantees a successful escape.
    // Note: The game logic evaluates the hook escape auto-success flag for beartraps as well.
    if (this->_hookEscapeAutoSucess == true)
    {
        return true;
    }

    // Otherwise, rely on the standard RNG system to determine if the beartrap escape attempt succeeds
    bool escapeResult = UDBDDiceRoller::GetRollResultByDiceRollType(this, VE_EscapeTrap);

    if (escapeResult == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// starts at line 1705
void ACamperPlayer::Server_SetHookEscapeAutoSuccess_Implementation(bool enabled)
{
    this->Multicast_SetHookEscapeAutoSuccess(enabled);
}

// starts at line 1710
void ACamperPlayer::Server_SetOnHookDrainToNextStage_Implementation(bool enabled)
{
    this->Multicast_SetOnHookDrainToNextStage(enabled);
}

// starts at line 1720
void ACamperPlayer::Multicast_SetOnHookDrainToNextStage_Implementation(bool enabled)
{
    this->_onHookDrainToNextStage = enabled;
}

// starts at line 1736
void ACamperPlayer::Multicast_SetHookEscapeAutoSuccess_Implementation(bool enabled)
{
    this->_hookEscapeAutoSucess = enabled;
}

// starts at line 1747
bool ACamperPlayer::IsIncapacitated() const
{
    // First, evaluate the base class implementation.
    // Note: We explicitly call the base class method without 'this->' to avoid infinite recursion.
    if (Super::IsIncapacitated() == true)
    {
        return true;
    }

    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Check if the camper is currently in an immobilized state (non-zero value).
        if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_None)
        {
            // Verify the state is not a specific excluded state.
            // Note: IDA often hallucinates 'VE_EscapedInjured' for standard integer values. 
            // This likely represents a specific non-incapacitating enum value.
            if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_Hiding)
            {
                return true;
            }
        }
    }

    // Evaluate all other specific vulnerable or transitional states
    if (this->IsDead() == true)
    {
        return true;
    }

    if (this->IsCrawling() == true)
    {
        return true;
    }

    if (this->IsBeingCarried() == true)
    {
        return true;
    }

    if (this->IsBeingPickedUp() == true)
    {
        return true;
    }

    if (this->IsBeingPutDown() == true)
    {
        return true;
    }

    // If none of the incapacitating conditions are met, the player is not incapacitated
    return false;
}

// starts at line 1753
bool ACamperPlayer::IsUnintentionallyIncapacitated() const
{
    // First, evaluate the base class implementation.
    // Note: We explicitly call the base class method without 'this->' to avoid infinite recursion.
    if (Super::IsUnintentionallyIncapacitated() == true)
    {
        return true;
    }

    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Check if the camper is currently in an immobilized state (non-zero value).
        if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_None)
        {
            // Verify the state is not a specific excluded state.
            // Note: IDA often hallucinates 'VE_EscapedInjured' for standard integer values. 
            // This likely represents a specific non-incapacitating enum value.
            if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_Hiding)
            {
                return true;
            }
        }
    }

    // Evaluate all other specific vulnerable or transitional states
    if (this->IsDead() == true)
    {
        return true;
    }

    if (this->IsCrawling() == true)
    {
        return true;
    }

    if (this->IsBeingCarried() == true)
    {
        return true;
    }

    if (this->IsBeingPickedUp() == true)
    {
        return true;
    }

    if (this->IsBeingPutDown() == true)
    {
        return true;
    }

    // If none of the incapacitating conditions are met, the player is not unintentionally incapacitated
    return false;
}

// starts at line 1758
bool ACamperPlayer::IsImmobilized() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is currently in an immobilized state (a non-zero enum value)
        if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_None)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or the immobilize state is zero
    return false;
}

// starts at line 1763
bool ACamperPlayer::IsUnintentionallyImmobilized() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is currently in an immobilized state (a non-zero enum value)
        if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_None)
        {
            // Verify the state is not a specific excluded state (e.g., an intentional immobilization)
            // Note: IDA often hallucinates 'VE_EscapedInjured' for standard integer values.
            if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_Hiding)
            {
                return true;
            }
        }
    }

    // Return false if the player data is invalid, the state is zero, or the state is intentional
    return false;
}

// starts at line 1768
bool ACamperPlayer::IsHooked() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper's immobilize state corresponds to being on a hook.
        // Note: IDA frequently hallucinates enum names like 'VE_Dead' for standard integer values.
        // In this context, the underlying integer value represents the 'Hooked' state.
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or the camper is not hooked
    return false;
}

// starts at line 1773
bool ACamperPlayer::IsSacrificed()
{
    return this->_sacrificed;
}

// starts at line 1778
void ACamperPlayer::SetIsSacrificed(bool sacrificed)
{
    this->_sacrificed = sacrificed;
}

// starts at line 1783
bool ACamperPlayer::IsTrapped() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper's immobilize state corresponds to being trapped.
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Trapped)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or the camper is not hooked
    return false;
}

// starts at line 1788
bool ACamperPlayer::IsInNeed() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);
    
    bool needsHelp = false;

    // Check if the camper is in any state that would warrant assistance from others
    if (camperData != nullptr)
    {
        // 1 corresponds to being hooked
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
        {
            needsHelp = true;
        }
        // 2 corresponds to being trapped
        else if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Trapped)
        {
            needsHelp = true;
        }
    }

    if (needsHelp == false)
    {
        if (this->IsCrawling() == true)
        {
            needsHelp = true;
        }
        else if (this->IsInjured() == true)
        {
            needsHelp = true;
        }
        // Inline offset 1579h in disassembly corresponds to a bleeding out bool
        else if (this->_isInInjuredBleedout == true)
        {
            needsHelp = true;
        }
    }

    // If the camper is in a state requiring help, verify they are not actually dead
    if (needsHelp == true)
    {
        if (camperData != nullptr)
        {
            // The assembly checks `cmp al, 3` for the DamageState. 3 corresponds to VE_Dead.
            if (camperData->GetDamageState() == ECamperDamageState::VE_Dead)
            {
                return false;
            }
        }

        // Check if the bleed-out or hook drain timer has fully elapsed
        // Inline offset 14B0h in disassembly corresponds to `_drainTimer`
        if (this->_drainTimer.IsDone() == false)
        {
            return true;
        }
    }

    // Default to false if none of the active need conditions apply
    return false;
}

// starts at line 1793
bool ACamperPlayer::IsObsessionTarget() const
{
    // Retrieve the GameState to access global match information
    UWorld* world = this->GetWorld();
    
    if (world != nullptr)
    {
        ADBDGameState* gameState = Cast<ADBDGameState>(world->GetGameState());

        // Ensure the cast was successful and the gameState pointer is valid
        if (gameState != nullptr)
        {
            // Verify the GameState is a valid active entity (VTable offset 0x140 check in assembly)
            // Retrieve the current obsession target from the GameState.
            // The abstract pointer offset `GameState[1]...Padding.Pad` (or `rbx+720h` in assembly)
            // maps directly to the obsession target property on ADBDGameState.
            ACamperPlayer* currentObsession = gameState->_obsessionTarget;

            if (currentObsession != nullptr)
            {
                // Verify the obsession target is a valid active entity (VTable 0x1070 maps to IsValidImpl)
                if (currentObsession->IsValidImpl() != false)
                {
                    // Check if this specific camper instance matches the current obsession
                    if (currentObsession == this)
                    {
                        return true;
                    }
                }
            }
        }
    }

    // Return false if any validation checks fail or if the camper is not the obsession
    return false;
}

// starts at line 1799
bool ACamperPlayer::IsInBasement() const
{
    TArray<AActor*> overlappingActors;

    // Retrieve all actors currently overlapping with this camper's collision.
    // Passing nullptr (0) as the class filter retrieves all overlapping actor types.
    this->GetOverlappingActors(overlappingActors, nullptr);

    // Search the retrieved actors for the first one that possesses the specific Basement tag.
    // Note: The assembly copies the array by value here, which is standard UFUNCTION parameter passing behavior.
    AActor* basementActor = UDBDUtilities::GetFirstActorWithTag(DBDActorTags::BASEMENT, overlappingActors);

    if (basementActor != nullptr)
    {
        // Verify the returned actor is fully valid and not pending destruction or garbage collection.
        // The assembly explicitly checks GUObjectArray flags and offset 320, mapping to UE's standard IsValid().
        if (IsValid(basementActor) == true)
        {
            return true;
        }
    }

    // If no valid basement actor is found in the overlap, the camper is not in the basement
    return false;
}

// starts at line 1807
ECamperDamageState ACamperPlayer::GetDamageState() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Retrieve and return the damage state from the camper data
        return camperData->GetDamageState();
    }

    // If the player data is invalid or missing, default to returning 0.
    // In the context of ECamperDamageState, 0 typically maps to VE_Healthy.
    return ECamperDamageState::VE_Healthy;
}

// starts at line 1816
bool ACamperPlayer::CanBecomeHealthy() const
{
    // Retrieve the GameState to access global match settings
    UWorld* world = this->GetWorld();
    if (world != nullptr)
    {
        ADBDGameState* gameState = Cast<ADBDGameState>(world->GetGameState());

        // Verify the GameState cast was successful and the object is valid
        if (gameState != nullptr)
        {
            // Verify the GameState is a valid active entity (VTable offset 0x140 check in assembly)
            // Offset 0x6F0 in the GameState (represented by the byte cast in pseudo-code) 
            // likely corresponds to a flag enforcing heal limits, e.g., a "Hardcore Mode" or specific match modifier.
            if (gameState->_usingWeakenedMechanic == true)
            {
                // Retrieve the GameInstance to access design tunables
                UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
                if (gameInstance != nullptr)
                {
                    // Access the tunable values registry
                    UDBDDesignTunables* designTunables = gameInstance->DesignTunables;
                    
                    if (designTunables != nullptr)
                    {
                        // Retrieve the maximum number of times a camper can be healed to healthy
                        float maxHealsFloat = designTunables->GetTunableValue(TunableValues::CAMPER_MAX_HEALTHY_HEALS, false);
                        int32_t maxHeals = static_cast<int32_t>(maxHealsFloat);

                        // If the camper has already reached or exceeded the maximum allowed heals, they cannot become healthy again
                        if (this->_numHealthyHeals >= maxHeals)
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }

    // Retrieve the Perk Manager via the IPerkManagerOwnerInterface
    UPerkManager* perkManager = this->GetPerkManager();

    // If the perk manager doesn't exist, assume there are no perk restrictions preventing healing
    if (perkManager == nullptr)
    {
        return true;
    }

    // Check if the camper has a perk that explicitly prevents healing to healthy (e.g., No Mither or Broken status effect)
    // 0x47 (71) corresponds to EGameplayModifierFlag::VE_CannotHealToHealthy
    // 0x4 corresponds to EGameplayModifierSource::VE_All (or Perks)
    if (perkManager->HasPerkFlag(EGameplayModifierFlag::VE_CannotHealToHealthy, EGameplayModifierSource::VE_All) == true)
    {
        return false;
    }

    // If no limits or perks restrict healing, the camper can become healthy
    return true;
}

// starts at line 1838
bool ACamperPlayer::CanBeHealedFromInjured() const
{
    // Check if the camper has reached a heal limit or is restricted by perks
    if (this->CanBecomeHealthy() == false)
    {
        return false;
    }

    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is currently in the Injured state
        // The assembly checks `cmp al, 1`, which corresponds to VE_Injured. 
        // Note: The pseudo-code hallucinated VE_Dead (3) here.
        if (camperData->GetDamageState() == ECamperDamageState::VE_Injured)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not in the Injured state
    return false;
}

// starts at line 1843
bool ACamperPlayer::CanBeHealed() const
{
    // Retrieve the GameState to determine if specific game rules apply
    UWorld* world = this->GetWorld();
    bool limitHealthyHeals = false;

    if (world != nullptr)
    {
        ADBDGameState* gameState = Cast<ADBDGameState>(world->GetGameState());

        // Verify the GameState cast was successful and the object is valid
        if (gameState != nullptr)
        {
            // The assembly checks `cmp byte ptr [rsi+6F0h], dil` which maps to a boolean rule flag
            // This is the same flag used in CanBecomeHealthy to track match-specific healing limitations
            if (gameState->_usingWeakenedMechanic == true)
            {
                limitHealthyHeals = true;
            }
        }
    }

    // Players cannot be healed while they are actively running
    // Note: The pseudo-code hallucinated a complex vtable call here, but the assembly 
    // simply executes `call qword ptr [rax+0E38h]` which maps to IsRunning().
    if (this->IsRunning() == true)
    {
        return false;
    }

    if (limitHealthyHeals == false)
    {
        // If healing limits are not enforced, standard healing rules apply:
        // A player can be healed if they can go from Injured to Healthy
        if (this->CanBeHealedFromInjured() == true)
        {
            return true;
        }

        // A player can also be healed if they are in the dying state (Knocked Out)
        if (this->IsKO() == true)
        {
            return true;
        }
    }
    else
    {
        // If healing limits ARE enforced, the rules are slightly modified:
        if (this->CanBeHealedFromInjured() == true)
        {
            return true;
        }

        if (this->IsKO() == true)
        {
            return true;
        }

        // Additionally, if limits are enforced, check if the player is in an active bleedout state while injured.
        // The assembly checks `cmp byte ptr [rbx+1579h], dil` which maps to this specific boolean.
        if (this->_isInInjuredBleedout == true)
        {
            return true;
        }
    }

    // If none of the conditions are met, the player cannot be healed
    return false;
}

// starts at line 1858
bool ACamperPlayer::CanHealSelf() const
{
    // Retrieve the GameState to determine if specific game rules apply
    UWorld* world = this->GetWorld();
    bool limitHealthyHeals = false;

    if (world != nullptr)
    {
        ADBDGameState* gameState = Cast<ADBDGameState>(world->GetGameState());

        // Verify the GameState cast was successful and the object is valid
        if (gameState != nullptr)
        {
            // Offset 0x6F0 maps to the boolean flag enforcing heal limits
            if (gameState->_usingWeakenedMechanic == true)
            {
                limitHealthyHeals = true;
            }
        }
    }

    if (limitHealthyHeals == false)
    {
        // Standard healing rules: Player must be eligible to become healthy
        if (this->CanBecomeHealthy() == false)
        {
            return false;
        }

        // Safely cast the player data object to the specific camper data type
        UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

        if (camperData != nullptr)
        {
            // Verify the camper is exactly in the Injured state
            // Assembly: `cmp al, 1`. Value 1 corresponds to VE_Injured.
            if (camperData->GetDamageState() == ECamperDamageState::VE_Injured)
            {
                // Verify the camper is not currently locked into another guided action (e.g. being carried)
                // Assembly: `cmp [rax+10Ah], bl` where `bl` is 0. 0 corresponds to VE_None / VE_Active.
                if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_None)
                {
                    return true;
                }
            }
        }
    }
    else
    {
        // Hardcore/Limited healing rules: Rely on the CanBeHealedFromInjured wrapper
        if (this->CanBeHealedFromInjured() == true)
        {
            // Ensure the camper is not locked in a guided action preventing them from healing
            if (this->HasGuidedAction() == false)
            {
                return true;
            }
        }
    }

    // If none of the valid conditions are met, the player cannot heal themselves
    return false;
}

// starts at line 1873
bool ACamperPlayer::CanStruggle() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is in an immobilize state that allows struggling.
        // Note: As seen in previous functions, IDA hallucinates enum names for these integer states.
        // 1 (hallucinated as VE_Dead) corresponds to the Hooked state.
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
        {
            return true;
        }

        // 2 (hallucinated as VE_Escaped) corresponds to the Trapped state (e.g., Bear Trap).
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Trapped)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not in a state that allows struggling
    return false;
}

// starts at line 1881
bool ACamperPlayer::CanCrouchGameSpecific()
{
    // A camper cannot crouch if they are in the dying/crawling state
    if (this->IsCrawling() == true)
    {
        return false;
    }

    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // A camper cannot crouch if they are currently immobilized (e.g., trapped or hooked)
        // The assembly evaluates `cmp byte ptr [rax+109h], 0`. 
        // 0 corresponds to a free/none state (IDA hallucinated VE_Active).
        if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_None)
        {
            return false;
        }
    }

    // A camper cannot crouch if they are generally incapacitated
    // Note: Calling the base class explicitly to avoid recursive loops, even though
    // this specific function isn't IsIncapacitated.
    if (Super::IsIncapacitated() == true)
    {
        return false;
    }

    // Finally, rely on the base class rules for crouching (e.g., movement mode checks)
    if (Super::CanCrouchGameSpecific() == false)
    {
        return false;
    }

    // If all checks pass, the camper is allowed to crouch
    return true;
}

// starts at line 1886
bool ACamperPlayer::IsHealthy() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Retrieve the current damage state and check if it represents being healthy.
        // The assembly evaluates `test al, al` (comparing to 0). 
        // 0 corresponds to the Healthy state. (IDA hallucinated 'VE_Active').
        if (camperData->GetDamageState() == ECamperDamageState::VE_Healthy)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not in the healthy state
    return false;
}

// starts at line 1892
bool ACamperPlayer::IsInjured() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is currently in the Injured state.
        // Note: The pseudo-code hallucinated checking against 'VE_Dead' and returning an enum. 
        // Based on the function name and standard boolean truthiness, it is checking if the state equals 1 (VE_Injured).
        if (camperData->GetDamageState() == ECamperDamageState::VE_Injured)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not in the Injured state
    return false;
}

// starts at line 1897
bool ACamperPlayer::IsKO() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is currently in the Knocked Out (Dying) state.
        // Note: The pseudo-code hallucinated checking against 'VE_Escaped' (value 2). 
        // Based on the established damage state sequence (0 = Healthy, 1 = Injured, 3 = Dead),
        // a value of 2 maps logically to the Knocked Out / Dying state.
        if (camperData->GetDamageState() == ECamperDamageState::VE_KO)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not in the Knocked Out state
    return false;
}

// starts at line 1902
bool ACamperPlayer::IsInInjuredBleedout()
{
    return this->_isInInjuredBleedout;
}

// starts at line 1907
bool ACamperPlayer::CanBeImmediatelyKO() const
{
    // Retrieve the Perk Manager via the IPerkManagerOwnerInterface
    UPerkManager* perkManager = this->GetPerkManager();

    // Check if the perk manager is valid
    if (perkManager != nullptr)
    {
        // Check if the camper has a modifier that prevents them from being immediately knocked out.
        // The assembly passes 0x98 (152) to HasPerkModifierOfType. 
        // 0x98 corresponds to the EGameplayModifierType::VE_SetInjuredBleedoutTime modifier.
        if (perkManager->HasPerkModifierOfType(EGameplayModifierType::VE_SetInjuredBleedoutTime) == false)
        {
            return true;
        }
    }

    // If the perk manager is invalid, or if the player HAS the VE_SetInjuredBleedoutTime modifier, they cannot be immediately KO'd.
    return false;
}

// starts at line 1912
bool ACamperPlayer::CanBeShocked() const
{
    // A camper must be in a valid state to be slashed in order to be shocked
    // The assembly performs a virtual call here (vtable offset 0xE90)
    if (this->CanBeSlashed() == false)
    {
        return false;
    }

    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Check if the camper is currently in an immobilize state that grants shock immunity.
        // Assembly: `cmp byte ptr [rax+109h], 1`. 
        // State 1 corresponds to being Hooked (IDA hallucinated VE_Dead).
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
        {
            return false;
        }

        // Assembly: `cmp byte ptr [rax+109h], 3`. 
        // State 3 corresponds to being hidden (e.g., in a locker) or fully killed (IDA hallucinated VE_EscapedInjured).
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hiding)
        {
            return false;
        }
    }

    // A camper cannot be shocked if they are generally incapacitated.
    // Note: We explicitly call the base class method to avoid overriding logic.
    if (Super::IsIncapacitated() == true)
    {
        return false;
    }

    // A camper cannot be shocked if they have already escaped the trial
    if (this->IsEscaped() == true)
    {
        return false;
    }

    // If all checks pass, the camper is vulnerable to being shocked
    return true;
}

// starts at line 1917
bool ACamperPlayer::IsDead() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is currently in the Dead state.
        // Note: The pseudo-code hallucinated checking against 'VE_EscapedInjured'. 
        // Based on the established damage state sequence (0 = Healthy, 1 = Injured, 2 = KO, 3 = Dead),
        // we map this back to the integer value 3.
        if (camperData->GetDamageState() == ECamperDamageState::VE_Dead)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not dead
    return false;
}

// starts at line 1922
bool ACamperPlayer::IsCrawling() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is in the Knocked Out (Dying) damage state.
        // The assembly evaluates `cmp al, 2`. State 2 represents the KO/Dying state.
        if (camperData->GetDamageState() == ECamperDamageState::VE_KO)
        {
            // Verify that the camper is not currently immobilized on a hook.
            // The assembly evaluates `cmp byte ptr [rax+109h], 1`. State 1 corresponds to being Hooked.
            // Note: The pseudo-code hallucinated 'VE_Dead' for this integer value.
            if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_Hooked)
            {
                return true;
            }
        }
    }

    // Return false if the player data is invalid, the player is not in the KO state, or they are hooked
    return false;
}

// starts at line 1927
bool ACamperPlayer::IsBeingKilled() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is currently locked in a "being killed" guided action (e.g., a Killer Mori).
        // Note: The pseudo-code hallucinated checking against the enum 'VE_Escaped'. 
        // Based on our established pattern of IDA hallucinating this enum for the integer value 2, 
        // we map this back to 2, which logically represents the 'Being Killed' guided action state.
        if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_BeingKilled)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not currently being killed
    return false;
}

// starts at line 1932
bool ACamperPlayer::IsBeingPickedUp() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_BeingPickedUp)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not currently being killed
    return false;
}

// starts at line 1937
bool ACamperPlayer::IsBeingCarried() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_BeingCarried)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not currently being killed
    return false;
}

// starts at line 1942
bool ACamperPlayer::IsBeingHealed() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_BeingHealed)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not currently being killed
    return false;
}

// starts at line 1947
bool ACamperPlayer::IsBeingPutDown() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_BeingPutDown)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not currently being killed
    return false;
}

// starts at line 1952
bool ACamperPlayer::IsBeingPutOnHook() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_BeingPutOnHook)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not currently being killed
    return false;
}

// starts at line 1957
bool ACamperPlayer::CanRushQuietly() const
{
    // Retrieve the Perk Manager via the IPerkManagerOwnerInterface
    UPerkManager* perkManager = this->GetPerkManager();

    // Explicitly check if the perk manager is valid
    if (perkManager != nullptr)
    {
        // Check if the camper has the specific modifier allowing quiet rushes.
        // The assembly passes 0xA8 (168) to HasPerkModifierOfType.
        // Note: The IDA pseudo-code hallucinated a bizarre bitwise OR here (VE_DisableStalking|0x80).
        // 0xA8 corresponds to the specific EGameplayModifierType that silences rush actions.
        if (perkManager->HasPerkModifierOfType(EGameplayModifierType::VE_ModifyQuickActionLoudNoiseRange) == true)
        {
            return true;
        }
    }

    // Return false if the perk manager is invalid or the player lacks the required modifier
    return false;
}

// starts at line 1962
bool ACamperPlayer::IsInsideCloset() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hiding)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or they are not currently being killed
    return false;
}

// starts at line 1967
bool ACamperPlayer::IsTotallyInsane() const
{
    // Retrieve the Perk Manager via the IPerkManagerOwnerInterface
    UPerkManager* perkManager = this->GetPerkManager();

    // Check if the perk manager exists
    if (perkManager != nullptr)
    {
        // Check if the camper has the specific "Totally Insane" flag.
        // The assembly passes 0x34 (52) for the modifier flag and 4 for the source.
        // Note: The IDA pseudo-code actually got the enum name right this time ('VE_TotallyInsane').
        // 0x34 corresponds to EGameplayModifierFlag::VE_TotallyInsane.
        // 4 corresponds to EGameplayModifierSource::VE_All.
        if (perkManager->HasPerkFlag(EGameplayModifierFlag::VE_TotallyInsane, EGameplayModifierSource::VE_All) == true)
        {
            return true;
        }
    }

    // Return false if the perk manager is invalid or the player does not have the insanity flag
    return false;
}

// starts at line 1972
bool ACamperPlayer::CanGainInsanity() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // A camper cannot gain insanity if they are dead.
        // The assembly evaluates `cmp al, 3`. We established earlier that state 3 is Dead.
        if (camperData->GetDamageState() == ECamperDamageState::VE_Dead)
        {
            return false;
        }
    }

    // Safely cast the generic Unreal Engine PlayerState to the game-specific ADBDPlayerState
    ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->PlayerState); 

    if (playerState != nullptr)
    {
        // A camper cannot gain insanity if they have already escaped the trial
        if (playerState->IsEscaped() == true)
        {
            return false;
        }
    }

    if (camperData != nullptr)
    {
        // A camper cannot gain insanity if they are currently hooked.
        // The assembly evaluates `cmp byte ptr [rax+109h], 1`. State 1 is Hooked.
        // Note: The IDA pseudo-code hallucinated 'VE_Dead' here.
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
        {
            return false;
        }
    }

    // A camper cannot gain insanity if they are hiding inside a locker/closet
    if (this->IsInsideCloset() == true)
    {
        return false;
    }

    // A camper cannot gain insanity while being carried by the killer
    if (this->IsBeingCarried() == true)
    {
        return false;
    }

    // A camper cannot gain insanity if they are generally incapacitated.
    // We explicitly call the base class method here to avoid dynamic dispatch loops.
    if (Super::IsIncapacitated() == true)
    {
        return false;
    }

    // If none of the immunity conditions are met, the camper can gain insanity
    return true;
}

// starts at line 1977
bool ACamperPlayer::IsPartiallyHidden() const
{
    // Explicitly verify the stillness tracker component is valid before accessing it
    if (this->_stillnessTracker != nullptr)
    {
        // Retrieve the camper's current stillness value and evaluate if it falls below the partially hidden threshold
        if (this->_stillnessTracker->GetSpeedStillness() < this->PartiallyHiddenStillnessThreshold)
        {
            return true;
        }
    }

    // Return false if the component is invalid or the player is moving too much to be considered hidden
    return false;
}

// starts at line 1982
bool ACamperPlayer::IsTotallyHidden() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Check if the camper is in an immobilize state that guarantees they are hidden.
        // The assembly evaluates `cmp byte ptr [rax+109h], 3`.
        // As we saw in CanBeShocked, state 3 corresponds to being hidden (e.g., inside a locker).
        // Note: The IDA pseudo-code hallucinated 'VE_EscapedInjured' here.
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hiding)
        {
            return true;
        }
    }

    // If the camper is not in a locker, they can still be totally hidden if they are
    // both partially hidden (based on stillness) AND actively crouching.
    
    // The assembly performs a virtual call here (vtable offset 0xD80) mapped to IsPartiallyHidden.
    if (this->IsPartiallyHidden() == true)
    {
        // A direct call to the base ADBDPlayer class to check the crouching state
        if (this->IsCrouching() == true)
        {
            return true;
        }
    }

    // If none of the conditions are met, the player is visible to some degree
    return false;
}

// starts at line 1987
bool ACamperPlayer::IsEscaped() const
{
    // Safely cast the generic Unreal Engine PlayerState to the game-specific ADBDPlayerState
    ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->PlayerState);

    // Explicitly check if the cast was successful and the pointer is valid
    if (playerState != nullptr)
    {
        // Delegate the logic to the specific PlayerState's IsEscaped method
        return playerState->IsEscaped();
    }

    // Return false if the PlayerState is invalid or missing
    return false;
}

// starts at line 2001
AActor* ACamperPlayer::GetEscapeObject()
{
    return this->_escape;
}

// starts at line 2006
bool ACamperPlayer::CanBeKilled() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData == nullptr)
    {
        return false;
    }

    // A camper can only be killed (e.g., via a Killer Mori) if they are in the Knocked Out (Dying) state.
    // The assembly evaluates `cmp al, 2`. We established earlier that state 2 is KO.
    if (camperData->GetDamageState() != ECamperDamageState::VE_KO)
    {
        return false;
    }

    // A camper cannot be killed if they are currently Hooked.
    // The assembly evaluates `cmp byte ptr [rax+109h], 1`. State 1 is Hooked.
    // Note: The IDA pseudo-code hallucinated 'VE_Dead' here.
    if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
    {
        return false;
    }

    // A camper cannot be killed if they are currently Trapped (e.g., in a Bear Trap).
    // The assembly evaluates `cmp byte ptr [rax+109h], 2`. State 2 is Trapped.
    // Note: The IDA pseudo-code hallucinated 'VE_Escaped' here (just like in CanStruggle).
    if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Trapped)
    {
        return false;
    }

    // A camper cannot be killed if they are actively being interacted with (e.g., being healed by another survivor).
    if (this->IsBeingInteractedWith() == true)
    {
        return false;
    }

    // If all vulnerability checks pass, the camper is eligible to be killed
    return true;
}

// starts at line 2014
bool ACamperPlayer::CanBePickedUp() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData == nullptr)
    {
        return false;
    }

    // A camper can be picked up if they are in the Knocked Out (Dying) state.
    // The assembly evaluates `cmp al, 2`. State 2 is KO.
    bool isKO = (camperData->GetDamageState() == ECamperDamageState::VE_KO);

    // A camper can also be picked up if they are currently Trapped (e.g., in a Bear Trap).
    // The assembly evaluates `cmp byte ptr [rax+109h], 2`. State 2 is Trapped.
    // Note: The IDA pseudo-code hallucinated 'VE_Escaped' here.
    bool isTrapped = (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Trapped);

    // The player MUST be either KO or Trapped to be picked up.
    if (isKO == false && isTrapped == false)
    {
        return false;
    }

    // A camper cannot be picked up if they are currently Hooked.
    // The assembly evaluates `cmp byte ptr [rax+109h], 1`. State 1 is Hooked (IDA: VE_Dead).
    if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
    {
        return false;
    }

    // A camper cannot be picked up if they are currently in a Locker / Killed.
    // The assembly evaluates `cmp byte ptr [rax+109h], 3`. State 3 is Hidden/Killed (IDA: VE_EscapedInjured).
    if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hiding)
    {
        return false;
    }

    // A camper cannot be picked up if they are already locked in a guided action (e.g., being Mori'd).
    // The assembly evaluates `cmp byte ptr [rax+10Ah], 0`. State 0 is None/Free (IDA: VE_Active).
    if (camperData->CurrentGuidedAction != ECamperGuidedAction::VE_None)
    {
        return false;
    }

    // A camper cannot be picked up if they have already escaped the trial.
    if (this->IsEscaped() == true)
    {
        return false;
    }

    // If all state checks validate, the camper is eligible to be picked up by the killer.
    return true;
}

// starts at line 2024
bool ACamperPlayer::IsBeingInteractedWith() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the camper is currently locked in any guided action (e.g., being healed by a teammate).
        // The pseudo-code checks for truthiness, meaning any non-zero value indicates an active interaction.
        // As we saw in CanBePickedUp, a value of 0 corresponds to being free/none.
        if (camperData->CurrentGuidedAction != ECamperGuidedAction::VE_None)
        {
            return true;
        }
    }

    // Return false if the player data is invalid or the player is not currently in a guided action
    return false;
}

// starts at line 2029
void ACamperPlayer::MoveForward(float Val)
{
    // Ensure the player is actively controlled and the input value is non-zero
    // The assembly evaluates `ucomiss xmm6, xmm0` (where xmm0 is 0.0f)
    if (this->Controller != nullptr && Val != 0.0f)
    {
        // Check if the Controller is a valid Unreal Engine object.
        // The massive block of GUObjectArray checks in the assembly maps to the standard UE IsValid() check.
        if (IsValid(this->Controller) == true)
        {
            // Get the controller's current rotation (where the player's camera is looking)
            FRotator rotation = this->Controller->GetControlRotation();

            // Zero out pitch and roll to ensure movement is strictly restricted to the horizontal plane.
            // Assembly: `mov [Direction.X], 0` (Pitch), `mov [Direction.Z], 0` (Roll).
            FRotator yawRotation = FRotator(0.0f, rotation.Yaw, 0.0f);

            // Create a rotation matrix from the yaw rotation and extract the forward vector (X-axis).
            // Assembly uses `FRotationMatrix::FRotationMatrix` and `FMatrix::GetUnitAxis(EAxis::X)`.
            // Note: In UE's EAxis enum, 1 corresponds to EAxis::X (Forward).
            FRotationMatrix rotationMatrix = FRotationMatrix(yawRotation);
            FVector forwardVector = rotationMatrix.GetUnitAxis(EAxis::X);

            // Apply the custom movement speed multiplier specific to the Camper Player
            // Assembly: `mulss` instructions against `this->_movementSpeedMultiplier` (offset 0xE40).
            FVector directionalInput = forwardVector * this->_movementSpeedMultiplier;

            // Apply the calculated directional vector and the input scale value to the movement component.
            // Assembly performs a virtual call (`call qword ptr [rax+6A8h]`) mapping to AddMovementInput.
            this->AddMovementInput(directionalInput, Val, false);
        }
    }
}

// starts at line 2043
void ACamperPlayer::MoveRight(float Val)
{
    // Ensure the player is actively controlled and the input scale is non-zero
    if (this->Controller != nullptr && Val != 0.0f)
    {
        // Check if the Controller is a valid Unreal Engine object
        // The assembly expands standard IsValid() checks against GUObjectArray
        if (IsValid(this->Controller) == true)
        {
            // Get the controller's current rotation
            FRotator rotation = this->Controller->GetControlRotation();

            // Zero out pitch and roll to ensure strafing is restricted to the horizontal plane.
            // Pitch (X) and Roll (Z) are set to 0.0f.
            FRotator yawRotation = FRotator(0.0f, rotation.Yaw, 0.0f);

            // Create a rotation matrix from the yaw rotation and extract the right vector (Y-axis).
            // Assembly uses `mov r8d, 2` for the InAxis parameter.
            // In Unreal's EAxis enum, 2 corresponds to EAxis::Y (Right).
            FRotationMatrix rotationMatrix = FRotationMatrix(yawRotation);
            FVector rightVector = rotationMatrix.GetUnitAxis(EAxis::Y);

            // Apply the camper-specific movement speed multiplier
            // Assembly: mulss instructions against _movementSpeedMultiplier at offset 0xE40
            FVector directionalInput = rightVector * this->_movementSpeedMultiplier;

            // Apply the calculated directional vector and the input scale value.
            // Assembly performs a virtual call at vtable offset 0x6A8 mapping to AddMovementInput.
            this->AddMovementInput(directionalInput, Val, false);
        }
    }
}

// starts at line 2058
void ACamperPlayer::RunInputPressed()
{
    // Ensure player data is valid before processing input
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (IsValid(camperData) == true)
    {
        // Check if the player is currently holding an item
        ACollectable* currentItem = this->GetItem();

        if (IsValid(currentItem) == true)
        {
            // If the item is currently being used AND it is flagged to stop running on use,
            // we must release the item use to prioritize the run input.
            // Assembly offsets: 0x3A9 (IsInUse), 0x3A3 (StopRunningOnUse)
            if (currentItem->IsInUse == true && currentItem->StopRunningOnUse == true)
            {
                this->ItemUseReleased();
            }
        }

        // Reset the footprint timer so a footprint is generated immediately upon running.
        // Assembly offset: 0x13D8
        this->_secondsUntilNextFootprint = 0.0f;

        // If this character is controlled by the local player, update state and notify server.
        // Assembly virtual call offset: 0x630
        if (this->IsLocallyControlled() == true)
        {
            // Set the local run input state. Assembly offset: 0x1590
            this->_runInputPressed = true;

            // Broadcast the run input to the server.
            // Assembly virtual call offset: 0x1248
            this->Broadcast_SendRunInput_Server(true);
        }
    }
}

// starts at line 2072
void ACamperPlayer::StruggleInputPressed()
{
    // Safely cast and validate the player data object
    // The assembly performs standard GUObjectArray checks here which map to UE's IsValid().
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (IsValid(camperData) == true)
    {
        // Attempt to trigger the Struggle interaction type.
        // The assembly loads the interaction type enum value (0x1D / 29) into the dl register 
        // before jumping directly to ADBDPlayer::TryInteractionType.
        // 29 corresponds to EInputInteractionType::VE_Struggle.
        this->TryInteractionType(EInputInteractionType::VE_Struggle, this);
    }
}

// starts at line 2080
void ACamperPlayer::RunInputReleased()
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Check if the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Check if the character is controlled by the local player.
        // Assembly virtual call offset: 0x630
        if (this->IsLocallyControlled() == true)
        {
            // Clear the local run input state flag. Assembly offset: 0x1590
            this->_runInputPressed = false;

            // Broadcast the released input state to the server.
            // Assembly tail-call virtual jump offset: 0x1248
            this->Broadcast_SendRunInput_Server(false);
        }
    }
}

// starts at line 2088
void ACamperPlayer::ActionInputPressed()
{
    // Attempt to trigger the primary Action interaction type.
    // The integer value 30 maps to the Action interaction (e.g., repairing, healing, unhooking).
    // 30 corresponds to EInputInteractionType::VE_Action (or similar naming convention).
    this->TryInteractionType(EInputInteractionType::VE_Action, this);

    // Ensure the player data pointer is valid before modifying its internal state
    if (this->_playerData != nullptr)
    {
        // Flag the action input as actively pressed within the player's data structure
        this->_playerData->_actionInputPressed = true;
    }
}

// starts at line 2094
void ACamperPlayer::ActionInputReleased()
{
    this->_playerData->_actionInputPressed = 0;
}

// starts at line 2099
void ACamperPlayer::Gesture01Pressed()
{
    this->TryInteractionType(EInputInteractionType::VE_Gesture01, this);
}

// starts at line 2104
void ACamperPlayer::Gesture02Pressed()
{
    this->TryInteractionType(EInputInteractionType::VE_Gesture02, this);
}

// starts at line 2109
void ACamperPlayer::Gesture03Pressed()
{
    this->TryInteractionType(EInputInteractionType::VE_Gesture03, this);
}

// starts at line 2114
void ACamperPlayer::Gesture04Pressed()
{
    this->TryInteractionType(EInputInteractionType::VE_Gesture04, this);
}

// starts at line 2119
void ACamperPlayer::DoItemUsePressed()
{
    // Retrieve the currently equipped item
    ACollectable* currentItem = this->GetItem();

    // Ensure the item is valid.
    // The assembly performs standard compiler-unrolled GUObjectArray validation here.
    if (IsValid(currentItem) == true)
    {
        // Check if using this item forces the player to stop running.
        // Assembly offset: 0x3A3 (StopRunningOnUse)
        if (currentItem->StopRunningOnUse == true)
        {
            // Verify the player is locally controlled before sending network state updates.
            // Assembly virtual call offset: 0x630 mapped to IsLocallyControlled.
            if (this->IsLocallyControlled() == true)
            {
                // Set the local flag indicating running is prevented by the current item.
                // Assembly offset: 0x1591
                this->_itemPreventRunning = true;

                // Broadcast this state restriction to the server.
                // Assembly virtual call offset: 0x1258
                this->Broadcast_SendItemPreventRun_Server(true);
            }
        }

        // Check if the player is currently crouching
        if (this->IsCrouching() == true)
        {
            // Check if the item forces the player into a strafing/standing posture when used.
            // Assembly offset: 0x3A0 (StrafeOnUse)
            if (currentItem->StrafeOnUse == true)
            {
                // Force the player out of the crouched state
                this->CrouchInputReleased();
            }
        }
    }

    // Pass execution to the base class to handle the core item use mechanics.
    // The assembly performs a tail-call optimization here (jmp ?DoItemUsePressed@ADBDPlayer...)
    Super::DoItemUsePressed();
}

// starts at line 2137
void ACamperPlayer::DoItemUseReleased()
{
    // Verify the player is locally controlled before sending network state updates.
    if (this->IsLocallyControlled() == true)
    {
        // Clear the local flag indicating running is restricted by an item.
        this->_itemPreventRunning = false;

        // Broadcast this state clearance to the server so the player can sprint again.
        this->Broadcast_SendItemPreventRun_Server(false);
    }

    // Pass execution to the base class to handle the core item release mechanics.
    Super::DoItemUseReleased();
}

// starts at line 2144
float ACamperPlayer::GetMaximumMovementSpeed() const
{
    // Retrieve the base Character Movement component and safely cast it to the Camper-specific movement component
    UCamperMovementComponent* camperMovement = Cast<UCamperMovementComponent>(this->CharacterMovement);

    // Explicitly check if the cast was successful and the pointer is valid
    if (camperMovement != nullptr)
    {
        // Return the specific maximum movement speed value.
        // The assembly reads a float from offset 0x778 (movss xmm0, dword ptr [rbx+778h]).
        // Note: The IDA pseudo-code hallucinated an array length lookup here.
        return camperMovement->MaxRunSpeed;
    }

    // Return 0.0f if the movement component is missing or invalid
    return 0.0f;
}

// starts at line 2157
void ACamperPlayer::OnInteractionZoneOverlap(UInteractionDefinition* interactionDef, bool overlapping)
{
    // Explicit safety check for the incoming pointer, even though the pseudo-code 
    // implies it assumes validity before accessing OwnerPrivate.
    if (interactionDef != nullptr)
    {
        // Retrieve the Actor that owns this interaction component.
        // In Unreal Engine, ActorComponents store their owning Actor in OwnerPrivate.
        AActor* interactionOwner = interactionDef->GetOwner();

        // Check if the interaction is attached to a valid actor in the world.
        // The pseudo-code's GUObjectArray checks map to this standard UE validation.
        if (IsValid(interactionOwner) == true)
        {
            // Check if this Camper Player is the ultimate owner of that actor.
            // If the player owns the interaction (e.g., an item they are already holding),
            // we intentionally return early to avoid processing a self-overlap.
            if (interactionOwner->GetOwner() == this)
            {
                return;
            }
        }
    }

    // For all other interactions (not owned by this player, or lacking an owner),
    // pass the event up to the base class to be processed normally.
    Super::OnInteractionZoneOverlap(interactionDef, overlapping);
}

// starts at line 2167
void ACamperPlayer::SetImmobilized(ECamperImmobilizeState state)
{
    this->Server_SetImmobilized(state);
}

// starts at line 2172
ECamperImmobilizeState ACamperPlayer::GetImmobilizedState() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Check if the cast was successful and the object is valid within Unreal Engine.
    // The pseudo-code explicitly expands the GUObjectArray checks, which maps to IsValid().
    if (IsValid(camperData) == true)
    {
        // Return the current immobilize state from the camper data
        return camperData->ImmobilizeState;
    }

    // Return 0 (typically mapping to VE_None / free state) if the player data is missing or invalid
    return ECamperImmobilizeState::VE_None;
}

// starts at line 2181
void ACamperPlayer::Server_SetImmobilized_Implementation(ECamperImmobilizeState state)
{
    // The massive block of GUObjectArray checks, bitwise operations, and ClassTreeIndex math
    // in the pseudo-code represents standard Unreal Engine validation. 
    // It is checking if the Controller is valid, and if its PlayerState is a valid ADBDPlayerState.
    if (IsValid(this->Controller) == true)
    {
        ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->Controller->PlayerState);
        if (IsValid(playerState) == true)
        {
            // The pseudo-code ends this validation block with a completely unused SafeCast.
            // This suggests that either a developer cast it and did nothing, or it's an inline expansion
            // artifact where the compiler discarded the result because it wasn't assigned.
            // I have included it here to strictly mirror the compiled output.
            Cast<UDBDCamperData>(this->_playerData);
        }
    }

    // Call the Multicast RPC to distribute the new immobilized state to all connected clients
    this->Multicast_SetImmobilized(state);

    // Update the character's collisions (e.g., removing collision if the player is being carried/hooked)
    // The assembly performs a tail-call optimization here (jmp ?UpdateCharacterCollisions@ADBDPlayer@@QEAAXXZ)
    this->UpdateCharacterCollisions();
}

// starts at line 2205
void ACamperPlayer::Multicast_SetImmobilized_Implementation(ECamperImmobilizeState state)
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Apply the replicated immobilize state to the local data component
        camperData->ImmobilizeState = state;
    }

    // Check if the incoming state represents hiding (e.g., inside a locker).
    // The assembly compares against 3 (`cmp dil, 3`).
    // Note: The IDA pseudo-code hallucinated 'VE_EscapedInjured' for this integer.
    if (state == ECamperImmobilizeState::VE_Hiding)
    {
        // Reset the hide timer using the default time-to-hidden duration
        this->_hideTimer.Reset(this->_timeToHidden);
    }

    // Determine if the camper currently requires assistance (e.g., dying or hooked)
    if (this->IsInNeed() == true)
    {
        // If the player needs saving, enable the proximity zone to detect potential saviors
        // The assembly passes 1 to the virtual function, which maps to ECollisionEnabled::QueryOnly
        if (this->ProximityZone != nullptr)
        {
            this->ProximityZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }
    }
    else
    {
        // If the player does not need saving, turn off the proximity zone's collision
        // The assembly passes 0 to the virtual function, which maps to ECollisionEnabled::NoCollision
        if (this->ProximityZone != nullptr)
        {
            this->ProximityZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        // Empty the list of potential saviors since the player no longer needs saving.
        // The IDA pseudo-code completely hallucinated a RecastNavMesh template evaluation here.
        // The raw assembly manually resets ArrayNum to 0 and calls ResizeTo(0), which is the standard
        // implementation of the Unreal Engine TArray::Empty() macro.
        this->_potentialSaviors.Empty();
    }
}

// starts at line 2225
void ACamperPlayer::SetDamageState(ECamperDamageState e, bool fireScoreEvent)
{
    // Check if the player is actually a Survivor/Camper.
    // The assembly evaluates `cmp byte ptr [this+110h], 3`. 
    // In EPlayerRole, 3 typically corresponds to the Camper role.
    if (this->Role == ROLE_Authority)
    {
        // Safely cast the player data object to the specific camper data type
        UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

        if (camperData != nullptr)
        {
            // Apply the new damage state to the camper data.
            // *NOTE:* The assembly explicitly calls UActorKnowledgeCollection::Authority_SetSharingType.
            // This is a linker optimization (ICF). The compiler realized that setting a 1-byte enum
            // for DamageState and setting a 1-byte enum for SharingType resulted in the exact same 
            // machine code, so it merged the functions. IDA just picked the wrong symbol name!
            camperData->SetDamageState(e);
        }

        // Trigger the internal event to update animations, HUD, and audio
        this->DamageStateChanged(fireScoreEvent);

        if (camperData != nullptr)
        {
            // Check if the newly applied state is Knocked Out (Dying).
            // The assembly evaluates `cmp al, 2`. State 2 is KO.
            // Note: The IDA pseudo-code hallucinated 'VE_Escaped' here again.
            if (camperData->GetDamageState() == ECamperDamageState::VE_KO)
            {
                // Broadcast to all clients that this player has been knocked down
                this->Multicast_OnKO();
            }
        }
    }
}

// starts at line 2238
void ACamperPlayer::Multicast_OnKO_Implementation(ACamperPlayer *this)
{
    if (this->IsLocallyControlled())
    {
        this->ItemUseReleased();
    }
}

// starts at line 2265
void ACamperPlayer::Broadcast_SendRunInput_Server_Implementation(bool pressed)
{
    this->Broadcast_SendRunInput_Multicast(pressed);
}

// starts at line 2270
void ACamperPlayer::Broadcast_SendRunInput_Multicast_Implementation(bool pressed)
{
    if (this->IsLocallyControlled(this) == false)
    {
        this->_runInputPressed = pressed;
    }
}

// starts at line 2297
void ACamperPlayer::Broadcast_SendItemPreventRun_Server_Implementation(bool prevent)
{
    this->Broadcast_SendItemPreventRun_Multicast(prevent);
}

// starts at line 2302
void ACamperPlayer::Broadcast_SendItemPreventRun_Multicast_Implementation(bool prevent)
{
    if (this->IsLocallyControlled(this) == false)
    {
        this->_itemPreventRunning = prevent;
    }
}

// starts at line 2310
void ACamperPlayer::Server_IncreaseHealth_Implementation()
{
    this->ApplyHeal();
}

// starts at line 2319
void ACamperPlayer::Server_RemoveHealth_Implementation()
{
    // The assembly evaluates `cmp byte ptr [this+110h], 3`.
    if (this->Role == ROLE_Authority)
    {
        // Safely cast the player data object
        UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

        if (camperData != nullptr)
        {
            // Retrieve the current damage state
            ECamperDamageState currentState = camperData->GetDamageState();
            ECamperDamageState targetState;

            // Calculate the target state based on the current health tier.
            // In the assembly, this is the `test al, al`, `sub ecx, 1`, and `cmp ecx, 1` block.
            if (currentState == ECamperDamageState::VE_Healthy)
            {
                targetState = ECamperDamageState::VE_Injured; // 1 = Injured
            }
            else if (currentState == ECamperDamageState::VE_Injured || currentState == ECamperDamageState::VE_KO) // 1 = Injured, 2 = KO/Dying
            {
                // If they are already injured, or somehow taking damage while already KO'd,
                // the target state clamps to 2 (KO).
                targetState = ECamperDamageState::VE_KO;
            }
            else
            {
                // If they are in any other state (e.g., Dead, Escaped), we cannot remove health.
                return;
            }

            // Prepare local variables to receive out-parameters from the damage application
            bool didDamage = false;
            bool causedKO = false;

            // Apply the new damage state authoritatively
            this->Authority_ApplyDamageToState(didDamage, causedKO, targetState);
        }
    }
}

// starts at line 2329
void ACamperPlayer::Server_SetItemEnergyLevel_Implementation(float level)
{
    // Retrieve the currently equipped item
    ACollectable* currentItem = this->GetItem();

    // Ensure the item exists and is valid.
    // The massive block of assembly translates to this standard engine macro.
    if (IsValid(currentItem) == true)
    {
        // Attempt to fetch the component that implements the Energy Powered Interface.
        // This is a common UE architecture pattern to avoid casting to specific item classes.
        IEnergyPoweredInterface* energyInterface = ActorExt::GetComponentByInterface<IEnergyPoweredInterface>(currentItem);

        if (energyInterface != nullptr)
        {
            // Check if the incoming energy level exceeds the item's current maximum capacity.
            // The assembly performs a virtual call at offset 0x30 (GetMaxEnergyLevel) 
            // and compares the result using `comiss xmm0, xmm6`.
            if (energyInterface->GetMaxEnergyLevel() < level)
            {
                // If the new level is higher, permanently raise the maximum capacity to accommodate it.
                // Assembly virtual call offset: 0x38
                energyInterface->SetMaxEnergyLevel(level);
            }

            // Finally, set the current energy level of the item.
            // Assembly virtual call offset: 0x28
            energyInterface->SetEnergyLevel(level);
        }
    }
}

// starts at line 2350
void ACamperPlayer::Server_SetItemNormalizedEnergyLevel_Implementation(float normalizedLevel)
{
    // Retrieve the currently equipped item
    ACollectable* currentItem = this->GetItem();

    // Ensure the item exists and is valid.
    if (IsValid(currentItem) == true)
    {
        // Fetch the component that implements the Energy Powered Interface.
        IEnergyPoweredInterface* energyInterface = ActorExt::GetComponentByInterface<IEnergyPoweredInterface>(currentItem);

        if (energyInterface != nullptr)
        {
            // Clamp the normalized level between 0.0f and 1.0f.
            // The assembly executes a highly optimized float clamp here which the decompiler missed.
            float clampedLevel = FMath::Clamp(normalizedLevel, 0.0f, 1.0f);

            // Set the clamped normalized energy level (e.g., 0.5 for 50% capacity).
            // Assembly virtual call offset: 0x18
            energyInterface->SetNormalizedEnergyLevel(clampedLevel);
        }
    }
}

// starts at line 2367
void ACamperPlayer::Server_MakeInvulnerable_Implementation()
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Ensure the cast was successful and the pointer is valid
    if (camperData != nullptr)
    {
        // Toggle the damage vulnerability flag off
        camperData->ShouldTakeDamage = false;
    }
}

// starts at line 2384
void ACamperPlayer::SetCameraAttachment(USceneComponent* parent, FName socketName)
{
    // Create a temporary configuration struct to pack the parameters
    FCameraAttachment attachment;

    // Assign the passed parameters to the struct's respective fields
    attachment.Parent = parent;
    attachment.SocketName = socketName;

    // Forward the packed struct to the primary, overloaded version of this method.
    // The pseudo-code shows it passing the memory address (&attachment), which in standard 
    // Unreal Engine C++ usually translates to passing by reference or const-reference.
    this->SetCameraAttachment(attachment);
}

// starts at line 2392
void ACamperPlayer::SetCameraAttachment(const FCameraAttachment& attachment)
{
    // Store the current attachment before overwriting it so we can clean it up
    FCameraAttachment oldAttachment = this->_currentCameraAttachment;

    // Update the internal state to the new attachment configuration
    this->_currentCameraAttachment = attachment;

    // Ensure the player's Camera Anchor component exists and is valid
    if (IsValid(this->CameraAnchor) == true)
    {
        // If the camera was previously attached to a valid parent, detach it first.
        // FDetachmentTransformRules::KeepWorldTransform ensures the camera doesn't visually
        // snap or glitch during the frame it detaches.
        if (IsValid(oldAttachment.Parent) == true)
        {
            this->CameraAnchor->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        }

        // If the new attachment configuration has a valid parent, attach the camera to it
        if (IsValid(this->_currentCameraAttachment.Parent) == true)
        {
            // Attach the camera anchor to the specified socket on the new parent
            this->CameraAnchor->AttachToComponent(
                this->_currentCameraAttachment.Parent,
                FAttachmentTransformRules::KeepRelativeTransform,
                this->_currentCameraAttachment.SocketName
            );

            // Fetch the predefined, intended local offset for the camera anchor
            FVector originalLocation = this->_cameraAnchorOriginalRelativeCameraLocation;

            // Fetch the anchor's current relative rotation. 
            // NOTE: The massive SIMD/SSE float math block in the assembly is simply the 
            // compiler inlining Unreal Engine's FRotator normalization (wrapping 
            // angles between -180 and 180) and caching it into a Quaternion (FQuat).
            FQuat currentRotation = this->CameraAnchor->GetRelativeRotation().Quaternion();

            // Explicitly enforce the original relative location and updated rotation
            this->CameraAnchor->SetRelativeLocationAndRotation(
                originalLocation, 
                currentRotation, 
                false,     // bSweep
                nullptr,   // OutSweepHitResult
                ETeleportType::None
            );
        }
    }
}

// starts at line 2412
void ACamperPlayer::DetachCamera()
{
    // Create a default, zero-initialized camera attachment struct.
    // The assembly uses SSE instructions (movups, movaps) to rapidly zero out a 32-byte 
    // memory block on the stack, which the IDA pseudo-code interprets as a memset.
    // In Unreal Engine, this results in the Parent being nullptr and the SocketName being NAME_None.
    FCameraAttachment emptyAttachment = FCameraAttachment();

    // Pass the empty struct to the primary camera attachment function we just reversed.
    // Because the Parent is nullptr, it will execute the detachment logic but skip the re-attachment logic.
    this->SetCameraAttachment(emptyAttachment);
}

// starts at line 2417
void ACamperPlayer::ResetCameraAttachment()
{
    // Retrieve the default camera attachment configuration stored on the player.
    // Assembly offset: 0x1470 corresponds to _defaultCameraAttachment.
    FCameraAttachment defaultAttachment = this->_defaultCameraAttachment;

    // Pass the default configuration to the primary camera attachment function.
    // This will detach the camera from any current object and reattach it to the 
    // default anchor/socket defined by the character's baseline setup.
    this->SetCameraAttachment(defaultAttachment);
}

// starts at line 2422
bool ACamperPlayer::IsCameraDetached() const
{
    // Check if the current camera attachment has a valid parent component.
    // The massive block of assembly is simply expanding the standard Unreal Engine IsValid() macro.
    if (IsValid(this->_currentCameraAttachment.Parent) == false)
    {
        // If the parent is null or marked for garbage collection, the camera is currently detached.
        // The assembly implements this by inverting the validity result using `setz al` at the very end.
        return true;
    }

    // If a valid parent exists, the camera is actively attached to a socket.
    return false;
}

// starts at line 2427
bool ACamperPlayer::IsCameraAttached() const
{
    // Check if the current camera attachment has a valid parent component.
    // The pseudo-code explicitly expands the GUObjectArray checks, which maps to the standard IsValid().
    if (IsValid(this->_currentCameraAttachment.Parent) == true)
    {
        // If the parent exists and is not pending destruction, the camera is attached
        return true;
    }

    // If the parent is null or destroyed, the camera is not attached
    return false;
}

// starts at line 2432
void ACamperPlayer::TriggerChaseStart()
{
    this->_isBeingChased = 1;
    this->_hideTimer->Reset(this->_timeToHidden);
}

// starts at line 2438
void ACamperPlayer::TriggerChaseEnd()
{
    // Clear the boolean flag indicating the player is actively in a chase.
    // Assembly offset: 0x1578
    this->_isBeingChased = false;

    // Reset the post-chase timer using the predefined duration.
    // Assembly offsets: 0xEFC (Timer) and 0xF1C (Duration)
    this->_postChaseTimer.Reset(this->_postChaseTime);

    // Broadcast the OnChaseEnded delegate to any listening systems (HUD, Audio, Scoring, etc.),
    // passing this specific Camper instance as the context parameter.
    // The assembly explicitly calls the underlying ProcessMulticastDelegate template function,
    // which is the compiled expansion of Unreal Engine's standard .Broadcast() macro.
    this->OnChaseEnded.Broadcast(this);
}

// starts at line 2446
bool ACamperPlayer::CanBeChased() const
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Check if the player is Dead/Sacrificed/Escaped.
        // The assembly evaluates `cmp al, 3`. 
        // Note: IDA hallucinated 'VE_EscapedInjured' here.
        if (camperData->GetDamageState() == ECamperDamageState::VE_Dead)
        {
            return false;
        }
    }

    // A player cannot be chased if they are already in the Dying state (crawling)
    if (this->IsCrawling() == true)
    {
        return false;
    }

    if (camperData != nullptr)
    {
        // Check if the player is currently immobilized (e.g., Hooked).
        // The assembly evaluates `cmp byte ptr [rax+109h], 1`.
        // Note: IDA hallucinated 'VE_Dead' for this enum value.
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
        {
            return false;
        }
    }

    // A player cannot be chased if they are stuck in a Bear Trap
    if (this->IsTrapped() == true)
    {
        return false;
    }

    // A player cannot be chased if they are being carried by the Slasher (Killer)
    if (this->IsGuidedBySlasher() == true)
    {
        return false;
    }

    // If all checks pass, the Survivor is eligible to be chased
    return true;
}

// starts at line 2451
bool ACamperPlayer::IsSuccessfullyHidden()
{
    return this->_isBeingChased && this->_hideTimerIsDone();
}

// starts at line 2456
void ACamperPlayer::ResetHideTimer()
{
    this->_hideTimer->Reset(this->_timeToHidden);
}

// starts at line 2461
float ACamperPlayer::GetPercentDyingTimer()
{
    return this->_dyingTimer->GetPercentTimeLeft();
}

// starts at line 2466
bool ACamperPlayer::IsDyingTimerExpired()
{
    return this->_dyingTimer->IsDone();
}

// starts at line 2471
float ACamperPlayer::GetPercentInjuredBleedoutTimer()
{
    return this->_injuredBleedoutTimer->GetPercentTimeLeft();
}

// starts at line 2476
bool ACamperPlayer::IsInjuredBleedoutTimerExpired()
{
    return this->_injuredBleedoutTimer->IsDone();
}

// starts at line 2481
float ACamperPlayer::GetPercentDrainTimer()
{
    return this->_drainTimer->GetPercentTimeLeft();
}

// starts at line 2486
bool ACamperPlayer::IsDrainTimerExpired()
{
    return this->_drainTimer->IsDone();
}

// starts at line 2491
void ACamperPlayer::TickSoundEvents(float DeltaSeconds)
{
    // Check if the player is moving fast enough to trigger sprint-level events.
    // The assembly compares against 0.89999998f (roughly 0.9f), which is a common 
    // threshold to distinguish sprinting from walking/crouching.
    if (this->GetPercentMovementSpeed() > 0.9f)
    {
        // Decrement the footprint/audio tick timer
        this->_secondsUntilNextFootprint -= DeltaSeconds;

        if (this->_secondsUntilNextFootprint < 0.0f)
        {
            // Reset the timer for the next cycle
            this->_secondsUntilNextFootprint = this->SecondsUntilFootprintTrigger;

            FVector location = FVector::ZeroVector;
            if (IsValid(this->RootComponent) == true)
            {
                // The assembly uses SIMD instructions to directly extract the Translation 
                // vector from the ComponentToWorld matrix. In Unreal Engine C++, this 
                // is the inline expansion of GetComponentLocation().
                location = this->RootComponent->GetComponentLocation();
            }

            // Safely cast to the game's specific GameInstance class
            UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
            
            if (gameInstance != nullptr)
            {
                // Trigger the sound/footprint event in the world.
                // The assembly loads 0x46c80000 into xmm0, which represents 25600.0f in IEEE-754.
                gameInstance->FireSoundEvent(
                    this,       // instigator
                    location,   // location
                    false,      // shouldTrack
                    25600.0f,   // audibleRange
                    nullptr,    // instigatingPlayer
                    false       // isQuickAction
                );
            }
        }

        // --- SCORING LOGIC ---
        // Award bloodpoints if the survivor is sprinting inside the Killer's Terror Radius 
        // while avoiding an active chase (boldness/evasion).
        ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->PlayerState);
        
        if (IsValid(playerState) == true)
        {
            if (this->IsInTerrorRadius() == true && this->_isBeingChased == false)
            {
                // Fire the score event, passing DeltaSeconds to gradually award points.
                // The assembly passes 0x43 (67 in decimal), which Hex-Rays correctly 
                // resolved as DBDCamperScore_SprintingNearSlasherPerSecond.
                playerState->FireScoreEvent(
                    EDBDScoreTypes::DBDCamperScore_SprintingNearSlasherPerSecond,  // scoreType
                    DeltaSeconds,  // percentToAward
                    true           // redirectToGameEventSystem
                );
            }
        }
    }
}

// starts at line 2510
void ACamperPlayer::OnHealthBarCharged(bool complete)
{
    if (complete)
    {
        this->ApplyHeal();
    }
}

// starts at line 2518
void ACamperPlayer::UpdateHealthBars()
{
    // Safely cast the player data object to the specific camper data type
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Retrieve the current damage state (e.g., Healthy, Injured, KO)
        ECamperDamageState currentState = camperData->GetDamageState();

        // Check if the state has changed since the last time this function ran.
        // Assembly offset: 0x1490 corresponds to _previousDamageState.
        if (this->_previousDamageState != currentState)
        {
            // Retrieve the chargeable component handling the current health bar (e.g., Healing progress)
            UChargeableComponent* currentHPSlot = this->GetCurrentHPSlot();

            // Ensure the health slot component exists and is valid
            if (IsValid(currentHPSlot) == true)
            {
                // Reset the charge progress. 
                // Gameplay context: If a survivor is 90% healed and gets hit, the healing progress is wiped.
                currentHPSlot->Reset();
            }

            // Update the cached state so this logic doesn't fire again until the next state change
            if (camperData != nullptr)
            {
                this->_previousDamageState = camperData->GetDamageState();
            }
            else
            {
                // Fallback to the default 0 state (Healthy / Active) if the data is suddenly lost
                this->_previousDamageState = ECamperDamageState::VE_Healthy;
            }
        }
    }
}

// starts at line 2534
void ACamperPlayer::ResetHealthBars()
{
    // Retrieve the first health segment component (typically representing the transition to Injured)
    UChargeableComponent* hpSlot01 = this->_hpSlot01;

    // Ensure the first health slot exists and is valid within the engine
    if (IsValid(hpSlot01) == true)
    {
        // Reset the healing/regression progress for the first slot
        hpSlot01->Reset();
    }

    // Retrieve the second health segment component (typically representing the transition to Dying/KO)
    UChargeableComponent* hpSlot02 = this->_hpSlot02;

    // Ensure the second health slot exists and is valid within the engine
    if (IsValid(hpSlot02) == true)
    {
        // Reset the healing/regression progress for the second slot
        hpSlot02->Reset();
    }
}

// starts at line 2546
float ACamperPlayer::GetCurrentTotalHealthBarPercent() const
{
    // Retrieve the progress percentage of the second health segment (Injured -> Dying).
    // GetPercentComplete returns a value between 0.0f and 1.0f.
    float dyingTierPercent = this->_hpSlot02->GetPercentComplete();

    // Retrieve the progress percentage of the first health segment (Healthy -> Injured).
    float injuredTierPercent = this->_hpSlot01->GetPercentComplete();

    // Return the combined total. 
    // If both bars are full (meaning the player is fully healed/healthy), 
    // this will return 2.0f.
    return dyingTierPercent + injuredTierPercent;
}

// starts at line 2551
UChargeableComponent* ACamperPlayer::GetCurrentHPSlot()
{
    // Safely cast the player data object to access the damage state
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Retrieve the current damage state (0: Healthy, 1: Injured, 2: KO)
        ECamperDamageState damageState = camperData->GetDamageState();

        // State 1: Injured.
        // When injured, the active slot is _hpSlot02 (healing back to Healthy).
        if (damageState == ECamperDamageState::VE_Injured)
        {
            return this->_hpSlot02;
        }

        // State 2: KO / Dying.
        // When on the ground, the active slot is _hpSlot01 (recovering/being healed to Injured).
        if (damageState == ECamperDamageState::VE_KO)
        {
            return this->_hpSlot01;
        }
    }

    // If Healthy (0) or in an invalid state, no health bar slot is currently active.
    return nullptr;
}

// starts at line 2563
FRotator ACamperPlayer::GetToAimPointRotation() const
{
    // Retrieve the base transform of the player (or use Identity if none exists)
    FTransform rootTransform = FTransform::Identity;
    if (IsValid(this->RootComponent) == true)
    {
        rootTransform = this->RootComponent->GetComponentTransform();
    }

    // Retrieve the current aim point in local space, and then transform it into World Space.
    // The massive SIMD block in the pseudo-code (v9 through v14) is just the compiler 
    // manually inlining rootTransform.TransformPosition(AimPoint).
    FVector localAimPoint = this->GetAimPoint();
    FVector worldAimPoint = rootTransform.TransformPosition(localAimPoint);

    USkeletalMeshComponent* mesh = this->GetPawnMesh();

    // Get the exact world-space position of the player's right shoulder socket.
    // The assembly offsets show the transform being stored in `var_70`, and then 
    // `var_60` (the translation vector part of the transform) is subtracted from the AimPoint.
    FTransform shoulderTransform = mesh->GetSocketTransform(ACamperPlayer::RIGHT_SHOULDER, RTS_World);
    FVector shoulderLocation = shoulderTransform.GetLocation();

    // Calculate the directional vector from the shoulder to the calculated world aim point
    FVector directionToTarget = worldAimPoint - shoulderLocation;

    // Get the player's current world rotation. 
    // The second block of SIMD math in the assembly is the engine's built-in 
    // FRotator caching logic (checking if the Quat changed, and updating the Rotator if so).
    FRotator currentRotation = FRotator::ZeroRotator;
    if (IsValid(this->RootComponent) == true)
    {
        currentRotation = this->RootComponent->GetComponentRotation();
    }

    // Convert the directional vector into a target rotation (Pitch, Yaw, Roll),
    // and then subtract the player's current rotation to get the relative offset 
    // required to aim at the target.
    FRotator targetRotation = directionToTarget.ToOrientationRotator();
    FRotator relativeAimRotation = targetRotation - currentRotation;

    return relativeAimRotation;
}

// starts at line 2572
FVector ACamperPlayer::GetAimPoint() const
{
    // Retrieve the currently equipped item and attempt to find an Aimable interface.
    ACollectable* item = this->GetItem();
    IAimableInterface* aimableComponent = ActorExt::GetComponentByInterface<IAimableInterface>(item);

    // If the item isn't aimable or currently shouldn't be aimed, return the last known good location.
    if (aimableComponent == nullptr || aimableComponent->GetAimingStrategy() == nullptr)
    {
        return this->_lastValidAimingPointLocalSpace;
    }

    // Get the origin (the camera's physical location) and the direction the camera is looking.
    FVector cameraLocation = this->GetCameraAnchorLocation();
    FRotator cameraRotation = this->GetRepControlRotation();

    // Adjust the rotation slightly based on the camera component's relative roll/pitch/yaw.
    if (IsValid(this->Camera) == true)
    {
        cameraLocation.Z = this->Camera->GetComponentTransform().GetLocation().Z;
        cameraRotation = this->Camera->GetComponentRotation();
    }

    // Ask the item where the beam/aim starts (e.g., the tip of the flashlight).
    FVector aimingTip;
    FVector aimingDirection;
    aimableComponent->GetAimingTip(aimingTip, aimingDirection);

    // Pack the aiming parameters to pass to the raycast/collision query.
    FAimingInfo aimingInfo;
    aimingInfo.Origin = cameraLocation;
    aimingInfo.Orientation = cameraRotation;
    aimingInfo.MaxDistance = aimableComponent->GetMaxAimingDistance();

    // Prepare collision parameters.
    // The player themselves must be excluded so the raycast doesn't immediately hit their own back.
    FOcclusionInfo occlusionInfo;
    occlusionInfo.Excluded.Add(const_cast<AActor*>(this)); 

    // Compute the actual impact point in the world (where the raycast hits level geometry).
    FVector worldAimPoint;
    aimableComponent->ComputeAimPoint(worldAimPoint, aimingInfo, occlusionInfo);

    // Calculate the distance squared between the tip of the item and the impact point.
    // The assembly calculates: (X*X) + (Y*Y) + (Z*Z).
    float distanceSq = FVector::DistSquared(worldAimPoint, aimingTip);

    // If the hit point is valid (distance squared >= 0.0), update the local space cache.
    // (Note: DistSquared is inherently >= 0, this is likely an engine validity/infinity check).
    if (distanceSq >= 0.0f) 
    {
        FTransform rootTransform = FTransform::Identity;
        if (IsValid(this->RootComponent) == true)
        {
            rootTransform = this->RootComponent->GetComponentTransform();
        }

        // Convert the world-space impact point into a location relative to the player's root.
        // The pseudo-code's massive SIMD block using QINV_SIGN_MASK and RCP (Reciprocal) 
        // is the compiler manually inlining an InverseTransform calculation!
        FVector localAimPoint = rootTransform.InverseTransformPosition(worldAimPoint);

        // Update the cached value (mutable operation).
        // A const_cast is required because the function signature is marked const, 
        // but it intentionally updates a cache variable.
        const_cast<ACamperPlayer*>(this)->_lastValidAimingPointLocalSpace = localAimPoint;
    }

    // Free the dynamically allocated array memory for the excluded actors.
    // TArray handles this automatically in standard C++, so manual FMemory::Free is omitted.

    return this->_lastValidAimingPointLocalSpace;
}

// starts at line 2633
void ACamperPlayer::Server_SendInteractMashedInput_Implementation(bool pressed)
{
    this->_playerData->_interactionInputMashed = pressed;
}

// starts at line 2643
void ACamperPlayer::Server_SendLeftRightMashedInput_Implementation(bool pressed)
{
    this->_playerData->_leftRightInputMashed = pressed;
}

// starts at line 2665
void ACamperPlayer::MashInputChanged(bool isMashing)
{
    // If the player has started mashing, attempt to trigger the corresponding interaction.
    // NOTE: IDA hallucinated 'VE_ManuallyLeftMatch' here. In the context of the 
    // struggle/mash mechanic, this is likely EInputInteractionType::VE_Struggle 
    // or a similar value used for rapid-input events.
    if (isMashing == true)
    {
        this->TryInteractionType(EInputInteractionType::VE_InteractMash, this);
    }

    // Retrieve the player's data component
    UDBDPlayerData* playerData = this->_playerData;

    if (playerData != nullptr)
    {
        // Determine the current effective mash state. 
        // Auto-mash is often an accessibility feature or a cheat-prevention fallback.
        bool currentEffectiveMash = playerData->_interactionInputMashed || playerData->_autoInteractionMash;

        // Only update and replicate if the physical input state has actually changed 
        // relative to the stored state.
        if (currentEffectiveMash != isMashing)
        {
            // Update the local state
            playerData->_interactionInputMashed = isMashing;

            // Replicate the mashing state to the server for validation
            this->Server_SendInteractMashedInput(isMashing);
        }
    }
}

// starts at line 2682
void ACamperPlayer::LeftRightMashedInputChanged(bool isMashing)
{
    // If the player has started the alternating "Left-Right" mash (Wiggling),
    // attempt to trigger the corresponding interaction type.
    // Integer 8 maps to EInputInteractionType::VE_Wiggle.
    if (isMashing == true)
    {
        this->TryInteractionType(EInputInteractionType::VE_LeftRightMash, this);
    }

    // Retrieve the player's data component
    UDBDPlayerData* playerData = this->_playerData;

    if (playerData != nullptr)
    {
        // Check the current effective mash state, accounting for accessibility/auto-wiggle features.
        bool currentEffectiveMash = playerData->_leftRightInputMashed || playerData->_autoLeftRightMash;

        // Only replicate to the server if the input state has actually changed.
        if (currentEffectiveMash != isMashing)
        {
            // Update the local state in the player data
            playerData->_leftRightInputMashed = isMashing;

            // Replicate the wiggle mash state to the server
            this->Server_SendLeftRightMashedInput(isMashing);
        }
    }
}

// starts at line 2699
void ACamperPlayer::UpdateDyingTimer(float deltaTime)
{
    // Retrieve the player's data component.
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    // Check if the player is in the "Dying" state.
    // NOTE: IDA hallucinated 'VE_EscapedInjured' for the enum value.
    // Based on the context of this timer, the check is verifying that the 
    // player is NOT in a free/healthy state (or specifically IS in state 2: Dying).
    if (camperData == nullptr || camperData->ImmobilizeState != ECamperImmobilizeState::VE_Hiding)
    {
        // Advance the dying (bleed-out) timer by the frame's delta time.
        this->_dyingTimer.Update(deltaTime);
    }
}

// starts at line 2723
void ACamperPlayer::Authority_OnFailedStruggling(UInteractionDefinition* interaction)
{
    if (this->Role == ENetRole::ROLE_Authority)
    {
        this->Multicast_OnFailedStruggling(interaction);
    }
}

// starts at line 2731
void ACamperPlayer::Multicast_OnFailedStruggling_Implementation(const FGuid& interaction)
{
    // Execute the delegate responsible for struggle failure.
    // In Unreal Engine, ExecuteIfBound safely checks if any function is currently 
    // listening to this event before attempting to call it.
    // The FGuid 'interaction' identifies which specific interaction instance failed.
    this->FailedStrugglingDelegate.ExecuteIfBound(interaction);
}

// starts at line 2741
void ACamperPlayer::UpdateDrainTimer(float deltaTime)
{
    this->_drainTimer->Update(deltaTime);
}

// starts at line 2753
void ACamperPlayer::Authority_SetDrainTimerPercentElapsed(float percentTime)
{
    if (this->Role == ENetRole::ROLE_Authority)
    {
        this->_drainTimer->SetPercentTimeElapsed(percentTime);
    }
}

// starts at line 2761
void ACamperPlayer::Authority_SetDrainTimerPercentLeft(float percentTime)
{
    if (this->Role == ENetRole::ROLE_Authority)
    {
        this->_drainTimer->SetPercentTimeLeft(percentTime);
    }
}

// starts at line 2769
bool ACamperPlayer::CheckTransitionGuidedAction() const
{
    // Retrieve the camper data component
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Access the current guided action state.
        // Assembly offset: [rax + 10Ah]
        ECamperGuidedAction guidedAction = camperData->CurrentGuidedAction;

        // Check if the current guided action is one of the "blocking" states.
        // In these states, a transition to a new guided action is typically disallowed.
        
        if (guidedAction == ECamperGuidedAction::VE_None || guidedAction == ECamperGuidedAction::VE_PrepBeingKilled ||
            guidedAction == ECamperGuidedAction::VE_BeingPickedUp || guidedAction == ECamperGuidedAction::VE_BeingCarried)
        {
            return false;
        }

        // If the current guided action is not a blocking state (e.g., being carried or a mid-air vault),
        // the transition is permitted.
        return true;
    }

    // If data is missing, we cannot validate a transition.
    return false;
}

// starts at line 2777
bool ACamperPlayer::CanInteractWithItems() const
{
    // Consolidate the redundant SafeCasts into a single pointer
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Check if the player is Dead/Sacrificed/Escaped.
        // The assembly evaluates `cmp al, 3`.
        if (camperData->GetDamageState() == ECamperDamageState::VE_Dead)
        {
            return false;
        }

        // Check if the player is currently immobilized (e.g., Hooked/Trapped).
        // The assembly evaluates `cmp byte ptr [rax+109h], 0`. If it's not 0 (Active), they can't interact.
        if (camperData->ImmobilizeState != ECamperImmobilizeState::VE_None)
        {
            return false;
        }
    }

    // A player cannot interact with items if they are already locked into another interaction
    // Assembly offset: 0x0C04 corresponds to the IsInteracting flag.
    if (this->IsInteracting || this->IsCrawling() || this->IsBeingCarried() || this->IsBeingPickedUp() || this->IsBeingPutDown())
    {
        return false;
    }

    // If all state restrictions are cleared, the Survivor is permitted to use/interact with items
    return true;
}

// starts at line 2788
bool ACamperPlayer::CanInteractWith(const UInteractionDefinition* interaction)
{
    // First, check the base class implementation. If the core ADBDPlayer logic 
    // says this interaction is invalid, we immediately fail.
    if (Super::CanInteractWith(interaction) == false)
    {
        return false;
    }

    // Safely cast the generic PlayerState to the DBD-specific PlayerState
    ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->PlayerState);

    // Check if the player state exists and is valid
    if (playerState != nullptr)
    {
        // A Survivor who has successfully escaped the trial transitions into a spectator state.
        // Even if their physical pawn hasn't been completely destroyed yet, they are strictly
        // prohibited from triggering any further world interactions (like opening chests or vaulting).
        if (playerState->IsEscaped() == true)
        {
            return false;
        }
    }

    // If the base class allows it and the player hasn't escaped, the interaction is permitted.
    return true;
}

// starts at line 2793
bool ACamperPlayer::CanBeSlashed() const
{
    // Safely retrieve the player's data component
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Check if the Survivor is already in a terminal guided action (8 = VE_BeingCarried)
        if (camperData->CurrentGuidedAction == ECamperGuidedAction::VE_BeingCarried)
        {
            return false;
        }

        // Check if the Survivor's damage state is terminal (3 = Dead/Escaped)
        // NOTE: IDA hallucinated 'VE_EscapedInjured' here.
        if (camperData->GetDamageState() == ECamperDamageState::VE_Dead)
        {
            return false;
        }
    }

    // A Survivor who has already been sacrificed cannot be slashed
    if (this->_sacrificed == true)
    {
        return false;
    }

    // A Survivor in the Dying state (crawling) cannot be slashed, 
    // UNLESS they are somehow also registered as Hooked. 
    // This is likely an engine edge-case safety check.
    if (this->IsCrawling() == true && this->IsHooked() == false)
    {
        return false;
    }

    // A Survivor with active damage immunity (e.g., the Endurance status effect) cannot be slashed
    if (this->HasDamageImmunity() == true)
    {
        return false;
    }

    // If all checks pass, the Survivor is vulnerable to an attack
    return true;
}

// starts at line 2798
UAnimationMontageSlave* ACamperPlayer::GetMontageFollower() const
{
    // Retrieve the component responsible for syncing animation montages.
    // The pseudo-code shows the compiler unrolling the standard Unreal Engine 
    // template method into a raw static class lookup and function pointer call.
    return this->FindComponentByClass<UAnimationMontageSlave>();
}

// starts at line 2803
float ACamperPlayer::CheckHeightDelta(ADBDPlayer* otherPlayer)
{
    // Retrieve the base position (feet) of this survivor
    FVector myFeet = this->GetFeetPosition();

    // Retrieve the base position of the target player
    FVector otherFeet = otherPlayer->GetFeetPosition();

    // Calculate the absolute difference in height (Z-axis).
    // The assembly uses a hardware-level bitmask to strip the sign bit for a highly optimized absolute value.
    float deltaZ = FMath::Abs(myFeet.Z - otherFeet.Z);

    // Check if the elevation difference is less than 150.0 Unreal Units (cm)
    if (deltaZ < 150.0f)
    {
        // The characters are on relatively the same vertical level.
        // The assembly explicitly converts the boolean true (1) to a float (1.0f).
        return 1.0f;
    }

    // The characters are on different floors/elevations.
    // Converts boolean false (0) to float (0.0f).
    return 0.0f;
}

// starts at line 2808
void ACamperPlayer::OnProximityEntered(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // The assembly signature matches standard UE primitive overlap delegates.
    // Ensure the other actor is valid, is a CamperPlayer, and is not this same player.
    ACamperPlayer* otherCamper = Cast<ACamperPlayer>(OtherActor);

    if (IsValid(otherCamper) == true && otherCamper->IsValidImpl() && otherCamper != this)
    {
        // Check if the other player is already in our list of potential saviors.
        // The pseudo-code's massive `while (*v14 != OtherActor)` loop and `ResizeGrow` 
        // calls are the compiler expanding `TArray::AddUnique()`.
        if (this->_potentialSaviors.Contains(otherCamper) == false)
        {
            // Check if the players are on roughly the same floor/elevation
            if (this->CheckHeightDelta(otherCamper) != 0.0f)
            {
                // A player is only considered a "savior" if they are approaching someone 
                // who isn't already being helped or carried by the killer.
                if (otherCamper->IsInNeed() == false && otherCamper->IsBeingCarried() == false)
                {
                    // Add them to the list of nearby allies who could potentially heal/rescue
                    this->_potentialSaviors.Add(otherCamper);
                }
            }
        }

        // --- SCORING LOGIC ---
        // If this player is a Camper (Role == 3) and is currently in need (dying/hooked), 
        // and a valid ally just entered proximity, award score events to the ally.
        // Note: The assembly checks `[r14+110h], 3` which maps to Role.Value == 3.
        if (this->Role == ROLE_Authority && this->IsInNeed() == true)
        {
            ADBDPlayerState* allyPlayerState = Cast<ADBDPlayerState>(otherCamper->PlayerState);
            
            if (IsValid(allyPlayerState) == true)
            {
                ADBDGameState* gameState = Cast<ADBDGameState>(this->GetWorld()->GetGameState());
                
                EDBDScoreTypes scoreTypeToAward = EDBDScoreTypes::DBDCamperScore_NearFriendInNeed; // Default: NearFriendInNeed

                if (IsValid(gameState) == true)
                {
                    // If the exit gates are powered, award a higher tier of points for late-game altruism
                    if (gameState->IsEscapeDoorActivated() == true)
                    {
                        // The assembly loads 0xB2 (178) into dl
                        scoreTypeToAward = EDBDScoreTypes::DBDCamperScore_NearFriendInNeed_GeneratorsComplete;
                    }
                }

                // Fire the score event for entering proximity (award 100% of the base value)
                allyPlayerState->FireScoreEvent(
                    scoreTypeToAward, 
                    1.0f, // percentToAward
                    true  // redirectToGameEventSystem
                );

                // If the exit gates are physically open, award an additional bonus score event
                if (IsValid(gameState) == true)
                {
                    if (gameState->IsEscapeDoorOpen() == true)
                    {
                        // The assembly loads 0x0B (11) into dl
                        allyPlayerState->FireScoreEvent(
                            EDBDScoreTypes::DBDCamperScore_NearFriendInNeed_PostExit,
                            1.0f, 
                            true
                        );
                    }
                }
            }
        }
    }
}

// starts at line 2844
void ACamperPlayer::OnPawnSensed(APawn* pawn)
{
    if (pawn == nullptr)
    {
        return;
    }

    // Try to cast the sensed pawn to a Survivor (Camper)
    ACamperPlayer* sensedCamper = Cast<ACamperPlayer>(pawn);
    
    if (sensedCamper != nullptr && sensedCamper != this)
    {
        // Check if we haven't already spotted this specific ally recently.
        // The massive loop in the pseudo-code is just the expansion of TArray::Contains()
        if (this->_foundCampers.Contains(sensedCamper) == false)
        {
            // Check if they are on roughly the same vertical level. 
            // This prevents getting "Reunion" points for a survivor on the floor above you.
            if (this->CheckHeightDelta(sensedCamper) != 0.0f) 
            {
                // Add them to the list so we don't repeatedly trigger the event
                this->_foundCampers.Add(sensedCamper);

                // Broadcast the sensing event. This is likely caught by the game's scoring 
                // manager or specific perks (like Prove Thyself or Leader).
                sensedCamper->OnSensed.Broadcast(this);

                // --- SCORING LOGIC ---
                // If neither player is dead or incapacitated (dying/carried/hooked), 
                // award the "Found Camper" bloodpoints.
                if (sensedCamper->IsDead() == false && sensedCamper->IsIncapacitated() == false &&
                    this->IsDead() == false && this->IsIncapacitated() == false)
                {
                    // The assembly loads 0x0C (12), which Hex-Rays resolved to DBDCamperScore_FoundCamper
                    this->TryFireScoreEvent(
                        EDBDScoreTypes::DBDCamperScore_FoundCamper,  // scoreType
                        1.0f,                            // percentToAward
                        true                             // redirectToGameEventSystem
                    );
                }
            }
        }
        
        // If the pawn was a Camper, exit early.
        return; 
    }

    // If the sensed pawn was NOT a Survivor, check if it is the Killer (Slasher)
    ASlasherPlayer* sensedSlasher = Cast<ASlasherPlayer>(pawn);
    
    if (sensedSlasher != nullptr)
    {
        // Standard Unreal Engine validity/pending kill check
        if (IsValid(sensedSlasher) == true) 
        {
            // Notify the Killer that they were looked at by this Survivor.
            // In gameplay terms, this broadcast is what powers perks like 
            // 'Premonition', 'Spine Chill' (legacy), or 'Object of Obsession'.
            sensedSlasher->OnSensed.Broadcast(this);
        }
    }
}

// starts at line 2866
float ACamperPlayer::GetStillness()
{
    return this->_stillnessTracker->GetStillness();
}

// starts at line 2871
void ACamperPlayer::IncreaseHPSlotSize(float percent)
{
    this->_hpSlot01->IncreaseMaxCharge(percent);
    this->_hpSlot02->IncreaseMaxCharge(percent);
}

// starts at line 2877
void ACamperPlayer::UpdateRTPCValues()
{
    // Always call the base class implementation first so core player audio updates
    Super::UpdateRTPCValues();

    // Check if the Survivor is currently considered "idle" (ticks > 0)
    // Assembly offset: 0x0C18 corresponds to _idleStart.Ticks
    if (this->_idleStart.GetTicks() != 0)
    {
        // Create a 1-second timespan. 
        // The FTimespan::Assign arguments are (Days, Hours, Minutes, Seconds, Milliseconds, Microseconds)
        FTimespan oneSecond = FTimespan(0, 0, 0, 1, 0, 0);

        // We only want to spam the audio engine with updates once per second, not every tick.
        // Check if current time >= last update time + 1 second.
        if (FDateTime::UtcNow().GetTicks() >= (this->_lastUpdate.GetTicks() + oneSecond.GetTicks()))
        {
            // Update the last update timestamp to right now
            this->_lastUpdate = FDateTime::UtcNow();

            // Calculate how long the player has been idle.
            // Ticks are in 100-nanosecond intervals. Multiplying by 0.0000001 (1e-7) converts ticks to Seconds.
            double idleDurationSeconds = static_cast<double>(FDateTime::UtcNow().GetTicks() - this->_idleStart.GetTicks()) * 0.0000001;

            // Send this duration to the Wwise audio engine
            FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
            
            if (audioDevice != nullptr)
            {
                // Set the RTPC value. The Audio team uses "AudioRTPC_Survivor_Time_Stop_Moving" to blend sounds.
                AKRESULT result = audioDevice->SetRTPCValue(TEXT("AudioRTPC_Survivor_Time_Stop_Moving"), static_cast<float>(idleDurationSeconds), 0, nullptr);

                // If the Wwise call failed, log an error (only if logging verbosity is high enough)
                if (result != AKRESULT::AK_Success)
                {
                    UE_LOG(LogDBDGeneral, Warning, TEXT("ACamperPlayer::UpdateRTPCValues: SetRTPCValue on \"AudioRTPC_Survivor_Time_Stop_Moving\" failed with error code %d"), result);
                }
            }
        }
    }

    // --- PERK AUDIO MODIFIERS ---
    
    // Retrieve the Perk Manager to check if any equipped perks affect standard audio volume.
    UPerkManager* perkManager = this->GetPerkManager();
    
    if (IsValid(perkManager) == true)
    {
        // Get the modifier for generic audio volume.
        // NOTE: Hex-Rays hallucinated `VE_ShowIllusionaryPallets | VE_AutomaticBlinkAttack` 
        // because the hex value is 0x6F (111). This maps to a specific Audio Volume modifier enum.
        float perkVolumeModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyNormalVoiceLevel, EGameplayModifierSource::VE_All);

        // Send the perk's volume modifier to Wwise
        FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
        if (audioDevice != nullptr)
        {
            audioDevice->SetRTPCValue(TEXT("AudioRTPC_Perk_Volume_Normal_State"), perkVolumeModifier, 0, nullptr);
        }
    }
}

// starts at line 2900
bool ACamperPlayer::CanEscapeCarry()
{
    return this->_carryEscapeProgress->GetComplete();
}

// starts at line 2905
void ACamperPlayer::Authority_OnSlashed(ADBDPlayer* attacker)
{
    // Check if this player is actually a Camper/Survivor (Role Value == 3)
    if (this->Role == ENetRole::ROLE_Authority)
    {
        UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

        // --- TRAP ESCAPE LOGIC ---
        if (camperData != nullptr)
        {
            // Check if the Survivor is currently Immobilized in state 2 (Trapped).
            // NOTE: IDA hallucinated 'VE_Escaped' here. Value 2 corresponds to Bear Traps.
            if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Trapped)
            {
                if (IsValid(attacker) == true)
                {
                    // If hit while in a trap, the Survivor is forcibly freed from the trap 
                    // (usually transitioning directly to the Dying/Carried state).
                    this->Client_OnSlashedOutOfTrap(attacker);
                }
            }
        }

        // --- AUTHORITATIVE MOVEMENT (SPEED BOOST) LOGIC ---
        bool enableMovementAuth = false;

        // Check if the Survivor is now in Damage State 1 (Injured).
        // NOTE: IDA hallucinated 'VE_Dead' here.
        if (camperData != nullptr && camperData->GetDamageState() == ECamperDamageState::VE_Injured)
        {
            // Retrieve the tunable duration for the post-hit speed boost (default is 5.0 seconds).
            // The assembly shows 0x40a00000, which is exactly 5.0f.
            float pushTime = this->GetTunableValue(TunableValues::CAMPER_SLASHER_PUSH_TIME, 5.0f, false);
            
            // Start the timer for how long the client has movement authority
            this->_slashedAuthoritativeMovementTimer.Reset(pushTime);
            enableMovementAuth = true;
        }
        else
        {
            // If they went down (Dying) or are otherwise not just "Injured", stop the timer
            UDBDUtilTimer::Stop(this->_slashedAuthoritativeMovementTimer);
            enableMovementAuth = false;
        }

        // Broadcast to all clients whether this Survivor currently has authoritative movement.
        // Enum Value 1 corresponds to EAuthoritativeMovementFlag::SLASHED.
        this->Multicast_SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag::SLASHED, enableMovementAuth);
    }

    // Call the base class implementation to handle standard damage application, blood effects, etc.
    // The assembly uses a `jmp` instruction at the very end to achieve this tail-call optimization.
    ADBDPlayer::Authority_OnSlashed(attacker);
}

// starts at line 2931
void ACamperPlayer::Authority_ActivateAuthoritativeMovementOnDropped()
{
    // Ensure the player calling this is actually a Survivor
    if (this->Role == ENetRole::ROLE_Authority)
    {
        // Retrieve the tunable duration for authoritative movement after being dropped.
        // The assembly loads cs:__real@40400000, which is exactly 3.0f.
        float dropPushTime = this->GetTunableValue(TunableValues::CAMPER_SLASHER_PUSH_TIME_ON_DROP, 3.0f, false);
        
        // Start the timer for how long the client has movement authority
        this->_droppedAuthoritativeMovementTimer.Reset(dropPushTime);

        // Broadcast to all clients that this player now has authoritative movement.
        // The assembly passes integer 3 (edx = 3), which maps to EAuthoritativeMovementFlag::DROPPED.
        this->Multicast_SetAuthoritativeMovementFlag(EAuthoritativeMovementFlag::DROPPED, true);
    }
}

// starts at line 2941
void ACamperPlayer::Client_OnSlashedOutOfTrap_Implementation(ADBDPlayer* requester)
{
    // Force the client to attempt the specific interaction for being slashed out of a trap.
    // The assembly passes 0x13 (19 in decimal) as the interaction input type.
    this->TryInteractionType(EInputInteractionType::VE_ExternalRequestSlashedOutOfTrap, requester);
}

// starts at line 2951
void ACamperPlayer::Multicast_SetAuthoritativeMovementFlag_Implementation(EAuthoritativeMovementFlag flag, bool value)
{
    this->SetAuthoritativeMovementFlag(flag, value);
}

// starts at line 2961
bool ACamperPlayer::IsAuthoritativeMoveable() const
{
    // Retrieve the Camper's data component
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // A Survivor cannot drive their own movement if they are immobilized (e.g., Hooked/Carried).
        // The assembly evaluates `cmp byte ptr [rax+109h], 1`.
        // NOTE: IDA hallucinated 'VE_Dead' here. Value 1 typically represents Hooked/Carried.
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
        {
            return false;
        }

        // A Survivor cannot drive their own movement if they are in a terminal state.
        // The assembly evaluates `cmp al, 3`.
        // NOTE: IDA hallucinated 'VE_EscapedInjured'. Value 3 represents Dead/Escaped.
        if (camperData->GetDamageState() == ECamperDamageState::VE_Dead)
        {
            return false;
        }
    }

    // A Survivor in the Dying state (crawling) does not have movement authority.
    // Their slow crawling speed and position must be strictly server-validated.
    if (this->IsCrawling() == true)
    {
        return false;
    }

    // A Survivor caught in a Bear Trap does not have movement authority.
    if (this->IsTrapped() == true)
    {
        return false;
    }

    // Finally, query the base ADBDPlayer class.
    // This handles core checks like "Are we currently locked in an interaction?"
    // or "Did the server explicitly grant/revoke authority?"
    if (Super::IsAuthoritativeMoveable() == false)
    {
        return false;
    }

    // If all state checks pass and the base class allows it, the client has movement authority.
    return true;
}

// starts at line 2967
void ACamperPlayer::Landed(const FHitResult& Hit)
{
    // Always call the base Unreal Engine ACharacter implementation first 
    // to handle core physics state changes (like switching from 'Falling' to 'Walking').
    Super::Landed(Hit);

    // Ensure the stagger duration curve exists and is valid
    if (IsValid(this->DropStaggerDuration) == true)
    {
        // Calculate the total distance fallen. 
        // _fallFromHeight is recorded right before the fall begins.
        // GetFeetPosition().Z gets the current elevation upon landing.
        float fallDistance = this->_fallFromHeight - this->GetFeetPosition().Z;

        // Look up the base stagger duration from the curve asset based on the distance fallen.
        float staggerDuration = this->DropStaggerDuration->GetFloatValue(fallDistance);

        // If the fall was far enough to trigger a stagger (> 0.0001 seconds)
        // The assembly evaluates against cs:__real@38d1b717, which is a tiny epsilon value (0.0000999...).
        if (staggerDuration > 0.0001) 
        {
            // Check for perks that modify stagger duration (e.g., Balanced Landing).
            UPerkManager* perkManager = this->GetPerkManager();
            
            // Hex-Rays hallucinated `VE_OutlineSharedHallucination|VE_IntensifyBloodColourMedium` 
            // because the hex value is 0x60 (96). This maps to a specific Modifier enum.
            float staggerModifier = 1.0f;
            if (IsValid(perkManager) == true)
            {
                staggerModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyDropStaggerDuration);
            }

            // If the perk cooldown is finished AND we actually have a modifier (not exactly 1.0)
            if (this->_dropStaggerReductionCooldownTimer.IsDone() == true && staggerModifier != 1.0f)
            {
                // Apply the perk's reduction (e.g., multiply stagger duration by 0.0 or 0.25)
                staggerDuration = staggerDuration * staggerModifier;

                // Reset the perk's cooldown timer so it cannot trigger again immediately.
                // It fetches CAMPER_DROP_STAGGER_COOLDOWN from the tunable data tables.
                float cooldownTime = this->GetNonPlayerSpecificTunableValue(TunableValues::CAMPER_DROP_STAGGER_COOLDOWN);
                this->_dropStaggerReductionCooldownTimer.Reset(cooldownTime);
            }

            // After applying modifiers, check if the stagger was completely nullified
            if (staggerDuration <= 0.0001)
            {
                // Fire the game event for successfully triggering the stagger negation.
                // The assembly loads 0x91 (145) into dl, which Hex-Rays mapped to DBDPlayerScore_BalancedLanding.
                UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetWorld()->GetGameInstance());
                if (IsValid(gameInstance) == true)
                {
                    gameInstance->GameEventTracker->FireGameEvent(
                        EDBDScoreTypes::DBDPlayerScore_BalancedLanding,  // eventType
                        1.0f,    // amount
                        this,    // instigator
                        nullptr  // target
                    );
                }
            }
            else
            {
                // If there is still a stagger duration remaining, start the stagger timer.
                // While this timer is active, the Survivor's movement speed will be drastically reduced.
                this->_dropStaggerTimer.Reset(staggerDuration);
            }
        }
    }

    // Reset the fall height cache regardless of what happened
    // Assembly: mov [rbx+141Ch], edi (where edi was zeroed out earlier via `xor edi, edi`)
    this->_fallFromHeight = 0.0f;
}

// starts at line 3007
void ACamperPlayer::UpdateDropStagger(float DeltaSeconds)
{
    // Check if the player is currently actively staggered from a fall
    if (this->_dropStaggerTimer.IsDone() == false)
    {
        // Advance the stagger timer by the frame's delta time
        this->_dropStaggerTimer.Update(DeltaSeconds);

        // Check if the timer just reached zero during this exact frame
        if (this->_dropStaggerTimer.IsDone() == true)
        {
            // Retrieve the custom Game Instance via the utility library.
            // The pseudo-code passed 0 (nullptr), but in Unreal Engine, it's always 
            // safer to pass the current actor context (`this`) to resolve the correct world.
            UDBDGameInstance* gameInstance = UDBDUtilities::GetDBDGameInstance(this);
            
            if (IsValid(gameInstance) == true && IsValid(gameInstance->GameEventTracker) == true)
            {
                // Fire the event indicating the player has fully recovered from the fall.
                // NOTE: Hex-Rays mapped this to DBDPlayerScore_BalancedLanding again, 
                // but in this context, it is likely serving as a generic "Stagger Ended" event broadcast.
                gameInstance->GameEventTracker->FireGameEvent(
                    EDBDScoreTypes::DBDPlayerScore_BalancedLanding,  // eventType
                    1.0f,    // amount
                    this,    // instigator
                    nullptr  // target
                );
            }
        }
    }

    // Check if the player is currently on cooldown from a stagger-reduction perk (like Balanced Landing)
    if (this->_dropStaggerReductionCooldownTimer.IsDone() == false)
    {
        // Advance the cooldown timer. 
        // Once this finishes, the player will be eligible to reduce stagger on their next big fall.
        this->_dropStaggerReductionCooldownTimer.Update(DeltaSeconds);
    }
}

// starts at line 3031
float ACamperPlayer::GetPercentDropStaggerCooldownTimer() const
{
    return this->_dropStaggerReductionCooldownTimer->GetPercentTimeLeft();
}

// starts at line 3036
float ACamperPlayer::GetDropStaggerTimeLeft() const
{
    return UDBDUtilTimer::GetTimeLeft(this->_dropStaggerTimer);
}

// starts at line 3041
float ACamperPlayer::GetDropStaggerPercentTimeElapsed() const
{
    return this->_dropStaggerTimer->GetPercentTimeElapsed();
}

// starts at line 3046
void ACamperPlayer::SetFallFromHeight()
{
    // Retrieve the exact position of the player's feet in world space at the start of the fall
    FVector currentFeetPosition = this->GetFeetPosition();

    // Store the Z (vertical) axis coordinate. 
    // This is the exact value that the `Landed` function will later use to 
    // calculate the total distance fallen (Starting Z - Landing Z).
    this->_fallFromHeight = currentFeetPosition.Z;
}

// starts at line 3056
UDBDClipRegionComponent* ACamperPlayer::GetEntityRenderRegionAtIndex(int32 InIndex) const
{
    // Check if the provided index is within the valid bounds of the array.
    // The assembly manually checks if `InIndex < 0` or `InIndex >= ArrayNum`.
    // In standard Unreal Engine C++, this is perfectly encapsulated by IsValidIndex().
    if (this->_clipRegionsToUseDuringHookingAndSacrifice.IsValidIndex(InIndex) == true)
    {
        // The array stores weak pointers (TWeakObjectPtr) to the components.
        // Weak pointers do not prevent the Garbage Collector from destroying the object.
        // We must call .Get() to resolve the weak reference back into a raw C++ pointer.
        return this->_clipRegionsToUseDuringHookingAndSacrifice[InIndex].Get();
    }

    // Return null if the index is out of bounds.
    return nullptr;
}

// starts at line 3065
TArray<UDBDClipRegionComponent*> ACamperPlayer::GetAllEntityRenderRegions() const
{
    // Initialize the return array
    TArray<UDBDClipRegionComponent*> result;

    // Iterate through all the weak pointers in the internal array
    for (const TWeakObjectPtr<UDBDClipRegionComponent>& clipRegionWeakPtr : this->_clipRegionsToUseDuringHookingAndSacrifice)
    {
        // FWeakObjectPtr::IsValid() ensures the object hasn't been garbage collected
        if (clipRegionWeakPtr.IsValid() == true) 
        {
            // If valid, resolve the weak pointer into a raw pointer and add it to the return array
            // The massive block of pointer math and `ResizeGrow` in the pseudocode 
            // is the compiler unrolling standard `TArray::Add()`.
            result.Add(clipRegionWeakPtr.Get());
        }
    }

    // By returning a value, the MSVC x64 ABI expects the caller to pre-allocate memory 
    // and pass it as a hidden pointer (the `result` parameter in the assembly).
    return result;
}

// starts at line 3078
int32 ACamperPlayer::GetNumberOfEntityRenderRegions() const
{
    return this->_clipRegionsToUseDuringHookingAndSacrifice.Num();
}

// starts at line 3088
void ACamperPlayer::Crouch(bool bClientSimulation)
{
    Super::Crouch(bClientSimulation);
    this->_hideTimer->Reset(this->_timeToHidden);
}

// starts at line 3094
void ACamperPlayer::UnCrouch(bool bClientSimulation)
{
    Super::UnCrouch(bClientSimulation);
}

// starts at line 3099
void ACamperPlayer::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    // Restore the capsule component used for hit registration (SlashableZone) to its standing height.
    // The assembly passes '0' (false) for the bUpdateOverlaps parameter to defer physics updates.
    this->SlashableZone->SetCapsuleSize(
        this->SlashableZone->GetUnscaledCapsuleRadius(), 
        this->_standingCapsuleHalfHeight,
        false 
    );

    // Reset the relative location of the SlashableZone to FVector::ZeroVector (0, 0, 0),
    // but maintain its current relative rotation. 
    // The massive SIMD block in the pseudocode is just the engine validating the Rotator cache 
    // and extracting the FQuat for this function call.
    this->SlashableZone->SetRelativeLocationAndRotation(
        FVector::ZeroVector, 
        this->SlashableZone->GetRelativeRotation().Quaternion(), 
        false, 
        nullptr, 
        ETeleportType::None
    );

    // Reset the relative location of the camera anchor to FVector::ZeroVector (0, 0, 0),
    // but maintain its current relative rotation.
    // The second massive SIMD block does the exact same Quat cache extraction for the camera.
    this->CameraAnchor->SetRelativeLocationAndRotation(
        FVector::ZeroVector, 
        this->CameraAnchor->GetRelativeRotation().Quaternion(), 
        false, 
        nullptr, 
        ETeleportType::None
    );
}

// starts at line 3106
void ACamperPlayer::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    // Shrink the capsule component used for hit registration (SlashableZone) to the crouching height.
    // The assembly passes '0' (false) for the bUpdateOverlaps parameter.
    this->SlashableZone->SetCapsuleSize(
        this->SlashableZone->GetUnscaledCapsuleRadius(), 
        this->CrouchCapsuleHalfHeight,
        false
    );

    // Calculate the vertical offset required to keep the child components physically 
    // grounded relative to the newly shrunken capsule.
    // Assembly: offset.Z = this->CrouchCapsuleHalfHeight - this->_standingCapsuleHalfHeight;
    float zOffset = this->CrouchCapsuleHalfHeight - this->_standingCapsuleHalfHeight;
    FVector relativeOffset = FVector(0.0f, 0.0f, zOffset);

    // Apply the downward offset to the SlashableZone, maintaining its current relative rotation.
    // The SIMD block extracts the FQuat for this function call.
    this->SlashableZone->SetRelativeLocationAndRotation(
        relativeOffset, 
        this->SlashableZone->GetRelativeRotation().Quaternion(), 
        false, 
        nullptr, 
        ETeleportType::None
    );

    // Apply the exact same downward offset to the camera anchor, so the player's 
    // first-person/third-person view actually drops toward the floor.
    this->CameraAnchor->SetRelativeLocationAndRotation(
        relativeOffset, 
        this->CameraAnchor->GetRelativeRotation().Quaternion(), 
        false, 
        nullptr, 
        ETeleportType::None
    );
}

// starts at line 3115
float ACamperPlayer::GetObsessionTargetWeight() const
{
    // Retrieve the Perk Manager component.
    // The pseudo-code's `&this->IPerkManagerOwnerInterface` is how the compiler 
    // resolves multiple inheritance and interface casting in standard C++. 
    // In high-level Unreal C++, this is abstracted away.
    UPerkManager* perkManager = this->GetPerkManager();

    if (IsValid(perkManager) == true)
    {
        // Ask the Perk Manager to calculate the total Obsession Weight modifier 
        // applied to this player from ALL sources (Perks, Add-ons, base stats).
        // The integer 128 (0x80) maps specifically to the Obsession Weight modifier enum.
        return perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyObsessionTargetWeight, EGameplayModifierSource::VE_All);
    }

    // If the player has no Perk Manager (which shouldn't happen in a normal match),
    // they contribute a baseline weight of 0.0 to the selection pool.
    return 0.0f;
}

// starts at line 3125
void ACamperPlayer::TriggerWiggleSkillCheck()
{
    // Retrieve the component responsible for managing skill checks and interactions
    UPlayerInteractionHandler* interactionHandler = this->GetPlayerInteractionHandler();

    if (interactionHandler != nullptr)
    {
        // Command the handler to begin a specialized skill check.
        // The value 1.0f likely represents the starting progression or scaling multiplier.
        // NOTE: Hex-Rays hallucinated the enum 'VE_EscapedInjured' because its integer value is 3.
        interactionHandler->StartCustomSkillCheck(ESkillCheckCustomType::VE_DecisiveStrikeWhileWiggling, 1.0f);
    }
}

// starts at line 3130
bool ACamperPlayer::CanBeAuthoritativePushed() const
{
    // Retrieve the Camper's data component
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // A Survivor cannot be pushed if they are currently immobilized (e.g., Hooked or Carried).
        // NOTE: IDA hallucinated 'VE_Dead'. Based on our previous functions, Immobilize State 1 is Hooked/Carried.
        if (camperData->ImmobilizeState == ECamperImmobilizeState::VE_Hooked)
        {
            return false;
        }

        // A Survivor cannot be pushed if they are in the middle of a guided action 
        // (like vaulting a window or being picked up). 
        // NOTE: IDA hallucinated 'VE_Active'. Guided Action 0 means 'None' or Idle.
        if (camperData->CurrentGuidedAction != ECamperGuidedAction::VE_None)
        {
            return false;
        }
    }

    // Finally, query the base ADBDPlayer class.
    // This handles core engine checks, such as whether the pawn's physics are completely disabled.
    if (Super::CanBeAuthoritativePushed() == false)
    {
        return false;
    }

    // If all state checks pass, the server is allowed to physically push the Camper.
    return true;
}

// starts at line 3135
void ACamperPlayer::IncrementHookedCount()
{
    // Ensure this camper is actually actively being drained on a hook right now.
    // The assembly checks `cmp byte ptr [this+1588h], 0`.
    if (this->_onHookDrainToNextStage == true)
    {
        // Increment the player's personal hooked tracker (max 3 usually).
        this->_hookedCount++;

        // Safely retrieve the specialized DBD game instance
        UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

        if (IsValid(gameInstance) == true)
        {
            // Trigger the Killer's analytics controller to log the successful hook event.
            // This powers end-of-match scoring, emblems, and player statistics.
            // Hex-Rays completely misidentified the memory offset `[rbx+7C8h]` as being part 
            // of the `OnlineSession` property. It is actually a dedicated analytics struct.
            gameInstance->_analyticsManager.GameplayKillerAnalytics.IncrementHook();
        }
    }
}

// starts at line 3150
int32 ACamperPlayer::GetHookedCount() const
{
    return this->_hookedCount;
}

// starts at line 3160
void ACamperPlayer::GetAnimTags(TArray<FName>& outTags) const
{
    // Retrieve the Camper's data component.
    // The Hex-Rays pseudo-code failed to recognize `_playerData` due to interface offset shifting.
    UDBDCamperData* camperData = Cast<UDBDCamperData>(this->_playerData);

    if (camperData != nullptr)
    {
        // Check if the survivor is in the Injured state.
        // NOTE: Hex-Rays hallucinated VE_Dead here. State 1 is Injured.
        if (camperData->GetDamageState() == ECamperDamageState::VE_Injured)
        {
            outTags.AddUnique(AnimMapping::ANIMTAG_INJURED);
        }
    }

    // Check if the survivor is currently crouching.
    if (this->IsCrouching() == true)
    {
        outTags.AddUnique(AnimMapping::ANIMTAG_CROUCHED);
    }

    // Determine the survivor's gender and add the corresponding animation tag.
    // The gender is stored at offset 0x0B40 (EGender enum).
    // Hex-Rays misidentified the offset as `_emulatedYawMinimumInput`.
    if (this->Gender == EGender::VE_Male)
    {
        outTags.AddUnique(AnimMapping::ANIMTAG_MALE);
    }
    else if (this->Gender == EGender::VE_Female)
    {
        outTags.AddUnique(AnimMapping::ANIMTAG_FEMALE);
    }
}

// starts at line 3180
float ACamperPlayer::GetLuck() const
{
    // 1. Calculate Luck bonus from Perks
    UPerkManager* perkManager = this->GetPerkManager();
    float perkLuckBonus = 0.0f;

    if (IsValid(perkManager) == true)
    {
        // The value 0x87 (135) corresponds to the Luck gameplay modifier.
        // IDA hallucinated 'VE_RevealToKiller|0x80' by bit-math (7 + 128).
        perkLuckBonus = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyLuck, EGameplayModifierSource::VE_All);
    }

    float totalLuck = perkLuckBonus;

    // 2. Calculate Luck bonus from Offerings
    // Offerings are managed at the GameInstance level because they apply to the whole trial.
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (IsValid(gameInstance) == true)
    {
        int32 playerId = -1; // Default to -1 (global/all players) if State is invalid
        ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->PlayerState);

        if (IsValid(playerState) == true)
        {
            playerId = playerState->PlayerId;
        }

        // Query the GameInstance for the sum of all Luck offerings (Effect Type 0x0C).
        float offeringLuckBonus = gameInstance->GetOfferingsModification(EOfferingEffectType::Luck, playerId);
        totalLuck = totalLuck + offeringLuckBonus;
    }

    return totalLuck;
}

// starts at line 3194
float ACamperPlayer::GetBaseFOV() const
{
    return this->GetNonPlayerSpecificTunableValue(TunableValues::CAMPER_BASE_FOV);
}

// starts at line 3199
void ACamperPlayer::UpdateStrafe()
{
    // Retrieve the current strafing state
    bool isStrafing = this->IsStrafing();

    // 1. Camera Offset Interpolation Setup
    // Only execute this block if the strafing state has changed this frame
    if (isStrafing != this->_wasStrafingLastFrame)
    {
        this->_wasStrafingLastFrame = isStrafing;

        // If the spring arm holding the camera exists
        if (IsValid(this->CameraBoom) == true)
        {
            // Snapshot the current physical offset of the camera boom
            this->_startStrafeOffset = this->CameraBoom->SocketOffset;
            
            // Reset the interpolation alpha (timer)
            this->_strafeOffsetUpdateAlpha = this->kStrafeOffsetUpdateDuration;

            if (isStrafing == false)
            {
                // Returning to normal movement: Target the center (0, 0, 0)
                this->_targetStrafeOffset = FVector::ZeroVector;
            }
            else
            {
                // Starting to strafe: Target the specific over-the-shoulder offset
                this->_targetStrafeOffset = this->StrafingOffset;
            }
        }
    }

    // 2. Character Movement & Rotation Flags
    bool isSnapping = this->IsSnapping();
    bool isSnappingRotation = this->IsSnappingRotation();

    // Evaluate APawn::bUseControllerRotationYaw (Should the character visually rotate with the mouse?)
    bool useControllerYaw = false;
    if (isStrafing == true && (isSnapping == false || isSnappingRotation == false))
    {
        useControllerYaw = true;
    }
    this->bUseControllerRotationYaw = useControllerYaw;

    // Evaluate UCharacterMovementComponent::bOrientRotationToMovement (Should the character face the direction they are walking?)
    bool orientToMovement = false;
    if (isSnappingRotation == false)
    {
        if (isStrafing == false || isSnapping == true)
        {
            orientToMovement = true;
        }
    }
    
    if (IsValid(this->GetCharacterMovement()) == true)
    {
        this->GetCharacterMovement()->bOrientRotationToMovement = orientToMovement;
    }
}

// starts at line 3224
void ACamperPlayer::UpdateSurvivorAnalytics(float deltaSeconds)
{
    // Retrieve the specialized DBD Game Instance
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (IsValid(gameInstance) == true)
    {
        // Analytics are only tracked for the player actually controlling this character 
        // (to prevent the server or other clients from double-reporting data).
        if (this->IsLocallyControlled() == true)
        {
            // Do not record traversal speed if the player is locked into an interaction
            if (this->IsInteracting == false)
            {
                // Ensure the player is actively capable of moving. 
                // We still want to track speed if they are crawling (Dying state).
                if (this->IsUnintentionallyIncapacitated() == false || this->IsCrawling() == true)
                {
                    // Retrieve the player's current velocity vector
                    FVector velocity = this->GetVelocity();

                    // Calculate the magnitude (speed) of the velocity vector.
                    // The assembly squares X, Y, and Z, adds them, and takes the square root (`sqrtss`).
                    // In standard Unreal Engine, this is simply Size().
                    float speedUEUnits = velocity.Size();

                    // Convert Unreal Units (cm/s) to Meters per Second (m/s).
                    // The assembly multiplies by cs:__real@3c23d70a (0.01f).
                    float speedMetersPerSecond = speedUEUnits * 0.01f;

                    // Send the data to the global Survivor Analytics Controller.
                    // NOTE: IDA incorrectly guessed the struct offset as `&GameInstance[9].FExec`. 
                    // It is actually the dedicated analytics controller property.
                    gameInstance->_analyticsManager.GameplaySurvivorAnalytics.RecordSpeed(speedMetersPerSecond, deltaSeconds);
                }
            }
        }
    }
}

// starts at line 3252
void ACamperPlayer::SetEnableHookSlashableZone(bool bEnable)
{
    // 1. Toggle the collision state.
    // In Unreal Engine, ECollisionEnabled::NoCollision is 0, and QueryOnly is 1.
    // The compiler simply casts the boolean directly to the enum.
    this->HookSlashableZone->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);

    // 2. Toggle the overlapping event flag.
    // The massive block of XOR/AND bitwise math in the pseudo-code is the compiler 
    // updating the UPrimitiveComponent::bGenerateOverlapEvents bitfield.
    this->HookSlashableZone->bGenerateOverlapEvents = bEnable;

    // 3. Force the physics engine to re-evaluate the collision state immediately.
    // The assembly executes this via a tail-call jump (jmp qword ptr [rax+3E0h]).
    this->HookSlashableZone->UpdateOverlaps(nullptr, true, nullptr);
}

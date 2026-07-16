// starts at line 83 in slasherplayer.h
UDBDSlasherData* ASlasherPlayer::GetSlasherData()
{
    return Cast<UDBDSlasherData>(this->_playerData);
}

// starts at line 137 in slasherplayer.h
TArray<UItemAddon*> ASlasherPlayer::GetItemAddons() const
{
    // The decompiled assembly shows an inline copy of a TArray at offset 0xC70.
    // Based on the context of the user prompt and the return type, this corresponds to the _powerAddons array.
    // The interface function IPerkManagerOwnerInterface::GetItemAddons returns a copy of this array by value.
    return this->_powerAddons;
}

// starts at line 90
ASlasherPlayer::ASlasherPlayer(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USlasherMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Initialize aspect ratio threshold
    this->_screenAspectRatio = 4.0f / 3.0f; // approx 4:3 (1086324736 is hex 40AABBA0 -> roughly 1.333)

    // Initialize animation direction threshold
    this->kAnimDirectionSideThreshold = 60.0f;

    // Initialize Tunable Values
    this->LoudNoiseDisplayTime = 5.0f;
    this->TerrorRadius = 3200.0f;
    this->CamperExposer = nullptr; // Nullptr assignment to class reference

    // Initialize Target Priority
    this->TargetPriorityFactor = FVector(0.75f, 0.75f, 1.0f);

    // Initialize Cheat Flag
    this->CheatChainsaw = false;

    // Initialize Mechanic Flag
    this->TempUseRageMechanic = true;

    // Initialize Audio Component (ID = -1, Serial = 0 implies uninitialized/invalid)
    this->_audioComponent = nullptr; 

    // Initialize Slasher specific attributes
    this->_attackAcceleration = 112500.0f;
    this->_slasherStealthRatio = 1.0f;
    this->_slasherLightIntensity = 1.0f;
    this->_guidedCamper = nullptr;

    this->_stalkTierWalkSpeedMultiplier = 1.0f;
    this->_stealthEnabled = true;
    this->_isCloaked = false;
    this->_isChainsawSprinting = false;
    this->_allowedKillCount = 0;
    this->_lungeType = EAttackType::VE_None;

    // Load Global Tunable DB
    this->GlobalTunableDB.Reset(FString(TEXT("/Game/Data/KillerTunableDB.KillerTunableDB")));

    // Create State Machine Subobject
    this->_SM = CreateDefaultSubobject<USlasherStateMachine>(TEXT("SlasherStateMachine"));

    // Create Player Data Subobject
    this->_playerData = CreateDefaultSubobject<UDBDSlasherData>(TEXT("PlayerData"));

    // Create Camper Sensor Subobject
    this->_camperSensor = CreateDefaultSubobject<UDBDPawnSensingComponent>(TEXT("CamperSensor"));
    
    // Bind OnSeePawn Delegate
    // The assembly extracts the function name using wcsstr and binds it via __Internal_AddDynamic
    this->_camperSensor->OnSeePawn.AddDynamic(this, &ASlasherPlayer::OnChaseTargetFound);

    // Create Flashlight Receiver Component
    this->_fullBodyFlashLightReceiver = CreateDefaultSubobject<UFlashLightReceiverComponent>(TEXT("FullBodyFlashLightReceiver"));
    this->_fullBodyFlashLightReceiver->ComponentTags.Add(UDBDTags::FULLBODY);

    // Create Invisibility Burnout Component
    this->_invisibilityBurnoutChargeableComponent = CreateDefaultSubobject<UChargeableComponent>(TEXT("InvisibilityBurnoutChargeableComponent"));
    this->_invisibilityBurnoutChargeableComponent->ComponentTags.Add(UDBDTags::INVISIBILITY_BURNOUT);
    this->_invisibilityBurnoutChargeableComponent->ChargeableComponentID = UDBDTags::INVISIBILITY_BURNOUT.ToString();
    this->_invisibilityBurnoutChargeableComponent->OnInteractionCompletionStateChanged.AddUObject(this, &ASlasherPlayer::OnInvisibilityBurnoutChargeCompletionChanged);
    this->_invisibilityBurnoutChargeableComponent->OnInteractionEmptiedEvent.AddDynamic(this, &ASlasherPlayer::OnInvisibilityChargeEmptied);

    // Create Stillness Tracker Component
    this->_stillnessTracker = CreateDefaultSubobject<UStillnessTrackerComponent>(TEXT("StillnessTracker"));
}

// starts at line 113
void ASlasherPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // Call the base class implementation to ensure inherited properties are replicated
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Register Slasher specific properties for network replication using standard Unreal Engine macros
    DOREPLIFETIME(ASlasherPlayer, _powerAddons);
    DOREPLIFETIME(ASlasherPlayer, _ragePercent);
}

// starts at line 120
void ASlasherPlayer::InitializeTunableValues()
{
    // Initialize tunable values from the base player class
    Super::InitializeTunableValues();

    // Retrieve the Game Instance to access design tunables
    UGameInstance* GameInstance = this->GetGameInstance();
    if (GameInstance == nullptr)
    {
        return;
    }

    // Attempt to cast the Game Instance to the game-specific class
    UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
    if (DBDGameInstance == nullptr)
    {
        return;
    }

    // Assuming DesignTunables is an accessible field/property of UDBDGameInstance based on pseudo-code offset access
    UDBDDesignTunables* DesignTunables = DBDGameInstance->DesignTunables;
    if (DesignTunables == nullptr)
    {
        return;
    }

    // Initialize chase-related maximum distance values
    this->_chaseMaxDistance = DesignTunables->GetTunableValue(TunableValues::CHASE_MAXIMUM_DISTANCE, false);
    
    float chaseStartMaxDistance = DesignTunables->GetTunableValue(TunableValues::CHASE_MAXIMUM_START_DISTANCE, false);
    float resolvedStartMaxDistance = 0.0f;
    if (chaseStartMaxDistance >= 0.0f)
    {
        resolvedStartMaxDistance = FMath::Min(this->_chaseMaxDistance, chaseStartMaxDistance);
    }
    this->_chaseStartMaxDistance = resolvedStartMaxDistance;

    // Initialize chase cooldown and post-chase times
    this->_chaseSightCooldown = DesignTunables->GetTunableValue(TunableValues::CHASE_SLASHER_SIGHT_COOLDOWN, false);
    this->_postChaseTime = DesignTunables->GetTunableValue(TunableValues::POST_CHASE_SLASHER_COOLDOWN, false);

    // Apply values to the camper sensor component if it exists
    if (this->_camperSensor != nullptr)
    {
        this->_camperSensor->SightRadius = this->_chaseMaxDistance;
        
        float viewAngle = DesignTunables->GetTunableValue(TunableValues::CHASE_VIEW_ANGLE, false);
        this->_camperSensor->SetPeripheralVisionAngle(viewAngle);
        
        this->_camperSensor->PartiallyHiddenSensingDistance = DesignTunables->GetTunableValue(TunableValues::CHASE_CAMPER_PARTIALLY_HIDDEN_RANGE, false);
    }

    // Initialize stealth values (ratio delay, increase, and decrease rates)
    this->_stealthDelay = this->GetTunableValue(TunableValues::SLASHER_STEALTH_RATIO_DELAY, this->_stealthDelay, false);
    this->_stealthIncreaseRate = DesignTunables->GetTunableValue(TunableValues::SLASHER_STEALTH_INCREASE_RATE, false);
    this->_stealthDecreaseRate = DesignTunables->GetTunableValue(TunableValues::SLASHER_STEALTH_DECREASE_RATE, false);

    // Initialize flashlight evasion cooldown
    this->_flashlightEvasionScoreCooldown = DesignTunables->GetTunableValue(TunableValues::FLASHLIGHT_EVASION_SCORE_COOLDOWN, false);

    // Attempt to retrieve and cast the Slasher Movement Component
    USlasherMovementComponent* SlasherMovement = Cast<USlasherMovementComponent>(this->CharacterMovement);
    
    if (SlasherMovement != nullptr)
    {
        // Set specific movement speed limits for Slasher abilities
        SlasherMovement->BlinkMaxWalkSpeedPercent = DesignTunables->GetTunableValue(TunableValues::SLASHER_BLINK_MAX_WALK_SPEED_PERCENT, false);
        SlasherMovement->StalkModeSpeedFactor = DesignTunables->GetTunableValue(TunableValues::SLASHER_STALK_MODE_SPEED_FACTOR, false);
        SlasherMovement->StalkModeWhileStalkingSomeoneSpeedFactor = DesignTunables->GetTunableValue(TunableValues::SLASHER_STALK_MODE_WHILE_STALKING_SOMEONE_SPEED_FACTOR, false);
    }

    // Update the base character max walk speed
    if (this->CharacterMovement != nullptr)
    {
        this->CharacterMovement->MaxWalkSpeed = this->GetTunableValue(TunableValues::MAX_WALK_SPEED, this->CharacterMovement->MaxWalkSpeed, false);
    }

    // Set carrying and cloaked movement speed factors based on base walk speed
    if (SlasherMovement != nullptr)
    {
        float maxWalkSpeed = 0.0f;
        if (this->CharacterMovement != nullptr)
        {
            maxWalkSpeed = this->CharacterMovement->MaxWalkSpeed;
        }
        
        SlasherMovement->CarryingMaxWalkSpeedPercent = this->GetTunableValue(TunableValues::CARRYING_MAX_WALK_SPEED_PERCENT, maxWalkSpeed, false);
    }

    // Initialize terror radius
    this->_baseTerrorRadius = this->GetTunableValue(TunableValues::TERROR_RADIUS, this->TerrorRadius, false);

    // Set cloaked walk speed factor
    if (SlasherMovement != nullptr)
    {
        SlasherMovement->CloakedMaxWalkSpeedPercent = this->GetTunableValue(TunableValues::CLOAKED_MAX_WALK_SPEED_PERCENT, 1.0f, false);
    }

    // Initialize invisibility burnout decharge rate if the component exists
    if (this->_invisibilityBurnoutChargeableComponent != nullptr)
    {
        this->_invisibilityBurnoutChargeableComponent->DechargeRate = DesignTunables->GetTunableValue(TunableValues::INVISIBILITY_BURNOUT_DECHARGE_RATE, false);
    }

    // Initialize stillness tracker limits and flags
    if (this->_stillnessTracker != nullptr)
    {
        this->_stillnessTracker->SpeedThreshold = FLT_MAX;
        this->_stillnessTracker->DistanceThreshold = DesignTunables->GetTunableValue(TunableValues::SLASHER_STILLNESS_DISTANCE_THRESHOLD, false);
        this->_stillnessTracker->DistanceDecay = DesignTunables->GetTunableValue(TunableValues::SLASHER_STILLNESS_DISTANCE_DECAY, false);
        this->_stillnessTracker->DecayWhileMoving = false;
    }
}

// starts at line 168
float ASlasherPlayer::GetAttackOpenTime(EAttackType attackType) const
{
    float openTime = 0.0f;

    // Determine the base open time based on the attack type
    if (attackType >= EAttackType::VE_Pounce)
    {
        if (attackType <= EAttackType::VE_Chainsaw)
        {
            // Pounce or Ambush attack
            openTime = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_POUNCE_OPEN_TIME);
        }
        else if (attackType == EAttackType::VE_Blink)
        {
            // Blink attack
            openTime = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_BLINK_OPEN_TIME);
        }
        else if (attackType == EAttackType::VE_Lunge)
        {
            // Stalk attack
            UStalkerComponent* stalkerComponent = Cast<UStalkerComponent>(this->GetFirstComponentByClass(UStalkerComponent::StaticClass()));
            
            if (stalkerComponent != nullptr)
            {
                // If a stalker component exists, get the open time based on the current stalk tier
                int currentTier = stalkerComponent->GetCurrentTier();
                // Array access based on stalk tier
                openTime = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_POUNCE_OPEN_TIME_STALK[currentTier]);
            }
            else
            {
                // Fallback to default pounce open time if no stalker component is found
                openTime = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_POUNCE_OPEN_TIME);
            }
        }
    }

    // Apply multiplicative and additive modifiers from perks
    UPerkManager* perkManager = this->GetPerkManager();
    if (perkManager != nullptr)
    {
        // Assuming 130 is EGameplayModifierType::VE_ModifyAttackLungeDurationMultiplicative based on the multiplicative context
        float multiplicativeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyAttackLungeDurationMultiplicative);
        float modifiedOpenTime = openTime * multiplicativeModifier;
        
        // Assuming 129 is EGameplayModifierType::VE_ModifyAttackLungeDurationAdditive and 4 is EGameplayModifierSource::VE_All
        float additiveModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyAttackLungeDurationAdditive, EGameplayModifierSource::VE_All);
        
        return modifiedOpenTime + additiveModifier;
    }

    return openTime;
}

// starts at line 199
float ASlasherPlayer::GetAttackHittingTime(EAttackType attackType) const
{
    // Determine the hitting time based on the attack type
    switch (attackType)
    {
    case EAttackType::VE_Slash:
        // Slash attack
        return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_SLASH_HITTING_TIME);

    case EAttackType::VE_Chainsaw:
        // Chainsaw attack
        return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_CHAINSAW_HITTING_TIME);

    case EAttackType::VE_Pounce:
    case EAttackType::VE_Blink:
    case EAttackType::VE_Lunge:
        // Pounce, Blink, or Lunge attack (all use pounce hitting time)
        return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_POUNCE_HITTING_TIME);

    default:
        // Return 0 for invalid attack types
        return 0.0f;
    }
}

// starts at line 219
float ASlasherPlayer::GetAttackSucceedCooldownTime(EAttackType attackType) const
{
    // Check if the fast attack cooldown debug console variable is active
    static IConsoleVariable* CVarFastAttackCoolDown = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.FastAttackCoolDown"));
    if (CVarFastAttackCoolDown != nullptr && CVarFastAttackCoolDown->GetInt() != 0)
    {
        return 0.0f;
    }

    // Retrieve the PerkManager via the IPerkManagerOwnerInterface (vtable offset 0x7A8)
    UPerkManager* perkManager = this->GetPerkManager();
    float multiplicativeModifier = 0.0f;
    float baseCooldown = 0.0f;

    // Determine the succeed cooldown time based on the attack type
    if (attackType == EAttackType::VE_Slash)
    {
        // Slash attack
        multiplicativeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyAttackHitCooldown);
        baseCooldown = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_SLASH_SUCCEED_COOLDOWN_TIME);
        
        return baseCooldown * multiplicativeModifier;
    }
    
    if (attackType == EAttackType::VE_Chainsaw)
    {
        // Chainsaw attack
        multiplicativeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyChainsawCooldownSuccess);
        baseCooldown = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_CHAINSAW_SUCCEED_COOLDOWN_TIME);
        
        return baseCooldown * multiplicativeModifier;
    }
    
    if (attackType == EAttackType::VE_Pounce || attackType == EAttackType::VE_Blink || attackType == EAttackType::VE_Lunge)
    {
        // Pounce, Blink, or Stalk attack
        multiplicativeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyAttackHitCooldown);
        baseCooldown = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_POUNCE_SUCCEED_COOLDOWN_TIME);
        
        return baseCooldown * multiplicativeModifier;
    }

    // Return 0.0f for invalid attack types
    return 0.0f;
}

// starts at line 242
float ASlasherPlayer::GetAttackMissCooldownTime(EAttackType attackType) const
{
    // Check if the fast attack cooldown console variable is active
    static IConsoleVariable* CVarFastAttackCoolDown = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.FastAttackCoolDown"));
    if (CVarFastAttackCoolDown != nullptr && CVarFastAttackCoolDown->GetInt() != 0)
    {
        return 0.0f;
    }

    // Retrieve the PerkManager via the IPerkManagerOwnerInterface (vtable offset 0x7A8)
    UPerkManager* perkManager = this->GetPerkManager();
    float multiplicativeModifier = 0.0f;
    float baseCooldown = 0.0f;

    // Determine the miss cooldown time based on the attack type
    if (attackType == EAttackType::VE_Slash)
    {
        // Slash attack
        multiplicativeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyAttackMissCooldown);
        baseCooldown = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_SLASH_MISS_COOLDOWN_TIME);
        
        return baseCooldown * multiplicativeModifier;
    }
    
    if (attackType == EAttackType::VE_Chainsaw)
    {
        // Chainsaw attack
        multiplicativeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyChainsawCooldownSuccess);
        baseCooldown = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_CHAINSAW_MISS_COOLDOWN_TIME);
        
        return baseCooldown * multiplicativeModifier;
    }
    
    if (attackType == EAttackType::VE_Pounce || attackType == EAttackType::VE_Blink || attackType == EAttackType::VE_Lunge)
    {
        // Pounce, Blink, or Stalk attack
        multiplicativeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyAttackMissCooldown);
        baseCooldown = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_POUNCE_MISS_COOLDOWN_TIME);
        
        return baseCooldown * multiplicativeModifier;
    }

    // Return 0.0f for invalid attack types
    return 0.0f;
}

// starts at line 265
float ASlasherPlayer::GetAttackObstructCooldownTime(EAttackType attackType) const
{
    // Check if the fast attack cooldown console variable is active
    static IConsoleVariable* CVarFastAttackCoolDown = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.FastAttackCoolDown"));
    if (CVarFastAttackCoolDown != nullptr && CVarFastAttackCoolDown->GetInt() != 0)
    {
        return 0.0f;
    }

    // Determine the obstruct cooldown time based on the attack type
    if (attackType == EAttackType::VE_Slash)
    {
        // Slash attack
        return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_SLASH_OBSTRUCT_COOLDOWN_TIME);
    }
    else if (attackType == EAttackType::VE_Chainsaw)
    {
        // Chainsaw attack
        // Retrieve the PerkManager via the IPerkManagerOwnerInterface (vtable offset 0x7A8)
        UPerkManager* perkManager = this->GetPerkManager();

        // Assuming 0x4F (79) is EGameplayModifierType::VE_ModifyChainsawCooldownObstruction and 4 is EGameplayModifierSource::VE_All
        float additiveModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyChainsawCooldownObstruction, EGameplayModifierSource::VE_All);
        
        // Additive modifier acts as a percentage increase, so add 1.0f
        float multiplier = additiveModifier + 1.0f;
        float baseCooldown = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_CHAINSAW_OBSTRUCT_COOLDOWN_TIME);
        
        return baseCooldown * multiplier;
    }
    else if (attackType == EAttackType::VE_Pounce || attackType == EAttackType::VE_Blink || attackType == EAttackType::VE_Lunge)
    {
        // Pounce, Blink, or Lunge attack
        return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_POUNCE_OBSTRUCT_COOLDOWN_TIME);
    }

    // Return 0.0f for invalid attack types
    return 0.0f;
}

// starts at line 288
float ASlasherPlayer::GetAttackSnapDistance(EAttackType attackType) const
{
    // Determine the snap distance based on the attack type
    switch (attackType)
    {
    case EAttackType::VE_Slash:
        // Slash attack snap distance
        return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_SLASH_SNAP_DISTANCE);

    case EAttackType::VE_Chainsaw:
        // Chainsaw attack snap distance
        return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_CHAINSAW_SNAP_DISTANCE);

    case EAttackType::VE_Pounce:
    case EAttackType::VE_Blink:
    case EAttackType::VE_Lunge:
        // Pounce, Blink, or Lunge attack (all use the pounce snap distance)
        return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_POUNCE_SNAP_DISTANCE);

    default:
        // Return 0.0f for unhandled attack types
        return 0.0f;
    }
}

// starts at line 309
float ASlasherPlayer::GetAttackHittingPercentDamageBegin() const
{
    return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_HITTING_PERCENT_DAMAGE_BEGIN);
}

// starts at line 314
float ASlasherPlayer::GetAttackAngleTrackingSpeed() const
{
    return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_ANGLE_TRACKING_SPEED);
}

// starts at line 319
float ASlasherPlayer::GetAttackYawSpeed(EAttackType attackType) const
{
    // Return 0.0f if there is no valid attack type (value is 0)
    if (attackType == EAttackType::None)
    {
        return 0.0f;
    }

    // Determine the yaw speed based on the attack type
    switch (attackType)
    {
        case EAttackType::VE_Chainsaw:
        {
            // Retrieve the PerkManager via the IPerkManagerOwnerInterface
            UPerkManager* perkManager = this->GetPerkManager();
            
            // 0x4B (75) is the specific EGameplayModifierType multiplier for Chainsaw attack yaw speed
            float multiplicativeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyChainsawChargeYawSpeed);
            
            float baseYawSpeed = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_CHAINSAW_YAW_SPEED);
            
            return baseYawSpeed * multiplicativeModifier;
        }

        case EAttackType::VE_Slash:
        case EAttackType::VE_Pounce:
        case EAttackType::VE_Blink:
        case EAttackType::VE_Lunge:
        {
            // Slash, Pounce, Blink, and Lunge attacks all share the same lunge yaw speed
            return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_LUNGE_YAW_SPEED);
        }

        default:
        {
            // Return 0.0f for any unhandled attack types beyond 5
            return 0.0f;
        }
    }
}

// starts at line 337
float ASlasherPlayer::GetAttackSnapTime(EAttackType attackType) const
{
    switch (attackType)
    {
        case EAttackType::VE_Slash:
        case EAttackType::VE_Pounce:
        case EAttackType::VE_Chainsaw:
        case EAttackType::VE_Blink:
        {
            return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_SNAP_TIME);
        }
        case EAttackType::VE_Lunge:
        {
            return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_ATTACK_LUNGE_SNAP_TIME);
        }
        default:
        {
            // Return 0.0f for any unhandled attack types beyond 5
            return 0.0f;
        }
    }
}

// starts at line 360
void ASlasherPlayer::AuthorityHandleLoadoutOnSpawn()
{
    this->SpawnSlasherPower();
}

// starts at line 366
void ASlasherPlayer::AuthorityHandleOfferingsOnSpawn()
{
    // Retrieve the base game instance from the actor
    UGameInstance* gameInstance = this->GetGameInstance();

    // Verify that the game instance is valid before proceeding
    if (gameInstance != nullptr)
    {
        // Cast the base game instance to the project-specific UDBDGameInstance
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(gameInstance);

        // Verify that the cast was successful
        if (dbdGameInstance != nullptr)
        {
            // Ensure the PlayerState is valid before attempting to access the PlayerId
            if (this->PlayerState != nullptr)
            {
                // Request the offering modification for KillAllowedModifier (Enum value 23 / 0x17)
                int32_t offeringKillAllowedCount = static_cast<int32_t>(dbdGameInstance->GetOfferingsModification(EOfferingEffectType::KillAllowedModifier, this->PlayerState->PlayerId));

                // Determine the highest value between the current allowed kills and the offering modification
                int32_t resolvedKillCount = offeringKillAllowedCount;
                if (this->_allowedKillCount >= offeringKillAllowedCount)
                {
                    resolvedKillCount = this->_allowedKillCount;
                }

                // Broadcast the updated kill count allowance to all clients
                this->Multicast_AllowKilling(resolvedKillCount);

                // Request the offering modification for KillAllowedAfterHook (Enum value 33 / 0x21)
                int32_t offeringKillAfterHookCount = static_cast<int32_t>(dbdGameInstance->GetOfferingsModification(EOfferingEffectType::KillAllowedAfterHook, this->PlayerState->PlayerId));

                // Determine the highest value between the current allowed kills after hooking and the offering modification
                int32_t resolvedKillAfterHookCount = offeringKillAfterHookCount;
                if (this->_allowedKillAfterHookingCount >= offeringKillAfterHookCount)
                {
                    resolvedKillAfterHookCount = this->_allowedKillAfterHookingCount;
                }

                // Broadcast the updated kill after hooking count allowance to all clients
                this->Multicast_AllowKillingAfterHooking(resolvedKillAfterHookCount);
            }
        }
    }
}

// starts at line 380
void ASlasherPlayer::SpawnSlasherPower_Implementation()
{
    // Retrieve the base game instance
    UGameInstance* gameInstance = this->GetGameInstance();
    if (gameInstance == nullptr)
    {
        return;
    }

    // Cast to the project-specific game instance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(gameInstance);
    if (dbdGameInstance == nullptr)
    {
        return;
    }

    // Check if this instance has authority (ROLE_Authority == 3)
    if (this->Role == ROLE_Authority)
    {
        if (this->PlayerState == nullptr)
        {
            return;
        }

        // Default to the base Slasher power
        FName powerToSpawn = this->DefaultSlasherPower;

        // Assuming _persistentData is a property of UDBDGameInstance located at offset 0x3B8
        UDBDPersistentData* persistentDataManager = dbdGameInstance->_persistentData;

        if (persistentDataManager != nullptr)
        {
            // Fetch the player's persistent data using their unique network ID
            FPlayerPersistentData* playerPersistentData = persistentDataManager->GetPlayerPersistentData(this->PlayerState->UniqueId);

            if (playerPersistentData != nullptr)
            {
                FName equippedPower = playerPersistentData->CurrentLoadout.Power;

                // Override the default power if the player has a valid power equipped in their loadout
                if (equippedPower != LoadoutSlotDefaultValue::EMPTY && equippedPower != LoadoutSlotDefaultValue::LOCKED)
                {
                    powerToSpawn = equippedPower;
                }
            }
        }

        // Spawn the resolved power item
        this->SpawnCollectedItem(powerToSpawn);
    }
}

// starts at line 400
void ASlasherPlayer::BeginPlay()
{
    // Call the base class implementation
    Super::BeginPlay();

    // Setup GameState bindings if available
    UWorld* world = this->GetWorld();
    if (world != nullptr)
    {
        AGameState* gameState = world->GameState;
        if (gameState != nullptr)
        {
            ADBDGameState* dbdGameState = Cast<ADBDGameState>(gameState);
            if (dbdGameState != nullptr)
            {
                // Ensure we are authoritative (ROLE_Authority == 3) or not simulated proxy before binding
                if (this->Role != ROLE_SimulatedProxy)
                {
                    // Add dynamic delegate binding to OnSurvivorsLeftChanged event
                    // Note: The specific macro or function for this binding in the custom engine version isn't standard, 
                    // translating the intent of the delegate binding.
                    dbdGameState->OnSurvivorsLeftChanged.AddDynamic(this, &ASlasherPlayer::OnSurvivorsLeftChanged);
                }
            }
        }
    }

    // Setup GameInstance bindings and components if available
    UGameInstance* gameInstance = this->GetGameInstance();
    if (gameInstance != nullptr)
    {
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(gameInstance);
        if (dbdGameInstance != nullptr)
        {
            // Find and cache the LoudNoiseHUDIndicator component
            this->_loudNoiseIndicator = Cast<ULoudNoiseHUDIndicator>(this->FindComponentByClass(ULoudNoiseHUDIndicator::StaticClass()));
            
            if (this->_loudNoiseIndicator != nullptr)
            {
                // The exact signature for creating and passing this delegate differs by UE version/customization.
                // Representing the creation and registration of the sound event listener.
                TBaseDelegate<void, AActor*, FVector, bool, float> soundDelegate;
                soundDelegate.BindUObject(this, &ASlasherPlayer::OnSoundTriggered);
                
                this->_soundEventHandle = dbdGameInstance->RegisterSoundEventListener(soundDelegate);
            }
        }
    }
}

// starts at line 423
UStillnessTrackerComponent* ASlasherPlayer::GetStillnessComponent()
{
    return this->_stillnessTracker;
}

// starts at line 428
bool ASlasherPlayer::IsSlasherLocallyControlled() const
{
    // Check if the player currently has a valid controller assigned
    if (this->Controller == nullptr)
    {
        return false;
    }

    // The bitwise check corresponds to the inline 'bIsPlayerController' flag in UE4's AController
    if (this->Controller->IsPlayerController() == false)
    {
        return false;
    }

    // Check if the controller is locally controlled (not a remote network client)
    if (this->Controller->IsLocalController() == false)
    {
        return false;
    }

    return true;
}

// starts at line 485
void ASlasherPlayer::Tick(float DeltaSeconds)
{
    // Call base class Tick
    Super::Tick(DeltaSeconds);

    // Update animations based on direction
    this->UpdateAnimDirection(DeltaSeconds);

    // If not acting as an authority, interpolate the rage percentage for smooth client-side visual/audio updates
    if (this->Role != ROLE_Authority)
    {
        float interpolatedRage = FMath::FInterpTo(this->_ragePecentSmooth, this->_ragePercent, DeltaSeconds, 1.0f);
        this->SetRage(FMath::Max(interpolatedRage, 0.0f));
    }

    // Update timers
    if (this->_recentlyCloakedTimer.IsDone() == false)
    {
        this->_recentlyCloakedTimer.Update(DeltaSeconds);
    }
    
    this->_flashlightBlindEvasionScoreTimer.Update(DeltaSeconds);
    this->_flashlightBurnoutEvasionScoreTimer.Update(DeltaSeconds);

    // Handle movement of a carried camper
    if (this->IsCarrying() == true)
    {
        if (this->_guidedCamper != nullptr)
        {
            if (this->_guidedCamper->IsPendingKill() == false)
            {
                if (this->_guidedCamper->IsValidImpl() == true)
                {
                    if (this->_guidedCamper->IsMovementGuided() == true)
                    {
                        this->_guidedCamper->BeingCarriedMovement();
                    }
                }
            }
        }
    }

    // Check stillness state
    FVector velocity = this->GetVelocity();
    bool isMoving = (velocity.X != FVector::ZeroVector.X || velocity.Y != FVector::ZeroVector.Y || velocity.Z != FVector::ZeroVector.Z);

    if (isMoving == true)
    {
        // If moving but idle start time is set, it means the player just started moving
        if (this->_idleStart.GetTicks() != 0)
        {
            // If they were idle for at least 10 seconds and aren't cloaked, play a shock sound
            FDateTime now = FDateTime::UtcNow();
            double idleDurationSeconds = static_cast<double>(now.GetTicks() - this->_idleStart.GetTicks()) * 0.0000001; // convert ticks to seconds
            
            if (idleDurationSeconds >= 10.0 && this->IsCloaked() == false)
            {
                // Note: The specific audio system implementation relies on Wwise (FAkAudioDevice)
                FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
                if (audioDevice != nullptr)
                {
                    // The argument structure for PostEvent varies wildly depending on the Wwise integration version
                    audioDevice->PostEvent(TEXT("AudioEvent_Killer_Moving_Shock"), this, 0, nullptr, nullptr, false);
                }
            }

            // Reset idle tracking and audio RTPC
            this->_idleStart = FDateTime(0);
            
            FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
            if (audioDevice != nullptr)
            {
                audioDevice->SetRTPCValue(TEXT("AudioRTPC_Killer_Time_Stop_Moving"), 0.0f, 0, nullptr);
            }
        }
        
        // Moving breaks stealth building
        this->_stealthStartTime = FDateTime(0);
    }
    else
    {
        // If not moving and idle start isn't set, initialize the idle tracking timers
        if (this->_idleStart.GetTicks() == 0)
        {
            FDateTime now = FDateTime::UtcNow();
            this->_idleStart = now;
            this->_lastUpdate = now;
        }

        // Determine if an interaction breaks stillness/stealth building
        bool actionBreaksStillness = false;
        if (this->IsAttacking == true)
        {
            actionBreaksStillness = true;
        }
        else
        {
            UInteractionDefinition* currentInteraction = this->GetCurrentInteraction();
            if (currentInteraction != nullptr)
            {
                if (currentInteraction->AffectsStillness == true)
                {
                    actionBreaksStillness = true;
                }
            }
        }

        if (actionBreaksStillness == true)
        {
            // Reset stealth build up if performing a stillness breaking action
            this->_stealthStartTime = FDateTime(0);
        }
        else
        {
            // If completely still and not attacking/interacting, start building stealth
            if (this->_stealthStartTime.GetTicks() == 0)
            {
                this->_stealthStartTime = FDateTime::UtcNow();
            }
        }
    }

    // Update various Slasher-specific systems
    this->UpdateSlasherStealthRatio(DeltaSeconds);
    this->UpdateTerrorRadius();
    this->UpdateChaseSequence(DeltaSeconds);
    this->UpdateCloakStalkTimers(DeltaSeconds);
    this->UpdateKillerAnalytics(DeltaSeconds);
    this->HandleGuidedCamperDeconnection();
}

// starts at line 564
void ASlasherPlayer::UpdateSlasherStealthRatio_Implementation(float deltaSeconds)
{
    // If stealth is completely disabled, reset ratio to 1.0 (fully visible) and exit
    if (this->_stealthEnabled == false)
    {
        this->_slasherStealthRatio = 1.0f;
        return;
    }

    // State check, note: IsUncloaking is called for side-effects if the condition is met based on assembly
    if (this->IsCloaked() == true || this->IsCloaking() == true)
    {
        this->IsUncloaking();
    }

    // Calculate how long the slasher has been still/stealthy in seconds
    FDateTime now = FDateTime::UtcNow();
    float timeSinceStealthStart = static_cast<float>((now - this->_stealthStartTime).GetTotalSeconds());

    bool isStealthy = false;
    
    // Check if player has the perk flag allowing stealth
    UPerkManager* perkManager = this->GetPerkManager();
    // 0x0E (14) represents VE_AllowSlasherStealth flag, 4 is VE_All
    if (perkManager->HasPerkFlag(EGameplayModifierFlag::VE_AllowSlasherStealth, EGameplayModifierSource::VE_All) == true)
    {
        if (this->_stealthStartTime.GetTicks() != 0)
        {
            // 0x45 (69) represents the modifier type modifying stealth delay
            float stealthModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifySlasherEnterStealthSpeed, EGameplayModifierSource::VE_All);
            
            // Calculate effective delay. The denominator is clamped to avoid division by zero or negative values.
            float denominator = FMath::Max(stealthModifier + 1.0f, 0.0001f);
            float effectiveStealthDelay = this->_stealthDelay * (1.0f / denominator);

            if (timeSinceStealthStart >= effectiveStealthDelay)
            {
                isStealthy = true;
            }
        }
    }

    if (isStealthy == true || this->IsCloaked() == true)
    {
        // Increase stealth (decrease ratio towards 0.0)
        if (this->_slasherStealthRatio > 0.0f)
        {
            this->_slasherStealthRatio = FMath::Max(this->_slasherStealthRatio - (deltaSeconds * this->_stealthIncreaseRate), 0.0f);
        }
    }
    else
    {
        // Decrease stealth (increase ratio towards 1.0)
        if (this->IsCloaked() == false)
        {
            if (this->_slasherStealthRatio < 1.0f)
            {
                this->_slasherStealthRatio = FMath::Min(this->_slasherStealthRatio + (deltaSeconds * this->_stealthDecreaseRate), 1.0f);
            }
        }
    }
}

// starts at line 596
void ASlasherPlayer::SetLightIntensity(float intensity)
{
    this->_slasherLightIntensity = intensity;
}

// starts at line 601
void ASlasherPlayer::SetBaseTerrorRadius(float baseTerrorRadius)
{
    this->_baseTerrorRadius = baseTerrorRadius;
}

// starts at line 606
void ASlasherPlayer::SetStalkTierWalkSpeedMultiplier(float stalkTierWalkSpeedMultiplier)
{
    this->_stalkTierWalkSpeedMultiplier = stalkTierWalkSpeedMultiplier;
}

// starts at line 611
void ASlasherPlayer::UpdateTerrorRadius()
{
    // Obtain the perk manager via the interface to evaluate modifiers
    UPerkManager* perkManager = this->GetPerkManager();

    if (perkManager != nullptr)
    {
        // Check if there is a perk flag suppressing the terror radius (Enum flag 0x42 / 66)
        if (perkManager->HasPerkFlag(EGameplayModifierFlag::VE_SuppressTerrorRadius, EGameplayModifierSource::VE_All) == true)
        {
            this->_terrorRadius = 0.0f;
            return;
        }

        // Get additive modifier for terror radius (Enum modifier 0x1C / 28)
        float additiveModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_IncreaseTerrorRadiusAdditive, EGameplayModifierSource::VE_All);
        float modifiedBaseRadius = this->_baseTerrorRadius + additiveModifier;

        // Get multiplicative modifier for terror radius (Enum modifier 0x1D / 29)
        // Add 1.0f because multiplicative modifiers from perks are typically delta multipliers (e.g., +0.2 for 20% increase)
        float multiplicativeModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_IncreaseTerrorRadiusMultiplicative, EGameplayModifierSource::VE_All) + 1.0f;

        // Apply modifiers and the stealth ratio to determine the final terror radius
        this->_terrorRadius = (modifiedBaseRadius * multiplicativeModifier) * this->_slasherStealthRatio;
    }
    else
    {
        // If no perk manager exists, the radius is purely driven by base stats and stealth ratio
        this->_terrorRadius = this->_baseTerrorRadius * this->_slasherStealthRatio;
    }
}

// starts at line 623
void ASlasherPlayer::UpdateChaseSequence(float deltaTime)
{
    float maxChaseTime = 0.0f;
    TArray<ADBDPlayer*> toRemove;

    // Iterate through all active chase timers
    for (auto& it : this->_chaseTimers)
    {
        ADBDPlayer* camper = it.Key;
        FChaseTimer& chaseTimer = it.Value;

        // Update the chase timer
        chaseTimer.CooldownTimer.Update(deltaTime);
        chaseTimer.TotalChaseTime += deltaTime;

        // Verify if the camper is still valid for a chase
        bool isInvalidCamper = false;
        
        if (camper == nullptr)
        {
            isInvalidCamper = true;
        }
        else
        {
            // Check if the camper is valid, not pending kill, etc.
            if (camper->IsValidImpl() == false || camper->IsPendingKill() == true)
            {
                isInvalidCamper = true;
            }
        }

        bool endChase = false;
        
        if (isInvalidCamper)
        {
            endChase = true;
        }
        else
        {
            // If the chase timer has run out, evaluate distance and visibility
            if (chaseTimer.CooldownTimer.IsDone())
            {
                float distanceSquared = 0.0f;
                
                // Get Slasher position
                FVector slasherLocation = FVector::ZeroVector;
                if (this->RootComponent != nullptr)
                {
                    slasherLocation = this->RootComponent->GetComponentLocation();
                }
                
                // Get Camper position
                FVector camperLocation = FVector::ZeroVector;
                if (camper->RootComponent != nullptr)
                {
                    camperLocation = camper->RootComponent->GetComponentLocation();
                }

                distanceSquared = FVector::DistSquared(slasherLocation, camperLocation);

                // Assuming Cast is successful based on context
                ACamperPlayer* camperPlayer = Cast<ACamperPlayer>(camper);

                // Chase ends if too far, camper is successfully hidden, camper cannot be chased, or killer is cloaked
                if (distanceSquared >= (this->_chaseMaxDistance * this->_chaseMaxDistance) ||
                    (camperPlayer != nullptr && camperPlayer->IsSuccessfullyHidden()) ||
                    (camperPlayer != nullptr && camperPlayer->CanBeChased() == false) ||
                    this->IsCloaked())
                {
                    endChase = true;
                }
            }
        }

        if (endChase)
        {
            // Schedule the camper to be removed from the active chases map
            toRemove.Add(camper);
            
            if (camper != nullptr)
            {
                ACamperPlayer* camperPlayer = Cast<ACamperPlayer>(camper);
                this->TriggerChaseEnd(camperPlayer, chaseTimer.TotalChaseTime);
                
                // Track the maximum chase time of ended chases
                maxChaseTime = FMath::Max(chaseTimer.TotalChaseTime, maxChaseTime);

                // If we are the server, process escape score events
                if (this->Role == ROLE_Authority)
                {
                    if (camperPlayer != nullptr && camperPlayer->IsValidImpl() && !camperPlayer->IsPendingKill())
                    {
                        // Player is not in a carried/hooked state
                        if (!camperPlayer->IsCrawling() && !camperPlayer->IsHooked() && !camperPlayer->IsGuidedBySlasher())
                        {
                            ADBDPlayerState* playerState = Cast<ADBDPlayerState>(camperPlayer->PlayerState);
                            if (playerState != nullptr && playerState->IsValidImpl() && !playerState->IsPendingKill())
                            {
                                // Fire the "Chase Escape" score event for the camper
                                // Assuming 0x3E (62) corresponds to EDBDScoreTypes::DBDCamperScore_ChaseEscape
                                playerState->FireScoreEvent(EDBDScoreTypes::DBDCamperScore_ChaseEscape, 1.0f, true);
                            }
                        }
                    }
                }
            }
        }
    }

    // Remove the evaluated players from the active chase timers map
    for (ADBDPlayer* playerToRemove : toRemove)
    {
        this->_chaseTimers.Remove(playerToRemove);
    }

    // If no more chases are active, evaluate if a post-chase cooldown is required
    if (this->_chaseTimers.Num() == 0)
    {
        float longChaseDuration = UDBDUtilities::GetTunableValue(this, TunableValues::LONG_CHASE_SLASHER_DURATION);
        
        if (maxChaseTime > longChaseDuration)
        {
            this->_postChaseTimer.Reset(this->_postChaseTime);
        }
    }

    // Update the Wwise audio RTPC based on whether a chase is currently active
    if (this->IsLocallyObserved())
    {
        float isChasingRTPCValue = 0.0f;
        if (this->_chaseTimers.Num() > 0)
        {
            isChasingRTPCValue = 1.0f;
        }

        FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
        if (audioDevice != nullptr)
        {
            audioDevice->SetRTPCValue(TEXT("AudioRTPC_Chase_Killer"), isChasingRTPCValue, 0, nullptr);
        }
    }
}

// starts at line 697
void ASlasherPlayer::UpdateCloakStalkTimers(float deltaTime)
{
    TArray<ADBDPlayer*> toRemove;

    // Iterate through all active stalk timers
    for (auto& it : this->_stalkTimers)
    {
        ADBDPlayer* camper = it.Key;
        FChaseTimer& stalkTimer = it.Value;

        // Update the stalk timer
        stalkTimer.CooldownTimer.Update(deltaTime);
        stalkTimer.TotalChaseTime += deltaTime;

        bool isInvalidCamper = false;

        // Verify if the camper is still valid for stalking
        if (camper == nullptr)
        {
            isInvalidCamper = true;
        }
        else
        {
            // Check if the camper is valid, not pending kill, etc.
            if (camper->IsValidImpl() == false || camper->IsPendingKill() == true)
            {
                isInvalidCamper = true;
            }
        }

        bool endStalk = false;

        if (isInvalidCamper)
        {
            endStalk = true;
        }
        else
        {
            // If the stalk timer has run out, evaluate distance and visibility
            if (stalkTimer.CooldownTimer.IsDone())
            {
                float distanceSquared = 0.0f;

                // Get Slasher position
                FVector slasherLocation = FVector::ZeroVector;
                if (this->RootComponent != nullptr)
                {
                    slasherLocation = this->RootComponent->GetComponentLocation();
                }

                // Get Camper position
                FVector camperLocation = FVector::ZeroVector;
                if (camper->RootComponent != nullptr)
                {
                    camperLocation = camper->RootComponent->GetComponentLocation();
                }

                distanceSquared = FVector::DistSquared(slasherLocation, camperLocation);

                // Assuming Cast is successful based on context
                ACamperPlayer* camperPlayer = Cast<ACamperPlayer>(camper);

                // Stalk ends if too far, camper is successfully hidden, camper cannot be chased, or killer is NOT cloaked
                if (distanceSquared >= (this->_chaseMaxDistance * this->_chaseMaxDistance) ||
                    (camperPlayer != nullptr && camperPlayer->IsSuccessfullyHidden()) ||
                    (camperPlayer != nullptr && camperPlayer->CanBeChased() == false) ||
                    this->IsCloaked() == false)
                {
                    endStalk = true;
                }
            }
        }

        if (endStalk)
        {
            // Schedule the camper to be removed from the active stalk map
            toRemove.Add(camper);

            // If we are the server, process the cloak stalk score event
            if (this->Role == ROLE_Authority)
            {
                // Fire the "Cloak Stalk Per Second" score event for the killer based on accumulated time
                // Assuming 0x64 (100) corresponds to EDBDScoreTypes::DBDSlasherScore_CloakStalkPerSecond
                this->TryFireScoreEvent(EDBDScoreTypes::DBDSlasherScore_CloakStalkPerSecond, stalkTimer.TotalChaseTime, true);
            }
        }
    }

    // Remove the evaluated players from the active stalk timers map
    for (ADBDPlayer* playerToRemove : toRemove)
    {
        this->_stalkTimers.Remove(playerToRemove);
    }
}

// starts at line 740
float ASlasherPlayer::GetLongestChaseDuration() const
{
    float maxDuration = -1.0f;

    // Iterate through all active chase timers in the map
    for (const auto& it : this->_chaseTimers)
    {
        const FChaseTimer& chaseTimer = it.Value;

        // Update the max duration if the current chase timer has a longer duration
        if (maxDuration < chaseTimer.TotalChaseTime)
        {
            maxDuration = chaseTimer.TotalChaseTime;
        }
    }

    return maxDuration;
}

// starts at line 751
void ASlasherPlayer::Multicast_OnChaseStart_Implementation(ACamperPlayer* camper)
{
    // Broadcast the event to any bound listeners
    if (this->OnChaseStarted.IsBound())
    {
        this->OnChaseStarted.Broadcast(camper);
    }

    // Check if the camper is the obsession and the slasher is the locally observed player
    if (camper != nullptr && camper->IsObsessionTarget() && this->IsLocallyObserved())
    {
        // Get the audio device to trigger the specific obsession chase sound
        FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
        if (audioDevice != nullptr)
        {
            // Assuming default PostEvent parameters based on disassembly pattern
            audioDevice->PostEvent(TEXT("AudioEvent_Obsession_Start_Chase"), nullptr, 0, nullptr, nullptr, false);
        }
    }

    // If the slasher is the locally observed player, update the chase audio RTPC
    if (this->IsLocallyObserved())
    {
        FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
        if (audioDevice != nullptr)
        {
            // Set the audio parameter indicating the killer is currently in a chase
            audioDevice->SetRTPCValue(TEXT("AudioRTPC_Chase_Killer"), 1.0f, 0, nullptr);
        }
    }
}

// starts at line 769
void ASlasherPlayer::TriggerChaseStart(ACamperPlayer* camper)
{
    // Check if the camper is already being chased by looking up their ID in the chase timers map
    if (camper != nullptr && this->_chaseTimers.Contains(camper) == false)
    {
        // Verify camper validity (not pending kill and internally valid)
        if (camper->IsPendingKill() == false && camper->IsValidImpl() == true)
        {
            // Ensure the camper is in a state where a chase can actually start
            if (camper->IsDead() == false && camper->IsCrawling() == false && camper->IsImmobilized() == false)
            {
                // Stop the post-chase cooldown timer as a new chase has begun
                UDBDUtilTimer::Stop(this->_postChaseTimer);

                // Notify the camper and all clients that a chase has started
                camper->TriggerChaseStart();
                this->Multicast_OnChaseStart(camper);

                // If we are the server (ROLE_Authority == 3), process score events
                if (this->Role == ROLE_Authority)
                {
                    AController* slasherController = this->Controller;
                    if (slasherController != nullptr && slasherController->IsPendingKill() == false)
                    {
                        ADBDPlayerState* slasherPlayerState = Cast<ADBDPlayerState>(slasherController->PlayerState);
                        if (slasherPlayerState != nullptr && slasherPlayerState->IsPendingKill() == false)
                        {
                            // Fire the base "Chase Start" score event
                            // Assuming 0x62 (98) is EDBDScoreTypes::DBDSlasherScore_ChaseStart
                            slasherPlayerState->FireScoreEvent(EDBDScoreTypes::DBDSlasherScore_ChaseStart, 1.0f, true);

                            // If the slasher is currently chain-blinking (Nurse), fire an additional score event
                            if (this->_isChainBlinking == true)
                            {
                                // Assuming 0x6F (111) is EDBDScoreTypes::DBDSlasherScore_ChainBlinkStartChase
                                this->TryFireScoreEvent(EDBDScoreTypes::DBDSlasherScore_ChainBlinkStartChase, 1.0f, true);
                            }
                        }
                    }
                }
            }
        }
    }

    // Re-check the map to either find the existing timer or create a new entry
    FChaseTimer* chaseTimer = this->_chaseTimers.Find(camper);

    if (chaseTimer == nullptr && camper != nullptr)
    {
        // Add a new entry for this camper into the chase timers map
        chaseTimer = &this->_chaseTimers.Add(camper, FChaseTimer());
    }

    if (chaseTimer != nullptr)
    {
        // Explicitly set the tracked camper and reset the sight loss timer
        chaseTimer->Camper = camper;
        chaseTimer->CooldownTimer.Reset(this->_chaseSightCooldown);
    }
}

// starts at line 802
void ASlasherPlayer::TriggerChaseEnd(ACamperPlayer* camper, float chaseTime)
{
    if (camper == nullptr)
    {
        this->Multicast_OnChaseEnd(camper, chaseTime);
        return;
    }

    // Verify the camper is valid
    if (camper->IsPendingKill() == false && camper->IsValidImpl() == true)
    {
        // If we are the server, process score events
        if (this->Role == ROLE_Authority)
        {
            if (this->Controller != nullptr && this->Controller->IsPendingKill() == false)
            {
                ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->Controller->PlayerState);
                if (playerState != nullptr && playerState->IsPendingKill() == false)
                {
                    // Fire the "Chase Per Second" score event based on the total chase time
                    // Assuming 0x3D (61) corresponds to EDBDScoreTypes::DBDSlasherScore_ChasePerSecond
                    playerState->FireScoreEvent(EDBDScoreTypes::DBDCamperScore_ChasePerSecond, chaseTime, true);
                }
            }

            if (camper->Controller != nullptr && camper->Controller->IsPendingKill() == false)
            {
                ADBDPlayerState* camperPlayerState = Cast<ADBDPlayerState>(camper->Controller->PlayerState);
                if (camperPlayerState != nullptr && camperPlayerState->IsPendingKill() == false)
                {
                    // Fire the camper's "Chase Per Second" score event
                    // Assuming 0x3F (63) corresponds to EDBDScoreTypes::DBDCamperScore_ChasePerSecond
                    camperPlayerState->FireScoreEvent(EDBDScoreTypes::DBDCamperScore_ChaseSteal, chaseTime, true);
                }
            }

            // Remove the camper from the active chase timers map
            this->_chaseTimers.Remove(camper);
        }

        // Notify the camper that their chase has ended
        camper->TriggerChaseEnd();

        // Record chase analytics if locally controlled
        UGameInstance* gameInstance = this->GetGameInstance();
        if (gameInstance != nullptr)
        {
            UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(gameInstance);
            if (dbdGameInstance != nullptr && this->IsLocallyControlled() == true)
            {
                // Determine if the chase ended successfully (e.g., camper was downed/caught)
                bool chaseSuccess = false;
                if (camper->IsCrawling() == true || camper->IsHooked() == true || camper->IsGuidedBySlasher() == true)
                {
                    chaseSuccess = true;
                }

                // Determine the correct rage percent to use based on authority
                float currentRagePercent = this->_ragePecentSmooth;
                if (this->Role == ROLE_Authority)
                {
                    currentRagePercent = this->_ragePercent;
                }

                int32_t rageTier = this->GetRageTierFromRagePercent(currentRagePercent);

                // Record the chase outcome in the analytics controller
                // Assuming KillerAnalyticsController is accessible via UDBDGameInstance (offset 0x7C8 based on assembly)
                dbdGameInstance->_analyticsManager.GameplayKillerAnalytics.RecordChase(rageTier, chaseSuccess);
            }
        }
    }

    // Broadcast the end of the chase to all clients
    this->Multicast_OnChaseEnd(camper, chaseTime);
}

// starts at line 859
void ASlasherPlayer::Multicast_OnChaseEnd_Implementation(ACamperPlayer* camper, float chaseTime)
{
    // Broadcast the OnChaseEnded event to all bound listeners with the camper and chase duration
    this->OnChaseEnded.Broadcast(camper, chaseTime);
}

// starts at line 864
bool ASlasherPlayer::IsInChase() const
{
    // Check if the number of active elements in the chase timers map is greater than zero
    // In Unreal Engine, TMap::Num() internally calculates (ArrayNum - NumFreeIndices)
    return this->_chaseTimers.Num() > 0;
}

// starts at line 869
bool ASlasherPlayer::CanChase()
{
  return this->IsCloaked() == false && this->_slasherStealthRatio > 0.0 && this->IsInStalkMode() == false;
}

// starts at line 880
void ASlasherPlayer::OnChaseTargetFound(ACamperPlayer* pawn)
{
    if (pawn == nullptr)
    {
        return;
    }

    // Verify the camper is a valid object and hasn't been queued for destruction
    if (pawn->IsPendingKill() == false && pawn->IsValidImpl() == true)
    {
        // A chase can only start if the camper can be chased and they are running
        if (pawn->CanBeChased() == true && pawn->IsRunning() == true)
        {
            // Calculate the distance between the Slasher and the Camper
            float distance = 0.0f;
            
            FVector slasherLocation = FVector::ZeroVector;
            if (this->RootComponent != nullptr)
            {
                slasherLocation = this->RootComponent->GetComponentLocation();
            }
            
            FVector camperLocation = FVector::ZeroVector;
            if (pawn->RootComponent != nullptr)
            {
                camperLocation = pawn->RootComponent->GetComponentLocation();
            }

            distance = FVector::Dist(slasherLocation, camperLocation);

            // Verify the camper is within the maximum allowed distance to start a chase
            if (distance <= this->_chaseStartMaxDistance)
            {
                // Verify the Slasher is currently in a state where they are allowed to initiate a chase
                if (this->CanChase() == true)
                {
                    this->TriggerChaseStart(pawn);
                }
                else if (this->IsCloaked() == true)
                {
                    // If the Slasher is cloaked, they cannot trigger a standard chase, but they can "stalk"
                    
                    // Look for the camper in the stalk timers map
                    FChaseTimer* stalkTimer = this->_stalkTimers.Find(pawn);

                    if (stalkTimer == nullptr)
                    {
                        // Add a new entry for this camper into the stalk timers map if they aren't already tracked
                        stalkTimer = &this->_stalkTimers.Add(pawn, FChaseTimer());
                    }
                    
                    // Update the tracked camper and reset the stalk timer using the chase sight cooldown
                    stalkTimer->Camper = pawn;
                    stalkTimer->CooldownTimer.Reset(this->_chaseSightCooldown);
                }
            }
        }
    }
}

// starts at line 903
void ASlasherPlayer::OnSoundTriggered(AActor* originator, FVector location, bool shouldTrack, float audibleRange)
{
    // Check if the sound should be tracked and if the originator is within audible range
    if (shouldTrack == true && this->GetDistanceTo(originator) < audibleRange)
    {
        // Check if the loud noise indicator component is valid and active
        if (this->_loudNoiseIndicator != nullptr && this->_loudNoiseIndicator->IsValidLowLevel())
        {
            // Add the tracked noise to the HUD indicator using the configured display time
            this->_loudNoiseIndicator->AddTrackedNoise(location, this->LoudNoiseDisplayTime);
        }
    }
}

// starts at line 915
void ASlasherPlayer::OnLoudNoiseIndicatorDestroyed()
{
    // Retrieve the base game instance
    UGameInstance* gameInstance = this->GetGameInstance();

    // Verify that the game instance is valid
    if (gameInstance != nullptr)
    {
        // Attempt to cast the game instance to the project-specific class
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(gameInstance);

        // Check if the cast was successful
        if (dbdGameInstance != nullptr)
        {
            // Unregister the sound event listener using the stored delegate handle
            dbdGameInstance->UnregisterSoundEventListener(this->_soundEventHandle);
        }
    }
}

// starts at line 924
void ASlasherPlayer::UpdateRTPCValues()
{
    // Call the base class UpdateRTPCValues
    Super::UpdateRTPCValues();

    // Check if the player is currently idle
    if (this->_idleStart.GetTicks() != 0)
    {
        // Check if one second has passed since the last update
        FTimespan oneSecond;
        oneSecond.Assign(0, 0, 0, 1, 0, 0); // (Days, Hours, Minutes, Seconds, Milliseconds, Microseconds)

        if (FDateTime::UtcNow().GetTicks() >= (this->_lastUpdate.GetTicks() + oneSecond.GetTicks()))
        {
            // Update the last update time
            this->_lastUpdate = FDateTime::UtcNow();

            // Calculate the duration the player has been idle in seconds
            double idleDurationSeconds = static_cast<double>(FDateTime::UtcNow().GetTicks() - this->_idleStart.GetTicks()) * 0.0000001;

            // Update Wwise RTPC for idle time
            FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
            if (audioDevice != nullptr)
            {
                // The result is stored as an AKRESULT (enum)
                AKRESULT akResult = audioDevice->SetRTPCValue(TEXT("AudioRTPC_Killer_Time_Stop_Moving"), static_cast<float>(idleDurationSeconds), 0, nullptr);

                // Log a warning if the RTPC update failed (AK_Success is typically 1)
                if (akResult != AK_Success)
                {
                    UE_LOG(LogDBDGeneral, Warning, TEXT("ASlasherPlayer::UpdateRTPCValues: SetRTPCValue on \"AudioRTPC_Killer_Time_Stop_Moving\" failed with error code %d"), static_cast<int32>(akResult));
                }
            }
        }
    }

    // Retrieve the Perk Manager via the owner interface (vtable offset 0x7A8)
    UPerkManager* perkManager = this->GetPerkManager();
    
    if (perkManager != nullptr)
    {
        // Get the multiplicative modifier for the Chainsaw Range (Assuming 0x59 / 89 is the GameplayModifierType)
        float chainsawRangeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyChainsawSoundRange);
        
        FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
        if (audioDevice != nullptr)
        {
            audioDevice->SetRTPCValue(TEXT("AudioRTPC_Addons_Chainsaw_Range"), chainsawRangeModifier * 100.0f, 0, nullptr);
        }

        // Get the multiplicative modifier for the Invisibility Bell Range (Assuming 0x3E / 62 is the GameplayModifierType)
        float bellRangeModifier = perkManager->GetMultiplicativeModifierValue(EGameplayModifierType::VE_ModifyInvisibilityBellSoundRange);
        
        if (audioDevice != nullptr)
        {
            audioDevice->SetRTPCValue(TEXT("AudioRTPC_Addons_InvisibilityBell_Range"), bellRangeModifier * 100.0f, 0, nullptr);
        }
    }
}

// starts at line 946
void ASlasherPlayer::UpdateKillerAnalytics(float deltaSeconds)
{
    // Retrieve the base game instance
    UGameInstance* gameInstance = this->GetGameInstance();
    if (gameInstance == nullptr)
    {
        return;
    }

    // Cast the game instance to the project-specific instance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(gameInstance);
    if (dbdGameInstance == nullptr)
    {
        return;
    }

    // Analytics are only tracked for locally controlled players
    if (this->IsLocallyControlled() == true)
    {
        // Calculate the magnitude of the player's velocity
        FVector velocity = this->GetVelocity();
        float speed = FMath::Sqrt((velocity.X * velocity.X) + (velocity.Y * velocity.Y) + (velocity.Z * velocity.Z));

        // Scale the speed (multiply by 0.01)
        float scaledSpeed = speed * 0.01f;

        // Record the scaled speed and time delta in the analytics controller
        // Assuming KillerAnalyticsController is an accessible object within UDBDGameInstance
        dbdGameInstance->_analyticsManager.GameplayKillerAnalytics.RecordSpeed(scaledSpeed, deltaSeconds);
    }
}

// starts at line 955
void ASlasherPlayer::PostNetReceiveLocationAndRotation()
{
    // Call the base class implementation to handle standard location and rotation updates from the network
    Super::PostNetReceiveLocationAndRotation();

    // Check if the Slasher is currently carrying a camper
    if (this->IsCarrying() == true)
    {
        // Ensure the pointer to the guided camper is valid
        if (this->_guidedCamper != nullptr)
        {
            // Check if the camper's movement is currently being guided by the Slasher
            if (this->_guidedCamper->IsMovementGuided() == true)
            {
                // Update the camper's movement to match the Slasher while being carried
                this->_guidedCamper->BeingCarriedMovement();
            }
        }
    }
}

// starts at line 964
void ASlasherPlayer::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
    // Call the base class implementation first to ensure foundational input is set up
    Super::SetupPlayerInputComponent(InputComponent);

    // Bind Slasher-specific interactions
    // Note: The specific macro/template used by UE4 for binding actions is simplified here for clarity

    // Basic Interactions
    InputComponent->BindAction(FName("Interact_Slasher"), IE_Pressed, this, &ADBDPlayer::InteractionInputPressed);
    InputComponent->BindAction(FName("Interact_Slasher"), IE_Released, this, &ADBDPlayer::InteractionInputReleased);

    // Attacking
    InputComponent->BindAction(FName("Attack_Slasher"), IE_Pressed, this, &ASlasherPlayer::AttackInputPressed);
    InputComponent->BindAction(FName("Attack_Slasher"), IE_Released, this, &ASlasherPlayer::AttackInputReleased);

    // Item Usage
    InputComponent->BindAction(FName("ItemUse_Slasher"), IE_Pressed, this, &ADBDPlayer::ItemUsePressed);
    InputComponent->BindAction(FName("ItemUse_Slasher"), IE_Released, this, &ADBDPlayer::ItemUseReleased);

    // Item Dropping
    InputComponent->BindAction(FName("ItemDrop_Slasher"), IE_Pressed, this, &ADBDPlayer::ItemDropPressed);
    InputComponent->BindAction(FName("ItemDrop_Slasher"), IE_Released, this, &ADBDPlayer::ItemDropReleased);

    // Secondary Action (Power)
    // The specific virtual function invoked here depends on the vtable layout, but typically maps to a secondary action trigger
    InputComponent->BindAction(FName("SecondaryAction_Slasher"), IE_Pressed, this, &ADBDPlayer::SecondaryActionInputPressed);
}

// starts at line 984
void ASlasherPlayer::SetInteractingCamperBlueprint(ACamperPlayer* camper)
{
    this->SetGuidedCamper(camper);
}

// starts at line 989
void ASlasherPlayer::SetGuidedCamper(ACamperPlayer* camper)
{
    // Step 1: Handle setting up ignore actors when picking up a camper
    if (camper != nullptr)
    {
        // Check if the provided camper is valid and not pending kill
        if (camper->IsPendingKill() == false && camper->IsValidImpl() == true)
        {
            // Note: The original logic skips to the audio setup here, but intuitively
            // one would expect the killer to store their weapon when picking someone up.
            // The assembly branches to the "equip item" section if 'camper' is null,
            // but also seems to do it if the previously guided camper was valid.
            // Let's adhere to the structure shown in the decompiler which first checks if the NEW camper is valid.

            this->StoreItem();
            this->MoveIgnoreActorAdd(camper);
            camper->MoveIgnoreActorAdd(this);
        }
    }
    else
    {
        // Step 2: Handle cleanup if the Slasher is dropping/hooking a camper
        if (this->_guidedCamper != nullptr)
        {
            // Check if the previously guided camper is still valid
            if (this->_guidedCamper->IsPendingKill() == false && this->_guidedCamper->IsValidImpl() == true)
            {
                // Re-equip the killer's weapon/power
                this->EquipItem();
                
                // Remove the ignore actor rules so they collide again
                this->MoveIgnoreActorRemove(this->_guidedCamper);
                this->_guidedCamper->MoveIgnoreActorRemove(this);
            }
        }
    }

    // Step 3: Update Audio States
    bool isValidCamper = false;
    if (camper != nullptr && camper->IsPendingKill() == false && camper->IsValidImpl() == true)
    {
        isValidCamper = true;
    }

    // Determine the string state for Wwise
    const TCHAR* switchState = isValidCamper ? TEXT("On") : TEXT("Off");
    const TCHAR* switchGroup = TEXT("AudioSwitchStatesKillerCarrying");

    // If the killer has a dedicated audio component, set the switch there
    if (this->_audioComponent.IsValid() == true)
    {
        UAkComponent* akComp = Cast<UAkComponent>(this->_audioComponent.Get());
        if (akComp != nullptr)
        {
            akComp->SetSwitch(switchGroup, switchState);
        }
    }
    else
    {
        // Fallback: set the switch globally on the actor via the Audio Device
        FAkAudioDevice* audioDevice = FAkAudioDevice::Get();
        if (audioDevice != nullptr)
        {
            audioDevice->SetSwitch(switchGroup, switchState, this);
        }
    }

    // Step 4: Actually assign the pointer
    this->_guidedCamper = camper;
}

// starts at line 1016
void ASlasherPlayer::Multicast_CamperHitAesthetic_Implementation(ACamperPlayer* camper, bool causedKO)
{
    if (camper == nullptr)
    {
        return;
    }

    // Verify camper validity
    if (camper->IsPendingKill() == false && camper->IsValidImpl() == true)
    {
        // 1. Determine relative hit direction to play the correct flinch animation
        FVector camperLocation = FVector::ZeroVector;
        if (camper->RootComponent != nullptr)
        {
            camperLocation = camper->RootComponent->GetComponentLocation();
        }

        FVector slasherLocation = FVector::ZeroVector;
        if (this->RootComponent != nullptr)
        {
            slasherLocation = this->RootComponent->GetComponentLocation();
        }

        // Vector from Slasher to Camper
        FVector toCamper = camperLocation - slasherLocation;
        toCamper.Z = 0.0f; // Ignore height difference for directional calculations
        
        float toCamperLengthSquared = toCamper.SizeSquared();
        if (toCamperLengthSquared > KINDA_SMALL_NUMBER)
        {
            toCamper /= FMath::Sqrt(toCamperLengthSquared); // Normalize
        }

        // Get the camper's forward direction
        FVector camperForward = camper->GetPlayerDirection().Vector();
        camperForward.Z = 0.0f;
        
        float camperForwardLengthSquared = camperForward.SizeSquared();
        if (camperForwardLengthSquared > KINDA_SMALL_NUMBER)
        {
            camperForward /= FMath::Sqrt(camperForwardLengthSquared); // Normalize
        }

        // Calculate dot products to determine hit sector (Front, Back, Left, Right)
        float dotForward = FVector::DotProduct(camperForward, toCamper);
        
        // Rotate the camper forward vector by 90 degrees around Z axis to get the right vector
        FVector camperRight = camperForward.RotateAngleAxis(90.0f, FVector::UpVector);
        float dotRight = FVector::DotProduct(camperRight, toCamper);

        // If the camper is not immobilized or interacting, play a directional flinch montage
        if (camper->IsImmobilized() == false && camper->GetIsInteracting() == false)
        {
            FName montageToPlay = NAME_None;
            
            // Choose montage based on the largest dot product (closest cardinal direction)
            if (FMath::Abs(dotForward) <= FMath::Abs(dotRight))
            {
                if (dotRight >= 0.0f)
                {
                    montageToPlay = causedKO ? AnimMapping::MONTAGEID_HIT_INJURED_TO_CRAWL_RIGHT : AnimMapping::MONTAGEID_HIT_HEALTHY_TO_INJURED_RIGHT;
                }
                else
                {
                    montageToPlay = causedKO ? AnimMapping::MONTAGEID_HIT_INJURED_TO_CRAWL_LEFT : AnimMapping::MONTAGEID_HIT_HEALTHY_TO_INJURED_LEFT;
                }
            }
            else if (dotForward <= 0.0f)
            {
                montageToPlay = causedKO ? AnimMapping::MONTAGEID_HIT_INJURED_TO_CRAWL_BACK : AnimMapping::MONTAGEID_HIT_HEALTHY_TO_INJURED_BACK;
            }
            else
            {
                montageToPlay = causedKO ? AnimMapping::MONTAGEID_HIT_INJURED_TO_CRAWL_FRONT : AnimMapping::MONTAGEID_HIT_HEALTHY_TO_INJURED_FRONT;
            }

            if (montageToPlay != NAME_None)
            {
                FAnimationMontageDescriptor descriptor;
                descriptor.MontageID = montageToPlay;
                // Assuming default play rate of 1.0f and bStopAllMontages false based on arguments (1.0, 0)
                camper->PlayMontage(descriptor, 1.0f, false);
            }

            // Force a movement component update on hit
            UDBDCharacterMovementComponent* moveComp = Cast<UDBDCharacterMovementComponent>(camper->GetMovementComponent());
            if (moveComp != nullptr && moveComp->IsPendingKill() == false)
            {
                moveComp->OnHit();
            }
        }

        // 2. Perform a collision sweep to find the exact hit location for blood/FX
        FCollisionShape collisionShape;
        collisionShape.SetBox(FVector(50.0f, 50.0f, 50.0f)); // HalfExtentX is set to 50.0

        FVector cameraPos = FVector::ZeroVector;
        if (this->Mesh != nullptr)
        {
            cameraPos = this->Mesh->GetBoneLocation(FName("joint_Cam_01"), EBoneSpaces::WorldSpace);
        }

        FRotator cameraRot = this->GetRepControlRotation();
        
        // Calculate sweep start and end points based on camera rotation
        FVector forwardVector = cameraRot.Vector();
        FVector rightVector = FRotationMatrix(cameraRot).GetScaledAxis(EAxis::Y);
        FVector upVector = FRotationMatrix(cameraRot).GetScaledAxis(EAxis::Z);

        // Sweep slightly in front, to the right, and up from the camera
        FVector collisionStart = cameraPos + (forwardVector * 100.0f) + (rightVector * 50.0f) + (upVector * 100.0f);
        // Sweep towards the camper's general area
        FVector collisionEnd = cameraPos + (forwardVector * 50.0f) + (rightVector * -50.0f) + (upVector * -50.0f);

        FCollisionQueryParams collisionParams;
        collisionParams.bFindInitialOverlaps = true;
        collisionParams.bTraceComplex = false; // implied by memory layout and 0 init
        collisionParams.AddIgnoredActor(this);
        
        // ECC_Pawn usually corresponds to 32 (0x20) in UE
        FCollisionObjectQueryParams collisionObjectTypes;
        collisionObjectTypes.AddObjectTypesToQuery(ECC_Pawn);

        TArray<FHitResult> outHits;
        FQuat rot = cameraRot.Quaternion();

        UWorld* world = this->GetWorld();
        if (world != nullptr)
        {
            world->SweepMultiByObjectType(outHits, collisionStart, collisionEnd, rot, collisionObjectTypes, collisionShape, collisionParams);
        }

        FHitResult* resolvedHit = nullptr;
        FHitResult defaultHit;

        // Try to find the specific camper in the sweep results
        bool foundCamper = false;
        for (FHitResult& hit : outHits)
        {
            if (hit.GetActor() == camper)
            {
                this->OnCamperHit(hit);
                resolvedHit = &hit;
                foundCamper = true;
                break;
            }
        }

        // If the sweep missed the camper, construct a synthetic hit result
        if (foundCamper == false)
        {
            defaultHit = FHitResult();
            defaultHit.Time = 1.0f;
            
            float halfHeight = 0.0f;
            if (camper->CapsuleComponent != nullptr)
            {
                FVector scale = camper->CapsuleComponent->GetComponentScale();
                float minScale = FMath::Min3(FMath::Abs(scale.X), FMath::Abs(scale.Y), FMath::Abs(scale.Z));
                halfHeight = minScale * camper->CapsuleComponent->GetUnscaledCapsuleHalfHeight();
            }

            FVector camperLoc = FVector::ZeroVector;
            if (this->RootComponent != nullptr) // Yes, the assembly uses `this->RootComponent` here for some reason to build the camper's synthetic hit point, likely an error in the original code or an offset relative to the killer.
            {
               camperLoc = this->RootComponent->GetComponentLocation();
            }

            defaultHit.ImpactPoint = camperLoc + (FVector::UpVector * halfHeight);
            defaultHit.ImpactNormal = FVector::UpVector;
            
            resolvedHit = &defaultHit;
        }

        // Process the aesthetic effects of the hit on the camper
        camper->ProcessHit(*resolvedHit, 0, causedKO);
    }
}

// starts at line 1138
void ASlasherPlayer::AttackInputPressed()
{
    // Ensure this input is only processed on the controlling client/server
    if (this->IsLocallyControlled() == false)
    {
        return;
    }

    bool skipAttackRequest = false;

    // First, try to interrupt (grab) a survivor if we are not already carrying someone
    if (this->IsCarrying() == false)
    {
        // Enable the specific detection zone for interruptions (Enum value 5)
        this->SetDetectionZoneEnabled(EDetectionZone::VE_Interruption, true);
        
        ADBDPlayer* target = this->FindClosestSlashablePlayerInDetectionZone(EDetectionZone::VE_Interruption);
        
        this->SetDetectionZoneEnabled(EDetectionZone::VE_Interruption, false);

        // Validate the potential interrupt target
        if (target != nullptr && target->IsPendingKill() == false && target->IsValidImpl() == true)
        {
            // Only try to interrupt if the target is currently performing an interaction
            if (target->GetIsInteracting() == true)
            {
                if (this->RequestInterruption(target) == true)
                {
                    // An interruption was successfully requested, skip the normal attack request
                    skipAttackRequest = true;
                }
            }
        }
    }

    if (skipAttackRequest == false)
    {
        EAttackType attackType;

        // Check for the blink attack debug cheat console variable
        static IConsoleVariable* CVarDebugBlinkAttack = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.DebugBlinkAttack"));
        bool isDebugBlink = (CVarDebugBlinkAttack != nullptr && CVarDebugBlinkAttack->GetInt() != 0);

        if (isDebugBlink == true || this->_isChainBlinking == true)
        {
            // Enum value 4 corresponds to a Blink Attack
            attackType = EAttackType::VE_Blink;
            this->Local_RequestAttack(attackType);
        }
        else
        {
            UPlayerInteractionHandler* interactionHandler = this->GetPlayerInteractionHandler();
            
            // Try to trigger an attack interaction (Enum value 2 usually corresponds to EInputInteractionType::Attack)
            if (interactionHandler == nullptr || interactionHandler->TryFirstAvailableInteractionOfType(EInputInteractionType::VE_AttackInteract, this) == false)
            {
                // If no interaction handled it, determine the raw attack type
                if (this->_lungeType == 1)
                {
                    if (this->IsBlinded() == true || this->IsCarrying() == true)
                    {
                        attackType = EAttackType::VE_Slash; // Quick/Basic Attack
                    }
                    else
                    {
                        attackType = EAttackType::VE_Lunge; // Lunge Attack
                    }
                }
                else
                {
                    if (this->IsBlinded() == true || this->IsCarrying() == true)
                    {
                        attackType = EAttackType::VE_Slash; // Quick/Basic Attack
                    }
                    else
                    {
                        attackType = EAttackType::VE_Pounce; // Pounce Attack
                    }
                }
                
                this->Local_RequestAttack(attackType);
            }
        }
    }

    // Cache the input state into the specific slasher player data object
    UDBDSlasherData* slasherData = Cast<UDBDSlasherData>(this->_playerData);
    if (slasherData != nullptr)
    {
        slasherData->_attackInputPressed = true;
    }

    // Send the input state to the server
    this->Server_SendAttackInput(true);
}

// starts at line 1186
void ASlasherPlayer::AttackInputReleased()
{
    this->_playerData->_attackInputPressed = 0;
    this->Server_SendAttackInput(0);
}

// starts at line 1192
void ASlasherPlayer::Server_SendAttackInput_Implementation(bool pressed)
{
    // Retrieve the player data and cast it to the Slasher-specific data class
    UDBDSlasherData* slasherData = Cast<UDBDSlasherData>(this->_playerData);

    // Verify the data pointer is valid before attempting to modify it
    if (slasherData != nullptr)
    {
        slasherData->_attackInputPressed = pressed;
    }
}

// starts at line 1202
void ASlasherPlayer::OnPickupStart(ADBDPlayer* target)
{
    // Call the base class implementation
    Super::OnPickupStart(target);

    // Attempt to cast the target to a camper player
    ACamperPlayer* camper = Cast<ACamperPlayer>(target);

    // Set the guided camper (SetGuidedCamper handles nullptrs properly based on its implementation)
    this->SetGuidedCamper(camper);

    // Call the Blueprint-implementable event
    this->OnPickupStart_BP();
}

// starts at line 1209
void ASlasherPlayer::OnPickupEnd(ADBDPlayer* target)
{
    Super::OnPickupEnd(target);
}

// starts at line 1220
bool ASlasherPlayer::IsCarrying() const
{
    // Check if the killer currently has a reference to a guided camper
    if (this->_guidedCamper != nullptr)
    {
        // Verify the camper object is still valid and not marked for destruction
        if (this->_guidedCamper->IsPendingKill() == false && this->_guidedCamper->IsValidImpl() == true)
        {
            return true;
        }
    }

    return false;
}

// starts at line 1225
bool ASlasherPlayer::IsHooking() const
{
    // The FString literal "Hang" represents the hooking interaction in the game
    FString hangInteraction = TEXT("Hang");
    
    // Check if the killer is currently performing the interaction
    return this->IsPerformingInteraction(hangInteraction);
}

// starts at line 1230
void ASlasherPlayer::SetGuidedAction(ESlasherGuidedAction action)
{
    // Retrieve the base player data and cast it to the specific slasher data class
    UDBDSlasherData* slasherData = Cast<UDBDSlasherData>(this->_playerData);

    // If the cast is successful and the data is valid, multicast the action change
    if (slasherData != nullptr && slasherData->IsValidLowLevel() == true)
    {
        this->Multicast_SetGuidedAction(action);
    }
}

// starts at line 1238
void ASlasherPlayer::Multicast_SetGuidedAction_Implementation(ESlasherGuidedAction action)
{
    // Retrieve the player data and cast it to the Slasher-specific data class
    UDBDSlasherData* slasherData = Cast<UDBDSlasherData>(this->_playerData);

    // Update the guided action state if the data object is valid
    if (slasherData != nullptr)
    {
        // Assuming the field is named GuidedAction based on the setter context (offset 0x10A)
        slasherData->CurrentGuidedAction = action;
    }

    // Retrieve the World to access the Game Mode
    UWorld* world = this->GetWorld();
    if (world != nullptr)
    {
        // Get the authoritative game mode and cast it to the project-specific class
        ADBDGameMode* dbdGameMode = Cast<ADBDGameMode>(world->GetAuthGameMode());
        
        // Notify the Game Mode that a guided action was modified
        if (dbdGameMode != nullptr)
        {
            dbdGameMode->GuidedActionModified();
        }
    }
}

// starts at line 1253
ESlasherGuidedAction ASlasherPlayer::GetGuidedAction() const
{
    // Attempt to cast the generic player data to the Slasher-specific data class
    UDBDSlasherData* slasherData = Cast<UDBDSlasherData>(this->_playerData);

    // If the cast is successful and the data is valid, return the current guided action
    if (slasherData != nullptr)
    {
        // Assuming the field is named GuidedAction based on offset 0x10A in previous context
        return slasherData->CurrentGuidedAction;
    }

    // Return a default value (VE_None) if the data is unavailable or the cast fails
    return ESlasherGuidedAction::VE_None;
}

// starts at line 1262
FRotator ASlasherPlayer::GetLookRotation() const
{
    // Retrieve and return the replicated control rotation from the base ADBDPlayer class
    return this->GetRepControlRotation();
}

// starts at line 1267
bool ASlasherPlayer::IsAllowedToKill(ACamperPlayer* camper) const
{
    // Check if the killer has an ability or offering that allows killing after a hook (e.g., Cypress Mori, Rancor)
    if (this->_allowedKillAfterHookingCount > 0 && camper != nullptr)
    {
        // Verify the camper object is valid and not pending destruction
        if (camper->IsPendingKill() == false && camper->IsValidImpl() == true)
        {
            // The camper must have been hooked at least once to be killed under this condition
            if (camper->GetHookedCount() > 0)
            {
                return true;
            }
        }
    }

    // Fallback: Check if the killer has a general kill allowance (e.g., Ebony/Ivory Mori, Shape Tier 3 with Tombstone)
    return this->_allowedKillCount > 0;
}

// starts at line 1291
ACamperPlayer* ASlasherPlayer::GetInteractingCamper() const
{
    return this->_guidedCamper;
}

// starts at line 1301
void ASlasherPlayer::SetPlayerDirection(FRotator rotation)
{
    // Determine if we should update the control rotation
    // We only update the control rotation if we are locally controlled or the server (authority)
    if (this->IsLocallyControlled() == true || this->Role == ROLE_Authority)
    {
        // Get the controller attached to this player
        AController* controller = this->Controller;
        if (controller != nullptr)
        {
            // Verify the controller is valid and not pending kill
            if (controller->IsPendingKill() == false && controller->IsValidImpl() == true)
            {
                // Update the controller's rotation
                controller->SetControlRotation(rotation);
            }
        }
    }

    // Set the actual actor's rotation
    // Note: The disassembly explicitly zeroes out Pitch and Roll, only applying Yaw.
    // XMM1 contains rotation.Yaw (offset 4 in FRotator). XMM0 is zeroed.
    // The unpack low places (0.0, Yaw) into a 64-bit value, which maps to (Pitch, Yaw) in FRotator.
    FRotator newRotation;
    newRotation.Pitch = 0.0f;
    newRotation.Yaw = rotation.Yaw;
    newRotation.Roll = 0.0f;

    // Apply the new rotation to the actor (Teleport type is None/0)
    this->SetActorRotation(newRotation, ETeleportType::None);
}

// starts at line 1314
void ASlasherPlayer::TurnAtRate(float Val)
{
    if (this->AllowTurningInput)
    {
        Super::TurnAtRate(Val);
    }
}

// starts at line 1323
void ASlasherPlayer::LookUpAtRate(float Val)
{
    if (this->AllowPitchInput)
    {
        Super::LookUpAtRate(Val);
    }
}

// starts at line 1332
void ASlasherPlayer::Turn(float Val)
{
    if (this->AllowTurningInput)
    {
        Super::Turn(Val);
    }
}

// starts at line 1341
void ASlasherPlayer::LookUp(float Val)
{
    if (this->AllowPitchInput)
    {
        Super::LookUp(Val);
    }
}

// starts at line 1350
void ASlasherPlayer::SetMontageMode(bool montageMode)
{
    // Call the base class implementation first
    Super::SetMontageMode(montageMode);

    // Retrieve the controller
    AController* controller = this->Controller;
    if (controller != nullptr)
    {
        // Ensure the controller is an ADBDPlayerControllerBase
        ADBDPlayerControllerBase* dbdController = Cast<ADBDPlayerControllerBase>(controller);
        
        // Ensure the controller is valid and not pending kill
        if (dbdController != nullptr && dbdController->IsPendingKill() == false && dbdController->IsValidImpl() == true)
        {
            // Only update input ignoring if we are the server (authority) or the locally controlled player
            if (this->Role == ROLE_Authority || this->IsLocallyControlled() == true)
            {
                // In montage mode, we ignore look input. Otherwise, we restore it.
                dbdController->SetIgnoreLookInput(montageMode);

                // Update the bUseControllerRotationYaw flag on the APawn
                // The bitwise operation unsets the flag (bit 1, value 2), then sets it if we are NOT in montage mode.
                this->bUseControllerRotationYaw = !montageMode;
            }
        }
    }
}

// starts at line 1365
void ASlasherPlayer::UpdateMontageMode()
{
    // Retrieve the controller
    AController* controller = this->Controller;
    if (controller != nullptr)
    {
        // Cast to APlayerController to ensure it's a player and not an AI (or at least derived from PlayerController)
        APlayerController* playerController = Cast<APlayerController>(controller);
        
        // Verify the controller is valid and not pending kill
        if (playerController != nullptr && playerController->IsPendingKill() == false && playerController->IsValidImpl() == true)
        {
            // Get the current control rotation
            FRotator rotation = playerController->GetControlRotation();

            // Match the control rotation Yaw to the actor's current Yaw
            if (this->RootComponent != nullptr)
            {
                rotation.Yaw = this->RootComponent->GetComponentRotation().Yaw;
            }
            else
            {
                rotation.Yaw = 0.0f;
            }

            // Match the control rotation Pitch to the camera's current Pitch
            UCameraComponent* playerCamera = this->GetPlayerCamera();
            if (playerCamera != nullptr && playerCamera->IsPendingKill() == false && playerCamera->IsValidImpl() == true)
            {
                rotation.Pitch = playerCamera->GetComponentRotation().Pitch;
            }

            // Apply the updated rotation back to the controller
            playerController->SetControlRotation(rotation);
        }
    }
}

// starts at line 1383
void ASlasherPlayer::Local_RequestAttack(EAttackType attackType)
{
    // The request is only valid if processed on the locally controlled instance
    // and if the player's current state allows for an attack (not stunned, carrying, etc.)
    if (this->IsLocallyControlled() && this->CanAttack())
    {
        // This function interacts with a PollableEvent buffer (likely part of a custom networking or input system).
        // It manually manages an event buffer (TArray) within the AttackEvent object.        
        PollableEvent<enum EAttackType>* attackEventObject = this->AttackEvent.Get();
        if (attackEventObject != nullptr)
        {
            // Add the attack type to the event buffer for polling in the next tick/update
            attackEventObject->_eventBuffer.Add(attackType);
        }
    }
}

// starts at line 1392
bool ASlasherPlayer::DetectObstruction(UPhysicalMaterial*& physicalMaterialOut, FVector& positionOut, FVector& normalOut, EDetectionZone detectionZoneID) const
{
    // CVar used for debugging obstruction sweeps
    static IConsoleVariable* CVarDebugSlashObstruction = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.DebugSlashObstruction"));
    UE_LOG(LogSlasher, Verbose, TEXT("DetectObstruction Start."));

    bool bHitObstruction = false;
    physicalMaterialOut = nullptr;

    // Retrieve the detection zone primitive component for the given ID
    UPrimitiveComponent* detectionZone = this->GetDetectionZone(detectionZoneID);

    if (detectionZone != nullptr && detectionZone->IsPendingKill() == false && detectionZone->IsValidImpl() == true)
    {
        // Set up the collision response parameters
        // We want to trace against the environment, so we create a custom response container
        FCollisionResponseContainer responseContainer;
        responseContainer.SetAllChannels(ECR_Ignore);
        // ECC_GameTraceChannel15 is typically a custom channel in DBD, likely for specific environmental blocking (e.g. walls/obstacles)
        responseContainer.SetResponse(ECC_GameTraceChannel15, ECR_Block);
        
        FCollisionResponseParams responseParams(responseContainer);

        // Configure standard query parameters
        FCollisionQueryParams queryParams;
        queryParams.bTraceComplex = false;
        queryParams.bReturnPhysicalMaterial = true; // Important: we need the physical material out
        queryParams.bReturnFaceIndex = true;
        queryParams.AddIgnoredActor(this);

        // Get the shape of the detection zone component
        FCollisionShape collisionShape;
        detectionZone->GetCollisionShape(collisionShape);

        // By default, assume it's a Box and grab its extents.
        // If it's a sphere/capsule, the internal logic attempts to map it, but we assume Box logic here based on the disassembly.
        FVector extents = FVector::ZeroVector;
        if (collisionShape.ShapeType == ECollisionShape::Box)
        {
            extents = collisionShape.GetBox();
        }
        else if (collisionShape.ShapeType == ECollisionShape::Sphere)
        {
            extents = FVector(collisionShape.GetSphereRadius());
        }
        else if (collisionShape.ShapeType == ECollisionShape::Capsule)
        {
            // Mapping capsule to box approximation for the sweep
            extents = FVector(collisionShape.GetCapsuleRadius(), collisionShape.GetCapsuleRadius(), collisionShape.GetCapsuleHalfHeight());
        }
        else
        {
            // Fallback for unknown shape types (handled loosely via struct offsets in assembly)
            extents = FVector(collisionShape.GetExtent());
        }

        // Calculate sweep parameters based on a spring component attached to the slasher
        // This suggests the detection zone is pushed forward or angled
        UPhysicsSpringComponent* springComp = Cast<UPhysicsSpringComponent>(detectionZone);
        FVector springDirection = FVector::ForwardVector; // Fallback
        if (springComp != nullptr)
        {
            springDirection = springComp->GetSpringDirection();
        }

        FVector translation = detectionZone->GetComponentLocation();
        
        // Calculate start and end points for the sweep. The logic in the ASM is a bit convoluted, 
        // essentially performing a box sweep backwards along the spring direction.
        FVector startOffset = springDirection * extents.X;
        FVector start = translation - startOffset;
        FVector end = translation + startOffset;

        FQuat rotation = detectionZone->GetComponentQuat();
        
        // Debug drawing
        if (CVarDebugSlashObstruction != nullptr && CVarDebugSlashObstruction->GetInt() != 0)
        {
            UWorld* world = this->GetWorld();
            if (world != nullptr)
            {
                // Draw start and end boxes
                DrawDebugBox(world, start, FVector(1.0f, extents.Y, extents.Z), rotation, FColor::Red, false, 60.0f, 0, 0.0f);
                DrawDebugBox(world, end, FVector(1.0f, extents.Y, extents.Z), rotation, FColor::Green, false, 60.0f, 0, 0.0f);
            }
        }

        // Perform the actual sweep
        FHitResult hitResult;
        // ECC_GameTraceChannel3 is another custom channel (e.g. Interaction, Obstruction)
        bool bHit = this->GetWorld()->SweepSingleByChannel(hitResult, start, end, rotation, ECC_GameTraceChannel3, collisionShape, queryParams, responseParams);

        if (bHit)
        {
            bHitObstruction = true;

            // Extract hit info
            positionOut = hitResult.ImpactPoint;
            normalOut = hitResult.ImpactNormal;

            UPrimitiveComponent* hitComponent = hitResult.GetComponent();

            // Debug drawing for the hit
            if (CVarDebugSlashObstruction != nullptr && CVarDebugSlashObstruction->GetInt() != 0 && hitComponent != nullptr)
            {
                UWorld* world = this->GetWorld();
                if (world != nullptr)
                {
                    DrawDebugSphere(world, hitResult.Location, 10.0f, 8, FColor::Green, false, 10.0f, 0, 0.0f);
                }
            }

            // Attempt to resolve the physical material
            if (hitComponent != nullptr && hitComponent->IsPendingKill() == false)
            {
                // If bReturnPhysicalMaterial is true, PhysMaterial is populated
                if (hitResult.PhysMaterial.IsValid())
                {
                    physicalMaterialOut = hitResult.PhysMaterial.Get();                    
                    UE_LOG(LogSlasher, Log, TEXT("PhysicalMaterial hit %s"), *physicalMaterialOut->GetName());
                }
                else
                {
                    UE_LOG(LogSlasher, Log, TEXT("No Valid PhysicalMaterial hit."));
                }
            }
        }
    }

    UE_LOG(LogSlasher, Verbose, TEXT("DetectObstruction End."));

    return bHitObstruction;
}

// starts at line 1464
void ASlasherPlayer::AuthorityFailInterruption(ADBDPlayer* interruptee)
{
    // Retrieve the PollableEvent buffer for attack events
    PollableEvent<enum EAttackType>* attackEventObject = this->AttackEvent.Get();
    
    // Ensure the event object is valid before attempting to modify its buffer
    if (attackEventObject != nullptr)
    {
        // When an interruption fails, force a basic/quick attack (Enum value 1) 
        // to be processed by adding it to the event buffer.
        attackEventObject->_eventBuffer.Add(EAttackType::VE_Slash);
    }
}

// starts at line 1470
void ASlasherPlayer::AuthorityFlashlighted(ADBDPlayer* blinder)
{
    // Call the base class implementation first
    // Note: The assembly calls a virtual function here which IDA misidentified as a struct serializer.
    // Given the inheritance hierarchy, it is almost certainly Super::AuthorityFlashlighted.
    Super::AuthorityFlashlighted(blinder);

    // Ensure this logic only runs on the server (Authority)
    if (this->Role != ROLE_Authority)
    {
        return;
    }

    // Virtual call at offset 0xEA8 is likely IsCloaked() for The Wraith
    bool bIsCloaked = this->IsCloaked();
    
    // Check if The Nurse is charging her blink
    bool bIsChargingBlink = this->IsChargingBlink(); 

    EDBDScoreTypes scoreType = EDBDScoreTypes::DBDScore_Count;

    // Handle Wraith "Lightburn"
    if (bIsCloaked == true)
    {
        // Trigger the lightburn interaction/stun for the Wraith (Enum value 10)
        this->Client_TryInteractionType(EInputInteractionType::VE_ExternalRequestFlashlightStunUncloak, blinder);
        scoreType = EDBDScoreTypes::DBDCamperScore_SlasherBurnInvisibility;
    }
    // Handle Nurse "Lightburn"
    else if (bIsChargingBlink == true)
    {
        // Trigger the lightburn interaction/stun for the Nurse (Enum value 12)
        this->Client_TryInteractionType(EInputInteractionType::VE_ExternalRequestStun, blinder);
        scoreType = EDBDScoreTypes::DBDCamperScore_SlasherBurnBlink;
    }

    // If a valid lightburn score event was determined, award it to all instigators
    if (scoreType != EDBDScoreTypes::DBDScore_Count)
    {
        // Check the chargeable component that handles the flashlight "burn" build-up
        if (this->_invisibilityBurnoutChargeableComponent != nullptr)
        {
            // Create a local copy of the instigators array to safely iterate over
            // (IDA misidentified this as an AAkReverbVolume array, but it contains ADBDPlayer pointers)
            TArray<ADBDPlayer*> instigatorsCopy = this->_invisibilityBurnoutChargeableComponent->_instigatorsSinceEmpty;

            for (ADBDPlayer* instigator : instigatorsCopy)
            {
                if (instigator != nullptr)
                {
                    // Verify the instigator is valid and not pending kill
                    if (instigator->IsPendingKill() == false && instigator->IsValidImpl() == true)
                    {
                        // Award the lightburn score event to the survivor who contributed to the burn
                        instigator->TryFireScoreEvent(scoreType, 1.0f, true);
                    }
                }
            }
        }
    }
}

// starts at line 1505
void ASlasherPlayer::SetInParadise(bool inParadise)
{
    this->SetInParadise(inParadise);
}

// starts at line 1510
bool ASlasherPlayer::CanSlash(const ADBDPlayer* character) const
{
    return character->CanBeSlashed();
}

// starts at line 1520
FTransform ASlasherPlayer::GetEyesTransform() const
{
    USkeletalMeshComponent* meshComponent = this->GetPawnMesh();
    
    if (meshComponent != nullptr)
    {
        // EYES_SOCKET is a static FName defined in the class (e.g., "cam_socket" or "eyes")
        return meshComponent->GetSocketTransform(ASlasherPlayer::EYES_SOCKET, ERelativeTransformSpace::RTS_World);
    }
    
    // Fallback just in case the mesh is somehow null
    return this->GetActorTransform(); 
}

// starts at line 1527
bool ASlasherPlayer::CanInterrupt(ADBDPlayer* interruptee) const
{
    // Safety check, though the optimized assembly assumes 'interruptee' is valid 
    // by the time it reaches the final flag check.
    if (interruptee == nullptr)
    {
        return false;
    }

    // The Slasher can only perform a grab/interrupt if ALL of the following are true:
    // 1. The killer generally has the ability to interrupt enabled right now.
    // 2. The killer is not currently cloaked (a Wraith-specific check pulled up into the base class).
    // 3. Any Blueprint-specific logic or perk conditions allow the interrupt.
    // 4. The target survivor is currently performing an action that makes them vulnerable to grabs (e.g., repairing, vaulting).
    return this->bAllowInterrupting && this->IsCloaked() == false && this->CanInterrupt_BP() && interruptee->bIsInterruptable;
}

// starts at line 1532
bool ASlasherPlayer::CanPerformInterruption(const ADBDPlayer* interruptee, const UInterruptionDefinition* interruption) const
{
    // Safety check to ensure the interruption definition is valid
    if (interruption == nullptr)
    {
        return false;
    }

    // The killer can perform the interruption if they are NOT currently carrying a survivor,
    // OR if this specific type of interruption explicitly allows it while carrying.
    return interruption->_canInterruptWhileCarrying || this->IsCarrying() == false;
}

// starts at line 1537
void ASlasherPlayer::OnSurvivorKilled(ACamperPlayer* playerToKill, bool consumeKill)
{
    // Step 1: Consume the appropriate kill allowance token if requested
    if (consumeKill == true)
    {
        // If we don't have a "kill after hooking" token, OR the survivor hasn't been hooked,
        // we must consume a general kill token (e.g., from an Ebony Mori).
        if (this->_allowedKillAfterHookingCount <= 0 || (playerToKill != nullptr && playerToKill->GetHookedCount() <= 0))
        {
            this->_allowedKillCount--;
        }
        else
        {
            // Otherwise, consume the specific "kill after hooking" token (e.g., Cypress Mori, Rancor).
            this->_allowedKillAfterHookingCount--;
        }

        // Update local game analytics for the client playing the killer
        UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
        if (gameInstance != nullptr && this->IsLocallyControlled() == true)
        {
            dbdGameInstance->_analyticsManager.GameplayKillerAnalytics.IncrementMurder();
        }
    }

    // Step 2: Handle server-side logic for distributing score and executing the kill
    if (this->HasAuthority() == true)
    {
        ADBDPlayerState* playerState = Cast<ADBDPlayerState>(this->PlayerState);
        if (playerState != nullptr && playerState->IsPendingKill() == false && playerState->IsValidImpl() == true)
        {
            UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());
            if (gameInstance != nullptr)
            {
                // Fire the global game event for the Slasher successfully killing a survivor
                UGameEventTracker* eventTracker = gameInstance->GetGameEventTracker();
                if (eventTracker != nullptr)
                {
                    eventTracker->FireGameEvent(EDBDScoreTypes::DBDSlasherScore_Kill, 1.0f, this, playerToKill);
                }
            }
        }

        // Process the death on the survivor's end
        if (playerToKill != nullptr && playerToKill->IsPendingKill() == false && playerToKill->IsValidImpl() == true)
        {
            // Award the survivor their "Die by Kill" score event
            playerToKill->TryFireScoreEvent(EDBDScoreTypes::DBDCamperScore_DieKill, 1.0f, true);

            // Execute the server kill. Passing 'false' indicates this is a Mori/Kill, 
            // rather than a Sacrifice (which would typically be 'true').
            playerToKill->Server_Kill(false);
        }
    }
}

// starts at line 1586
void ASlasherPlayer::RotateToTargetPercent(FVector targetPosition, float percent, float deltaTime)
{
    // Note: 'deltaTime' is passed into the function but never actually used in the assembly. 
    // It's likely that 'percent' was pre-calculated by the caller using deltaTime.

    // 1. Get the camera's exact world position from the skeletal mesh bone
    FVector cameraLocation = FVector::ZeroVector;
    if (this->Mesh != nullptr)
    {
        cameraLocation = this->Mesh->GetBoneLocation(FName("joint_Cam_01"), EBoneSpaces::WorldSpace);
    }

    // 2. Calculate the desired rotation to look exactly at the target
    FVector vectorToTarget = targetPosition - cameraLocation;
    FRotator targetRotation = vectorToTarget.ToOrientationRotator();

    // 3. Get the player's current control rotation
    if (this->Controller == nullptr)
    {
        return;
    }
    FRotator currentRotation = this->Controller->GetControlRotation();

    // 4. Calculate the shortest path delta between current and target rotations
    FRotator deltaRotation = targetRotation - currentRotation;
    
    // This normalizes the angles to be within -180 to 180 degrees.
    // (This single call represents the massive block of SIMD math in the disassembly)
    deltaRotation.Normalize(); 

    // 5. Apply the interpolation percentage
    float yawStep = deltaRotation.Yaw * percent;
    float pitchStep = deltaRotation.Pitch * percent;

    // 6. Inject the exact degree rotation into the controller
    ADBDPlayerController* dbdController = Cast<ADBDPlayerController>(this->Controller);
    if (dbdController != nullptr)
    {
        // UE4's AddControllerXXXInput functions automatically multiply your input by the 
        // controller's InputScale (e.g., mouse sensitivity). 
        // To rotate the camera by an EXACT number of degrees, we must divide by the scale 
        // to cancel out that automatic multiplication.

        if (dbdController->InputYawScale != 0.0f)
        {
            this->AddControllerYawInput(yawStep / dbdController->InputYawScale);
        }

        if (dbdController->InputPitchScale != 0.0f)
        {
            // Pitch is inverted before being applied (indicated by the XOR with 0x80000000 in the ASM)
            this->AddControllerPitchInput(-(pitchStep / dbdController->InputPitchScale));
        }
    }
}

// starts at line 1602
void ASlasherPlayer::Authority_ApplyDamageToTarget(ADBDPlayer* target, bool forceCauseKO)
{
    if (this->Role != ROLE_Authority)
    {
        return;
    }

    if (target != nullptr)
    {
        ACamperPlayer* camperTarget = Cast<ACamperPlayer>(target);
        
        if (camperTarget != nullptr && camperTarget->IsPendingKill() == false && camperTarget->IsValidImpl() == true)
        {
            bool causedKO = false;
            bool didDamage = false;

            bool bForceDyingState = forceCauseKO;

            if (bForceDyingState == false)
            {
                UPerkManager* perkManager = this->GetPerkManager();
                if (perkManager != nullptr && perkManager->IsPendingKill() == false)
                {
                    // Enum value 5 maps to EGameplayModifierFlag::VE_AttackToDyingState (e.g., Exposed status effect)
                    // Enum value 4 maps to EGameplayModifierSource::VE_All
                    if (perkManager->HasPerkFlag(EGameplayModifierFlag::VE_AttackToDyingState, EGameplayModifierSource::VE_All) == true)
                    {
                        bForceDyingState = true;
                    }
                }
            }

            if (bForceDyingState == true)
            {
                // Enum value 2 maps to ECamperDamageState::VE_Dead or Dying
                camperTarget->Authority_ApplyDamageToState(didDamage, causedKO, ECamperDamageState::VE_KO);
            }
            else
            {
                camperTarget->DBDApplyDamage(didDamage, causedKO);
            }

            if (camperTarget->Role == ROLE_Authority)
            {
                camperTarget->Authority_OnSlashed(this);
            }

            this->Multicast_CamperHitAesthetic(camperTarget, causedKO);
            this->TriggerChaseStart(camperTarget);
        }
    }
}

// starts at line 1630
bool ASlasherPlayer::CanAttack_Internal()
{
    // If the slasher is already attacking, another attack cannot be initiated
    if (this->IsAttacking == true)
    {
        return false;
    }

    // If the slasher is not carrying a camper, they are free to attack
    if (this->IsCarrying() == false)
    {
        return true;
    }

    // Check if the guided camper pointer is null
    if (this->_guidedCamper == nullptr)
    {
        return true;
    }

    // The disassembly contains an inline expansion of the standard Unreal Engine IsValid() global function.
    // It manually checks the GUObjectArray for the RF_Unreachable flag (0x20000000) 
    // and checks AActor::bActorIsBeingDestroyed (bitfield offset 0x140, bit 3).
    // Replaced the expanded assembly with the standard UE macro as requested.
    if (IsValid(this->_guidedCamper) == false)
    {
        return true;
    }

    // Check the game-specific validity implementation for the camper via VTable
    if (this->_guidedCamper->IsValidImpl() == false)
    {
        return true;
    }

    // Check if the camper can escape the carry state
    if (this->CanEscapeCarry(this->_guidedCamper) == false)
    {
        return true;
    }

    // If the slasher is carrying a valid camper and the camper can escape, attack is disabled
    return false;
}

// starts at line 1636
void ASlasherPlayer::DBD_SetAttackZones(EAttackZoneSet attackZoneSet)
{
    this->Multicast_SetAttackDetectionZoneSet(attackZoneSet);
}

// starts at line 1641
void ASlasherPlayer::Multicast_SetAttackDetectionZoneSet_Implementation(EAttackZoneSet attackZoneSet)
{
    // Forward the call to update the actual attack detection zone set for this instance
    this->SetAttackDetectionZoneSet(attackZoneSet);
}

// starts at line 1647
void ASlasherPlayer::DBD_DisplayAttackZones(bool display)
{
    this->Multicast_DisplayAttackZones(display);
}

// starts at line 1652
void ASlasherPlayer::Multicast_DisplayAttackZones_Implementation(bool display)
{
    // Forward the call to update the actual attack zone display state for this instance
    this->DisplayAttackZones(display);
}

// starts at line 1668
void ASlasherPlayer::AuthorityAttemptEscapeCarry(ACamperPlayer* survivor)
{
    // Check if the current instance has network authority
    if (this->Role == ROLE_Authority)
    {
        // Verify if the given survivor is currently able to escape the carry
        if (this->CanEscapeCarry(survivor) == true)
        {
            // Request a stun on the slasher to release the survivor
            // 4 represents a specific EStunType (likely corresponding to an escape stun)
            this->RequestStun(EStunType::VE_WiggleFree, survivor);
        }
    }
}

// starts at line 1676
bool ASlasherPlayer::CanEscapeCarry(ACamperPlayer* survivor)
{
    // Check if the provided survivor pointer is null
    if (survivor == nullptr)
    {
        return false;
    }

    // Use standard Unreal Engine macro to check if the survivor object is valid and not pending kill
    if (IsValid(survivor) == false)
    {
        return false;
    }

    // Call the game-specific validation implementation for the survivor
    if (survivor->IsValidImpl() == false)
    {
        return false;
    }

    // Check if the survivor is in a state where they are allowed to escape
    if (survivor->CanEscapeCarry() == false)
    {
        return false;
    }

    // Check if the slasher is currently in the carrying state
    if (this->IsCarrying() == false)
    {
        return false;
    }

    // If the slasher is currently performing an interaction, the survivor cannot escape
    if (this->IsInteracting == true)
    {
        return false;
    }

    // If the slasher is interacting with another valid player, block the escape
    if (this->_interactingPlayer != nullptr)
    {
        if (IsValid(this->_interactingPlayer) == true)
        {
            if (this->_interactingPlayer->IsValidImpl() == true)
            {
                return false;
            }
        }
    }

    // Verify that the slasher actually has a guided camper
    if (this->_guidedCamper == nullptr)
    {
        return false;
    }

    // Check if the guided camper object is valid
    if (IsValid(this->_guidedCamper) == false)
    {
        return false;
    }

    // Call the game-specific validation implementation for the guided camper
    if (this->_guidedCamper->IsValidImpl() == false)
    {
        return false;
    }

    // Ensure the survivor trying to escape is the same one being carried by the slasher
    if (this->_guidedCamper != survivor)
    {
        return false;
    }

    // All conditions met, the survivor can escape
    return true;
}

// starts at line 1687
float ASlasherPlayer::GetPercentMovementSpeed() const
{
    // Retrieve the maximum movement speed for the slasher
    float maxSpeed = this->GetMaximumMovementSpeed();

    // If the slasher is incapacitated, return a fixed half-speed percentage
    if (this->IsIncapacitated() == true)
    {
        return 0.5f;
    }

    // Prevent division by zero; ensure max speed is strictly positive
    if (maxSpeed > 0.0f)
    {
        // Retrieve the current movement speed vector
        FVector currentMovementSpeed = this->GetMovementSpeed();

        // The disassembly explicitly calculates the vector length (sqrt(X^2 + Y^2 + Z^2))
        // and multiplies it by the reciprocal of the max speed (1.0f / maxSpeed). 
        // Replaced the inline math with the standard Unreal Engine Size() function as requested.
        return currentMovementSpeed.Size() * (1.0f / maxSpeed);
    }

    // Return zero if max speed is less than or equal to zero
    return 0.0f;
}

// starts at line 1708
bool ASlasherPlayer::CanPerformKillerAbility_Implementation(EKillerAbilities killerAbility) const
{
    // Retrieve the total number of abilities available to this slasher
    int32 totalAbilities = this->KillerAbilities.Num();

    // Iterate through the array of killer abilities assigned to this slasher
    for (int32 index = 0; index < totalAbilities; index = index + 1)
    {
        // Check if the current ability in the array matches the requested ability
        if (this->KillerAbilities[index] == killerAbility)
        {
            // The ability was found in the array, so the slasher can perform it
            return true;
        }
    }

    // If the loop finishes without finding a match, the slasher cannot perform the ability
    return false;
}

// starts at line 1738
bool ASlasherPlayer::IsCloaked() const
{
    return this->_isCloaked;
}

// starts at line 1743
bool ASlasherPlayer::IsInStalkMode() const
{
    return this->_isInStalkMode;
}

// starts at line 1760
void ASlasherPlayer::SetIsCloaked(bool isCloaked, bool forced)
{
    // Update the internal cloaked state
    this->_isCloaked = isCloaked;

    // If the state change is not forced, apply the recently cloaked cooldown
    if (forced == false)
    {
        UGameInstance* gameInstance = this->GetGameInstance();
        
        if (gameInstance != nullptr)
        {
            // The disassembly contains an inline expansion of Cast<UDBDGameInstance>.
            // It checks the ClassTreeIndex of the game instance against UDBDGameInstance's static class.
            UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(gameInstance);
            
            if (dbdGameInstance != nullptr)
            {
                // The disassembly checks GUObjectArray for the RF_Unreachable flag.
                // Replaced with the standard UE macro ::IsValid() as requested.
                if (IsValid(dbdGameInstance) == true)
                {
                    // IDA hallucinated the field name at offset 0xF0 as GameInstance[1].UObject::UObjectBaseUtility::UObjectBase::__vftable
                    // Named it DesignTunables based on the pointer type and usage.
                    UDBDDesignTunables* designTunables = dbdGameInstance->DesignTunables;

                    if (designTunables != nullptr)
                    {
                        // Validate the designTunables object using the standard UE macro
                        if (IsValid(designTunables) == true)
                        {
                            // Retrieve the cooldown tunable value using the specific FName constant.
                            // The second argument 'false' corresponds to warnIfRowMissing.
                            float cooldownTime = designTunables->GetTunableValue(TunableValues::SLASHER_ABILITY_RECENTLY_ACTIVATED_COOLDOWN, false);

                            // Reset the timer with the retrieved cooldown
                            this->_recentlyCloakedTimer.Reset(cooldownTime);
                        }
                    }
                }
            }
        }
    }
}

// starts at line 1776
void ASlasherPlayer::SetInStalkMode(bool stalkMode, bool forced)
{
    // Check if the current stalk mode state differs from the requested stalk mode
    if (this->_isInStalkMode != stalkMode)
    {
        // Update the internal stalk mode variable
        this->_isInStalkMode = stalkMode;

        // Trigger the event or function to handle the stalk mode change
        this->OnStalkModeChanged(stalkMode);
    }
}

// starts at line 1785
bool ASlasherPlayer::WasRecentlyCloaked() const
{
    return this->_isCloaked || this->_recentlyCloakedTimer->IsDone() == false;
}

// starts at line 1790
void ASlasherPlayer::Authority_Blinded(EBlindType blindType, float coolDown, AActor* effector)
{
    // First, call the base class implementation of Authority_Blinded
    Super::Authority_Blinded(blindType, coolDown, effector);

    // Only proceed if this instance has network authority
    if (this->Role == ROLE_Authority)
    {
        // Get the chargeable component that handles blinding
        UChargeableComponent* blindingComponent = this->BlindingChargeableComponent;
        
        // Create a copy of the instigators array from the chargeable component
        // This is done to safely iterate over the instigators without risking 
        // the array being modified during iteration.
        TArray<AActor*> instigatorsCopy = blindingComponent->_instigatorsSinceEmpty;

        // Iterate through all actors that contributed to the blinding
        for (AActor* instigatorActor : instigatorsCopy)
        {
            if (instigatorActor != nullptr)
            {
                // Attempt to cast the instigator to an ADBDPlayer
                ADBDPlayer* instigatorPlayer = Cast<ADBDPlayer>(instigatorActor);

                if (instigatorPlayer != nullptr)
                {
                    // Check if the instigator is valid using standard Unreal Engine macro
                    if (IsValid(instigatorPlayer) == true && instigatorPlayer->IsValidImpl() == true)
                    {
                        // Determine the score type to award. 
                        // Default to chase blind, switch to regular slasher blind if no active chases.
                        // The subtraction operation `_chaseTimers.Pairs.Elements.Data.ArrayNum - _chaseTimers.Pairs.Elements.NumFreeIndices` 
                        // is a manual calculation of the number of active elements in a TSparseArray, which TMap uses internally.
                        // Replaced with standard TMap::Num().
                        EDBDScoreTypes scoreTypeToAward = EDBDScoreTypes::DBDCamperScore_ChaseBlind;
                        
                        if (this->_chaseTimers.Num() <= 0)
                        {
                            scoreTypeToAward = EDBDScoreTypes::DBDCamperScore_SlasherBlind;
                        }

                        // Fire the score event for the instigator
                        // The arguments are: scoreType, percentToAward (1.0f), redirectToGameEventSystem (true)
                        instigatorPlayer->TryFireScoreEvent(scoreTypeToAward, 1.0f, true);
                    }
                }
            }
        }

        // Attempt to cast the primary effector (the one who triggered the blind) to an ADBDPlayer
        ADBDPlayer* effectorPlayer = Cast<ADBDPlayer>(effector);

        // If the effector wasn't an ADBDPlayer, clear the pointer
        if (effectorPlayer == nullptr)
        {
            return;
        }

        // Double-check authority before requesting the stun (redundant check present in assembly)
        if (this->Role == ROLE_Authority)
        {
            // Request a stun on the slasher, passing the effector as the instigator
            // EStunType value 3 corresponds to a blind stun
            this->RequestStun(EStunType::VE_Flashlight, effectorPlayer);
        }
    }
}

// starts at line 1816
void ASlasherPlayer::Authority_StopBlinding(AActor* effector)
{
    this->BlindingChargeableComponent->RemoveInstigator(effector);
}

// starts at line 1821
void ASlasherPlayer::RequestStun(EStunType stunType, ADBDPlayer* stunner)
{
    // Check if the current instance has network authority before processing the stun
    if (this->Role == ROLE_Authority)
    {
        EInputInteractionType interactionToTry;

        // Determine the appropriate interaction type to trigger based on the requested stun type
        switch (stunType)
        {
            case EStunType::VE_Bookshelf: // Assuming 1 and 6 map to generic/specific stuns based on context
            case EStunType::VE_Kicked: // IDA shows 6u, need to map to actual enum if available, using placeholder logic
                // Check if the slasher is currently cloaked
                // Note: The disassembly shows a call to an IsCloaked function at offset 0xEA8.
                // In previous contexts, _isCloaked was used directly or via a different getter. 
                // We'll use the vtable function IsCloaked() as requested by the assembly.
                if (this->IsCloaked() == true)
                {
                    // Interaction type 11 (0x0B) is likely a specific uncloak/stun interaction
                    interactionToTry = EInputInteractionType::VE_ExternalRequestStunUncloak;
                }
                // Check if the slasher is carrying a survivor
                else if (this->IsCarrying() == true)
                {
                    // Interaction type 13 (0x0D) is likely a drop/stun interaction while carrying
                    interactionToTry = EInputInteractionType::VE_ExternalRequestDropByStunning;
                }
                else
                {
                    // Determine interaction based on whether the stun type was 1 or 6
                    if (stunType == EStunType::VE_Bookshelf)
                    {
                        // Interaction type 12 (0x0C) is likely a standard stun
                        interactionToTry = EInputInteractionType::VE_ExternalRequestStun;
                    }
                    else
                    {
                        // Interaction type 27 (0x1B) is likely a specific stun variant
                        interactionToTry = EInputInteractionType::VE_ExternalRequestKickStun;
                    }
                }
                break;

            case EStunType::VE_EvilWithin:
                // Interaction type 26 (0x1A)
                interactionToTry = EInputInteractionType::VE_ExternalRequestStunEvilWithin;
                break;

            case EStunType::VE_Flashlight: // Blind Stun (from previous context)
                // Blind stuns only trigger an interaction if the slasher is carrying someone
                if (this->IsCarrying() == true)
                {
                    // Interaction type 13 (0x0D) - drop survivor
                    interactionToTry = EInputInteractionType::VE_ExternalRequestDropByStunning;
                }
                else
                {
                    // Do nothing if not carrying during a blind stun
                    return;
                }
                break;

            case EStunType::VE_WiggleFree: // Escape Stun (from previous context)
                // Escape stuns only trigger if the slasher is actually carrying someone
                if (this->IsCarrying() == true)
                {
                    // Interaction type 14 (0x0E) - survivor escapes carry
                    interactionToTry = EInputInteractionType::VE_ExternalRequestDropByWiggleFree;
                }
                else
                {
                    // Do nothing if not carrying
                    return;
                }
                break;

            case EStunType::VE_BearTrap:
                // Interaction type 16 (0x10)
                interactionToTry = EInputInteractionType::VE_ExternalRequestKillerCaughtInBearTrap;
                break;

            default:
                return;
        }

        // Trigger the determined interaction type on the client
        this->Client_TryInteractionType(interactionToTry, stunner);
    }
}

// starts at line 1869
void ASlasherPlayer::Authority_AddItemAddonToPlayer(UItemAddon* itemAddon)
{
    // Check if the current instance has network authority (Role == 3 is ROLE_Authority)
    if (this->Role == ROLE_Authority)
    {
        // The disassembly shows an inline expansion of the standard TArray Add function, 
        // including manual ArrayNum increment, ArrayMax capacity check, and ResizeGrow call.
        // Replaced with the standard Unreal Engine TArray function as requested.
        this->_powerAddons.Add(itemAddon);
    }
}

// starts at line 1877
void ASlasherPlayer::Authority_RequestStun(EStunType stunType, ADBDPlayer* stunner)
{
    if (this->Role == ROLE_Authority)
    {
        this->RequestStun(stunType, stunner);
    }
}

// starts at line 1885
void ASlasherPlayer::Authority_AddInvisibilityBurnoutCharge(ADBDPlayer* blinder, float charge, AActor* effector)
{
    // Ensure this action is only executed if the current instance has network authority
    if (this->Role == ROLE_Authority)
    {
        // Retrieve the component responsible for managing the invisibility burnout charge
        UChargeableComponent* burnoutChargeable = this->_invisibilityBurnoutChargeableComponent;

        if (burnoutChargeable != nullptr)
        {
            // Verify the chargeable component is valid using the standard UE macro
            if (IsValid(burnoutChargeable) == true)
            {
                // Default the modifier multiplier to 1.0f (no modification)
                float modifierValue = 1.0f;

                // Retrieve the Perk Manager via the interface
                UPerkManager* perkManager = this->GetPerkManager();

                if (perkManager != nullptr)
                {
                    // Verify the perk manager is valid using the standard UE macro
                    if (IsValid(perkManager) == true)
                    {
                        // Get the perk modifier affecting the burnout charge.
                        // The specific enums for modifier type and source are inferred from context (0x44 -> 68, likely a specific modifier type)
                        // and 0x04 -> 4 (likely source type). I will use generic casting as exact names are unavailable.
                        float perkModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyInvisibilityBurnoutSusceptability, EGameplayModifierSource::VE_All);
                        
                        // Add the retrieved modifier to the base multiplier
                        modifierValue = modifierValue + perkModifier;
                    }
                }

                // Ensure the final modifier is not negative (clamp to a minimum of 0.0)
                float finalMultiplier = FMath::Max(modifierValue, 0.0f);

                // Calculate the final charge amount to add
                float finalCharge = finalMultiplier * charge;

                // Add the calculated charge to the component, passing the effector as the instigator
                burnoutChargeable->AddCharge(finalCharge, effector);
            }
        }
    }
}

// starts at line 1896
void ASlasherPlayer::OnBlindChargeEmptied()
{
    // If the slasher was recently blinded, they do not get evasion points
    if (this->WasRecentlyBlinded() == true)
    {
        return;
    }

    // If the slasher is currently interacting, they cannot evade
    if (this->IsInteracting == true)
    {
        return;
    }

    // If the evasion score timer is not yet done, we cannot award points again
    if (this->_flashlightBlindEvasionScoreTimer.IsDone() == false)
    {
        return;
    }

    // Fire the score event for successfully evading a flashlight blind
    // 0x68 corresponds to DBDSlasherScore_EvadeBlind
    this->TryFireScoreEvent(EDBDScoreTypes::DBDSlasherScore_EvadeBlind, 1.0f, true);

    // Reset the evasion score timer using the defined cooldown duration
    this->_flashlightBlindEvasionScoreTimer.Reset(this->_flashlightEvasionScoreCooldown);
}

// starts at line 1908
void ASlasherPlayer::OnInvisibilityChargeEmptied()
{
    // Check if the slasher is currently cloaked
    if (this->IsCloaked() == false)
    {
        return;
    }

    // Check if the burnout evasion score timer has finished
    if (this->_flashlightBurnoutEvasionScoreTimer.IsDone() == false)
    {
        return;
    }

    // Fire the score event for successfully evading an invisibility burnout
    this->TryFireScoreEvent(EDBDScoreTypes::DBDSlasherScore_EvadeInvisBurn, 1.0f, true);

    // Reset the burnout evasion score timer with the defined cooldown
    this->_flashlightBurnoutEvasionScoreTimer.Reset(this->_flashlightEvasionScoreCooldown);
}

// starts at line 1918
void ASlasherPlayer::OnInvisibilityBurnoutChargeCompletionChanged(bool completed)
{
    if (completed && this->Role == ROLE_Authority)
    {
        this->AuthorityFlashlighted(nullptr);
    }    
}

// starts at line 1952
bool ASlasherPlayer::CanInteractWith(const UInteractionDefinition* interaction)
{
    // If there is no interaction definition provided, assume interaction is possible
    if (interaction == nullptr)
    {
        return true;
    }

    // The disassembly checks GUObjectArray for the RF_Unreachable flag.
    // Replaced with standard UE macro ::IsValid() as requested.
    if (IsValid(interaction) == false)
    {
        return true;
    }

    // Check if the interaction requires rotation snapping and testing player orientation
    // Note: Offsets 0x2B5 and 0x2BD point to boolean flags in UInteractionDefinition.
    // Assuming names based on IDA pseudo code: SnapRotation and TestPlayerOrientationOnSnap.
    if (interaction->SnapRotation == false)
    {
        return true;
    }

    if (interaction->TestPlayerOrientationOnSnap == false)
    {
        return true;
    }

    // Retrieve the player's current direction as a rotator
    FRotator playerDirection = this->GetPlayerDirection();

    // The disassembly constructs an FRotationMatrix to get the forward vector (X axis),
    // then performs a manual 2D vector normalization (setting Z to 0, calculating sqrt(X^2 + Y^2) 
    // using reciprocal square root approximation with Newton-Raphson iteration).
    // Replaced inline math with standard Unreal Engine vector math functions.
    FVector playerForward = playerDirection.Vector();
    playerForward.Z = 0.0f;
    
    // Normalize the 2D direction vector, using a small tolerance threshold (0.00000001f)
    if (playerForward.SizeSquared() > 1.e-8f)
    {
        playerForward.Normalize();
    }
    else
    {
        playerForward = FVector::ZeroVector;
    }

    // Retrieve the expected snap point rotation for this interaction relative to the player
    FRotator snapRotation = interaction->GetSnapPointRotation(this);

    // Get the forward vector from the snap rotation and normalize it in 2D
    FVector snapForward = snapRotation.Vector();
    snapForward.Z = 0.0f;

    if (snapForward.SizeSquared() > 1.e-8f)
    {
        snapForward.Normalize();
    }
    else
    {
        snapForward = FVector::ZeroVector;
    }

    // Calculate the dot product between the player's normalized 2D direction and the snap point's normalized 2D direction.
    // Return true if the dot product is greater than 0.5 (which corresponds to an angle difference of less than 60 degrees).
    return FVector::DotProduct(playerForward, snapForward) > 0.5f;
}

// starts at line 1969
void ASlasherPlayer::SetBeingWiggled(bool value)
{
    // Update the internal state indicating if a survivor is wiggling while being carried
    this->_isBeingWiggled = value;

    // Get the character movement component from the base class
    UCharacterMovementComponent* characterMovement = this->CharacterMovement;

    if (characterMovement != nullptr)
    {
        // It checks if the component's class is a child of UDBDCharacterMovementComponent.
        UDBDCharacterMovementComponent* dbdMovement = Cast<UDBDCharacterMovementComponent>(characterMovement);

        if (dbdMovement != nullptr)
        {
            // Verify the movement component object is valid using the standard Unreal Engine macro
            if (IsValid(dbdMovement) == true)
            {
                // If the slasher is being wiggled, apply the movement penalty/modifier.
                // The offset rbx+760h in UDBDCharacterMovementComponent likely refers to a wiggle speed multiplier.
                if (this->_isBeingWiggled == true)
                {
                    // Apply full wiggle effect (1.0f)
                    dbdMovement->_randomInputAmplitude = 1.0f;
                }
                else
                {
                    // Reset wiggle effect (0.0f)
                    dbdMovement->_randomInputAmplitude = 0.0f;
                }
            }
        }
    }
}

// starts at line 1979
void ASlasherPlayer::HandleGuidedCamperDeconnection()
{
    // Retrieve the pointer to the camper currently being guided/carried by the slasher
    ACamperPlayer* guidedCamper = this->_guidedCamper;

    // Check if the guided camper is null or if the object is no longer valid/accessible.
    // The disassembly contains an inline expansion of the standard Unreal Engine IsValid() macro 
    // and a call to the game-specific IsValidImpl() via VTable.
    if (guidedCamper == nullptr || IsValid(guidedCamper) == false || guidedCamper->IsValidImpl() == false)
    {
        // If the camper is disconnected or invalid, and the slasher is currently in a "being wiggled" state
        if (this->_isBeingWiggled == true)
        {
            // Retrieve the movement component to reset wiggle penalties
            UCharacterMovementComponent* characterMovement = this->CharacterMovement;

            if (characterMovement != nullptr)
            {
                // The disassembly contains an inline expansion of Cast<UDBDCharacterMovementComponent>.
                UDBDCharacterMovementComponent* dbdMovement = Cast<UDBDCharacterMovementComponent>(characterMovement);

                if (dbdMovement != nullptr)
                {
                    // Check if the movement component itself is valid in the global object array
                    if (IsValid(dbdMovement) == true)
                    {
                        // Reset the wiggle slowdown multiplier to zero since the camper is gone.
                        // The offset rbx+760h in UDBDCharacterMovementComponent maps to this functionality.
                        dbdMovement->_randomInputAmplitude = 0.0f;
                    }
                }
            }
        }
    }
}

// starts at line 1994
void ASlasherPlayer::OnSurvivorsLeftChanged(int32 survivorRemaining)
{
    // Check if the current instance has network authority
    if (this->Role == ROLE_Authority)
    {
        // Only trigger this logic if exactly one survivor is remaining
        if (survivorRemaining == 1)
        {
            // Retrieve the game instance
            UGameInstance* gameInstance = this->GetGameInstance();

            if (gameInstance != nullptr)
            {
                // The disassembly contains an inline expansion of Cast<UDBDGameInstance>.
                UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(gameInstance);

                if (dbdGameInstance != nullptr)
                {
                    // Check if the game instance object is valid using the standard UE macro
                    if (IsValid(dbdGameInstance) == true)
                    {
                        // Check if an offering that allows killing the last survivor is active
                        // Enum value 0x1C (28) corresponds to the 'KillLastSurvivor' offering effect type based on IDA pseudo.
                        if (dbdGameInstance->HasOfferingOfType(EOfferingEffectType::KillLastSurvivor) == true)
                        {
                            // Ensure the slasher has at least 1 allowed kill, preserving higher counts if they exist
                            int32 allowedKills = 1;
                            if (this->_allowedKillCount >= 1)
                            {
                                allowedKills = this->_allowedKillCount;
                            }
                            
                            this->_allowedKillCount = allowedKills;
                        }
                    }
                }
            }
        }
    }
}

// starts at line 2003
void ASlasherPlayer::OnLockOnTarget(ADBDPlayer* target)
{
    // The disassembly checks GUObjectArray for the RF_Unreachable flag.
    // Replaced with standard UE macro IsValid() as requested.
    if (IsValid(target) == true && target->IsValidImpl() == true)
    {
        // If the target is valid, notify the slasher that a chase target was found
        this->OnChaseTargetFound(target);
    }
}

// starts at line 2012
bool ASlasherPlayer::IsChainBlinking() const
{
    return this->_isChainBlinking;
}

// starts at line 2017
void ASlasherPlayer::SetIsChainBlinking(bool isChainBlinking)
{
    this->_isChainBlinking = isChainBlinking;
}

// starts at line 2022
bool ASlasherPlayer::IsChargingBlink() const
{
    // The disassembly manually constructs an FString by allocating memory and 
    // converting the 8-bit string "Blink" into a wide character string.
    // This is replaced by the standard Unreal Engine TEXT() macro for FString construction.
    FString blinkInteractionName = FString(TEXT("Blink"));

    // Call the game-specific function to determine if the interaction is currently happening
    return this->IsPerformingInteraction(blinkInteractionName);
}

// starts at line 2027
bool ASlasherPlayer::IsPerformingInteraction(FString interactionId) const
{
    // Retrieve the interaction currently being performed by the player
    UInteractionDefinition* currentInteraction = this->GetCurrentInteraction();

    // Check if the player is actually performing an interaction
    if (currentInteraction != nullptr && IsValid(currentInteraction) == true)
    {
        // Compare the ID of the current interaction with the provided interactionId.
        // The disassembly uses FGenericPlatformStricmp to compare the underlying TCHAR arrays.
        // This is equivalent to checking if the two FStrings are equal.
        return currentInteraction->InteractionID == interactionId;
    }

    // If no valid interaction is being performed, return false
    return false;
}

// starts at line 2038
bool ASlasherPlayer::IsASurvivorInTerrorRadius() const
{
    // Cast the game instance to the Dead by Daylight specific game instance
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    // Check if the cast was successful
    if (dbdGameInstance == nullptr || IsValid(dbdGameInstance) == false)
    {
        return false;
    }

    // Retrieve the array of survivors currently in the game
    TArray<ACamperPlayer*> inGameSurvivors = dbdGameInstance->GetInGameSurvivors();

    // Iterate through the array of survivors
    for (ACamperPlayer* currentSurvivor : inGameSurvivors)
    {
        // Ensure the pointer to the survivor is valid
        if (currentSurvivor != nullptr)
        {
            // Check if this specific survivor is currently within the terror radius
            if (currentSurvivor->IsInTerrorRadius() == true)
            {
                // A survivor was found inside the terror radius
                return true;
            }
        }
    }

    // No survivors were found inside the terror radius
    return false;
}

// starts at line 2053
bool ASlasherPlayer::IsInBlinkCooldown() const
{
    return this->_isInBlinkCooldown;
}

// starts at line 2058
void ASlasherPlayer::SetIsInBlinkCooldown(bool IsInBlinkCooldown)
{
    this->_isInBlinkCooldown = IsInBlinkCooldown;
}

// starts at line 2063
void ASlasherPlayer::CameraUpdated()
{
    // Call the base class implementation first
    Super::CameraUpdated();

    // Array to hold the retrieved components
    TArray<UActorComponent*> components;

    // Retrieve all components of type UScreenSpaceLocationComponent attached to this actor
    // The disassembly explicitly gets the private static class and passes it to GetComponentsByClass
    this->GetComponentsByClass(UScreenSpaceLocationComponent::StaticClass(), components);

    // Iterate through the retrieved components
    for (UActorComponent* component : components)
    {
        // Ensure the component pointer is not null
        if (component != nullptr)
        {
            // The disassembly contains an inline expansion of Cast<UScreenSpaceLocationComponent>
            // It checks the ClassTreeIndex against the static class of UScreenSpaceLocationComponent
            UScreenSpaceLocationComponent* screenSpaceComponent = Cast<UScreenSpaceLocationComponent>(component);

            if (screenSpaceComponent != nullptr && IsValid(screenSpaceComponent) == true)
            {
                // Update the screen space location for the valid component
                screenSpaceComponent->UpdatePosition();
            }
        }
    }
}

// starts at line 2077
void ASlasherPlayer::Authority_AllowKilling(int numKills)
{
    if (this->Role == ROLE_Authority)
    {
        this->Multicast_AllowKilling(numKills);
    }
}

// starts at line 2094
void ASlasherPlayer::Multicast_AllowKilling_Implementation(int32 numKills)
{
    // Calculate the new allowed kill count by adding the requested amount
    int32 newKillCount = this->_allowedKillCount + numKills;

    // The assembly uses a conditional move (cmovle) to clamp the value.
    // Ensure the allowed kill count does not fall below zero.
    if (newKillCount <= 0)
    {
        newKillCount = 0;
    }

    // Update the internal member variable with the valid count
    this->_allowedKillCount = newKillCount;
}

// starts at line 2104
void ASlasherPlayer::Server_AllowKillingAfterHooking_Implementation(int numKills)
{
    this->Multicast_AllowKilling(numKills);
}

// starts at line 2113
void ASlasherPlayer::Multicast_AllowKillingAfterHooking_Implementation(int32 numKills)
{
    // Calculate the new allowed kill after hooking count by adding the requested amount
    int32 newKillCount = this->_allowedKillAfterHookingCount + numKills;

    // Ensure the allowed kill after hooking count does not fall below zero
    if (newKillCount <= 0)
    {
        newKillCount = 0;
    }

    // Update the internal member variable with the valid count
    this->_allowedKillAfterHookingCount = newKillCount;
}

// starts at line 2123
UCurveFloat* ASlasherPlayer::GetAttackCurve(EAttackType attackType, EAttackSubstate attackSubstate) const
{
    // Attempt to retrieve the Stalker component from the player
    UStalkerComponent* stalkerComponent = Cast<UStalkerComponent>(this->GetFirstComponentByClass(UStalkerComponent::StaticClass()));

    // Validate the component using the standard Unreal Engine macro
    if (stalkerComponent != nullptr || IsValid(stalkerComponent) == false)
    {
        stalkerComponent = nullptr;
    }

    const TArray<UCurveFloat*>* selectedCurves = nullptr;

    // The disassembly reveals the true mapping of EAttackType enum values to curve arrays.
    switch (attackType)
    {
        case VE_Slash: // Slash Attack
            selectedCurves = &this->AttackSlashCurves;
            break;

        case VE_Pounce: // Pounce Attack
            if (stalkerComponent != nullptr)
            {
                int32 currentTier = stalkerComponent->GetCurrentTier();
                
                // The disassembly explicitly maps Tier 0, 1, and 2 directly to AttackPounceCurves_StalkTier2
                if (currentTier == 0 || currentTier == 1 || currentTier == 2)
                {
                    selectedCurves = &this->AttackPounceCurves_StalkTier2;
                }
                else
                {
                    return nullptr;
                }
            }
            else
            {
                selectedCurves = &this->AttackPounceCurves;
            }
            break;

        case VE_Chainsaw: // Chainsaw Attack
            selectedCurves = &this->AttackChainsawCurves;
            break;

        case VE_Blink: // Blink Attack
            selectedCurves = &this->AttackBlinkCurves;
            break;

        case VE_Lunge: // Lunge Attack
            selectedCurves = &this->AttackLungeCurves;
            break;

        default:
            return nullptr;
    }

    // If a valid curve array was selected, retrieve the curve based on the substate
    if (selectedCurves != nullptr)
    {
        // The substate is used as a 1-based index, so we subtract 1 to get the 0-based array index
        int32 curveIndex = static_cast<int32>(attackSubstate) - 1;

        // Perform bounds checking
        if (curveIndex >= 0 && curveIndex < selectedCurves->Num())
        {
            return (*selectedCurves)[curveIndex];
        }
    }

    return nullptr;
}

// starts at line 2194
ACamperPlayer* ASlasherPlayer::GetObsessionTarget() const
{
    // Retrieve the world associated with this player
    UWorld* world = this->GetWorld();

    // Check if the world pointer is null
    if (world == nullptr)
    {
        return nullptr;
    }

    // Cast the generic game state to the specific Dead by Daylight game state
    ADBDGameState* dbdGameState = Cast<ADBDGameState>(world->GameState);

    // Check if the cast was unsuccessful
    if (dbdGameState == nullptr || IsValid(dbdGameState) == false)
    {
        return nullptr;
    }

    // Return the obsession target camper player from the game state.
    return dbdGameState->_obsessionTarget;
}

// starts at line 2204
void ASlasherPlayer::SetChainsawSprinting(bool chainsawSprinting)
{
    this->_isChainsawSprinting = chainsawSprinting;
}

// starts at line 2209
bool ASlasherPlayer::IsStalkingSomeone() const
{
    // Retrieve the Stalker component attached to this player
    UStalkerComponent* stalkerComponent = Cast<UStalkerComponent>(this->GetFirstComponentByClass(UStalkerComponent::StaticClass()));

    // Check if the component is null
    if (stalkerComponent == nullptr || IsValid(stalkerComponent) == false)
    {
        return false;
    }

    // Forward the call to the stalker component to check if the slasher is actively stalking
    return stalkerComponent->IsStalkingSomeone();
}

// starts at line 2219
void ASlasherPlayer::GetAnimTags(TArray<FName>& outTags) const
{
    // Note: Because this function is implemented from the IAnimTagProvider interface 
    // at offset 0x7B0, the assembly naturally performs a pointer adjustment 
    // (this - 0x7B0) to get the base ASlasherPlayer pointer. 
    // In C++, the compiler handles this thunking automatically.

    // 1. Slasher Name Tag
    // The disassembly checks an FString's length (ArrayNum at offset 0x2C0).
    // If it's not empty, it uses that string; otherwise, it falls back to a default.
    FName slasherName;
    if (this->CharacterName.Len() > 0)
    {
        slasherName = FName(*this->CharacterName);
    }
    else
    {
        slasherName = NAME_None;
    }
    outTags.AddUnique(slasherName);

    // 2. Stalker Component Tier Tags
    UStalkerComponent* stalkerComponent = Cast<UStalkerComponent>(this->GetFirstComponentByClass(UStalkerComponent::StaticClass()));
    
    if (IsValid(stalkerComponent) == true)
    {
        int32 currentTier = stalkerComponent->GetCurrentTier();
        
        if (currentTier == 1)
        {
            outTags.AddUnique(AnimMapping::ANIMTAG_TIER1);
        }
        else if (currentTier == 2)
        {
            outTags.AddUnique(AnimMapping::ANIMTAG_TIER2);
        }
        else
        {
            outTags.AddUnique(AnimMapping::ANIMTAG_TIER0);
        }
    }
    else
    {
        // Default to Tier 0 if the slasher doesn't have a stalker component (e.g., not The Shape/Ghost Face)
        outTags.AddUnique(AnimMapping::ANIMTAG_TIER0);
    }

    // 3. First Person View (FPV) Tag
    // The assembly calls a virtual function at offset 0xDB8.
    if (this->IsLocallyObserved() == true)
    {
        outTags.AddUnique(AnimMapping::ANIMTAG_FPV);
    }

    // 4. Carrying Tag
    if (this->IsCarrying() == true)
    {
        outTags.AddUnique(AnimMapping::ANIMTAG_CARRY);
    }

    // 5. Strafing Tags
    // The disassembly reads two floats (offsets 0xD48 and 0xAF0). 
    // It compares the current movement/animation angle against a threshold to determine strafing.
    float animDirection = this->_animDirection; // Inferred from offset 0xD48
    float strafeThreshold = this->kAnimDirectionSideThreshold; // Inferred from offset 0xAF0

    // Check for right strafe
    if (animDirection > strafeThreshold)
    {
        outTags.AddUnique(AnimMapping::ANIMTAG_STRAFE_RIGHT);
    }
    // Check for left strafe (the assembly uses XOR 0x80000000 to flip the sign of the threshold)
    else if (animDirection < -strafeThreshold)
    {
        outTags.AddUnique(AnimMapping::ANIMTAG_STRAFE_LEFT);
    }
}

// starts at line 2261
void ASlasherPlayer::Server_SetLungeType_Implementation(int lungeType)
{
    if (this->Role == ROLE_Authority)
    {
        this->Multicast_SetLungeType(lungeType);
    }
}

// starts at line 2273
void ASlasherPlayer::Multicast_SetLungeType_Implementation(int lungeType)
{
    this->_lungeType = lungeType;
}

// starts at line 2282
float ASlasherPlayer::GetBaseFOV()
{
    return this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_BASE_FOV);
}

// starts at line 2287
void ASlasherPlayer::Authority_SetRagePercent(float ragePercent)
{
    if (this->Role == ROLE_Authority)
    {
        this->SetRage(ragePercent);
    }
}

// starts at line 2295
void ASlasherPlayer::Authority_AddRagePercent(float ragePercent)
{
    if (this->Role == ROLE_Authority)
    {
        this->SetRage(this->_ragePercent + ragePercent);
    }
}

// starts at line 2303
bool ASlasherPlayer::CanBeBlinded() const
{
    return this->_isKilling == false;
}

// starts at line 2308
float ASlasherPlayer::GetRagePercent() const
{
    if (this->Role == ROLE_Authority)
    {
        return this->_ragePercent;
    }
    return this->_ragePecentSmooth;
}

// starts at line 2325
void ASlasherPlayer::SetRage(float ragePercent)
{
    float currentRageValue = 0.0f;

    // Retrieve the current rage value based on network authority role
    if (this->Role == ROLE_Authority)
    {
        currentRageValue = this->_ragePercent;
    }
    else
    {
        currentRageValue = this->_ragePecentSmooth;
    }

    // Calculate the rage tier before applying the new value
    int32 previousTier = this->GetRageTierFromRagePercent(currentRageValue);

    // Clamp the new rage percent between 0.0f and 1.0f
    float clampedRagePercent = FMath::Clamp(ragePercent, 0.0f, 1.0f);

    // Update the correct internal rage tracking variable
    if (this->Role == ROLE_Authority)
    {
        this->_ragePercent = clampedRagePercent;
    }
    else
    {
        this->_ragePecentSmooth = clampedRagePercent;
    }

    float newRageValue = 0.0f;

    // Retrieve the newly updated rage value to calculate the new tier
    if (this->Role == ROLE_Authority)
    {
        newRageValue = this->_ragePercent;
    }
    else
    {
        newRageValue = this->_ragePecentSmooth;
    }

    // Calculate the new rage tier
    int32 currentTier = this->GetRageTierFromRagePercent(newRageValue);

    // Check if the tier has changed and update effects accordingly
    if (previousTier != currentTier)
    {
        this->UpdateRageTierEffect(previousTier, currentTier);
    }

    // The disassembly checks the ClassTreeIndex for UDBDGameInstance, which is standard for Cast<>
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(this->GetGameInstance());

    if (dbdGameInstance != nullptr)
    {
        // Verify the game instance using the standard Unreal Engine macro
        // This replaces the inline check of GUObjectArray for the RF_Unreachable flag
        if (IsValid(dbdGameInstance) == true)
        {
            // The disassembly accesses an inline struct at offset 0x7C8 in UDBDGameInstance.
            dbdGameInstance->_analyticsManager.GameplayKillerAnalytics.UpdateBloodlustState(currentTier);
        }
    }
}

// starts at line 2353
int32 ASlasherPlayer::GetRageTier() const
{
    if (this->Role == ROLE_Authority)
    {
        return this->GetRageTierFromRagePercent(this->_ragePercent);
    }
    return this->GetRageTierFromRagePercent(this->_ragePecentSmooth);
}

// starts at line 2358
int32 ASlasherPlayer::GetRageTierFromRagePercent(float ragePercent) const
{
    int32 currentTier = 0;

    // Retrieve the maximum number of rage tiers from the global tunable values
    float numRageTiersFloat = this->GetNonPlayerSpecificTunableValue(TunableValues::SLASHER_NUM_RAGE_TIERS);

    // Ensure there is at least one tier to check
    if (numRageTiersFloat > 0.0f)
    {
        int32 tierIndex = 0;

        // Iterate through each tier to find where the given rage percent falls
        while (true)
        {
            // Format the string for the specific tier's end value.
            // Example: "SLASHER_RAGE_TIER_END_0", "SLASHER_RAGE_TIER_END_1", etc.
            // The disassembly uses FString::AppendInt and FString::Printf manually, 
            // but FString::Printf can handle it directly.
            FString baseTierName = TunableValues::SLASHER_RAGE_TIER_END.ToString();
            FString tierNameString = FString::Printf(TEXT("%s_%d"), *baseTierName, tierIndex);

            // Create an FName from the formatted string to look up the tunable value
            FName tierEndName = FName(*tierNameString);

            // Retrieve the rage percent requirement for the current tier
            float tierEndValue = this->GetNonPlayerSpecificTunableValue(tierEndName);

            // If the player's rage percent is less than the tier's end value, they belong in this tier
            if (ragePercent < tierEndValue)
            {
                break;
            }

            // Increment the tier level and the loop index
            currentTier = currentTier + 1;
            tierIndex = tierIndex + 1;

            // Check if we've reached or exceeded the maximum number of tiers
            if (static_cast<float>(tierIndex) >= numRageTiersFloat)
            {
                return currentTier;
            }
        }
    }

    return currentTier;
}

// starts at line 2383
void ASlasherPlayer::UpdateAnimDirection(float deltaTime)
{
    // Store the previous animation direction
    float previousAnimDirection = this->_animDirection;

    // Calculate the current velocity angle in degrees on the XY plane relative to the actor's forward vector
    float currentVelocityAngle = UDBDUtilities::GetVelocityXYAngleDegrees(this);

    // Assuming offset 0x129C is a tunable or threshold value for the animation angle limit based on the XOR sign logic.
    // It's used as a symmetric bounds check (e.g., between -limit and +limit). Let's call it 'AngleThreshold'.
    float angleThreshold = this->kAnimDirectionForwardThreshold; 

    // Assuming offset 0x1298 is the interpolation speed. Let's call it 'InterpSpeed'.
    float interpSpeed = this->kAnimDirectionInterpSpeed;

    // Initially snap the animation direction to the current calculated angle
    this->_animDirection = currentVelocityAngle;

    // If both the current velocity angle AND the previous animation direction are within the angle threshold 
    // (meaning the slasher is moving generally forward-ish, not strafing too hard or walking backwards),
    // then smoothly interpolate between the old and new angles instead of snapping.
    // The XOR instruction with 0x80000000 essentially flips the sign bit to get the negative threshold.
    if (currentVelocityAngle < angleThreshold && currentVelocityAngle > -angleThreshold)
    {
        if (previousAnimDirection < angleThreshold && previousAnimDirection > -angleThreshold)
        {
            // Smoothly interpolate the animation direction to avoid sudden snapping while moving forward
            this->_animDirection = FMath::FInterpTo(previousAnimDirection, currentVelocityAngle, deltaTime, interpSpeed);
        }
    }
}

// starts at line 2403
float ASlasherPlayer::GetAnimDirection() const
{
    return this->_animDirection;
}

// starts at line 2408
void ASlasherPlayer::BroadcastOffensiveAction()
{
    // Broadcast the multicast delegate to notify all bound listeners that an offensive action occurred
    this->OnOffensiveAction.Broadcast();
}

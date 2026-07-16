// starts at line 24
UCamperMovementComponent::UCamperMovementComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Enable the bCanWalkOffLedges bit flag (bit 9 at offset 0x224 corresponding to 0x200)
    this->bCanWalkOffLedges = true;
}

// starts at line 29
float UCamperMovementComponent::GetMaxSpeed() const
{
    // Try to safely cast the character owner to a CamperPlayer (Survivor)
    ACamperPlayer* camperPlayer = Cast<ACamperPlayer>(this->CharacterOwner);
    
    // If the cast fails (no character owner or not a CamperPlayer), return 0
    if (camperPlayer == nullptr)
    {
        return 0.0f;
    }

    float baseMaxSpeed;

    // Determine base maximum speed based on player state
    if (camperPlayer->IsCrawling() == true)
    {
        if (camperPlayer->IsRunning() == true)
        {
            baseMaxSpeed = this->MaxRunSpeedInjured;
        }
        else
        {
            baseMaxSpeed = this->MaxWalkSpeedInjured;
        }
    }
    else if (camperPlayer->IsCrouching() == true)
    {
        baseMaxSpeed = this->MaxWalkSpeedCrouched;
    }
    else
    {
        if (camperPlayer->IsRunning() == true)
        {
            baseMaxSpeed = this->MaxRunSpeed;
        }
        else
        {
            baseMaxSpeed = this->MaxWalkSpeed;
        }
    }

    UPerkManager* perkManager = camperPlayer->GetPerkManager();
    float maxSpeedMultiplier = 1.0f;
    float currentMaxSpeed = baseMaxSpeed;

    if (perkManager != nullptr)
    {
        // Get multiplicative modifier (Type = 0x17, Source = 4)
        float multiplicativeModifier = perkManager->GetMultiplicativeModifierValueWithCompoundNegative(EGameplayModifierType::VE_IncreaseAllMovementSpeed, EGameplayModifierSource::VE_All);
        currentMaxSpeed = baseMaxSpeed * multiplicativeModifier;
    }

    if (camperPlayer->GetIsInteracting() == true)
    {
        currentMaxSpeed = currentMaxSpeed * camperPlayer->_interactingMaxSpeedPercent;
    }

    float crouchHealthyMultiplier = 1.0f;
    if (camperPlayer->IsCrouching() == true && camperPlayer->IsHealthy() == true)
    {
        if (perkManager != nullptr)
        {
            // Get perk modifier (Type = 0x61 ('a'), Source = 4)
            float crouchModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_IncreaseCrouchSpeed, EGameplayModifierSource::VE_All);
            crouchHealthyMultiplier = crouchHealthyMultiplier + crouchModifier;
        }
    }

    currentMaxSpeed = currentMaxSpeed * crouchHealthyMultiplier;

    if (perkManager != nullptr)
    {
        // Get additive perk modifier (Type = 0xA5, Source = 4)
        float additiveModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyMovementSpeedAdditive, EGameplayModifierSource::VE_All);
        currentMaxSpeed = currentMaxSpeed + additiveModifier;
    }

    // Check drop stagger timer remaining time
    if (UDBDUtilTimer::GetTimeLeft(camperPlayer->DropStaggerTimer) > KINDA_SMALL_NUMBER)
    {
        if (IsValid(camperPlayer->DropStaggerMaxSpeed) == true)
        {
            float percentTimeElapsed = camperPlayer->DropStaggerTimer.GetPercentTimeElapsed();

            float curveValue = camperPlayer->DropStaggerMaxSpeed->GetFloatValue(percentTimeElapsed);
            currentMaxSpeed = currentMaxSpeed * curveValue;
        }
    }

    // Apply hit speed boost curve
    if (this->_hitSpeedBoostTimer.IsDone() == false)
    {
        if (IsValid(this->OnHitMaxSpeedCurve) == true)
        {
            float percentTimeElapsed = this->_hitSpeedBoostTimer.GetPercentTimeElapsed();
            float curveValue = this->OnHitMaxSpeedCurve->FloatCurve.Eval(percentTimeElapsed, 0.0f);
            currentMaxSpeed = currentMaxSpeed * curveValue;
        }
    }

    // Apply sprint speed boost curve
    if (this->_sprintSpeedBoostDurationTimer.IsDone() == false)
    {
        if (IsValid(this->OnSprintMaxSpeedCurve) == true)
        {
            float percentTimeElapsed = this->_sprintSpeedBoostDurationTimer.GetPercentTimeElapsed();
            float curveValue = this->OnSprintMaxSpeedCurve->FloatCurve.Eval(percentTimeElapsed, 0.0f);
            currentMaxSpeed = currentMaxSpeed * curveValue;
        }
    }

    // Apply movement modifier actor multiplier if present
    if (this->_movementModifierActor != nullptr)
    {
        if (IsValid(this->_movementModifierActor) == true)
        {
            currentMaxSpeed = currentMaxSpeed * this->_movementModifierActor->MaxSpeedMultiplier;
        }
    }

    return currentMaxSpeed;
}

// starts at line 129
float UCamperMovementComponent::GetMaxAcceleration() const
{
    // Get the base max acceleration from the parent class
    float maxAcceleration = Super::GetMaxAcceleration();

    // Ensure the character owner is assigned
    if (this->CharacterOwner != nullptr)
    {
        // Try to cast the character owner to ACamperPlayer
        ACamperPlayer* camperPlayer = Cast<ACamperPlayer>(this->CharacterOwner);
        
        // If the cast is successful, it means the owner is a camper player
        if (camperPlayer != nullptr)
        {
            // Verify if the camper player object is valid using the standard Unreal Engine macro
            if (IsValid(camperPlayer) == true && camperPlayer->IsValidImpl() == true)
            {
                // Check if the camper is currently dashing
                if (camperPlayer->IsDashing() == true)
                {
                    // Multiply the base acceleration by the dash acceleration multiplier
                    maxAcceleration = maxAcceleration * this->MaxDashAcceleration;
                }
            }
        }
    }

    // Return the calculated max acceleration
    return maxAcceleration;
}

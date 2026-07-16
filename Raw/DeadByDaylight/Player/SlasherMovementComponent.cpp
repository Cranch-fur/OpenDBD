// starts at line 22
USlasherMovementComponent::USlasherMovementComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initialize the cloaked max speed percent
    this->_cloakedMaxSpeedPercent = 1.0f;

    // Initialize the carrying camper walk speed factor
    this->_carryingCamperWalkSpeedFactor = 1.0f;
}

// starts at line 26
float USlasherMovementComponent::GetMaxAcceleration() const
{
    // Get the base maximum acceleration from the parent class
    float maxAcceleration = UCharacterMovementComponent::GetMaxAcceleration();

    // Ensure the character owner is assigned
    if (this->CharacterOwner != nullptr)
    {
        // Attempt to cast the character owner to an ASlasherPlayer (Killer)
        ASlasherPlayer* slasherPlayer = Cast<ASlasherPlayer>(this->CharacterOwner);

        // If the cast is successful, the owner is a slasher player
        if (slasherPlayer != nullptr)
        {
            // Verify if the slasher player object is valid using the standard Unreal Engine macro
            if (IsValid(slasherPlayer) == true && slasherPlayer->IsValidImpl() == true)
            {
                // Check a specific boolean state on the slasher player (offset 0x7E2)
                if (slasherPlayer->IsAttacking == true)
                {
                    // Return a specific float value overriding the max acceleration (offset 0x1340)
                    return slasherPlayer->_attackAcceleration;
                }
            }
        }
    }

    // Return the base maximum acceleration if the specific conditions are not met
    return maxAcceleration;
}

// starts at line 40
float USlasherMovementComponent::GetMaxSpeed() const
{
    // Base maximum speed from the parent class
    float maxSpeed = Super::GetMaxSpeed();

    // Ensure the character owner is assigned
    if (this->CharacterOwner != nullptr)
    {
        // Try to safely cast the character owner to ASlasherPlayer (Killer)
        ASlasherPlayer* slasherPlayer = Cast<ASlasherPlayer>(this->CharacterOwner);
        
        // If the cast is successful, it means the owner is a slasher player
        if (slasherPlayer != nullptr)
        {
            // Verify if the slasher player object is valid using the standard Unreal Engine macro
            if (IsValid(slasherPlayer) == true && slasherPlayer->IsValidImpl() == true)
            {
                // Check if the slasher player is currently carrying a camper
                if (slasherPlayer->IsCarrying() == true)
                {
                    maxSpeed = maxSpeed * this->_carryingCamperWalkSpeedFactor;
                }
                // Check a specific boolean state on the slasher player (offset 0x7E2)
                else if (slasherPlayer->IsAttacking == true)
                {
                    // Apply a specific float multiplier from the slasher player (offset 0x1018)
                    maxSpeed = maxSpeed * slasherPlayer->_attackingMaxSpeedPercent;
                }
                // Check another specific vtable function state
                else if (slasherPlayer->IsCloaked() == true)
                {
                    maxSpeed = maxSpeed * this->_cloakedMaxSpeedPercent;
                }
                // Check another specific boolean state (offset 0x14DE)
                else if (slasherPlayer->_isChainBlinking == true)
                {
                    maxSpeed = maxSpeed * this->_blinkingMaxSpeedFactor;
                }

                // Check yet another specific vtable function state
                if (slasherPlayer->IsInStalkMode() == true)
                {
                    // Check if the slasher is currently stalking someone
                    if (slasherPlayer->IsStalkingSomeone() == true)
                    {
                        maxSpeed = maxSpeed * this->_stalkModeMaxSpeedWhileStalkingSomeoneFactor;
                    }
                    else
                    {
                        maxSpeed = maxSpeed * this->_stalkModeMaxSpeedFactor;
                    }
                }
                else
                {
                    // If not in the EB0 state, multiply by another specific float multiplier (offset 0x1480)
                    maxSpeed = maxSpeed * slasherPlayer->_stalkTierWalkSpeedMultiplier;
                }

                // Retrieve the PerkManager and apply multiplicative modifier (Type = 0x17, Source = 4)
                UPerkManager* perkManager = slasherPlayer->GetPerkManager();
                if (perkManager != nullptr)
                {
                    float multiplicativeModifier = perkManager->GetMultiplicativeModifierValueWithCompoundNegative(EGameplayModifierType::VE_IncreaseAllMovementSpeed, EGameplayModifierSource::VE_All);
                    maxSpeed = maxSpeed * multiplicativeModifier;
                    
                    // Apply additive perk modifier (Type = 0xA5, Source = 4)
                    float additiveModifier1 = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyMovementSpeedAdditive, EGameplayModifierSource::VE_All);
                    maxSpeed = maxSpeed + additiveModifier1;

                    // Apply another additive perk modifier (Type = 0xA6, Source = 4) if conditions are met
                    if (slasherPlayer->IsAttacking == false && slasherPlayer->IsInStalkMode() == false && slasherPlayer->IsCloaked() == false)
                    {
                        float additiveModifier2 = perkManager->GetPerkModifier(EGameplayModifierType::VE_RageModifyMovementSpeedAdditive, EGameplayModifierSource::VE_All);
                        maxSpeed = maxSpeed + additiveModifier2;
                    }
                }
            }
        }
    }

    // Apply sprint speed boost curve if the timer is not done
    if (this->_sprintSpeedBoostDurationTimer.IsDone() == false)
    {
        if (IsValid(this->OnSprintMaxSpeedCurve) == true)
        {
            float percentTimeElapsed = this->_sprintSpeedBoostDurationTimer.GetPercentTimeElapsed();
            float curveValue = this->OnSprintMaxSpeedCurve->FloatCurve.Eval(percentTimeElapsed, 0.0f);
            maxSpeed = maxSpeed * curveValue;
        }
    }

    // Apply movement modifier actor multiplier if present
    if (this->_movementModifierActor != nullptr)
    {
        if (IsValid(this->_movementModifierActor) == true)
        {
            maxSpeed = maxSpeed * this->_movementModifierActor->MaxSpeedMultiplier;
        }
    }

    return maxSpeed;
}

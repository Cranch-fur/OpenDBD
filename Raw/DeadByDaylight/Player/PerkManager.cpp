// starts at line 30
float UPerkManager::GetPerkModifier(EGameplayModifierType modifierType, EGameplayModifierSource filter) const
{
    // Initialize the total accumulated value for the modifiers
    float totalModifierValue = 0.0f;

    // Create an array to hold the retrieved effects
    TArray<UGameplayModifierContainer*> modifiers;

    // Populate the array with all active effects matching the filter
    this->GetAllEffects(modifiers, filter);

    // Iterate through the array of retrieved modifiers
    for (int32 i = 0; i < modifiers.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentModifier = modifiers[i];

        // Ensure the pointer to the modifier is valid before accessing it
        if (currentModifier == nullptr)
        {
            continue;
        }

        // Verify that the UObject is valid and not pending kill
        // This replaces the standard inline GUObjectArray check from the decompiled code
        if (IsValid(currentModifier) == true)
        {
            // Check if the current container holds the specific modifier type
            if (currentModifier->HasModifierOfType(modifierType) == true)
            {
                // Check if the modifier is currently active and applicable
                if (currentModifier->m_bIsApplicable == true)
                {
                    // Add the value of the modifier to the total calculation
                    totalModifierValue = totalModifierValue + currentModifier->GetModifierValue(modifierType);
                }
            }
        }
    }

    // Apply global meta modifiers to the total value if the type is not excluded
    if (modifierType != EGameplayModifierType::VE_ModifyAuraReadingRangeAdditive && 
        modifierType != EGameplayModifierType::VE_SpecialTinkererBonus)
    {
        totalModifierValue = this->ApplyMetaModifiers(modifierType, totalModifierValue);
    }

    // Return the final calculated modifier value
    // The memory allocated by the TArray will be automatically freed by its destructor
    return totalModifierValue;
}

// starts at line 47
float UPerkManager::GetMultiplicativeModifierValue(EGameplayModifierType modifierType) const
{
    // Retrieve the base perk modifier value, applying it to all gameplay modifier sources
    float baseModifierValue = this->GetPerkModifier(modifierType, EGameplayModifierSource::VE_All);

    // Calculate the multiplicative modifier by adding 1.0 to the base value
    float multiplicativeValue = baseModifierValue + 1.0f;

    // Return the final multiplicative modifier value
    return multiplicativeValue;
}

// starts at line 52
float UPerkManager::GetMultiplicativeModifierValueWithCompoundNegative(EGameplayModifierType ModifierType, EGameplayModifierSource filter) const
{
    // Initialize the base value for positive additive modifiers
    float additiveModifier = 1.0f;

    // Initialize the base value for compound negative modifiers
    float compoundNegativeModifier = 1.0f;

    // Create an array to hold the retrieved effects
    TArray<UGameplayModifierContainer*> modifiers;

    // Populate the array with all active effects matching the filter
    this->GetAllEffects(modifiers, filter);

    // Iterate through the array of retrieved modifiers
    for (int32 i = 0; i < modifiers.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentModifier = modifiers[i];

        // Ensure the pointer to the modifier is valid before accessing it
        if (currentModifier == nullptr)
        {
            continue;
        }

        // Verify that the UObject is valid and not pending kill
        // This replaces the standard inline GUObjectArray check from the decompiled code
        if (IsValid(currentModifier) == true)
        {
            // Check if the current container holds the specific modifier type
            if (currentModifier->HasModifierOfType(ModifierType) == true)
            {
                // Check if the modifier is currently active and applicable
                if (currentModifier->m_bIsApplicable == true)
                {
                    // Retrieve the actual numerical value of the modifier
                    float modifierValue = currentModifier->GetModifierValue(ModifierType);

                    // If the modifier is negative, apply it as a compound multiplicative penalty
                    if (modifierValue < 0.0f)
                    {
                        compoundNegativeModifier = compoundNegativeModifier * (modifierValue + 1.0f);
                    }
                    else
                    {
                        // If the modifier is positive, add it to the total additive pool
                        additiveModifier = additiveModifier + modifierValue;
                    }
                }
            }
        }
    }

    // Combine the additive positive modifiers with the compound negative modifiers
    float combinedModifierValue = additiveModifier * compoundNegativeModifier;


    // Apply global meta modifiers to the total value if the type is not excluded
    if (ModifierType != EGameplayModifierType::VE_ModifyAuraReadingRangeAdditive && 
        ModifierType != EGameplayModifierType::VE_SpecialTinkererBonus)
    {
        combinedModifierValue = this->ApplyMetaModifiers(ModifierType, combinedModifierValue);
    }

    // Return the final calculated modifier value
    // The memory allocated by the TArray will be automatically freed by its destructor
    return combinedModifierValue;
}

// starts at line 80
float UPerkManager::GetPerkModifierMaxValue(EGameplayModifierType modifierType) const
{
    // Initialize the maximum tracked value for the modifier
    float maxModifierValue = 0.0f;

    // Create an array to hold the retrieved effects
    TArray<UGameplayModifierContainer*> modifiers;

    // Populate the array with all active effects across all sources (VE_All is equal to 4)
    this->GetAllEffects(modifiers, EGameplayModifierSource::VE_All);

    // Iterate through the array of retrieved modifiers
    for (int32 i = 0; i < modifiers.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentModifier = modifiers[i];

        // Ensure the pointer to the modifier is valid before accessing it
        if (currentModifier == nullptr)
        {
            continue;
        }

        // Verify that the UObject is valid and not pending kill
        // This replaces the standard inline GUObjectArray check from the decompiled code
        if (IsValid(currentModifier) == true)
        {
            // Check if the current container holds the specific modifier type
            if (currentModifier->HasModifierOfType(modifierType) == true)
            {
                // Check if the modifier is currently active and applicable
                if (currentModifier->m_bIsApplicable == true)
                {
                    // Retrieve the actual numerical value of the modifier
                    float currentModifierValue = currentModifier->GetModifierValue(modifierType);

                    // If the retrieved value is strictly greater than our current maximum, update the maximum
                    if (maxModifierValue < currentModifierValue)
                    {
                        maxModifierValue = currentModifierValue;
                    }
                }
            }
        }
    }

    // Apply global meta modifiers to the total value if the type is not excluded
    if (modifierType != EGameplayModifierType::VE_ModifyAuraReadingRangeAdditive && 
        modifierType != EGameplayModifierType::VE_SpecialTinkererBonus)
    {
        maxModifierValue = this->ApplyMetaModifiers(modifierType, maxModifierValue);
    }

    // Return the highest found modifier value
    // The memory allocated by the TArray will be automatically freed by its destructor
    return maxModifierValue;
}

// starts at line 97
float UPerkManager::GetPerkModifierMinValue(EGameplayModifierType modifierType, float startValue) const
{
    // Initialize the minimum tracked value for the modifier using the provided start value
    float minModifierValue = startValue;

    // Create an array to hold the retrieved effects
    TArray<UGameplayModifierContainer*> modifiers;

    // Populate the array with all active effects across all sources (VE_All is equal to 4)
    this->GetAllEffects(modifiers, EGameplayModifierSource::VE_All);

    // Iterate through the array of retrieved modifiers
    for (int32 i = 0; i < modifiers.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentModifier = modifiers[i];

        // Ensure the pointer to the modifier is valid before accessing it
        if (currentModifier == nullptr)
        {
            continue;
        }

        // Verify that the UObject is valid and not pending kill
        // This replaces the standard inline GUObjectArray check from the decompiled code
        if (IsValid(currentModifier) == true)
        {
            // Check if the current container holds the specific modifier type
            if (currentModifier->HasModifierOfType(modifierType) == true)
            {
                // Check if the modifier is currently active and applicable
                if (currentModifier->m_bIsApplicable == true)
                {
                    // Retrieve the actual numerical value of the modifier
                    float currentModifierValue = currentModifier->GetModifierValue(modifierType);

                    // If the current minimum value is strictly greater than the retrieved value, update the minimum
                    if (minModifierValue > currentModifierValue)
                    {
                        minModifierValue = currentModifierValue;
                    }
                }
            }
        }
    }

    // Apply global meta modifiers to the total value if the type is not excluded
    if (modifierType != EGameplayModifierType::VE_ModifyAuraReadingRangeAdditive && 
        modifierType != EGameplayModifierType::VE_SpecialTinkererBonus)
    {
        minModifierValue = this->ApplyMetaModifiers(modifierType, minModifierValue);
    }

    // Return the lowest found modifier value
    // The memory allocated by the TArray will be automatically freed by its destructor
    return minModifierValue;
}

// starts at line 114
bool UPerkManager::HasPerkFlag(EGameplayModifierFlag perkFlag, EGameplayModifierSource filter) const
{
    // Initialize a tracking variable to determine if the requested flag is present
    bool bHasFlag = false;

    // Create an array to hold the retrieved effects
    TArray<UGameplayModifierContainer*> modifiers;

    // Populate the array with all active effects matching the filter
    this->GetAllEffects(modifiers, filter);

    // Iterate through the array of retrieved modifiers
    for (int32 i = 0; i < modifiers.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentModifier = modifiers[i];

        // Ensure the pointer to the modifier is valid before accessing it
        if (currentModifier == nullptr)
        {
            continue;
        }

        // Verify that the UObject is valid and not pending kill
        if (IsValid(currentModifier) == true)
        {
            // Check if the current container holds the specific flag
            if (currentModifier->HasFlag(perkFlag) == true)
            {
                // Check if the modifier is currently active and applicable
                if (currentModifier->m_bIsApplicable == true)
                {
                    // The flag is found and applicable, mark it and stop searching
                    bHasFlag = true;
                    break;
                }
            }
        }
    }

    // Define hardcoded constants based on the decompiled flag IDs
    EGameplayModifierFlag disruptAuraReadingFlag = static_cast<EGameplayModifierFlag>(0x2C);

    // Determine if the current flag falls under the aura reading category
    bool bIsAuraReadingFlag = false;
    
    if (perkFlag <= EGameplayModifierFlag::VE_RevealASurvivor)
    {
        bIsAuraReadingFlag = true;
    }
    else if (perkFlag == EGameplayModifierFlag::VE_RevealChests)
    {
        bIsAuraReadingFlag = true;
    }

    // If the requested flag was not found in the modifier array
    if (bHasFlag == false)
    {
        // If the flag is related to aura reading, check the disruption flag
        // The return value is discarded, but this mimics the side-effects or inline behavior of the original code
        if (bIsAuraReadingFlag == true)
        {
            this->HasPerkFlag(EGameplayModifierFlag::VE_DisruptAuraReading, EGameplayModifierSource::VE_All);
        }

        // Return false as the initial flag was not found
        return false;
    }

    // If the requested flag is the disruption flag itself, return true immediately
    if (perkFlag == EGameplayModifierFlag::VE_DisruptAuraReading)
    {
        return true;
    }

    // If the flag was found but it is an aura reading flag, we must check for disruptions
    if (bIsAuraReadingFlag == true)
    {
        // Check if aura reading is being disrupted across all sources
        bool bIsDisrupted = this->HasPerkFlag(EGameplayModifierFlag::VE_DisruptAuraReading, EGameplayModifierSource::VE_All);

        // If a disruption flag is present, the aura reading flag is negated
        if (bIsDisrupted == true)
        {
            return false;
        }
    }

    // The flag was found and is not disrupted by any overriding modifiers
    // The memory allocated by the TArray will be automatically freed by its destructor
    return true;
}

// starts at line 152
void UPerkManager::GetItemAddons(TArray<UGameplayModifierContainer*>& itemAddons) const
{
    // Retrieve the owner of this component
    AActor* owner = this->GetOwner();

    // Verify the owner pointer is valid and not pending kill
    if (IsValid(owner) == true)
    {
        // Check if the owner implements the required interface
        if (owner->Implements(UPerkOwnerInterface::StaticClass()) == true)
        {
            // Cast the owner to the interface to call its specific methods
            IPerkOwnerInterface* perkOwnerInterface = Cast<IPerkOwnerInterface>(owner);
            
            if (perkOwnerInterface != nullptr)
            {
                // Call the interface method (offset 0x28 in vtable based on assembly) to get the item addons
                // The interface method returns a TArray by value, which is then appended to the output array
                TArray<UGameplayModifierContainer*> retrievedAddons = perkOwnerInterface->GetItemAddons();
                
                // Append the retrieved addons to the provided reference array
                itemAddons.Append(retrievedAddons);
            }
        }
    }
}

// starts at line 176
void UPerkManager::GetAllEffects(TArray<UGameplayModifierContainer*>& combinedEffects, EGameplayModifierSource filter) const
{
    // Retrieve the owner of this component
    AActor* owner = this->GetOwner();

    // Ensure the owner pointer is valid and not pending kill
    if (IsValid(owner) == true)
    {
        // Check if the owner implements the required interface
        if (owner->Implements(UPerkOwnerInterface::StaticClass()) == true)
        {
            // Cast the owner to the interface to call its specific methods
            IPerkOwnerInterface* perkOwnerInterface = Cast<IPerkOwnerInterface>(owner);

            if (perkOwnerInterface != nullptr)
            {
                // Check if the filter includes Perks
                if (filter == EGameplayModifierSource::VE_All ||
                    filter == EGameplayModifierSource::VE_Perk ||
                    filter == EGameplayModifierSource::VE_PerkAndStatusEffect ||
                    filter == EGameplayModifierSource::VE_PerkStatusAndAddon)
                {
                    // Retrieve Perks and append them to the combined list
                    TArray<UGameplayModifierContainer*> perks;
                    perkOwnerInterface->AppendPerksTo(perks);
                    combinedEffects.Append(perks);
                }

                // Check if the filter includes Status Effects
                if (filter == EGameplayModifierSource::VE_All ||
                    filter == EGameplayModifierSource::VE_StatusEffect ||
                    filter == EGameplayModifierSource::VE_PerkAndStatusEffect ||
                    filter == EGameplayModifierSource::VE_PerkStatusAndAddon)
                {
                    // Retrieve Status Effects and append them to the combined list
                    TArray<UGameplayModifierContainer*> statusEffects;
                    perkOwnerInterface->AppendStatusEffectsTo(statusEffects);
                    combinedEffects.Append(statusEffects);
                }

                // Check if the filter includes Item Modifiers
                if (filter == EGameplayModifierSource::VE_All ||
                    filter == EGameplayModifierSource::VE_Item)
                {
                    // Retrieve Item Modifiers and append them to the combined list
                    // Note: The decompiler shows offset 48LL (0x30) for this method
                    TArray<UGameplayModifierContainer*> itemModifiers = perkOwnerInterface->GetItemModifiers();
                    combinedEffects.Append(itemModifiers);
                }

                // Check if the filter includes Item Addons
                if (filter == EGameplayModifierSource::VE_All ||
                    filter == EGameplayModifierSource::VE_ItemAddon ||
                    filter == EGameplayModifierSource::VE_PerkStatusAndAddon)
                {
                    // Retrieve Item Addons and append them to the combined list
                    // Note: The decompiler shows offset 40LL (0x28) for this method
                    TArray<UGameplayModifierContainer*> itemAddons = perkOwnerInterface->GetItemAddons();
                    combinedEffects.Append(itemAddons);
                }
            }
        }
    }
}

// starts at line 210
bool UPerkManager::HasAnyAddonWithTag(FName tag) const
{
    // Initialize the array to hold the retrieved item addons
    TArray<UGameplayModifierContainer*> addons;

    // Populate the array with all item addons attached to the owner
    this->GetItemAddons(addons);

    // Iterate through the array of retrieved addons
    for (int32 i = 0; i < addons.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentAddon = addons[i];

        // Ensure the pointer to the addon is valid before accessing it
        if (currentAddon == nullptr)
        {
            continue;
        }

        // Verify that the UObject is valid and not pending kill
        // This replaces the standard inline GUObjectArray check from the decompiled code
        if (IsValid(currentAddon) == true)
        {
            // Check if the current addon component possesses the specified tag
            // ComponentHasTag is a standard UActorComponent method
            if (currentAddon->ComponentHasTag(tag) == true)
            {
                // If any addon has the matching tag, immediately return true
                return true;
            }
        }
    }

    // Return false if no addons were found with the specified tag
    // The memory allocated by the TArray will be automatically freed by its destructor
    return false;
}

// starts at line 217
bool UPerkManager::HasAnyEffectWithID(FName id) const
{
    // Initialize the array to hold all retrieved effects
    TArray<UGameplayModifierContainer*> allEffects;

    // Populate the array with all active effects across all sources (VE_All is equal to 4)
    this->GetAllEffects(allEffects, EGameplayModifierSource::VE_All);

    // Iterate through the array of retrieved effects
    for (int32 i = 0; i < allEffects.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentEffect = allEffects[i];

        // Ensure the pointer to the effect is valid before accessing it
        if (currentEffect == nullptr)
        {
            continue;
        }

        // Check if the effect's ID matches the requested ID
        // The ID is located at offset 0xF0 in the UGameplayModifierContainer structure based on the assembly
        if (currentEffect->ID == id)
        {
            // If an effect with a matching ID is found, immediately return true
            return true;
        }
    }

    // Return false if no effects were found with the specified ID
    // The memory allocated by the TArray will be automatically freed by its destructor
    return false;
}

// starts at line 245
bool UPerkManager::HasAllCharacterPerksEquipped() const
{
    // Retrieve the UWorld instance from this component
    UWorld* world = this->GetWorld();

    if (world == nullptr)
    {
        return false;
    }

    // Retrieve the UDBDGameInstance from the world
    UDBDGameInstance* gameInstance = Cast<UDBDGameInstance>(world->GetGameInstance());

    if (gameInstance == nullptr)
    {
        return false;
    }

    // Retrieve the owner of this component
    AActor* owner = this->GetOwner();

    // Cast the owner to ADBDPlayer
    ADBDPlayer* player = Cast<ADBDPlayer>(owner);

    if (player == nullptr)
    {
        return false;
    }

    // Ensure the player is locally controlled (simulated by checking if it's the view target in some contexts)
    // The decompiled code calls a virtual function `BecomeViewTarget` which implies checking local ownership or view
    if (player->IsLocallyControlled() == false)
    {
        return false;
    }

    // Retrieve the PlayerState from the player
    ADBDPlayerState* playerState = Cast<ADBDPlayerState>(player->PlayerState);

    if (playerState == nullptr)
    {
        return false;
    }

    // Get the character saved profile data from the GameInstance using the PlayerState
    FCharacterSavedProfileData* profileData = gameInstance->GetCharacterSavedProfileDataForPlayerState(playerState);

    if (profileData == nullptr)
    {
        return false;
    }

    // Initialize an array to hold the equipped perks based on the player's role
    TArray<FName> characterPerks;

    // Check the player's role (Camper vs Slasher)
    // The role offset is at 0x750 in the PlayerState based on the assembly
    EPlayerRole playerRole = playerState->GameRole;

    if (playerRole == EPlayerRole::VE_Camper)
    {
        characterPerks = profileData->CharacterLoadoutData.CamperPerks;
    }
    else
    {
        characterPerks = profileData->CharacterLoadoutData.SlasherPerks;
    }

    // Initialize counters for the validation process
    int32 matchingCharacterPerksCount = 0;
    int32 equippedValidPerksCount = 0;

    // Retrieve the current character ID being played
    int32 currentCharacterID = playerState->GetCurrentCharacterID();

    // Iterate through the equipped perks
    for (int32 i = 0; i < characterPerks.Num(); i = i + 1)
    {
        FName perkID = characterPerks[i];

        // Skip default/empty slots
        if (perkID == LoadoutSlotDefaultValue::LOCKED || perkID == LoadoutSlotDefaultValue::EMPTY || perkID == NAME_None)
        {
            continue;
        }

        // Increment the count of valid equipped perks
        equippedValidPerksCount++;

        // Retrieve the properties of the perk from the Design Tunables
        FPerkProperties* perkData = gameInstance->DesignTunables->GetPerkData(perkID, true);

        if (perkData != nullptr)
        {
            // Check if the perk's associated character ID matches the currently played character ID
            if (perkData->AssociatedPlayerIndex == currentCharacterID)
            {
                matchingCharacterPerksCount++;
            }
        }
    }

    // Verify if exactly 3 unique character perks are equipped and they match the current character
    // A character's unique teachable loadout consists of exactly 3 perks
    if (matchingCharacterPerksCount == 3 && equippedValidPerksCount == 3)
    {
        return true;
    }

    return false;
}

// starts at line 378
bool UPerkManager::HasPerkModifierOfType(EGameplayModifierType ModifierType) const
{
    // Create an array to hold the retrieved effects
    TArray<UGameplayModifierContainer*> modifiers;

    // Populate the array with all active effects across all sources (VE_All is equal to 4)
    this->GetAllEffects(modifiers, EGameplayModifierSource::VE_All);

    // Iterate through the array of retrieved modifiers
    for (int32 i = 0; i < modifiers.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentModifier = modifiers[i];

        // Ensure the pointer to the modifier is valid before accessing it
        if (currentModifier == nullptr)
        {
            continue;
        }

        // Verify that the UObject is valid and not pending kill
        // This replaces the standard inline GUObjectArray check from the decompiled code
        if (IsValid(currentModifier) == true)
        {
            // Check if the current container holds the specific modifier type
            if (currentModifier->HasModifierOfType(ModifierType) == true)
            {
                // Check if the modifier is currently active and applicable
                if (currentModifier->m_bIsApplicable == true)
                {
                    // The modifier type is found and applicable, immediately return true
                    return true;
                }
            }
        }
    }

    // Return false if no applicable modifiers of the specified type were found
    // The memory allocated by the TArray will be automatically freed by its destructor
    return false;
}

// starts at line 391
float UPerkManager::ApplyMetaModifiers(EGameplayModifierType modifierType, float modifierValue) const
{
    // Initialize the final modifier value with the starting value
    float finalModifierValue = modifierValue;

    // If the modifier type is in the specified range and its value is positive
    if (modifierType >= EGameplayModifierType::VE_RevealKillerInRange && modifierType <= EGameplayModifierType::VE_RevealAllSurvivorsInRange)
    {
        if (modifierValue > 0.0f)
        {
            // Apply a specific meta modifier (ID 0x8A) and add it to the base value
            finalModifierValue = this->GetPerkModifier(EGameplayModifierType::VE_ModifyAuraReadingRangeAdditive, EGameplayModifierSource::VE_All) + modifierValue;
        }
    }

    // If the modifier type is exactly 0x10 (16), apply a multiplicative scaling
    if (modifierType == EGameplayModifierType::VE_RevealToKillerInRange)
    {
        float multiplier = this->GetPerkModifier(EGameplayModifierType::VE_ModifyRevealSurvivorsInRange, EGameplayModifierSource::VE_All) + 1.0f;
        return finalModifierValue * multiplier;
    }

    // If the modifier type is in the range [10, 15] inclusive, check for aura disruption
    if (modifierType >= EGameplayModifierType::VE_RevealKillerInRange && modifierType <= EGameplayModifierType::VE_RevealAllSurvivorsInRange)
    {
        // Initialize an array to hold all active effects
        TArray<UGameplayModifierContainer*> combinedEffects;
        this->GetAllEffects(combinedEffects, EGameplayModifierSource::VE_All);

        bool bIsAuraReadingDisrupted = false;

        // Iterate through all effects to see if any disrupt aura reading
        for (int32 i = 0; i < combinedEffects.Num(); i = i + 1)
        {
            UGameplayModifierContainer* currentEffect = combinedEffects[i];

            // Ensure the pointer to the effect is valid before accessing it
            if (currentEffect == nullptr)
            {
                continue;
            }

            // Verify that the UObject is valid and not pending kill
            if (IsValid(currentEffect) == true)
            {
                // Check if the effect has the flag to disrupt aura reading (ID 0x2C)
                if (currentEffect->HasFlag(EGameplayModifierFlag::VE_DisruptAuraReading) == true)
                {
                    // Check if the disruptive effect is currently active and applicable
                    if (currentEffect->m_bIsApplicable == true)
                    {
                        bIsAuraReadingDisrupted = true;
                        break;
                    }
                }
            }
        }

        // If aura reading is disrupted, the modifier value is forced to zero
        if (bIsAuraReadingDisrupted == true)
        {
            finalModifierValue = 0.0f;
        }
    }

    // Check if the modifier type is either 0x9C or 0x9D
    if (modifierType == EGameplayModifierType::VE_ModifyStaticFieldMadnessGain || modifierType == EGameplayModifierType::VE_ModifyShockTherapyMadnessGain)
    {
        if (modifierValue > 0.0f)
        {
            // Apply a specific meta modifier (ID 0x5B) and add it to the base value
            finalModifierValue = finalModifierValue + this->GetPerkModifier(EGameplayModifierType::VE_SpecialTinkererBonus, EGameplayModifierSource::VE_All);
        }
    }

    return finalModifierValue;
}

// starts at line 476
UPerk* UPerkManager::GetPerk(FName id) const
{
    // Retrieve the owner of this component
    AActor* owner = this->GetOwner();
    
    // Initialize an array to hold the retrieved perks
    TArray<UPerk*> perks;

    // Verify the owner pointer is valid and not pending kill
    if (IsValid(owner) == true)
    {
        // Check if the owner implements the required interface
        if (owner->Implements(UPerkOwnerInterface::StaticClass()) == true)
        {
            // Cast the owner to the interface to call its specific methods
            IPerkOwnerInterface* perkOwnerInterface = Cast<IPerkOwnerInterface>(owner);
            
            if (perkOwnerInterface != nullptr)
            {
                // Call the interface method (offset 0x10 in vtable based on assembly) to get the active perks
                // The interface method populates the provided array reference
                perkOwnerInterface->AppendPerksTo(perks);
            }
        }
    }

    // Initialize the pointer to return as null, updating it only if a match is found
    UPerk* foundPerk = nullptr;

    // Iterate through the array of retrieved perks
    for (int32 i = 0; i < perks.Num(); i = i + 1)
    {
        UPerk* currentPerk = perks[i];

        // Ensure the pointer to the perk is valid before accessing it
        if (currentPerk == nullptr)
        {
            continue;
        }

        // Check if the perk's ID matches the requested ID
        // The ID is located at offset 0xF0 based on the assembly comparison
        if (currentPerk->ID == id)
        {
            // Set the return pointer to the matching perk and break the loop
            foundPerk = currentPerk;
            break;
        }
    }

    // Return the found perk, or nullptr if no match was found
    return foundPerk;
}

// starts at line 490
UStatusEffect* UPerkManager::GetStatusEffect(FName id) const
{
    // Retrieve the owner of this component
    AActor* owner = this->GetOwner();
    
    // Initialize an array to hold the retrieved status effects
    TArray<UStatusEffect*> statusEffects;

    // Verify the owner pointer is valid and not pending kill
    if (IsValid(owner) == true)
    {
        // Check if the owner implements the required interface
        if (owner->Implements(UPerkOwnerInterface::StaticClass()) == true)
        {
            // Cast the owner to the interface to call its specific methods
            IPerkOwnerInterface* perkOwnerInterface = Cast<IPerkOwnerInterface>(owner);
            
            if (perkOwnerInterface != nullptr)
            {
                // Call the interface method (offset 0x20 in vtable based on assembly) to get the active status effects
                // The interface method populates the provided array reference
                perkOwnerInterface->AppendStatusEffectsTo(statusEffects);
            }
        }
    }

    // Initialize the pointer to return as null, updating it only if a match is found
    UStatusEffect* foundEffect = nullptr;

    // Iterate through the array of retrieved status effects
    for (int32 i = 0; i < statusEffects.Num(); i = i + 1)
    {
        UStatusEffect* currentEffect = statusEffects[i];

        // Ensure the pointer to the effect is valid before accessing it
        if (currentEffect == nullptr)
        {
            continue;
        }

        // Check if the effect's ID matches the requested ID
        // The ID is located at offset 0xF0 based on the assembly comparison
        if (currentEffect->ID == id)
        {
            // Set the return pointer to the matching effect and break the loop
            foundEffect = currentEffect;
            break;
        }
    }

    // Return the found status effect, or nullptr if no match was found
    // The memory allocated by the TArray will be automatically freed by its destructor
    return foundEffect;
}

// starts at line 504
UGameplayModifierContainer* UPerkManager::GetSourceWithModifier(EGameplayModifierType ModifierType, EGameplayModifierSource filter) const
{
    // Create an array to hold the retrieved effects
    TArray<UGameplayModifierContainer*> modifiers;

    // Populate the array with all active effects across all sources (VE_All is equal to 4)
    // The decompiled code hardcodes VE_All (4) for the filter parameter in GetAllEffects despite the function signature
    this->GetAllEffects(modifiers, EGameplayModifierSource::VE_All);

    // Initialize the pointer to return as null, updating it only if a match is found
    UGameplayModifierContainer* foundSource = nullptr;

    // Iterate through the array of retrieved modifiers
    for (int32 i = 0; i < modifiers.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentModifier = modifiers[i];

        // Ensure the pointer to the modifier is valid before accessing it
        if (currentModifier == nullptr)
        {
            continue;
        }

        // Verify that the UObject is valid and not pending kill
        // This replaces the standard inline GUObjectArray check from the decompiled code
        if (IsValid(currentModifier) == true)
        {
            // Check if the current container holds the specific modifier type
            if (currentModifier->HasModifierOfType(ModifierType) == true)
            {
                // Check if the modifier is currently active and applicable
                if (currentModifier->m_bIsApplicable == true)
                {
                    // Set the return pointer to the matching modifier container and break the loop
                    foundSource = currentModifier;
                    break;
                }
            }
        }
    }

    // Return the found modifier container, or nullptr if no match was found
    return foundSource;
}

// starts at line 518
UGameplayModifierContainer* UPerkManager::GetSourceWithFlag(EGameplayModifierFlag PerkFlag, EGameplayModifierSource filter) const
{
    // Create an array to hold the retrieved effects
    TArray<UGameplayModifierContainer*> modifiers;

    // Populate the array with all active effects across all sources (VE_All is equal to 4)
    // The decompiled code hardcodes VE_All (4) for the filter parameter in GetAllEffects despite the function signature
    this->GetAllEffects(modifiers, EGameplayModifierSource::VE_All);

    // Initialize the pointer to return as null, updating it only if a match is found
    UGameplayModifierContainer* foundSource = nullptr;

    // Iterate through the array of retrieved modifiers
    for (int32 i = 0; i < modifiers.Num(); i = i + 1)
    {
        UGameplayModifierContainer* currentModifier = modifiers[i];

        // Ensure the pointer to the modifier is valid before accessing it
        if (currentModifier == nullptr)
        {
            continue;
        }

        // Verify that the UObject is valid and not pending kill
        // This replaces the standard inline GUObjectArray check from the decompiled code
        if (IsValid(currentModifier) == true)
        {
            // Check if the current container holds the specific gameplay flag
            if (currentModifier->HasFlag(PerkFlag) == true)
            {
                // Check if the modifier is currently active and applicable
                if (currentModifier->m_bIsApplicable == true)
                {
                    // Set the return pointer to the matching modifier container and break the loop
                    foundSource = currentModifier;
                    break;
                }
            }
        }
    }

    // Return the found modifier container, or nullptr if no match was found
    // The memory allocated by the TArray will be automatically freed by its destructor
    return foundSource;
}

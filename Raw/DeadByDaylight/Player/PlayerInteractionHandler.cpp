// starts at line 56
void UPlayerInteractionHandler::SortAvailableInteractions()
{
    // The pseudo code creates a predicate and assigns 'this' to it.
    // This implies that FInteractionComparer stores a pointer to the current UPlayerInteractionHandler instance.
    UPlayerInteractionHandler::FInteractionComparer Predicate(this);

    // SortInternal and TDereferenceWrapper are the unrolled template implementations of Unreal Engine's TArray::Sort.
    // We replace the underlying raw array data and count manipulation with the standard TArray helper function.
    this->InteractionsInZone.Sort(Predicate);
}

// starts at line 62
void UPlayerInteractionHandler::AddInteraction(UInteractionDefinition* interaction)
{
    // Check if the interaction pointer is valid
    if (interaction != nullptr)
    {
        // The GUObjectArray lookup and 0x20000000 (RF_PendingKill) flag check is the unrolled implementation of IsPendingKill()
        if (interaction->IsPendingKill() == false)
        {
            // The pseudo-code shows TArray::AddUniqueImpl, which is the unrolled standard AddUnique macro/function
            this->InteractionsInZone.AddUnique(interaction);

            // The IDA pseudo-code hallucinates AOnlineBeaconClient::GetNetConnection due to a signature/offset match.
            // Using GetInteractor() as it is the logical function for a UInteractionDefinition object in this context.
            UInteractor* interactor = interaction->GetInteractor();

            // Bind the interactor to handle its destruction
            this->BindForInteractorDestruction(interactor);

            // The pseudo-code shows unrolled FString construction and FGenericPlatformString::Convert
            FString logMessage = TEXT("AddInteraction");
            
            // Log the interaction
            this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 67);
        }
    }
}

// starts at line 72
void UPlayerInteractionHandler::RemoveInteraction(UInteractionDefinition* interaction)
{
    // Check if the interaction pointer is valid
    if (interaction != nullptr)
    {
        // The GUObjectArray lookup and 0x20000000 (RF_PendingKill) flag check is the unrolled implementation of IsPendingKill()
        if (interaction->IsPendingKill() == false)
        {
            // The pseudo-code shows TArray::RemoveAll with a lambda comparing the item.
            // This is the unrolled standard template implementation of TArray::Remove.
            this->InteractionsInZone.Remove(interaction);

            // The IDA pseudo-code hallucinates AOnlineBeaconClient::GetNetConnection due to a signature/offset match.
            // Using GetInteractor() as it is the logical function for a UInteractionDefinition object in this context.
            UInteractor* interactor = interaction->GetInteractor();

            // Unbind the interactor from destruction events since the interaction is being removed
            this->UnbindFromInteractorDestruction(interactor);

            // The pseudo-code shows unrolled FString construction and FGenericPlatformString::Convert
            FString logMessage = TEXT("RemoveInteraction");
            
            // Log the interaction removal
            this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 77);
        }
    }
}

// starts at line 82
void UPlayerInteractionHandler::BindForInteractorDestruction(UInteractor* interactor)
{
    // Verify that the interactor pointer is valid before proceeding
    if (interactor != nullptr)
    {
        // The GUObjectArray lookup and 0x20000000 flag check is the unrolled implementation of Unreal Engine's IsPendingKill()
        if (interactor->IsPendingKill() == false)
        {
            // Retrieve the interactable actor associated with this interactor
            AInteractable* interactable = interactor->GetInteractable();

            // The pseudo-code shows string parsing for the function name and manual interaction with the multicast delegate's invocation list.
            // This is the expanded compiler representation of Unreal Engine's RemoveDynamic macro.
            // We remove the binding first to ensure we do not create duplicate bound events.
            interactable->OnDestroyed.RemoveDynamic(this, &UPlayerInteractionHandler::CleanInteractionArray);

            // The pseudo-code shows another string parsing block followed by a call to __Internal_AddDynamic.
            // This is the expanded compiler representation of Unreal Engine's AddDynamic macro.
            // We bind the target function to the actor's destruction event.
            interactable->OnDestroyed.AddDynamic(this, &UPlayerInteractionHandler::CleanInteractionArray);
        }
    }
}

// starts at line 91
void UPlayerInteractionHandler::UnbindFromInteractorDestruction(UInteractor* interactor)
{
    // Verify that the interactor pointer is valid before proceeding
    if (interactor != nullptr)
    {
        // The GUObjectArray lookup and 0x20000000 flag check is the unrolled implementation of Unreal Engine's IsPendingKill()
        if (interactor->IsPendingKill() == false)
        {
            // Retrieve the interactable actor associated with this interactor
            AInteractable* interactable = interactor->GetInteractable();

            // Verify that the interactable pointer is valid
            if (interactable != nullptr)
            {
                // The pseudo-code checks IsPendingKill again and offset 0x140 (320) bit 2 (value 4).
                // In Unreal Engine 4, this offset and bit mask correspond to the AActor::bActorIsBeingDestroyed flag.
                if (interactable->IsPendingKill() == false && interactable->IsActorBeingDestroyed() == false)
                {
                    // The pseudo-code shows string parsing for the function name and manual interaction with the multicast delegate's invocation list (RemoveInternal, CompactInvocationList).
                    // This is the expanded compiler representation of Unreal Engine's RemoveDynamic macro.
                    interactable->OnDestroyed.RemoveDynamic(this, &UPlayerInteractionHandler::CleanInteractionArray);
                }
            }
        }
    }
}

// starts at line 99
int32 UPlayerInteractionHandler::NumInteractionsInZone() const
{
    // The disassembly accesses the value at offset 0x100.
    // The InteractionsInZone TArray is located at offset 0x0F8. 
    // In a standard Unreal Engine TArray, the ArrayNum field is located 8 bytes after the data pointer.
    // Therefore, this function returns the number of elements in the InteractionsInZone array.
    return this->InteractionsInZone.Num();
}

// starts at line 104
void UPlayerInteractionHandler::CleanInteractionArray(AActor* DestroyedActor)
{
    // Retrieve the name of the destroyed actor to include in the log
    FString actorName = DestroyedActor->GetFName().ToString();
    
    // Format the string for the log output. 
    // The dereference operator (*) is the standard UE approach to pass FString to a %s format specifier.
    FString logMessage = FString::Printf(TEXT("ActorDestroyed [%s]"), *actorName);
    
    // Log the destruction of the actor
    this->InteractionHandlerLogPvt(logMessage, nullptr, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 106);
    
    // Prepare and output the state log before cleaning up the array
    FString beforeLog = TEXT("CleanInteractionArray BEFORE");
    this->InteractionHandlerLogPvt(beforeLog, nullptr, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 107);
    
    // Print the debug information for interactions in the zone before the modification
    this->DebugPrintInteractionsInZone(false);
    
    // Remove all interactions related to the destroyed actor using a lambda predicate.
    // The underlying assembly captures the DestroyedActor to compare against the array elements.
    this->InteractionsInZone.RemoveAll([DestroyedActor](UInteractionDefinition* interaction)
    {
        // We do not have the exact lambda assembly, but this represents the logical implementation
        if (interaction != nullptr)
        {
            // Assuming the interaction's outer or owner is the actor that was destroyed
            return interaction->GetOuter() == DestroyedActor;
        }
        return false;
    });
    
    // Prepare and output the state log after cleaning up the array
    FString afterLog = TEXT("CleanInteractionArray AFTER");
    this->InteractionHandlerLogPvt(afterLog, nullptr, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 124);
    
    // Print the debug information for interactions in the zone after the modification
    this->DebugPrintInteractionsInZone(false);
}

// starts at line 173
bool UPlayerInteractionHandler::IsInInteractionPriorityCone(const UInteractionDefinition* interaction)
{
    // Ensure the interaction pointer is valid
    if (interaction != nullptr)
    {
        // Try to cast the OwnerPrivate to ADBDPlayer. 
        // The pseudo-code does a manual class tree check which is the expanded Cast<ADBDPlayer>() macro.
        ADBDPlayer* playerOwner = Cast<ADBDPlayer>(this->GetOwner());
        if (playerOwner != nullptr)
        {
            // The pseudo code is fetching a WeakObjectPtr stored at offset 0x115C. 
            // In the context of a player character and interaction, this is likely fetching an attached component like a camera.
            UCameraComponent* cameraComponent = playerOwner->_cachedPlayerCamera.Get();
            
            // Check if the camera component exists and is not pending kill
            if (cameraComponent != nullptr && cameraComponent->IsPendingKill() == false)
            {
                // The pseudo-code performs matrix/quaternion math to extract the forward vector of the camera.
                // We simplify this using standard Unreal Engine functions.
                FVector cameraForward = cameraComponent->GetForwardVector();

                // Get the horizontal (XY) representation of the camera forward vector
                FVector characterCameraForward = UDBDUtilities::GetXYVector(cameraForward);
                
                // The pseudo-code manually checks if the length squared is greater than a small threshold (0.0000000099999999) 
                // and then performs a fast inverse square root approximation to normalize the vector.
                if (characterCameraForward.SizeSquared() > KINDA_SMALL_NUMBER)
                {
                    characterCameraForward.Normalize();
                }

                // Get the world location of the interaction component
                FVector interactionLocation = interaction->GetComponentLocation();
                
                // Get the actor location of the player owner
                FVector playerLocation = playerOwner->K2_GetActorLocation();

                // Calculate the direction vector from the player to the interaction
                FVector vectorToTarget = interactionLocation - playerLocation;

                // Get the horizontal (XY) representation of the direction vector
                FVector normalCharacterToTarget = UDBDUtilities::GetXYVector(vectorToTarget);

                // Normalize the direction vector using the same threshold and approximation logic
                if (normalCharacterToTarget.SizeSquared() > KINDA_SMALL_NUMBER)
                {
                    normalCharacterToTarget.Normalize();
                }

                // The pseudo code calculates the dot product between the two normalized vectors, 
                // gets the arccosine to find the angle in radians, and multiplies by 57.295776 (180/PI) to convert to degrees.
                float dotProduct = FVector::DotProduct(normalCharacterToTarget, characterCameraForward);
                float angleDegrees = FMath::Acos(dotProduct) * (180.f / PI);

                // Check if the angle is within the priority cone angle defined in the handler
                return angleDegrees < this->PriorityConeAngle;
            }
        }
    }
    
    return false;
}

// starts at line 196
void UPlayerInteractionHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    // Call the parent class TickComponent
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Check if the OwningCharacter weak pointer is valid
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();
        
        // The pseudo-code calls offset 0x630 (1584).
        // If it returns true, we proceed with ticking the interaction handler logic.
        if (owningCharacter->IsLocallyControlled() == true)
        {
            // Build the interaction keys for the player
            owningCharacter->BuildInteractionKeys();

            // The pseudo-code explicitly shows the unrolled TArray::Sort logic
            // We use the comparer struct to sort the interactions in the zone
            UPlayerInteractionHandler::FInteractionComparer comparer(this);
            this->InteractionsInZone.Sort(comparer);

            // The pseudo code is clearing the TMap _availableInteractions by setting its array count to 0 and reallocating its internal BitArray.
            // This is the expanded standard implementation of TMap::Empty()
            this->_availableInteractions.Empty();
            
            // The pseudo-code checks offset 0x0E18 (3608) on ADBDPlayer, checking IsPendingKill and IsActorBeingDestroyed, then calling a virtual function at offset 0x1070 (4208).
            // This corresponds to checking the player's current state, such as IsInInteractionState() or a similar gameplay check.
            ADBDPlayer* interactingPlayer = owningCharacter->_interactingPlayer;
            if (interactingPlayer == nullptr || IsValid(interactingPlayer) == false || interactingPlayer->IsValidImpl() == false)
            {
                // Iterate through all possible EInputInteractionType enum values (1 to 30)
                // The pseudo-code shows a loop from 1 to 31 (exclusive) skipping 3.
                for (uint8 interactionTypeIdx = 1; interactionTypeIdx < 31; ++interactionTypeIdx)
                {
                    // EInputInteractionType::None or a specific internal type is likely 3, which is skipped
                    if (interactionTypeIdx != EInputInteractionType::VE_ItemInteract)
                    {
                        EInputInteractionType interactionType = static_cast<EInputInteractionType>(interactionTypeIdx);
                        this->RefreshFirstAvailableInteractionOfType(interactionType);
                    }
                }
            }

            // Transfer the buffered request states to the current request states
            bool previousInteractionRequestBuffer = this->_hasInteractionRequestInBuffer;
            bool previousInterruptionRequestBuffer = this->_hasInterruptionRequestInBuffer;
            
            // Clear the buffers
            this->_hasInteractionRequestInBuffer = false;
            this->_hasInterruptionRequestInBuffer = false;

            // Apply the interaction request
            this->_hasInteractionRequest = previousInteractionRequestBuffer;
            
            // Handle the interruption request if one was buffered
            if (previousInterruptionRequestBuffer == true)
            {
                // Check if the owning character is the one interrupting
                if (this->_requestedInterruptionIsInterruptor == true)
                {
                    this->_hasInterruptionRequest = true;
                }
                else
                {
                    // The pseudo code checks two state variables on the owning character to determine if the interruption was successful.
                    // This is likely checking if the character can be interrupted (e.g., CanBeInterrupted() or a state flag).
                    bool canBeInterrupted = owningCharacter->IsInterruptable && owningCharacter->CheatIsInterruptable;
                    this->_hasInterruptionRequest = canBeInterrupted;
                    
                    // Answer the interruption request to the server
                    this->Server_AnswerInterruption_Interruptee(canBeInterrupted);
                }
            }
        }

        // The pseudo-code checks offset 0x110 (272) on ADBDPlayer.
        // In Unreal Engine 4 multiplayer, offset 0x110 on AActor corresponds to the LocalRole (ENetRole).
        // A value of 3 represents ROLE_Authority.
        if (owningCharacter->Role == ROLE_Authority)
        {
            this->AuthorityUpdateInteractionWaiting(DeltaTime);
        }
    }
}

// starts at line 258
void UPlayerInteractionHandler::AuthorityUpdateInteractionWaiting(float DeltaTime)
{
    // Check if the OwningCharacter is valid
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();

        // The pseudo-code explicitly checks the LocalRole (ENetRole) of the owning character at offset 0x110 (272).
        // A value of 3 corresponds to ROLE_Authority.
        // Update the timer if it is not done
        if (owningCharacter->GetLocalRole() == ROLE_Authority && this->_waitForOtherInteractionEndTimer.IsDone() == false)
        {
            this->_waitForOtherInteractionEndTimer.Update(DeltaTime);

            // Ensure the requested interaction is valid and not pending kill
            if (this->_requestedInteraction != nullptr && this->_requestedInteraction->IsPendingKill() == false)
            {
                // The pseudo code fetches the UInteractor and checks if it is valid and not pending kill.
                // This mirrors the logic in previous functions where we use GetInteractor().
                UInteractor* interactor = this->_requestedInteraction->GetInteractor();
                
                if (interactor != nullptr && interactor->IsPendingKill() == false)
                {
                    // Update player dependencies before checking if the interaction can proceed
                    this->_requestedInteraction->GetPlayerDependancy(owningCharacter);

                    // The interaction can start if the interactor is free and player dependencies are locked
                    if (interactor->IsInteracting() == false && this->ArePlayerDependenciesLocked() == true)
                    {
                        // Stop the waiting timer
                        UDBDUtilTimer::Stop(this->_waitForOtherInteractionEndTimer);

                        // Check if the interaction is still possible for the player
                        bool isPossible = this->_requestedInteraction->IsInteractionPossible(owningCharacter, this->_requestedInteractionType, false);
                        
                        if (isPossible == true)
                        {
                            // Log the beginning of the interaction
                            FString beginLog = TEXT("Interactor Released, now Beginning Interaction");
                            this->InteractionHandlerLogPvt(beginLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 283);

                            // Begin the interaction
                            this->_requestedInteraction->BeginInteraction(owningCharacter, this->_requestedInteractionType);
                            
                            // Broadcast success
                            this->Multicast_InteractionRequestResult(true);
                            return;
                        }
                        else
                        {
                            // Log that the interaction is no longer possible
                            FString noLongerPossibleLog = TEXT("Interactor Released, but interaction no longer possible");
                            this->InteractionHandlerLogPvt(noLongerPossibleLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 289);
                        }
                    }
                    else
                    {
                        // If the timer is not done, we continue waiting
                        if (this->_waitForOtherInteractionEndTimer.IsDone() == false)
                        {
                            return;
                        }

                        // The timer finished while we were still waiting
                        FString timedOutLog = TEXT("Timed out while waiting for interactor to be released for interaction");
                        this->InteractionHandlerLogPvt(timedOutLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 302);
                    }
                }
            }
            
            // If we reach this point, either the timer stopped due to an error, we timed out, or the interaction was no longer possible.
            // We stop the timer just in case it wasn't already stopped by the success path.
            UDBDUtilTimer::Stop(this->_waitForOtherInteractionEndTimer);
            
            // Broadcast failure
            this->Multicast_InteractionRequestResult(false);
            
            // Clean up dependencies and cancel the reservation on the interactor
            this->ClearPlayerDependencies();
            
            if (this->_requestedInteraction != nullptr)
            {
                UInteractor* cancelInteractor = this->_requestedInteraction->GetInteractor();
                if (cancelInteractor != nullptr)
                {
                    cancelInteractor->Authority_CancelReservation(owningCharacter);
                }
            }
        }
    }
}

// starts at line 325
UInteractionDefinition* UPlayerInteractionHandler::FindFirstAvailableInteractionOfTypeOnInteractor(EInputInteractionType interactionType, UInteractor* interactor, bool performClientCheck, TFunction<bool(UInteractionDefinition*, UInteractionDefinition*)> filter, UInteractionDefinition* ongoingInteraction)
{
    // The pseudo code is heavily convoluted with TFunction destructor calls due to pass-by-value in Unreal Engine.
    // In Unreal Engine 4, passing TFunction by value requires moving/copying and destroying it at the end of the scope.
    // We omit the manual destructor calls in the reconstructed C++ code as the compiler handles them automatically.

    // Check if the OwningCharacter is valid
    if (this->OwningCharacter.IsValid() == true)
    {
        // Ensure the interactor is valid and not pending kill
        if (interactor != nullptr && interactor->IsPendingKill() == false)
        {
            // Get all interaction definitions associated with the interactor
            TArray<UInteractionDefinition*> interactionDefinitions = interactor->GetInteractionDefinitions();
            
            // Iterate through all interactions
            for (int32 i = 0; i < interactionDefinitions.Num(); ++i)
            {
                UInteractionDefinition* interaction = interactionDefinitions[i];
                
                // Evaluate the provided filter function. The pseudo code shows an intricate check for the callable state.
                if (filter(interaction, ongoingInteraction) == true)
                {
                    ADBDPlayer* owningCharacter = this->OwningCharacter.Get();
                    
                    // Check if the interaction is possible
                    if (interaction->IsInteractionPossible(owningCharacter, interactionType, performClientCheck) == true)
                    {
                        return interaction;
                    }
                }
            }
        }
    }

    return nullptr;
}

// starts at line 352
bool UPlayerInteractionHandler::IsSwapPossible(UInteractionDefinition* oldInteraction, UInteractionDefinition* interaction) const
{
    // Verify that the old interaction pointer is valid
    if (oldInteraction != nullptr && oldInteraction->IsPendingKill() == false)
    {
        // Verify that the new interaction pointer is valid
        if (interaction != nullptr && interaction->IsPendingKill() == false)
        {
            // Check if the new interaction is allowed to override the old interaction
            if (interaction->CanOverrideInteraction(oldInteraction) == true)
            {
                // Ensure we are not trying to swap an interaction with itself
                if (oldInteraction != interaction)
                {
                    // Retrieve the owning character
                    ADBDPlayer* owningCharacter = Cast<ADBDPlayer>(this->OwningCharacter.Get());
                    
                    // Check if the character is valid
                    if (owningCharacter != nullptr)
                    {
                        // The pseudo-code accesses offset 0x1021.
                        if (owningCharacter->_isSwappingInteraction == false)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    
    return false;
}

// starts at line 358
bool UPlayerInteractionHandler::TryInteraction(UInteractionDefinition* interaction, EInputInteractionType interactionType, ADBDPlayer* requester, bool allowSwap)
{
    // Ensure the owning character is valid
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();
        
        // The pseudo-code calls a virtual function at offset 0x630 on the object.
        if (owningCharacter->IsLocallyControlled() == true)
        {
            // Verify the game is still active
            if (owningCharacter->HasGameEnded() == false)
            {
                // Check if an interaction is currently ongoing
                if (this->_currentInteraction != nullptr && this->_currentInteraction->IsPendingKill() == false)
                {
                    FString logMessage = TEXT("TryInteraction detected an interaction already ongoing");
                    this->InteractionHandlerLogPvt(logMessage, this->_currentInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 370);
                }

                // Check if an interaction is already requested
                if (this->_requestedInteraction != nullptr && this->_requestedInteraction->IsPendingKill() == false)
                {
                    FString logMessage = TEXT("TryInteraction detected an interaction already requested");
                    this->InteractionHandlerLogPvt(logMessage, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 374);
                }

                // The pseudo code checks two offset bits on the owning character (0x1021 and 0xC04).
                // Offset 0x1021 maps to bIsInteractionInputBlocked and 0xC04 likely maps to bIsInteracting or a similar state flag based on previous checks.
                bool isInteracting = owningCharacter->IsInteracting;
                bool isSwappingInteraction = owningCharacter->_isSwappingInteraction;

                // If the player is already interacting or input is blocked, we check if swapping is allowed and possible.
                if ((isInteracting == true || isSwappingInteraction == true) && (allowSwap == false || this->IsSwapPossible(this->_currentInteraction, interaction) == false))
                {
                    // The log message dynamically inserts values based on the failed checks
                    FString logMessage = FString::Printf(TEXT("TryInteraction Fails - CharacterInterating %d - CharacterSwappingInteraction %d - AllowSwap %d - SwapPossible %d"), 
                                                         isInteracting, 
                                                         isSwappingInteraction, 
                                                         allowSwap, 
                                                         this->IsSwapPossible(this->_currentInteraction, interaction));

                    this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 381);
                    return false;
                }

                // Check if another request is already in the pipeline (either pending or in the buffer)
                if ((this->_hasInteractionRequest == true || this->_hasInteractionRequestInBuffer == true) && 
                    (allowSwap == false || this->IsSwapPossible(this->_requestedInteraction, interaction) == false))
                {
                    FString logMessage = TEXT("TryInteraction Fails because another interaction is requested");
                    this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 390);
                    return false;
                }

                // Ensure the requested interaction is valid and not pending kill
                if (interaction == nullptr || interaction->IsPendingKill() == true)
                {
                    FString logMessage = TEXT("TryInteraction fails because no valid interaction of this type was found 2");
                    this->InteractionHandlerLogPvt(logMessage, nullptr, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 423);
                    return false;
                }

                // Check if the interaction is actually possible for the owning character
                if (interaction->IsInteractionPossible(owningCharacter, interactionType, true) == false)
                {
                    FString logMessage = TEXT("TryInteraction fails because interaction not possible");
                    this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 418);
                    return false;
                }

                // Interaction is valid and possible, configure the state to request it
                this->_requestedInteractionType = interactionType;
                this->_hasInteractionRequestInBuffer = true;
                
                // Initialize the player properties for the interaction
                FInteractionPlayerProperties playerProperties;
                this->FillInInteractionPlayerProperties(interaction, playerProperties, requester);
                
                // Broadcast the updated properties locally
                this->Local_BroadcastInteractionPlayerProperties(playerProperties);
                
                this->_requestedInteraction = interaction;

                // Log the successful attempt
                FString successLog = TEXT("TryInteraction SUCCESS");
                this->InteractionHandlerLogPvt(successLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 410);

                // Inform the server of the requested interaction
                this->Server_StoreInteraction(this->_requestedInteractionType, this->_requestedInteraction);

                return true;
            }
        }
    }
    
    return false;
}

// starts at line 429
bool UPlayerInteractionHandler::TryFirstAvailableInteractionOfType(EInputInteractionType interactionType, ADBDPlayer* requester)
{
    // The pseudo code checks the boolean variable at offset 0x30C. 
    // This maps to _scanForInteractions.
    // If scanning for interactions is disabled, return false immediately.
    if (this->_scanForInteractions == false)
    {
        return false;
    }

    // Try to get the available interaction of the requested type
    UInteractionDefinition* availableInteraction = this->GetAvailableInteraction(interactionType);

    // Ensure the interaction exists
    if (availableInteraction != nullptr)
    {
        // The GUObjectArray lookup and 0x20000000 flag check is the unrolled implementation of Unreal Engine's IsPendingKill()
        // Make sure the interaction is not marked for destruction
        if (availableInteraction->IsPendingKill() == false)
        {
            // The pseudo code fetches the available interaction again, but we can reuse the pointer we already validated.
            // Attempt the interaction with allowSwap explicitly set to false (0).
            return this->TryInteraction(availableInteraction, interactionType, requester, false);
        }
    }

    return false;
}

// starts at line 459
void UPlayerInteractionHandler::RefreshFirstAvailableInteractionOfType(EInputInteractionType interactionType)
{
    // Ensure the owning character is valid
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();
        
        // Remove the existing cached interaction for this specific type to recalculate it
        // The pseudo-code shows an explicit TSet::Remove call accessing the interactionType as an offset.
        this->_availableInteractions.Remove(owningCharacter->InteractionKeys[interactionType]);

        // Only proceed if scanning is enabled
        if (this->_scanForInteractions == true)
        {
            UInteractionDefinition* ongoingInteraction = nullptr;

            // Check if the player is currently interacting.
            if (owningCharacter->IsInteracting == true)
            {
                if (this->_currentInteraction != nullptr && this->_currentInteraction->IsPendingKill() == false)
                {
                    // If an interaction is currently in progress, we must check if we can override it.
                    ongoingInteraction = this->_currentInteraction;
                }
            }

            // Check if the handler has a specific interactor attached to it.
            if (this->_attachedInteractor.IsValid() == true)
            {
                UInteractor* attachedInteractor = this->_attachedInteractor.Get();                
                UInteractionDefinition* foundInteraction = nullptr;

                // If a filter is already defined (from the swapping logic), use it. 
                // Otherwise, use a default filter that accepts all interactions.
                if (ongoingInteraction != nullptr)
                {
                    auto filterFunc = TFunction<bool(UInteractionDefinition*, UInteractionDefinition*)>(
                        [](UInteractionDefinition* interaction, UInteractionDefinition* ongoing) { return interaction->CanOverrideInteraction(ongoing); }
                    );
                    // Find the first available interaction on the attached interactor
                    foundInteraction = this->FindFirstAvailableInteractionOfTypeOnInteractor(interactionType, attachedInteractor, true, filterFunc, ongoingInteraction);
                }
                else
                {
                    auto filterFunc = TFunction<bool(UInteractionDefinition*, UInteractionDefinition*)>(
                        [](UInteractionDefinition*, UInteractionDefinition*) { return true; }
                    );
                    // Find the first available interaction on the attached interactor
                    foundInteraction = this->FindFirstAvailableInteractionOfTypeOnInteractor(interactionType, attachedInteractor, true, filterFunc, ongoingInteraction);
                }
                
                
                if (foundInteraction != nullptr && foundInteraction->IsPendingKill() == false)
                {
                    // Cache the found interaction
                    this->SetAvailableInteraction(interactionType, foundInteraction);
                    return;
                }
            }
            else
            {
                // If no specific interactor is attached, search through all interactions currently in the zone.
                
                // Check if the player is equipping an item.
                // The pseudo code accesses offset 0x4F (79) and calls IsEquipped.
                // We simplify this to a direct check on the owning character.
                bool hasEquippedItem = false;
                ACollectable* equippedItem = nullptr;
                if (owningCharacter->Inventory.Num() > 0)
                {
                    equippedItem = owningCharacter->Inventory[0];
                }

                if (equippedItem != nullptr && equippedItem->IsEquipped() == true)
                {
                    hasEquippedItem = true;
                }

                // Iterate through all interactions in the zone
                for (int32 i = 0; i < this->InteractionsInZone.Num(); ++i)
                {
                    UInteractionDefinition* currentZoneInteraction = this->InteractionsInZone[i];
                    
                    // Skip if the interaction is pending kill or not in the priority cone
                    if (currentZoneInteraction != nullptr && currentZoneInteraction->IsPendingKill() == false && this->IsInInteractionPriorityCone(currentZoneInteraction) == true)
                    {
                        // If the player has an equipped item, we need to check the interactions on that item first.
                        if (hasEquippedItem == true)
                        {
                            UInteractor* itemInteractor = equippedItem->GetItemInteractor();
                            if (itemInteractor != nullptr && itemInteractor->IsPendingKill() == false)
                            {
                                // Define the filter function for the item interactor
                                auto filterFunc = ongoingInteraction != nullptr ? 
                                    TFunction<bool(UInteractionDefinition*, UInteractionDefinition*)>(
                                        [](UInteractionDefinition* interaction, UInteractionDefinition* ongoing) { return interaction->CanOverrideInteraction(ongoing); }
                                    ) : 
                                    TFunction<bool(UInteractionDefinition*, UInteractionDefinition*)>(
                                        [](UInteractionDefinition*, UInteractionDefinition*) { return true; }
                                    );

                                // Search on the item's interactor
                                UInteractionDefinition* foundInteraction = this->FindFirstAvailableInteractionOfTypeOnInteractor(interactionType, itemInteractor, true, filterFunc, ongoingInteraction);
                                
                                if (foundInteraction != nullptr && foundInteraction->IsPendingKill() == false)
                                {
                                    // Cache the found interaction
                                    this->SetAvailableInteraction(interactionType, foundInteraction);
                                    return;
                                }
                            }
                        }

                        // If no item interaction was found, or no item is equipped, evaluate the zone interaction.
                        // We use the filter to determine if the zone interaction is valid.
                        bool filterResult = true;
                        if (ongoingInteraction != nullptr)
                        {
                            filterResult = currentZoneInteraction->CanOverrideInteraction(ongoingInteraction);
                        }

                        if (filterResult == true)
                        {
                            // Check if the interaction is possible
                            if (currentZoneInteraction->IsInteractionPossible(owningCharacter, interactionType, true) == true)
                            {
                                // Cache the found interaction
                                this->SetAvailableInteraction(interactionType, currentZoneInteraction);
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

// starts at line 551
void UPlayerInteractionHandler::Server_StoreInteraction_Implementation(EInputInteractionType interactionType, UInteractionDefinition* interaction)
{
    // Verify that the owning character is valid
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();

        // Check if the current net role is Authority (Server).
        // The pseudo-code explicitly checks the LocalRole (ENetRole) of the owning character at offset 0x110 (272).
        // A value of 3 corresponds to ROLE_Authority.
        if (owningCharacter->Role == ROLE_Authority)
        {
            // If we are currently waiting on another interaction to end, handle the interruption
            if (this->_waitForOtherInteractionEndTimer.IsDone() == false)
            {
                // Log that an interaction was requested while waiting on another
                FString previousLog = TEXT("<=!=> Interaction requested while waiting on another! PREVIOUS Interaction: ");
                this->InteractionHandlerLogPvt(previousLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 561);

                FString newLog = TEXT("<=!=> Interaction requested while waiting on another! NEW Interaction: ");
                this->InteractionHandlerLogPvt(newLog, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 562);

                // Stop the waiting timer
                UDBDUtilTimer::Stop(this->_waitForOtherInteractionEndTimer);

                // If we had a previously requested interaction that is not pending kill, fail it
                if (this->_requestedInteraction != nullptr && this->_requestedInteraction->IsPendingKill() == false)
                {
                    this->Multicast_InteractionRequestResult(false);
                    this->ClearPlayerDependencies();
                }
            }

            // Log the storage implementation
            FString implementationLog = TEXT("Server_StoreInteraction_Implementation");
            this->InteractionHandlerLogPvt(implementationLog, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 570);

            // Replicate the stored interaction to clients via a Multicast call
            this->Multicast_StoreInteraction(interactionType, interaction);
        }
    }
}

// starts at line 581
void UPlayerInteractionHandler::Multicast_StoreInteraction_Implementation(EInputInteractionType interactionType, UInteractionDefinition* interaction)
{
    // Check if the OwningCharacter weak pointer is valid
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();

        // The pseudo-code calls a virtual function at offset 0x630 on the object. 
        if (owningCharacter->IsLocallyControlled() == false)
        {
            // Log the execution of the multicast implementation
            FString logMessage = TEXT("Multicast_StoreInteraction_Implementation");
            this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 589);

            // Store the interaction details locally on the client
            this->_requestedInteraction = interaction;
            this->_requestedInteractionType = interactionType;
        }
    }
}

// starts at line 601
void UPlayerInteractionHandler::RequestAndBeginInteraction()
{
    // Check if the OwningCharacter is valid
    if (this->OwningCharacter.IsValid() == true)
    {
        // Log the action of requesting and beginning the interaction
        FString logMessage = TEXT("RequestAndBeginInteraction");
        this->InteractionHandlerLogPvt(logMessage, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 608);

        // Transition requested state to current state
        this->_currentInteraction = this->_requestedInteraction;
        this->_currentInteractionType = this->_requestedInteractionType;
        
        // Save the properties of the current interaction
        this->_currentInteractionProperties = this->_currentInteraction->SaveProperties();

        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();
        
        // Ensure the owning character is initialized and active.
        // The pseudo-code calls offset 0x630 (1584).
        if (owningCharacter->IsLocallyControlled() == true)
        {
            this->_currentInteractionChargeCompleted = false;
            this->_requestedInteractionState = ERequestState::VE_Pending;

            // Fetch the world and timer manager. The pseudo code shows standard fallback logic if WorldPrivate is null.
            UWorld* world = this->GetWorld();
            if (world != nullptr)
            {
                FTimerManager& timerManager = world->GetTimerManager();

                // Clear any existing timer
                timerManager.ClearTimer(this->_skillCheckTimerHandle);

                // Set the skill check timer to trigger at the defined frequency
                // The pseudo-code shows the unrolled macro for SetTimer (Delegate binding and InternalSetTimer)
                timerManager.SetTimer(this->_skillCheckTimerHandle, this, &UPlayerInteractionHandler::OnSkillCheckTimerTrigger, this->kSkillCheckTriggerFrequency, true);
            }

            // Clear the progress-based skill checks array
            this->_progressBasedSkillChecks.Empty();

            // Populate the progress-based skill checks array based on the interaction's predefined thresholds
            for (int32 i = 0; i < this->_currentInteraction->ProgressBasedSkillChecks.Num(); ++i)
            {
                float skillCheckProgress = this->_currentInteraction->ProgressBasedSkillChecks[i];
                
                // Only add skill checks that are above the current charge percent
                if (skillCheckProgress > this->_currentInteraction->GetChargePercent())
                {
                    this->_progressBasedSkillChecks.Add(skillCheckProgress);
                }
            }

            // Inform the server about the requested interaction
            this->Server_RequestInteraction();
        }
    }
}

// starts at line 643
bool UPlayerInteractionHandler::HasAvailableInteraction(EInputInteractionType interactionType)
{
    // The pseudo code checks the boolean variable at offset 0x30C. 
    // This maps to _scanForInteractions.
    if (this->_scanForInteractions == true)
    {
        // Get the available interaction for the specified type
        UInteractionDefinition* availableInteraction = this->GetAvailableInteraction(interactionType);
        
        // Ensure the interaction exists
        if (availableInteraction != nullptr && availableInteraction->IsPendingKill() == false)
        {
            return true;
        }
    }
    
    return false;
}

// starts at line 648
UInteractionDefinition* UPlayerInteractionHandler::GetAvailableInteraction(EInputInteractionType interactionType) const
{
    // The pseudo code checks if the interaction type refresh should be skipped. 
    // This is likely checking a condition where the interaction isn't allowed to be refreshed right now.
    if (this->ShouldSkipRefreshForAction(interactionType) == true)
    {
        return nullptr;
    }

    // Attempt to get the player from the weak pointer
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();

        // The pseudo code looks up an FName from an array of Names stored on the ADBDPlayer using the interactionType as an index.
        // It then uses this FName to look up a value in the _availableInteractions TMap.
        FName interactionName = owningCharacter->InteractionKeys[interactionType];

        // The pseudo code contains the unrolled implementation of Unreal Engine's TMap::Find() logic, complete with hashing and iterating over elements.
        // The _availableInteractions field is defined in the class structure as a TMap where the key is FName and value is a TWeakObjectPtr to an interaction definition.
        const TWeakObjectPtr<UInteractionDefinition>* foundWeakPtr = this->_availableInteractions.Find(interactionName);

        // Check if the map returned a valid entry
        if (foundWeakPtr != nullptr)
        {
            // Ensure the weak pointer itself is valid
            if (foundWeakPtr->IsValid() == true)
            {
                // Return the raw pointer
                return foundWeakPtr->Get();
            }
        }
    }

    return nullptr;
}

// starts at line 665
bool UPlayerInteractionHandler::ShouldSkipRefreshForAction(EInputInteractionType inputType) const
{
    // Validate the OwningCharacter weak pointer before attempting to access it
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();

        // The pseudo code is attempting to access an array of FNames or similar mapping at offset 0x1198.
        // It uses the inputType as an index into this array. 
        FName targetActionName = owningCharacter->InteractionKeys[inputType];

        // Check if the mapped name is "None" (which evaluates to 0 or null).
        // If the target action name is "None" or the input type is 0 (None), we do not skip.
        if (targetActionName == NAME_None || inputType == EInputInteractionType::VE_None)
        {
            return false;
        }

        // Loop through all input types prior to the target inputType
        for (uint8 i = 0; i < static_cast<uint8>(inputType); ++i)
        {
            // Check if any previous input type maps to the exact same action name
            FName previousActionName = owningCharacter->InteractionKeys[i];

            if (targetActionName == previousActionName)
            {
                // If an earlier interaction type maps to the same action, skip refreshing for this later one to avoid redundant processing.
                return true;
            }
        }
    }

    return false;
}

// starts at line 684
void UPlayerInteractionHandler::GetAvailableInteractions(TArray<UInteractionDefinition*>& result) const
{
    // The pseudo code is heavily obfuscated due to the unrolling of the TMap iterator (TConstIterator).
    // The massive blocks of bitwise operations (e.g., _BitScanReverse, BitArrayIt, UnvisitedBitMask) 
    // represent standard Unreal Engine iteration over a sparse array underlying the TMap.
    
    // We can simplify the entire bitwise traversal by iterating over the map using standard C++11 range-based for loop.
    for (const auto& kvp : this->_availableInteractions)
    {
        // The value in the key-value pair is a TWeakObjectPtr<UInteractionDefinition>.
        // Check if the weak pointer is valid.
        if (kvp.Value.IsValid() == true)
        {
            // Get the underlying raw pointer
            UInteractionDefinition* interaction = kvp.Value.Get();

            // The pseudo code does a manual linear search through the 'result' array to check for uniqueness.
            // "while ( *(UObject **)Data != v26 )" -> If it reaches the end, it adds the item.
            // This is the unrolled implementation of TArray::AddUnique.
            result.AddUnique(interaction);
        }
    }
}

// starts at line 696
void UPlayerInteractionHandler::SetAvailableInteraction(EInputInteractionType interactionType, UInteractionDefinition* interaction)
{
    // The pseudo code is removing an entry from a map using the interactionType to fetch the corresponding FName key from the owning character.
    // Ensure the owning character is valid before proceeding.
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();

        // Get the specific FName key mapped to this interaction type
        FName interactionKey = owningCharacter->InteractionKeys[interactionType];
        
        // The pseudo code shows TSet::Remove, which is the underlying container implementation for TMap::Remove.
        this->_availableInteractions.Remove(interactionKey);
        
        // Check if the new interaction is valid
        if (interaction != nullptr && interaction->IsPendingKill() == false)
        {
            // The pseudo code shows TSet::Emplace with TPairInitializer, which is the underlying implementation of TMap::Add or TMap::Emplace.
            // The value is constructed as a TWeakObjectPtr.
            this->_availableInteractions.Add(interactionKey, interaction);
        }
    }
}

// starts at line 706
bool UPlayerInteractionHandler::IsAvailableInteractionChargeable(EInputInteractionType interactionType)
{
    // Retrieve the available interaction for the given interaction type
    UInteractionDefinition* availableInteraction = this->GetAvailableInteraction(interactionType);

    // The original assembly lacks a null check and performs a direct virtual tail call.
    // For functional and safe C++ code, we verify the pointer before making the call.
    if (availableInteraction != nullptr)
    {
        // The disassembly performs a direct tail call to the virtual function at vtable offset 0x528.
        // The pseudo-code identifies this function as IsChargeable.
        return availableInteraction->IsChargeable();
    }

    return false;
}

// starts at line 712
float UPlayerInteractionHandler::GetAvailableInteractionChargePercent(EInputInteractionType interactionType)
{
    // Retrieve the available interaction for the given interaction type
    UInteractionDefinition* availableInteraction = this->GetAvailableInteraction(interactionType);

    // The original assembly lacks a null check and performs a direct virtual tail call.
    // For functional and safe C++ code, we verify the pointer before making the call.
    if (availableInteraction != nullptr)
    {
        // The disassembly performs a direct tail call to the virtual function at vtable offset 0x530.
        // The pseudo-code identifies this function as GetChargePercent.
        return availableInteraction->GetChargePercent();
    }

    return 0.0f;
}

// starts at line 719
void UPlayerInteractionHandler::Server_RequestInteraction_Implementation()
{
    // Validate the OwningCharacter weak pointer
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();

        // The pseudo code checks offset 0xE18 (3608) on ADBDPlayer. 
        // In the context of interactions, this is likely checking if the player is currently in an interaction.
        ADBDPlayer* interactingPlayer = owningCharacter->_interactingPlayer;
        
        if (interactingPlayer != nullptr)
        {
            if (interactingPlayer->IsPendingKill() == false && interactingPlayer->IsActorBeingDestroyed() == false && owningCharacter->IsValidImpl() == true)
            {
                // If the verbosity is high enough (LogSM >= 2), log a warning.
                UE_LOG(LogSM, Warning, TEXT("!!!Server_RequestInteraction FAIL - Character is being interacted on!!!"));
                
                // Broadcast the failure to clients
                this->Multicast_InteractionRequestResult(false);
                return;
            }
        }
        
        // Log the request
        FString requestLog = TEXT("Server_RequestInteraction");
        this->InteractionHandlerLogPvt(requestLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 730);

        // Ensure the requested interaction is valid and not pending kill
        if (this->_requestedInteraction == nullptr || this->_requestedInteraction->IsPendingKill() == true)
        {
            UE_LOG(LogSM, Warning, TEXT("!!!Server_RequestInteraction FAIL - Invalid Interaction or interactor!!!"));
            this->Multicast_InteractionRequestResult(false);
            return;
        }

        // Get the interactor from the requested interaction
        UInteractor* interactor = this->_requestedInteraction->GetInteractor();
        if (interactor == nullptr || interactor->IsPendingKill() == true)
        {
            UE_LOG(LogSM, Warning, TEXT("!!!Server_RequestInteraction FAIL - Invalid Interaction or interactor!!!"));
            this->Multicast_InteractionRequestResult(false);
            return;
        }

        bool isFailInteractionCheatActive = owningCharacter->_debugFailAllInteractions;

        // Check if the interaction is possible on the server
        if (this->_requestedInteraction->IsInteractionPossible(owningCharacter, this->_requestedInteractionType, false) == false || isFailInteractionCheatActive == true)
        {
            // Log the failure reason
            FString failLog = TEXT("Server_RequestInteraction FAIL - Interaction not possible on Authority or fail interaction cheat active.");
            this->InteractionHandlerLogPvt(failLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 766);

            this->Multicast_InteractionRequestResult(false);
            return;
        }

        // The pseudo code uses VE_Success (2) and VE_Fail (3) from an ERequestState enum.
        // This enum tracks the state of the interaction lock/request. 
        ERequestState requestState = ERequestState::VE_Success;

        // Check if the interactor is currently in use
        if (interactor->IsInteracting() == true)
        {
            // Try to override and lock the interactor
            requestState = this->TryOverrideAndLockInteractor(requestState, interactor);
            
            // If it returned VE_Fail (3), it means it's waiting, so we don't proceed to begin the interaction yet.
            if (requestState == ERequestState::VE_Fail)
            {
                // The function just exits here in the assembly if state is 3. 
                // The server will update it later when the timer finishes or the interactor is freed.
                return;
            }
        }

        // Check for player dependencies (e.g., if this interaction requires another player, like healing)
        ADBDPlayer* playerDependency = this->_requestedInteraction->GetPlayerDependancy(owningCharacter);
        if (playerDependency != nullptr)
        {
            // Verify the dependency is valid and not pending kill/destruction
            if (playerDependency->IsPendingKill() == false && playerDependency->IsActorBeingDestroyed() == false && playerDependency->IsValidLowLevel() == true)
            {
                // Try to lock the dependency
                requestState = this->TryLockDependency(requestState, playerDependency);
            }
        }

        // If the state is VE_Success (2), it means all locks were granted successfully
        if (requestState == ERequestState::VE_Success)
        {
            // Stop any waiting timers since we are proceeding
            UDBDUtilTimer::Stop(this->_waitForOtherInteractionEndTimer);

            // Begin the interaction on the server
            this->_requestedInteraction->BeginInteraction(owningCharacter, this->_requestedInteractionType);

            // Log the success
            FString successLog = TEXT("Server_RequestInteraction SUCCESS - ");
            this->InteractionHandlerLogPvt(successLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 759);

            // Broadcast the success to clients
            this->Multicast_InteractionRequestResult(true);
        }
    }
}

// starts at line 785
ERequestState UPlayerInteractionHandler::TryOverrideAndLockInteractor(ERequestState state, UInteractor* interactor)
{
    // The pseudo code maps VE_Fail to 3.
    // In the context of the previous function, this is the "Waiting/Pending" state.
    // If we are already in the waiting state, we do not need to process further.
    if (state == ERequestState::VE_Fail)
    {
        return ERequestState::VE_Fail;
    }

    // Get the player and interaction currently occupying the interactor
    ADBDPlayer* interactingPlayer = interactor->GetInteractingPlayer();
    UInteractionDefinition* currentInteraction = interactor->GetCurrentInteraction();

    // Check if the interacting player is valid, not pending kill, not being destroyed, and generally valid
    if (interactingPlayer != nullptr && interactingPlayer->IsPendingKill() == false && 
        interactingPlayer->IsActorBeingDestroyed() == false && interactingPlayer->IsValidImpl() == true)
    {
        // If the interactor is already reserved, we maintain the current request state
        if (interactor->IsReserved() == true)
        {
            return state;
        }

        // Check if the requested interaction can override the current one
        if (this->_requestedInteraction->CanOverrideInteraction(currentInteraction) == true)
        {
            // Set the timeout timer
            this->_waitForOtherInteractionEndTimer.Reset(this->kWaitForOtherInteractionEndTimeout);

            // Inform the client that their interaction is being canceled
            interactingPlayer->Client_SendCancelInteraction();

            // Reserve the interactor for our owning character
            ADBDPlayer* owningCharacter = this->OwningCharacter.Get();
            interactor->Authority_Reserve(owningCharacter);

            // Log the wait
            FString waitLog = TEXT("Server_RequestInteraction WAIT -  Detected ongoing interaction of lesser priority. Waiting for interactor release.");
            this->InteractionHandlerLogPvt(waitLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 804);

            // Return 1, which likely maps to a specific ERequestState value indicating success in overriding, but still needing to wait for release.
            return ERequestState::VE_Pending;
        }
        else
        {
            // Log that the interaction cannot be overridden
            FString failLog = TEXT("Server_RequestInteraction FAIL -  Interaction already occuring on this interactor.");
            this->InteractionHandlerLogPvt(failLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 809);
        }
    }
    else
    {
        // Log that the interacting player is invalid
        FString invalidPlayerLog = TEXT("Server_RequestInteraction FAIL - Invalid player on requested interactor.");
        this->InteractionHandlerLogPvt(invalidPlayerLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 818);
    }

    // Broadcast failure if we couldn't override
    this->Multicast_InteractionRequestResult(false);
    
    // Return VE_Fail (3) to indicate a waiting/pending/failed state
    return ERequestState::VE_Fail;
}

// starts at line 828
ERequestState UPlayerInteractionHandler::TryLockDependency(ERequestState state, ADBDPlayer* playerDependency)
{
    if (state == ERequestState::VE_Fail)
    {
        return ERequestState::VE_Fail;
    }

    // Retrieve the current interaction of the dependent player.
    // The pseudo-code accesses the interaction handler directly via an offset (0xCF0).
    UPlayerInteractionHandler* dependencyInteractionHandler = playerDependency->_interactionHandler;
    UInteractionDefinition* currentInteraction = nullptr;

    if (dependencyInteractionHandler != nullptr)
    {
        currentInteraction = dependencyInteractionHandler->_currentInteraction;
    }

    // If the dependent player has no ongoing interaction or it is pending kill, we can add the dependency and proceed
    if (currentInteraction == nullptr || currentInteraction->IsPendingKill() == true)
    {
        this->AddPlayerDependency(playerDependency);
        return state;
    }
    else
    {
        // If the dependent player has an ongoing interaction, check if our requested interaction can override it
        if (this->_requestedInteraction->CanOverrideInteraction(currentInteraction) == true)
        {
            // Reset the wait timer
            this->_waitForOtherInteractionEndTimer.Reset(this->kWaitForOtherInteractionEndTimeout);

            // Notify the dependent player's client to cancel their current interaction
            playerDependency->Client_SendCancelInteraction();

            // Add the player as a dependency
            this->AddPlayerDependency(playerDependency);

            // Log the wait state
            FString waitLog = TEXT("Server_RequestInteraction WAIT - Detected ongoing interaction of lesser priority on dependent player. Waiting for player to stop interacting.");
            this->InteractionHandlerLogPvt(waitLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 841);

            // Return 1, indicating a successful override but requiring a wait
            return ERequestState::VE_Pending;
        }
        else
        {
            // Log the failure due to a higher priority interaction taking place on the dependency
            FString failLog = TEXT("Server_RequestInteraction FAIL - Interaction of higher priority taking place on dependent player.");
            this->InteractionHandlerLogPvt(failLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 846);

            // Broadcast the failure
            this->Multicast_InteractionRequestResult(false);

            // Return VE_Fail (3) indicating a failure
            return ERequestState::VE_Fail;
        }
    }
}

// starts at line 866
void UPlayerInteractionHandler::Server_EndInteraction_Implementation(UInteractionDefinition* interaction)
{
    // The pseudo code checks offset 0x110 (272).
    // A value of 3 corresponds to ROLE_Authority.
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningCharacter = this->OwningCharacter.Get();
        
        if (owningCharacter->Role == ROLE_Authority)
        {
            // Verify the interaction is valid and not pending kill
            if (interaction == nullptr || interaction->IsPendingKill() == true)
            {
                FString logMessage = TEXT("EndInteraction Server called on an INVALID interaction. Probably a disconnected character.");
                this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 898);
                return;
            }

            // Clear any locked dependencies since the interaction is ending
            this->ClearPlayerDependencies();

            // Fetch the interactor from the interaction
            UInteractor* interactor = interaction->GetInteractor();

            // Verify the interactor is valid and not pending kill
            if (interactor == nullptr || interactor->IsPendingKill() == true)
            {
                FString logMessage = TEXT("Interactor invalid.  UPlayerInteractionHandler::Server_EndInteraction_Implementation.");
                this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 875);
                return;
            }

            // Check if the interactor is currently engaged
            if (interactor->IsInteracting() == true)
            {
                ADBDPlayer* interactingPlayer = interactor->GetInteractingPlayer();

                // The pseudo code calls operator== on the FWeakObjectPtr and the player pointer
                // to verify if the owning character is the one actually performing the interaction
                if (owningCharacter == interactingPlayer)
                {
                    FString logMessage = TEXT("EndInteraction Server called on successful Interaction");
                    this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 883);

                    // Abort the interaction
                    interactor->AbortInteraction(owningCharacter, interaction);
                    return;
                }
                else
                {
                    FString logMessage = TEXT("EndInteraction Server called on another player's interactor. Not Aborting.");
                    this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 888);
                }
            }
            else
            {
                FString logMessage = TEXT("EndInteraction Server called on failed interaction. Not Aborting");
                this->InteractionHandlerLogPvt(logMessage, interaction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 893);
            }
        }
    }
}

// starts at line 909
void UPlayerInteractionHandler::Multicast_InteractionRequestResult_Implementation(bool success)
{
    // The pseudo code is generating FString representations and logging them.
    if (success == true)
    {
        // Log the success of the interaction request
        FString successLog = TEXT("Multicast_InteractionRequestResult_Implementation SUCCESS");
        this->InteractionHandlerLogPvt(successLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 912);
    }
    else
    {
        // Log the failure of the interaction request
        FString failureLog = TEXT("Multicast_InteractionRequestResult_Implementation FAIL");
        this->InteractionHandlerLogPvt(failureLog, this->_requestedInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 916);
    }

    if (success == true)
    {
        this->_requestedInteractionState = ERequestState::VE_Success;
    }
    else
    {
        this->_requestedInteractionState = ERequestState::VE_Fail;
    }
}

// starts at line 927
void UPlayerInteractionHandler::EndInteraction()
{
    // Validate the OwningCharacter weak pointer
    if (this->OwningCharacter.IsValid() == true)
    {
        // Log the call to EndInteraction
        FString callLog = TEXT("EndInteraction Called");
        this->InteractionHandlerLogPvt(callLog, this->_currentInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 933);

        // Stop the wait timer
        UDBDUtilTimer::Stop(this->_waitForOtherInteractionEndTimer);

        bool bIsInteractionValid = true;

        // Verify the current interaction is valid and not pending kill
        if (this->_currentInteraction == nullptr || this->_currentInteraction->IsPendingKill() == true)
        {
            bIsInteractionValid = false;
        }
        else
        {
            // Verify the interactor is valid
            UInteractor* interactor = this->_currentInteraction->GetInteractor();
            if (interactor == nullptr || interactor->IsPendingKill() == true)
            {
                bIsInteractionValid = false;
            }
            else
            {
                // Verify the interactable associated with the interactor is valid
                AInteractable* interactable = interactor->GetInteractable();
                if (interactable == nullptr || interactable->IsPendingKill() == true || interactable->IsActorBeingDestroyed() == true)
                {
                    bIsInteractionValid = false;
                }
            }
        }

        if (bIsInteractionValid == false)
        {
            // Log an error if the interaction chain is invalid
            UE_LOG(Interaction, Warning, TEXT("Invalid interaction in EndInteraction"));
        }
        else
        {
            ADBDPlayer* owningCharacter = this->OwningCharacter.Get();

            if (owningCharacter->IsLocallyControlled() == true)
            {
                // Log the local end of interaction
                FString localLog = TEXT("EndInteraction Local");
                this->InteractionHandlerLogPvt(localLog, this->_currentInteraction, "D:\\Jenkins\\workspace\\DBD_DocCube_Ship\\UnrealEngine\\DeadByDaylight\\Source\\DeadByDaylight\\Player\\PlayerInteractionHandler.cpp", 939);

                // Handle skill check cleanup if a skill check is active
                if (this->_skillCheck != nullptr && this->_skillCheck->IsPendingKill() == false)
                {
                    this->_skillCheck->OnEndInteraction();
                }

                // Clear the skill check timer
                UWorld* world = this->GetWorld();
                if (world != nullptr)
                {
                    FTimerManager& timerManager = world->GetTimerManager();
                    timerManager.ClearTimer(this->_skillCheckTimerHandle);
                }
            }
        }

        // Reset state
        this->_requestedInteractionState = ERequestState::VE_None;
        this->_currentInteraction = nullptr;

        // Cleanup saved interaction properties
        if (this->_currentInteractionProperties != nullptr)
        {
            // The pseudo-code explicitly frees the internal arrays of FGameplayTagContainer (AnimationMontageToPlayDescriptor.Tags) 
            // and TArray<AActor*> (ActorsToIgnoreOverlap) before deleting the struct.
            // In C++, the destructor of FInteractionProperties will handle this automatically when we delete it.
            delete this->_currentInteractionProperties;
            this->_currentInteractionProperties = nullptr;
        }
    }
}

// starts at line 989
UInteractor* UPlayerInteractionHandler::GetCurrentInteractor() const
{
    // Retrieve the currently active interaction
    UInteractionDefinition* currentInteraction = this->_currentInteraction;
    
    // Ensure the interaction is valid
    if (currentInteraction != nullptr && currentInteraction->IsPendingKill() == false)
    {
        return currentInteraction->GetInteractor();
    }
    
    return nullptr;
}

// starts at line 998
UInteractionDefinition* UPlayerInteractionHandler::GetRequestedInteraction() const
{
    return this->_requestedInteraction;
}

// starts at line 1008
UInteractor* UPlayerInteractionHandler::GetRequestedInteractor() const
{
    // Retrieve the interaction that has been requested but not yet necessarily started
    UInteractionDefinition* requestedInteraction = this->_requestedInteraction;
    
    // Ensure the interaction pointer is not null
    if (requestedInteraction != nullptr && requestedInteraction->IsPendingKill() == false)
    {
        return requestedInteraction->GetInteractor();
    }
    
    return nullptr;
}

// starts at line 1018
void UPlayerInteractionHandler::InteractionLogPvt(const FString& label, UInteractionDefinition* interaction, const char* file, int line)
{
    // Ensure the owning character is valid before proceeding with the log
    if (this->OwningCharacter.IsValid() == false)
    {
        return;
    }

    AInteractable* interactable = nullptr;

    // Safely retrieve the interactable object associated with the given interaction
    if (IsValid(interaction) == true)
    {
        UInteractor* interactor = interaction->GetInteractor();
        
        if (IsValid(interactor) == true)
        {
            interactable = interactor->GetInteractable();
        }
    }

    // Resolve the role (machine type) of the owning character
    FString role;
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
        role = owningPlayer->GetMachineTypeString();
    }
    else
    {
        role = TEXT("Invalid MachineType");
    }

    // Convert the source file name from ANSI to TCHAR string
    FString fileAsString;
    if (file != nullptr && file[0] != '\0')
    {
        fileAsString = FString(ANSI_TO_TCHAR(file));
    }

    // Resolve the debug name of the owning character
    FString playerName;
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
        playerName = owningPlayer->GetNameDebugString();
    }
    else
    {
        playerName = TEXT("Invalid MachineType");
    }

    // Extract the interactable's string representation
    FString interactableNameStr;
    const TCHAR* interactableName = TEXT("Null");

    if (IsValid(interactable) == true && interactable->IsActorBeingDestroyed() == false) 
    {
        interactableNameStr = interactable->GetFName().ToString();
        if (interactableNameStr.IsEmpty() == false)
        {
            interactableName = *interactableNameStr;
        }
    }

    // Extract the interaction's string representation
    FString interactionNameStr;
    const TCHAR* interactionName = TEXT("Null");

    if (IsValid(interaction) == true)
    {
        interactionNameStr = interaction->GetFName().ToString();
        if (interactionNameStr.IsEmpty() == false)
        {
            interactionName = *interactionNameStr;
        }
    }

    // Prepare safe C-style string pointers for the logging macro
    const TCHAR* labelStr = TEXT("");
    if (label.IsEmpty() == false)
    {
        labelStr = *label;
    }

    const TCHAR* playerNameStr = TEXT("");
    if (playerName.IsEmpty() == false)
    {
        playerNameStr = *playerName;
    }

    const TCHAR* roleStr = TEXT("");
    if (role.IsEmpty() == false)
    {
        roleStr = *role;
    }

    // Output the core interaction details using the standard Unreal Engine logging macro (Verbosity: Log / 5)
    UE_LOG(LogInteraction, Log, TEXT("[%s][%s] - [%s][%s] - [%s]"), interactionName, interactableName, playerNameStr, roleStr, labelStr);

    // Output specific file and line debug information (Verbosity: Verbose / 6)
    const TCHAR* fileStr = TEXT("");
    if (fileAsString.IsEmpty() == false)
    {
        fileStr = *fileAsString;
    }
    
    UE_LOG(LogInteraction, Verbose, TEXT("(Line: [%i] - File: [%s])"), line, fileStr);
}

// starts at line 1044
void UPlayerInteractionHandler::InteractionHandlerLogPvt(const FString& label, UInteractionDefinition* interaction, const char* file, int line)
{
    // Ensure the owning character is valid before proceeding with the log
    if (this->OwningCharacter.IsValid() == false)
    {
        return;
    }

    AInteractable* interactable = nullptr;

    // Safely retrieve the interactable object associated with the given interaction
    if (interaction != nullptr)
    {
        UInteractor* interactor = interaction->GetInteractor();
        
        if (interactor != nullptr)
        {
            interactable = interactor->GetInteractable();
        }
    }

    // Resolve the role (machine type) of the owning character
    FString role;
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
        role = owningPlayer->GetMachineTypeString();
    }
    else
    {
        role = TEXT("Invalid MachineType");
    }

    // Convert the source file name from ANSI to TCHAR string
    FString fileAsString;
    if (file != nullptr && file[0] != '\0')
    {
        fileAsString = FString(ANSI_TO_TCHAR(file));
    }

    // Resolve the debug name of the owning character
    FString playerName;
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
        playerName = owningPlayer->GetNameDebugString();
    }
    else
    {
        playerName = TEXT("Invalid MachineType");
    }

    // Extract the interactable's string representation
    FString interactableNameStr;
    const TCHAR* interactableName = TEXT("Null");

    if (interactable != nullptr && interactable->IsActorBeingDestroyed() == false) 
    {
        interactableNameStr = interactable->GetFName().ToString();
        if (interactableNameStr.IsEmpty() == false)
        {
            interactableName = *interactableNameStr;
        }
    }

    // Extract the interaction's string representation
    FString interactionNameStr;
    const TCHAR* interactionName = TEXT("Null");

    if (interaction != nullptr)
    {
        interactionNameStr = interaction->GetFName().ToString();
        if (interactionNameStr.IsEmpty() == false)
        {
            interactionName = *interactionNameStr;
        }
    }

    // Prepare safe C-style string pointers for the logging macro
    const TCHAR* labelStr = TEXT("");
    if (label.IsEmpty() == false)
    {
        labelStr = *label;
    }

    const TCHAR* playerNameStr = TEXT("");
    if (playerName.IsEmpty() == false)
    {
        playerNameStr = *playerName;
    }

    const TCHAR* roleStr = TEXT("");
    if (role.IsEmpty() == false)
    {
        roleStr = *role;
    }

    // Output the core interaction details using the standard Unreal Engine logging macro (Verbosity: Log / 5)
    // Note: The log category here is LogInteractionHandler, and the formatting order differs from InteractionLogPvt
    UE_LOG(LogInteractionHandler, Log, TEXT("[%s][%s] - [%s] - [%s][%s]"), playerNameStr, roleStr, labelStr, interactionName, interactableName);

    // Output specific file and line debug information (Verbosity: Verbose / 6)
    const TCHAR* fileStr = TEXT("");
    if (fileAsString.IsEmpty() == false)
    {
        fileStr = *fileAsString;
    }
    
    UE_LOG(LogInteractionHandler, Verbose, TEXT("(Line: [%i] - File: [%s])"), line, fileStr);
}

// starts at line 1088
void UPlayerInteractionHandler::InterruptionLog(const FString& label, UInterruptionDefinition* interruption, bool isInterruptor, ADBDPlayer* otherParty)
{
    // Ensure the owning character is valid before proceeding with the log
    if (this->OwningCharacter.IsValid() == false)
    {
        return;
    }

    UInteractionDefinition* interactionDef = nullptr;
    UInteractor* interactor = nullptr;
    AInteractable* interactable = nullptr;

    // Safely retrieve the interaction hierarchy associated with the given interruption
    if (IsValid(interruption) == true)
    {
        interactionDef = interruption->GetInteractionDefinition();
        
        if (IsValid(interactionDef) == true)
        {
            interactor = interactionDef->GetInteractor();
            
            if (IsValid(interactor) == true)
            {
                interactable = interactor->GetInteractable();
            }
        }
    }

    // Resolve the role (machine type) of the owning character
    FString role;
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
        role = owningPlayer->GetMachineTypeString();
    }
    else
    {
        role = TEXT("Invalid MachineType");
    }

    // Resolve the name of the owning character (prioritizing PlayerState over object name)
    FString owningCharacterNameStr;
    const TCHAR* owningCharacterName = TEXT("Null");
    
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
        
        if (owningPlayer->IsActorBeingDestroyed() == false)
        {
            if (IsValid(owningPlayer->PlayerState) == true && owningPlayer->PlayerState->IsActorBeingDestroyed() == false)
            {
                owningCharacterNameStr = owningPlayer->PlayerState->PlayerName;
            }
            else
            {
                owningCharacterNameStr = owningPlayer->GetFName().ToString();
            }
            
            if (owningCharacterNameStr.IsEmpty() == false)
            {
                owningCharacterName = *owningCharacterNameStr;
            }
        }
    }

    // Resolve the name of the other party (prioritizing PlayerState over object name)
    FString otherPartyNameStr;
    const TCHAR* otherPartyName = TEXT("Null");
    
    if (IsValid(otherParty) == true && otherParty->IsActorBeingDestroyed() == false)
    {
        if (IsValid(otherParty->PlayerState) == true && otherParty->PlayerState->IsActorBeingDestroyed() == false)
        {
            otherPartyNameStr = otherParty->PlayerState->PlayerName;
        }
        else
        {
            otherPartyNameStr = otherParty->GetFName().ToString();
        }
        
        if (otherPartyNameStr.IsEmpty() == false)
        {
            otherPartyName = *otherPartyNameStr;
        }
    }

    // Extract the string representation for the interruption definition
    FString interruptionNameStr;
    const TCHAR* interruptionName = TEXT("Null");
    if (IsValid(interruption) == true)
    {
        interruptionNameStr = interruption->GetFName().ToString();
        if (interruptionNameStr.IsEmpty() == false)
        {
            interruptionName = *interruptionNameStr;
        }
    }

    // Extract the string representation for the interaction definition
    FString interactionDefNameStr;
    const TCHAR* interactionDefName = TEXT("Null");
    if (IsValid(interactionDef) == true)
    {
        interactionDefNameStr = interactionDef->GetFName().ToString();
        if (interactionDefNameStr.IsEmpty() == false)
        {
            interactionDefName = *interactionDefNameStr;
        }
    }

    // Extract the string representation for the interactor
    FString interactorNameStr;
    const TCHAR* interactorName = TEXT("Null");
    if (IsValid(interactor) == true)
    {
        interactorNameStr = interactor->GetFName().ToString();
        if (interactorNameStr.IsEmpty() == false)
        {
            interactorName = *interactorNameStr;
        }
    }

    // Extract the string representation for the interactable
    FString interactableNameStr;
    const TCHAR* interactableName = TEXT("Null");
    
    if (IsValid(interactable) == true && interactable->IsActorBeingDestroyed() == false)
    {
        interactableNameStr = interactable->GetFName().ToString();
        if (interactableNameStr.IsEmpty() == false)
        {
            interactableName = *interactableNameStr;
        }
    }

    // Prepare safe C-style string pointers for the core log labels
    const TCHAR* labelStr = TEXT("");
    if (label.IsEmpty() == false)
    {
        labelStr = *label;
    }

    const TCHAR* roleStr = TEXT("");
    if (role.IsEmpty() == false)
    {
        roleStr = *role;
    }

    const TCHAR* isInterruptorStr = isInterruptor == true ? TEXT("Yes") : TEXT("No");

    // Output the core interruption details using the standard Unreal Engine logging macro (Verbosity: Log / 5)
    UE_LOG(LogInteraction, Log, TEXT("[%s] --- [%s] Character: [%s] - Interactable: [%s] - Interactor: [%s] - InteractionDefinition: [%s]- InterruptionDef: [%s] - IsInterruptor: [%s] - OtherParty: [%s]"), 
        roleStr, 
        labelStr, 
        owningCharacterName, 
        interactableName, 
        interactorName, 
        interactionDefName, 
        interruptionName, 
        isInterruptorStr, 
        otherPartyName
    );
}

// starts at line 1121
void UPlayerInteractionHandler::DebugPrintInteractionsInZone(bool printInteractionChecks)
{
    // Local array to hold the combined list of interactions to print
    TArray<UInteractionDefinition*> _interactions;

    // 1. Gather interactions directly from the InteractionsInZone array
    for (int i = 0; i < this->InteractionsInZone.Num(); ++i)
    {
        _interactions.Add(this->InteractionsInZone[i]);
    }

    // 2. Gather interactions from the currently attached interactor, if valid
    if (this->_attachedInteractor.IsValid() == true)
    {
        UInteractor* attachedInteractor = this->_attachedInteractor.Get();
        
        // Ensure we retrieve interactions to accurately append them
        TArray<UInteractionDefinition*> interactorDefinitions = attachedInteractor->GetInteractionDefinitions();
        
        for (int i = 0; i < interactorDefinitions.Num(); ++i)
        {
            _interactions.Add(interactorDefinitions[i]);
        }
    }

    // Log the header for the debug block
    UE_LOG(LogInteraction, Log, TEXT("------------------------"));
    UE_LOG(LogInteraction, Log, TEXT("- Interactions in Zone -"));

    // Iterate through all gathered interactions and print their details
    for (int i = 0; i < _interactions.Num(); ++i)
    {
        UInteractionDefinition* interactionDef = _interactions[i];
        
        UInteractor* interactor = nullptr;
        AInteractable* interactable = nullptr;

        if (IsValid(interactionDef) == true)
        {
            interactor = interactionDef->GetInteractor();
            
            if (IsValid(interactor) == true)
            {
                interactable = interactor->GetInteractable();
            }
        }

        // Resolve string representations for the log
        
        // 1. Interactable Name
        FString interactableNameStr;
        const TCHAR* interactableName = TEXT("Null");
        
        if (IsValid(interactable) == true && interactable->IsActorBeingDestroyed() == false)
        {
            interactableNameStr = interactable->GetFName().ToString();
            if (interactableNameStr.IsEmpty() == false)
            {
                interactableName = *interactableNameStr;
            }
        }

        // 2. Interactor Name
        FString interactorNameStr;
        const TCHAR* interactorName = TEXT("Null");
        if (IsValid(interactor) == true)
        {
            interactorNameStr = interactor->GetFName().ToString();
            if (interactorNameStr.IsEmpty() == false)
            {
                interactorName = *interactorNameStr;
            }
        }

        // 3. Interaction Definition Name
        FString interactionDefNameStr;
        const TCHAR* interactionDefName = TEXT("Null");
        if (IsValid(interactionDef) == true)
        {
            interactionDefNameStr = interactionDef->GetFName().ToString();
            if (interactionDefNameStr.IsEmpty() == false)
            {
                interactionDefName = *interactionDefNameStr;
            }
        }

        // Output the interaction line
        UE_LOG(LogInteraction, Log, TEXT("[%s] - [%s] - [%s]"), interactionDefName, interactorName, interactableName);

        // Print footer separator for each interaction
        UE_LOG(LogInteraction, Log, TEXT("----------------------------"));
    }
}

// starts at line 1178
void UPlayerInteractionHandler::Server_DebugRequestInteraction_Implementation(const FString& interactionName, const FString& interactorName, const FString& interactableName)
{
    // Ensure the owning character is valid before proceeding with the debug log
    if (this->OwningCharacter.IsValid() == false)
    {
        return;
    }

    // Resolve the debug name of the owning character (prioritizing PlayerState over object name)
    FString owningCharacterNameStr;
    const TCHAR* owningCharacterName = TEXT("Null");
    
    ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
    
    if (owningPlayer->IsActorBeingDestroyed() == false)
    {
        if (IsValid(owningPlayer->PlayerState) == true && owningPlayer->PlayerState->IsActorBeingDestroyed() == false)
        {
            owningCharacterNameStr = owningPlayer->PlayerState->PlayerName;
        }
        else
        {
            owningCharacterNameStr = owningPlayer->GetFName().ToString();
        }
        
        if (owningCharacterNameStr.IsEmpty() == false)
        {
            owningCharacterName = *owningCharacterNameStr;
        }
    }

    // Prepare safe C-style string pointers for the incoming arguments
    const TCHAR* interactableNameStr = TEXT("");
    if (interactableName.IsEmpty() == false)
    {
        interactableNameStr = *interactableName;
    }

    const TCHAR* interactorNameStr = TEXT("");
    if (interactorName.IsEmpty() == false)
    {
        interactorNameStr = *interactorName;
    }

    const TCHAR* interactionNameStr = TEXT("");
    if (interactionName.IsEmpty() == false)
    {
        interactionNameStr = *interactionName;
    }

    // Output the debug request details using the standard Unreal Engine logging macro (Verbosity: Log / 5)
    UE_LOG(LogInteraction, Log, TEXT("[SERVER DEBUG] About to receive request for interaction from [%s] on object called [%s] [%s] [%s]"), 
        owningCharacterName, 
        interactionNameStr, 
        interactorNameStr, 
        interactableNameStr
    );
}

// starts at line 1200
bool UPlayerInteractionHandler::CanPerformInteraction(FString interactionID) const
{
    // Iterate through all currently available interactions managed by the handler
    for (auto it = this->_availableInteractions.CreateConstIterator(); it; ++it)
    {
        // Obtain the Weak Object Pointer representing the interaction definition
        const TWeakObjectPtr<UInteractionDefinition>& interactionDefWeakPtr = it.Value();

        // Check if the pointer resolves to a valid UInteractionDefinition
        if (interactionDefWeakPtr.IsValid() == true)
        {
            UInteractionDefinition* interactionDef = interactionDefWeakPtr.Get();
            
            // Compare the definition's designated interaction ID against the requested ID
            // Ignore case during the comparison to ensure robust matching
            if (interactionDef->InteractionID.Equals(interactionID, ESearchCase::IgnoreCase) == true)
            {
                return true;
            }
        }
    }

    // Return false if no matching and valid interaction was found in the available list
    return false;
}

// starts at line 1218
bool UPlayerInteractionHandler::TryInterruption(ADBDPlayer* interruptee)
{
    // Ensure the owning character is valid before attempting an interruption
    if (this->OwningCharacter.IsValid() == false)
    {
        return false;
    }

    ADBDPlayer* owningPlayer = this->OwningCharacter.Get();

    /* Check internal interruption tracking flags */
    if (owningPlayer->IsInteracting == true || this->_hasInterruptionRequest == true || this->_hasInterruptionRequestInBuffer == true || owningPlayer->CanInterrupt(interruptee) == false)
    {
        FString logLabel = FString(TEXT("TryInterruption FAIL - Character cannot request interruption"));
        this->InterruptionLog(logLabel, this->_requestedInterruptionDefinition, true, this->_requestedInterruptionOtherParty);
        return false;
    }

    // Reset interruption buffer state
    this->_hasInterruptionRequestInBuffer = false;
    this->_requestedInterruptionOtherParty = nullptr;
    this->_requestedInterruptionDefinition = nullptr;
    this->_requestedInterruptionIsInterruptor = true;

    UInterruptionDefinition* possibleInterruption = nullptr;

    // Validate the interruptee and find a valid interruption definition
    if (IsValid(interruptee) == true && interruptee->IsActorBeingDestroyed() == false)
    {
        if (interruptee->IsValidImpl() == true && interruptee->IsInterruptable == true)
        {
            possibleInterruption = interruptee->FindPossibleInterruption(owningPlayer);
        }
    }

    // If no valid interruption was found or the definition is invalid, log failure and clear server state
    if (IsValid(possibleInterruption) == false)
    {
        FString logLabel = FString(TEXT("TryInterruption FAIL"));
        this->InterruptionLog(logLabel, this->_requestedInterruptionDefinition, true, this->_requestedInterruptionOtherParty);
        this->Server_StoreInterruption_Interruptor(nullptr, nullptr);
        return false;
    }

    // Store the valid interruption request
    this->_hasInterruptionRequestInBuffer = true;
    this->_requestedInterruptionOtherParty = interruptee;
    this->_requestedInterruptionDefinition = possibleInterruption;

    // Log success
    FString successLabel = FString(TEXT("TryInterruption SUCCESS"));
    this->InterruptionLog(successLabel, this->_requestedInterruptionDefinition, true, this->_requestedInterruptionOtherParty);

    // Notify the server of the new interruption request
    this->Server_StoreInterruption_Interruptor(this->_requestedInterruptionOtherParty, this->_requestedInterruptionDefinition);

    return true;
}

// starts at line 1259
void UPlayerInteractionHandler::RequestAndBeginInterruption_Interruptor()
{
    // Log the initiation of the interruption request
    FString logLabel = FString(TEXT("RequestAndBeginInterruption"));
    this->InterruptionLog(logLabel, this->_requestedInterruptionDefinition, true, this->_requestedInterruptionOtherParty);

    // Transfer the requested interruption state to the current active state
    this->_currentInterruptionOtherParty = this->_requestedInterruptionOtherParty;
    this->_currentInterruptionDefinition = this->_requestedInterruptionDefinition;
    this->_currentInterruptionIsInterruptor = this->_requestedInterruptionIsInterruptor;
    this->_interactionInProgress = true;

    // Check if the owning character has authority
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();

        if (owningPlayer->IsLocallyControlled() == true)
        {
            // Update the requested state and notify the server
            this->_requestedInterruptionState = ERequestState::VE_Pending;
            this->Server_RequestInterruption_Interruptor();
        }
    }
}

// starts at line 1277
void UPlayerInteractionHandler::Server_StoreInterruption_Interruptor_Implementation(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* interruptionDefinition)
{
    // Prepare a descriptive string for the log
    FString logLabel = FString(TEXT("Server_StoreInterruption_Implementation"));

    // Call the centralized logging function for interruptions
    // Note: isInterruptor is hardcoded to true in this context
    this->InterruptionLog(logLabel, interruptionDefinition, true, interruptionOtherParty);

    // Replicate the interruption state to all clients
    this->Multicast_StoreInterruption_Interruptor(interruptionOtherParty, interruptionDefinition);
}

// starts at line 1287
void UPlayerInteractionHandler::Multicast_StoreInterruption_Interruptor_Implementation(ADBDPlayer* interruptionOtherParty, UInterruptionDefinition* interruptionDefinition)
{
    // Ensure the owning character is valid before processing the multicast
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();

        if (owningPlayer->IsLocallyControlled() == false)
        {
            // Store the interruption request locally for non-authority clients
            this->_requestedInterruptionOtherParty = interruptionOtherParty;
            this->_requestedInterruptionDefinition = interruptionDefinition;
            this->_requestedInterruptionIsInterruptor = true;

            // Log the reception of the multicast
            FString logLabel = FString(TEXT("Multicast_StoreInterruption_Interruptor"));
            this->InterruptionLog(logLabel, this->_requestedInterruptionDefinition, true, this->_requestedInterruptionOtherParty);
        }
    }
}

// starts at line 1307
void UPlayerInteractionHandler::Server_RequestInterruption_Interruptor_Implementation()
{
    // Ensure the owning character is valid before proceeding
    if (this->OwningCharacter.IsValid() == false)
    {
        return;
    }

    // Log the success of the server request
    FString successLog = FString(TEXT("Server_RequestInterruption_Interruptor SUCCESS"));
    this->InterruptionLog(successLog, this->_requestedInterruptionDefinition, true, this->_requestedInterruptionOtherParty);

    bool isInterruptionValid = false;

    // Validate the requested other party and the interruption definition
    if (this->_requestedInterruptionOtherParty != nullptr && this->_requestedInterruptionOtherParty->IsActorBeingDestroyed() == false)
    {
        if (this->_requestedInterruptionOtherParty->IsValidImpl() == true)
        {
            if (this->_requestedInterruptionDefinition != nullptr && this->_requestedInterruptionDefinition->IsPendingKill() == false)
            {
                isInterruptionValid = true;
            }
        }
    }

    if (isInterruptionValid == false)
    {
        // Log an error if the state is invalid (Verbosity Level 2 -> Warning/Error)
        UE_LOG(LogSM, Error, TEXT("!!!Server_RequestInterruption_Interruptor_Implementation Invalid Interruption!!!"));
    }
    else
    {
        // Retrieve the owning player
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
        
        // Accessing _interactionHandler at offset 0xCF0 in ADBDPlayer
        UPlayerInteractionHandler* interrupteeHandler = this->_requestedInterruptionOtherParty->_interactionHandler;
        
        if (interrupteeHandler != nullptr)
        {
            // Multicast the interruption request to the interruptee's interaction handler
            interrupteeHandler->Multicast_RequestInterruption_Interruptee(owningPlayer, this->_requestedInterruptionDefinition);
        }
    }
}

// starts at line 1332
void UPlayerInteractionHandler::Multicast_InterruptionRequestResult_Interruptor_Implementation(bool success)
{
    // The compiler optimized the state assignment as (!success + 2).
    if (success == true)
    {
        this->_requestedInterruptionState = ERequestState::VE_Success;
    }
    else
    {
        this->_requestedInterruptionState = ERequestState::VE_Fail;
    }
}

// starts at line 1346
ERequestState UPlayerInteractionHandler::GetInterruptionRequestState() const
{
    return this->_requestedInterruptionState;
}

// starts at line 1351
void UPlayerInteractionHandler::Multicast_RequestInterruption_Interruptee_Implementation(ADBDPlayer *interruptionOtherParty, UInterruptionDefinition *interruptionDefinition)
    {
    this->_requestedInterruptionOtherParty = interruptionOtherParty;
    this->_requestedInterruptionDefinition = interruptionDefinition;
    this->_requestedInterruptionIsInterruptor = false;
    this->_hasInterruptionRequestInBuffer = true;
}

// starts at line 1364
void UPlayerInteractionHandler::BeginInterruption_Interruptee()
{
    this->_currentInterruptionOtherParty = this->_requestedInterruptionOtherParty;
    this->_currentInterruptionDefinition = this->_requestedInterruptionDefinition;
    this->_currentInterruptionIsInterruptor = this->_requestedInterruptionIsInterruptor;
    this->_interactionInProgress = true;
}

// starts at line 1372
void UPlayerInteractionHandler::Server_AnswerInterruption_Interruptee_Implementation(bool success)
{
    this->_requestedInterruptionOtherParty->_interactionHandler->Multicast_InterruptionRequestResult_Interruptor(success);
}

// starts at line 1389
void UPlayerInteractionHandler::EndInterruption()
{
    this->_currentInterruptionIsInterruptor = false;
    this->_interactionInProgress = false;
    this->_currentInterruptionOtherParty = nullptr;
    this->_currentInterruptionDefinition = nullptr;
}

// starts at line 1398
bool UPlayerInteractionHandler::GetShouldTriggerSkillCheck()
{
    // Ensure the owning character is valid before performing checks
    if (this->OwningCharacter.IsValid() == false)
    {
        return false;
    }

    ADBDPlayer* owningPlayer = this->OwningCharacter.Get();

    // Checking specific internal flags (offset 0xACD, 0xF40) for various player states
    if (owningPlayer->ForceDisableSkillChecks == true || owningPlayer->_isBeingInterrupted == true)
    {
        return false;
    }

    // Check if a skill check is already active or in failure cooldown
    if (this->_skillCheck != nullptr)
    {
        if (this->_skillCheck->IsActivated() == true || this->_skillCheck->IsInFailureCooldown() == true)
        {
            return false;
        }
    }

    // Ensure there is a current interaction and it is chargeable
    if (this->_currentInteraction == nullptr)
    {
        return false;
    }

    // Checking specific internal flags (offset 0x140) for destruction state
    if (this->_currentInteraction->IsActorBeingDestroyed() == true || this->_currentInteraction->IsChargeable() == false)
    {
        return false;
    }

    // Perform specific checks if the owning player is a ACamperPlayer
    if (owningPlayer != nullptr)
    {
        if (owningPlayer->IsA(ACamperPlayer::StaticClass()) == true)
        {
            // Checking specific internal flags (offset 0x140) for destruction state and IsValidImpl
            if (owningPlayer->IsActorBeingDestroyed() == false && owningPlayer->IsValidImpl() == true)
            {
                ACamperPlayer* camperPlayer = Cast<ACamperPlayer>(owningPlayer);
                if (camperPlayer != nullptr && camperPlayer->IsBeingPickedUp() == true)
                {
                    return false;
                }
            }
        }
    }

    // Checking specific internal flag (offset 0x9F1) for a specific status
    if (owningPlayer->ForceSkillChecks == true)
    {
        return true;
    }

    // Check if any progress-based skill checks should trigger
    for (int32 i = 0; i < this->_progressBasedSkillChecks.Num(); ++i)
    {
        float progressPoint = this->_progressBasedSkillChecks[i];
        float currentCharge = this->_currentInteraction->GetChargePercent();

        if (progressPoint < currentCharge)
        {
            // Remove the triggered progress point to prevent re-triggering
            this->_progressBasedSkillChecks.RemoveAll([progressPoint](float item) { return item == progressPoint; });
            return true;
        }
    }

    // Fallback to dice roll for triggering a skill check
    return UDBDDiceRoller::GetRollResultByDiceRollType(owningPlayer, EDiceRollType::VE_TriggerSkillCheck);
}

// starts at line 1437
void UPlayerInteractionHandler::StartCustomSkillCheck(ESkillCheckCustomType type, float delay)
{
    this->_skillCheck->ActivateCustomSkillCheck(type, delay);
}

// starts at line 1442
void UPlayerInteractionHandler::OnSkillCheckTimerTrigger()
{
    // Check if the character owning this component is valid
    if (this->OwningCharacter.IsValid() == false)
    {
        return;
    }

    // Check if there is an ongoing interaction and if it is not pending kill
    if (this->_currentInteraction == nullptr || this->_currentInteraction->IsPendingKill() == true)
    {
        return;
    }

    // Check if the skill check component is valid and not pending kill
    if (this->_skillCheck == nullptr || this->_skillCheck->IsPendingKill() == true)
    {
        return;
    }

    // Check if the owning character has authority
    ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
    
    if (owningPlayer->IsLocallyControlled() == false)
    {
        return;
    }

    // Attempt to activate the skill check if it is not already activated or displayed,
    // and if the conditions to trigger one are met.
    if (this->_skillCheck->_activated == false && this->_skillCheck->_displayed == false)
    {
        if (this->GetShouldTriggerSkillCheck() == true)
        {
            float delay = 1.0f;
            this->_skillCheck->ActivateSkillCheck(this->_currentInteraction, delay);
        }
    }
}

// starts at line 1461
float UPlayerInteractionHandler::GetMultiplicativeSkillCheckProbabilityModifier() const
{
    float probabilityModifier = 1.0f;

    // Retrieve the World safely
    UWorld* world = this->WorldPrivate;
    if (world == nullptr)
    {
        world = this->GetWorld_Uncached();
    }

    // Retrieve and validate the GameState
    ADBDGameState* dbdGameState = nullptr;
    if (world != nullptr && world->GameState != nullptr)
    {
        UClass* dbdGameStateClass = ADBDGameState::StaticClass();
        if (world->GameState->IsA(dbdGameStateClass) == true)
        {
            dbdGameState = Cast<ADBDGameState>(world->GameState);
        }
    }

    // Ensure the owning character is valid before proceeding
    if (this->OwningCharacter.IsValid() == false)
    {
        return probabilityModifier;
    }

    ADBDPlayer* owningPlayer = this->OwningCharacter.Get();

    if (owningPlayer == nullptr || owningPlayer->IsActorBeingDestroyed() == true || owningPlayer->IsPendingKill() == true || owningPlayer->IsValidImpl() == false)
    {
        return probabilityModifier;
    }

    // Check Killer modifiers if the GameState is valid
    if (dbdGameState != nullptr)
    {
        // Retrieving the Killer player from GameState at offset 0x5F0
        ADBDPlayer* killerPlayer = dbdGameState->Slasher;

        if (IsValid(killerPlayer) == true && killerPlayer->IsActorBeingDestroyed() == false && killerPlayer->IsPendingKill() == false)
        {
            if (killerPlayer->IsValidImpl() == true)
            {
                if (owningPlayer->IsInTerrorRadius() == true)
                {
                    // Retrieving UPerkManager interface/component from Killer at offset 0x7A8 via virtual call
                    UPerkManager* killerPerkManager = killerPlayer->GetPerkManager();

                    if (killerPerkManager != nullptr)
                    {
                        // EGameplayModifierType: 0x1A (26), EGameplayModifierSource: 4
                        probabilityModifier += killerPerkManager->GetPerkModifier(EGameplayModifierType::VE_ModifySkillCheckProbabilityInTerrorRadius, EGameplayModifierSource::VE_All);
                    }
                }
            }
        }
    }

    // Retrieving UPerkManager interface/component from OwningPlayer at offset 0x7A8 via virtual call
    UPerkManager* playerPerkManager = owningPlayer->GetPerkManager();

    if (playerPerkManager != nullptr)
    {
        // EGameplayModifierType: 5, EGameplayModifierSource: 4
        probabilityModifier += playerPerkManager->GetPerkModifier(EGameplayModifierType::VE_ModifySkillCheckProbability, EGameplayModifierSource::VE_All);
    }

    return probabilityModifier;
}

// starts at line 1481
float UPlayerInteractionHandler::GetAdditiveSkillCheckProbabilityModifier() const
{
    float probabilityModifier = 0.0f;

    // Retrieve the World safely
    UWorld* world = this->WorldPrivate;
    if (world == nullptr)
    {
        world = this->GetWorld_Uncached();
    }

    // Retrieve and validate the GameState
    ADBDGameState* dbdGameState = nullptr;
    if (world != nullptr && world->GameState != nullptr)
    {
        UClass* dbdGameStateClass = ADBDGameState::StaticClass();
        if (world->GameState->IsA(dbdGameStateClass) == true)
        {
            dbdGameState = Cast<ADBDGameState>(world->GameState);
        }
    }

    // Ensure the owning character is valid before proceeding
    if (this->OwningCharacter.IsValid() == false)
    {
        return probabilityModifier;
    }

    ADBDPlayer* owningPlayer = this->OwningCharacter.Get();

    if (owningPlayer == nullptr || owningPlayer->IsActorBeingDestroyed() == true || owningPlayer->IsPendingKill() == true || owningPlayer->IsValidImpl() == false)
    {
        return probabilityModifier;
    }

    // Check Killer modifiers if the GameState is valid
    if (dbdGameState != nullptr)
    {
        // Retrieving the Killer player from GameState at offset 0x5F0
        ADBDPlayer* killerPlayer = dbdGameState->Slasher;

        if (IsValid(killerPlayer) == true && killerPlayer->IsActorBeingDestroyed() == false && killerPlayer->IsPendingKill() == false)
        {
            if (killerPlayer->IsValidImpl() == true)
            {
                if (owningPlayer->IsInTerrorRadius() == true)
                {
                    // Retrieving UPerkManager interface/component from Killer at offset 0x7A8 via virtual cal
                    UPerkManager* killerPerkManager = killerPlayer->GetPerkManager();

                    if (killerPerkManager != nullptr)
                    {
                        // EGameplayModifierType: 0x63 (99), EGameplayModifierSource: 4
                        probabilityModifier += killerPerkManager->GetPerkModifier(EGameplayModifierType::VE_ModifySkillCheckProbabilityInTerrorRadiusAdditive, EGameplayModifierSource::VE_All);
                    }
                }
            }
        }
    }

    // Retrieving UPerkManager interface/component from OwningPlayer at offset 0x7A8 via virtual call
    UPerkManager* playerPerkManager = owningPlayer->GetPerkManager();

    if (playerPerkManager != nullptr)
    {
        // EGameplayModifierType: 0x62 (98), EGameplayModifierSource: 4
        probabilityModifier += playerPerkManager->GetPerkModifier(EGameplayModifierType::VE_ModifySkillCheckProbabilityAdditive, EGameplayModifierSource::VE_All);
    }

    return probabilityModifier;
}

// starts at line 1501
void UPlayerInteractionHandler::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    // First, call the base class implementation of OnComponentDestroyed
    Super::OnComponentDestroyed(bDestroyingHierarchy);

    // Retrieve the World safely
    UWorld* world = this->WorldPrivate;
    if (world == nullptr)
    {
        world = this->GetWorld_Uncached();
    }

    // Only proceed if a valid world was found
    if (world != nullptr && world->IsPendingKill() == false)
    {
        FTimerManager& timerManager = world->GetTimerManager();
        timerManager.ClearTimer(this->_skillCheckTimerHandle);
        
        // Invalidate the timer handle
        this->_skillCheckTimerHandle.Invalidate();
    }
}

// starts at line 1515
bool UPlayerInteractionHandler::CanOverrideInteraction(UInteractionDefinition* requestedInteraction, UInteractionDefinition* ongoingInteraction)
{
    // Ensure the requested interaction is valid and not pending kill
    if (IsValid(requestedInteraction) == true && requestedInteraction->IsPendingKill() == false)
    {
        // Delegate the overriding logic to the definition itself
        return requestedInteraction->CanOverrideInteraction(ongoingInteraction);
    }

    return false;
}

// starts at line 1525
void UPlayerInteractionHandler::SetScanForInteractionsEnabled(bool enabled)
{
    this->_scanForInteractions = enabled;
}

// starts at line 1531
void UPlayerInteractionHandler::Client_NotifyChargeCompleted_Implementation()
{
    this->_currentInteractionChargeCompleted = true;
}

// starts at line 1542
void UPlayerInteractionHandler::Broadcast_ConfirmChargedCompleted_Implementation(bool chargeComplete)
{
    this->Broadcast_Multicast_ConfirmChargedCompleted(chargeComplete);
}

// starts at line 1553
void UPlayerInteractionHandler::Broadcast_Multicast_ConfirmChargedCompleted_Implementation(bool chargeComplete)
{
    // Ensure the owning character is valid before executing the multicast confirmation
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
        
        if (owningPlayer->IsLocallyControlled() == false)
        {
            // Only update the local charge completion state for non-authority clients
            this->_currentInteractionChargeCompleted = chargeComplete;
        }
    }
}

// starts at line 1566
void UPlayerInteractionHandler::Local_BroadcastInteractionPlayerProperties(FInteractionPlayerProperties playerProperties)
{
    // Ensure the owning character is valid before broadcasting
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();
        
        if (owningPlayer->IsLocallyControlled() == true)
        {
            // First broadcast the properties using the internal protected/private function
            this->Broadcast_InteractionPlayerProperties(playerProperties);

            // Then store the updated properties in the local state
            this->_currentInteractionPlayerProperties = playerProperties;
        }
    }
}

// starts at line 1575
void UPlayerInteractionHandler::Broadcast_InteractionPlayerProperties_Implementation(FInteractionPlayerProperties playerProperties)
{
    // Check if the owning character is valid and has authority
    if (this->OwningCharacter.IsValid() == true)
    {
        ADBDPlayer* owningPlayer = this->OwningCharacter.Get();

        // Assuming ROLE_Authority is 3 based on standard UE4 networking enum
        if (owningPlayer->Role == ENetRole::ROLE_Authority)
        {
            // Multicast the interaction properties to all clients
            this->Broadcast_InteractionPlayerProperties_Multicast(playerProperties);
        }
    }
}

// starts at line 1587
void UPlayerInteractionHandler::Broadcast_InteractionPlayerProperties_Multicast_Implementation(FInteractionPlayerProperties playerProperties)
{
    // Retrieve the owning character object
    ADBDPlayer* owningPlayer = this->OwningCharacter.Get();

    if (owningPlayer != nullptr && owningPlayer->IsLocallyControlled() == false)
    {
        // Update the local player properties only for non-authority clients
        this->_currentInteractionPlayerProperties = playerProperties;
    }
}

// starts at line 1604
void UPlayerInteractionHandler::FillInInteractionPlayerProperties(UInteractionDefinition* interaction, FInteractionPlayerProperties& playerProperties, ADBDPlayer* requester)
{
    // Retrieve the owning character safely
    ADBDPlayer* owningPlayer = nullptr;
    if (this->OwningCharacter.IsValid() == true)
    {
        owningPlayer = this->OwningCharacter.Get();
    }

    if (owningPlayer != nullptr)
    {
        // Retrieving _averageVelocity at offset 0x1120
        playerProperties.AverageVelocityAtStart = owningPlayer->_averageVelocity;

        // Retrieving _stamina at offset 0x10E4
        playerProperties.StaminaAtStart = owningPlayer->_stamina;

        // Retrieve the current location of the owning player
        FVector playerPositionAtStart = FVector(0.0f, 0.0f, 0.0f);
        
        // Accessing root component at offset 0x160 to get location
        if (owningPlayer->RootComponent != nullptr)
        {
            // Extract the location from the component's transform (offset 0x140)
            playerPositionAtStart = owningPlayer->RootComponent->GetComponentLocation();
        }
        
        playerProperties.PlayerPositionAtStart = playerPositionAtStart;
    }

    // Assign the requester property
    playerProperties.Requester = requester;

    // Fill in snap-related properties querying the interaction definition
    if (IsValid(interaction) == true)
    {
        playerProperties.SnapPositionAtStart = interaction->GetSnapPointPosition(requester);
        playerProperties.SnapRotationAtStart = interaction->GetSnapPointRotation(requester);
        playerProperties.SnapDistanceAtStart = interaction->GetSnapDistance(requester);
        playerProperties.SnapTimeAtStart = interaction->GetSnapTime(requester);
    }
}

// starts at line 1616
void UPlayerInteractionHandler::TriggerSkillCheck()
{
    // Ensure there is a valid ongoing interaction before triggering a skill check
    if (this->_currentInteraction != nullptr && this->_currentInteraction->IsPendingKill() == false)
    {
        // Activate the skill check associated with the current interaction
        // Delay parameter is hardcoded to 1.0f
        if (this->_skillCheck != nullptr)
        {
            this->_skillCheck->ActivateSkillCheck(this->_currentInteraction, 1.0f);
        }
    }
}

// starts at line 1624
void UPlayerInteractionHandler::ForceBeginSkillCheckProgress()
{
    this->_skillCheck->SetDisplayed(true);
    this->_skillCheck->BeginProgress();
}

// starts at line 1630
void UPlayerInteractionHandler::SetSkillCheckTimer(float delay)
{
    // Retrieve the World safely
    UWorld* world = this->WorldPrivate;
    if (world == nullptr)
    {
        world = this->GetWorld_Uncached();
    }

    if (world != nullptr)
    {
        FTimerManager& timerManager = world->GetTimerManager();

        // Set the timer to trigger the skill check
        // - Calls UPlayerInteractionHandler::OnSkillCheckTimerTrigger
        // - The timer loops (true)
        // - InFirstDelay is configured as -1.0f (using default delay)
        timerManager.SetTimer(this->_skillCheckTimerHandle, this, &UPlayerInteractionHandler::OnSkillCheckTimerTrigger, delay, true, -1.0f);
    }
}

// starts at line 1635
void UPlayerInteractionHandler::IncrementSuccessiveSkillCheckCount()
{
    this->_skillCheck->SkillCheckCount += 1;
}

// starts at line 1640
void UPlayerInteractionHandler::ResetSuccessiveSkillCheckCount()
{
    this->_skillCheck->SkillCheckCount = 0;
}

// starts at line 1645
bool UPlayerInteractionHandler::HasActiveSkillCheck() const
{
    return this->_skillCheck->_activated;
}

// starts at line 1650
void UPlayerInteractionHandler::AddPlayerDependency(ADBDPlayer* playerDependency)
{
    // Ensure the player dependency passed in is valid
    if (IsValid(playerDependency) == true && playerDependency->IsActorBeingDestroyed() == false)
    {
        if (playerDependency->IsValidImpl() == true)
        {
            // Ensure the owning character is valid
            if (this->OwningCharacter.IsValid() == true)
            {
                ADBDPlayer* owningCharacter = this->OwningCharacter.Get();
                
                // Notify the dependent player that they are being interacted with
                playerDependency->Multicast_SetInteractingPlayer(owningCharacter);

                // Add the valid player dependency to the internal tracking array
                this->_currentPlayerDependencies.Add(playerDependency);
            }
        }
    }
}

// starts at line 1659
void UPlayerInteractionHandler::ClearPlayerDependencies()
{
    // Iterate through all currently tracked player dependencies
    for (int32 i = 0; i < this->_currentPlayerDependencies.Num(); ++i)
    {
        ADBDPlayer* dependentPlayer = this->_currentPlayerDependencies[i];

        if (IsValid(dependentPlayer) == true && dependentPlayer->IsActorBeingDestroyed() == false)
        {
            if (dependentPlayer->IsValidImpl() == true)
            {
                // Clear the interacting player on the dependent player
                dependentPlayer->Multicast_SetInteractingPlayer(nullptr);
            }
        }
    }

    // Clear the array of dependencies locally
    this->_currentPlayerDependencies.Empty();
}

// starts at line 1671
bool UPlayerInteractionHandler::ArePlayerDependenciesLocked() const
{
    // Iterate through all the tracked player dependencies
    for (int32 i = 0; i < this->_currentPlayerDependencies.Num(); ++i)
    {
        ADBDPlayer* dependentPlayer = this->_currentPlayerDependencies[i];

        if (IsValid(dependentPlayer) == true && dependentPlayer->IsActorBeingDestroyed() == false)
        {
            if (dependentPlayer->IsValidImpl() == true)
            {
                // Checking a specific byte property at offset 0xC04
                if (dependentPlayer->IsInteracting == true)
                {
                    // If any dependent player is locked, the dependencies are not fully free/locked for a new interaction
                    return false;
                }
            }
        }
    }

    // Return true if no dependencies exist or if all existing dependencies are not locked
    return true;
}

// starts at line 41
UAuthoritativeMovementComponent::UAuthoritativeMovementComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) // Call to the base class constructor UActorComponent
{
    // Enable component ticking and start with tick enabled.
    // In UE4's FTickFunction, these bits map to bCanEverTick (2) and bStartWithTickEnabled (4).
    this->PrimaryComponentTick.bCanEverTick = true;
    this->PrimaryComponentTick.bStartWithTickEnabled = true;

    // Initialize boolean flags to false.
    this->_enabled = false;
    this->_registeredToOverlap = false;
}

// starts at line 47
void UAuthoritativeMovementComponent::BeginDestroy()
{
    this->UnregisterFromOverlap();
    this->UnregisterFromPawnDetectorOverlap();
    
    Super::BeginDestroy();
}

// starts at line 54
void UAuthoritativeMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if (this->_enabled == false)
    {
        return;
    }

    int32 offset = 0;
    
    // CVarEnableAnyAuthoritativeMovement check
    static IConsoleVariable* CVarEnableAnyAuthoritativeMovement = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.EnableAnyAuthoritativeMovement"));
    if (CVarEnableAnyAuthoritativeMovement->GetInt() == 0) 
    {
        return;
    }

    this->PrintAuthoritativeMovementEnabled();

    // Remove invalid players based on the predicate
    this->_playersToPush.RemoveAll([](ADBDPlayer* Player) 
    {
        // Standard UE macro replacing manual GUObjectArray evaluations and IsPendingKill checks
        if (IsValid(Player) == false || Player->IsActorBeingDestroyed() == true || Player->IsValidImpl() == false)
        {
            // Return true to remove the invalid player from the array
            return true;
        }

        // Keep the valid and authoritative player in the array
        return false; 
    });

    TArray<ADBDPlayer*> overlappingPlayersCopy = this->_playersToPush;

    const TArray<AActor*>* MoveIgnoreActors = this->GetMoveIgnoreActors();
    this->PrintIgnores(MoveIgnoreActors, offset);

    ADBDPlayer* OwnerPrivate = Cast<ADBDPlayer>(this->GetOwner());

    if (OwnerPrivate->CanAuthoritativePush() == false) 
    {
        return;
    }

    for (int32 i = 0; i < overlappingPlayersCopy.Num(); i++)
    {
        ADBDPlayer* currentOverlappingPlayer = overlappingPlayersCopy[i];

        if (IsValid(currentOverlappingPlayer) == false || currentOverlappingPlayer->IsActorBeingDestroyed() == true || currentOverlappingPlayer->IsValidImpl() == false)
        {
            continue;
        }

        if (currentOverlappingPlayer->CanBeAuthoritativePushed() == true)
        {
            bool bShouldIgnore = false;

            if (MoveIgnoreActors != nullptr)
            {
                if (MoveIgnoreActors->Contains(currentOverlappingPlayer) == true)
                {
                    bShouldIgnore = true;
                }
            }

            if (bShouldIgnore == false)
            {
                if (this->_stopIgnorePlayerOnEndOverlap.Contains(currentOverlappingPlayer) == false)
                {
                    this->PrintMove(currentOverlappingPlayer, offset);
                    this->MoveOverlappingPlayer(DeltaTime, currentOverlappingPlayer);
                }
            }
        }
    }
}

// starts at line 99
void UAuthoritativeMovementComponent::MoveOverlappingPlayer(float deltaTime, ADBDPlayer* otherPlayer)
{
    // Check if the component has authority using the method explicitly referenced in the disassembly
    if (this->HasAuthority() == false)
    {
        return;
    }

    // Retrieve the capsule component bound to this instance
    UCapsuleComponent* capsule = this->GetCapsule();

    // Standard UE macros replacing the manual GUObjectArray address/flag evaluations
    if (IsValid(capsule) == false)
    {
        return;
    }

    // Ensure the overlapping actor is not scheduled for destruction
    if (IsValid(otherPlayer) == false || otherPlayer->IsActorBeingDestroyed() == true || otherPlayer->IsValidImpl() == false)
    {
        return;
    }

    // Retrieve the capsule component from the overlapping player
    UCapsuleComponent* otherCapsule = otherPlayer->GetCapsuleComponent();
    if (IsValid(otherCapsule) == false)
    {
        return;
    }

    // Retrieve the main character associated with this component
    ADBDPlayer* character = this->GetCharacter();
    if (IsValid(character) == false)
    {
        return;
    }

    // Get the root components to extract translation data in world space
    USceneComponent* rootComponent = character->GetRootComponent();
    if (IsValid(rootComponent) == false)
    {
        return;
    }

    USceneComponent* otherRootComponent = otherPlayer->GetRootComponent();
    if (IsValid(otherRootComponent) == false)
    {
        return;
    }

    // Calculate the translation differences between the two entities
    FVector thisTranslation = rootComponent->GetComponentLocation();
    FVector otherTranslation = otherRootComponent->GetComponentLocation();

    FVector direction = otherTranslation - thisTranslation;
    
    // Zero out the Z-axis to calculate the vector operation strictly on the 2D plane
    direction.Z = 0.0f;

    // Calculate the scaled radii for both capsule components
    float thisRadius = capsule->GetScaledCapsuleRadius();
    float otherRadius = otherCapsule->GetScaledCapsuleRadius();

    // Compute the squared distance and the absolute distance
    float distanceSquared = direction.SizeSquared2D();
    float distance = FMath::Sqrt(distanceSquared);

    // Determine the exact depth of the capsule overlap
    float overlapAmount = (thisRadius + otherRadius) - distance;

    // Verify the overlap meets the minimal significant threshold 
    if (overlapAmount > 0.0001f) // Maps to literal 0.000099999997 from disassembly
    {
        FVector forceDirection = FVector::ZeroVector;

        // Validate that distance is large enough to avoid division by zero during normalization
        if (distanceSquared >= 0.00000001f) // Maps to literal 0.0000000099999999 from disassembly
        {
            forceDirection = direction.GetSafeNormal2D();
        }

        // Fetch the movement component of the overlapping player
        UPawnMovementComponent* movementComponent = otherPlayer->GetMovementComponent();
        if (IsValid(movementComponent) == true)
        {
            // Verify that the retrieved component descends from UCharacterMovementComponent
            if (movementComponent->IsA(UCharacterMovementComponent::StaticClass()) == true)
            {
                // Access the shadow values of the unexposed console variables for force limits
                static IConsoleVariable* CVarAuthoritativeMovementForceFactor = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.AuthoritativeMovementForce"));
                float maxForce = CVarAuthoritativeMovementForceFactor->GetFloat();

                static IConsoleVariable* CVarAuthoritativeMovementForceFactorMin = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.AuthoritativeMovementForceMin"));
                float minForce = CVarAuthoritativeMovementForceFactorMin->GetFloat();

                // Determine the proportional magnitude of the corrective force
                float overlapRatio = overlapAmount / (otherRadius + thisRadius);
                float forceMagnitude = (overlapRatio * (maxForce - minForce)) + minForce;

                // Construct the final directional vector to be applied
                FVector appliedForce = forceDirection * forceMagnitude;

                // Execute the function at the target offset directly derived from the disassembly
                movementComponent->AddForce(appliedForce);
            }
        }
    }
}

// starts at line 125
void UAuthoritativeMovementComponent::SetEnabled(bool value)
{
    // Check if the requested state is already the current state
    if (this->_enabled == value)
    {
        return;
    }

    // Apply the new state
    this->_enabled = value;

    if (value == true)
    {
        // Register component to overlap events if not already registered
        if (this->_registeredToOverlap == false)
        {
            this->RegisterToOverlap();
        }
        
        return;
    }

    // Remove invalid players based on a lambda predicate
    this->_playersToPush.RemoveAll([](ADBDPlayer* player)
    {
        // Standard UE macro replacing manual GUObjectArray evaluations and IsPendingKill checks
        if (IsValid(player) == false || player->IsActorBeingDestroyed() == true || player->IsValidImpl() == false)
        {
            // Return true to remove the invalid player from the array
            return true;
        }

        // Keep the valid and authoritative player in the array
        return false;
    });

    // Check if there are any players left to process
    if (this->_playersToPush.Num() <= 0)
    {
        return;
    }

    // Iterate through all players scheduled for push
    for (int32 i = 0; i < this->_playersToPush.Num(); ++i)
    {
        ADBDPlayer* player = this->_playersToPush[i];

        // Standard UE macro replacing manual GUObjectArray evaluations and IsPendingKill checks
        if (IsValid(player) == true && player->IsActorBeingDestroyed() == false && player->IsValidImpl() == true)
        {
            this->SetIgnorePlayerCollisionUntilEndOverlap(player, true);

            ADBDPlayer* ownerPlayer = Cast<ADBDPlayer>(this->GetOwner());
            if (IsValid(ownerPlayer) == true && ownerPlayer->IsActorBeingDestroyed() == false && ownerPlayer->IsValidImpl() == true)
            {
                // Access the target player's UAuthoritativeMovementComponent directly from the hardcoded offset
                UAuthoritativeMovementComponent* otherAuthMoveComp = player->_authoritativeMovementComponent;

                // The disassembly directly executes the call without a nullptr check
                otherAuthMoveComp->SetIgnorePlayerCollisionUntilEndOverlap(ownerPlayer, true);
            }
        }
    }
}

// starts at line 175
void UAuthoritativeMovementComponent::SetIgnorePlayerCollisionUntilEndOverlap(ADBDPlayer* otherPlayer, bool ignore)
{
    // Retrieve the owner of this component
    ADBDPlayer* ownerPlayer = Cast<ADBDPlayer>(this->GetOwner());

    // Check if the owner actor is valid using standard Unreal Engine macro
    // This replaces manual GUObjectArray address and flag evaluations
    if (IsValid(ownerPlayer) == false || ownerPlayer->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Execute virtual function at offset 0x1070 on the owner
    if (ownerPlayer->IsValidImpl() == false)
    {
        return nullptr;
    }

    // Check if the other actor is valid using standard Unreal Engine macro
    // This replaces manual GUObjectArray address and flag evaluations
    if (IsValid(OtherActor) == false || OtherActor->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Execute virtual function at offset 0x1070 on the owner
    if (OtherActor->IsValidImpl() == false)
    {
        return nullptr;
    }

    // Ensure the owner is not trying to apply collision rules to itself
    if (ownerPlayer == otherPlayer)
    {
        return;
    }

    if (ignore == true)
    {
        // Check if the player is already in the ignore list to avoid duplicates
        // This replaces the manual array iteration found in the disassembly
        if (this->_stopIgnorePlayerOnEndOverlap.Contains(otherPlayer) == false)
        {
            // Add the player to the list and notify the owner to ignore collision
            this->_stopIgnorePlayerOnEndOverlap.AddUnique(otherPlayer);
            ownerPlayer->AuthoritativeMovementIgnoreActorAdd(otherPlayer);
        }
    }
    else
    {
        // Check if the player is actually in the ignore list before attempting removal
        // This replaces the manual array iteration found in the disassembly
        if (this->_stopIgnorePlayerOnEndOverlap.Contains(otherPlayer) == true)
        {
            // Remove the player using the standard UE array function instead of the expanded lambda
            this->_stopIgnorePlayerOnEndOverlap.Remove(otherPlayer);
            ownerPlayer->AuthoritativeMovementIgnoreActorRemove(otherPlayer);
        }
    }
}

// starts at line 193
const TArray<AActor*>* UAuthoritativeMovementComponent::GetMoveIgnoreActors() const
{
    // Retrieve the capsule component associated with this component
    UCapsuleComponent* capsule = this->GetCapsule();

    // Standard UE macro replacing the manual GUObjectArray address and flag evaluations
    if (IsValid(capsule) == false)
    {
        return nullptr;
    }

    // Return the address of the MoveIgnoreActors array from the capsule component
    return &capsule->MoveIgnoreActors;
}

// starts at line 203
void UAuthoritativeMovementComponent::OnOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ADBDPlayer* ownerPlayer = Cast<ADBDPlayer>(this->GetOwner());

    // Check if the owner actor is valid using standard Unreal Engine macro
    // This replaces manual GUObjectArray address and flag evaluations
    if (IsValid(ownerPlayer) == false || ownerPlayer->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Execute virtual function at offset 0x1070 on the owner
    if (ownerPlayer->IsValidImpl() == false)
    {
        return nullptr;
    }

    // Check if the other actor is valid using standard Unreal Engine macro
    // This replaces manual GUObjectArray address and flag evaluations
    if (IsValid(OtherActor) == false || OtherActor->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Execute virtual function at offset 0x1070 on the owner
    if (OtherActor->IsValidImpl() == false)
    {
        return nullptr;
    }

    // Ensure the owner is not processing an overlap with itself
    if (ownerPlayer == otherPlayer)
    {
        return;
    }

    // Check if the player is already in the push list to avoid duplicates
    // This replaces the manual array iteration found in the disassembly
    if (this->_playersToPush.Contains(otherPlayer) == false)
    {
        // Add the player to the list
        // This replaces the manual ResizeGrow and element assignment operations
        this->_playersToPush.Add(otherPlayer);
    }
}

// starts at line 217
void UAuthoritativeMovementComponent::OnOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ADBDPlayer* ownerPlayer = Cast<ADBDPlayer>(this->GetOwner());

    // Check if the owner actor is valid using standard Unreal Engine macro
    // This replaces manual GUObjectArray address and flag evaluations
    if (IsValid(ownerPlayer) == false || ownerPlayer->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Execute virtual function at offset 0x1070 on the owner
    if (ownerPlayer->IsValidImpl() == false)
    {
        return nullptr;
    }

    // Check if the other actor is valid using standard Unreal Engine macro
    // This replaces manual GUObjectArray address and flag evaluations
    if (IsValid(OtherActor) == false || OtherActor->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Execute virtual function at offset 0x1070 on the owner
    if (OtherActor->IsValidImpl() == false)
    {
        return nullptr;
    }

    // Ensure the owner is not processing an overlap exit with itself
    if (ownerPlayer == otherPlayer)
    {
        return;
    }

    // Remove the player from the push list
    // This replaces the manual RemoveAll lambda implementation found in the disassembly
    this->_playersToPush.Remove(otherPlayer);
}

// starts at line ???
void UAuthoritativeMovementComponent::OnPawnDetectorOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool, const FHitResult&)
{
    return;
}

// starts at line 235
void UAuthoritativeMovementComponent::OnPawnDetectorOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ADBDPlayer* ownerPlayer = Cast<ADBDPlayer>(this->GetOwner());

    // Check if the owner actor is valid using standard Unreal Engine macro
    // This replaces manual GUObjectArray address and flag evaluations
    if (IsValid(ownerPlayer) == false || ownerPlayer->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Execute virtual function at offset 0x1070 on the owner
    if (ownerPlayer->IsValidImpl() == false)
    {
        return nullptr;
    }

    // Check if the other actor is valid using standard Unreal Engine macro
    // This replaces manual GUObjectArray address and flag evaluations
    if (IsValid(OtherActor) == false || OtherActor->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Execute virtual function at offset 0x1070 on the owner
    if (OtherActor->IsValidImpl() == false)
    {
        return nullptr;
    }

    // Instruct the component to stop ignoring collision for this player
    this->SetIgnorePlayerCollisionUntilEndOverlap(otherPlayer, false);
}

// starts at line 248
void UAuthoritativeMovementComponent::RegisterToOverlap()
{
    // Retrieve the capsule component associated with this component
    UCapsuleComponent* capsule = this->GetCapsule();

    // Standard UE macro replacing the manual GUObjectArray validity and flag checks
    if (IsValid(capsule) == false)
    {
        return;
    }

    // Verify that the component is not already registered to avoid duplicate bindings
    if (this->_registeredToOverlap == false)
    {
        // Standard UE macro replacing the manual string parsing and __Internal_AddDynamic calls
        // Binds the OnOverlapEnter function to the capsule's begin overlap event (offset 0x4E0)
        capsule->OnComponentBeginOverlap.AddDynamic(this, &UAuthoritativeMovementComponent::OnOverlapEnter);

        // Binds the OnOverlapExit function to the capsule's end overlap event (offset 0x4F0)
        capsule->OnComponentEndOverlap.AddDynamic(this, &UAuthoritativeMovementComponent::OnOverlapExit);

        // Update the state to indicate that overlap events are now registered
        this->_registeredToOverlap = true;
    }
}

// starts at line 261
void UAuthoritativeMovementComponent::UnregisterFromOverlap()
{
    // Retrieve the capsule component associated with this component
    UCapsuleComponent* capsule = this->GetCapsule();

    // Standard UE macro replacing the manual GUObjectArray validity and flag checks
    if (IsValid(capsule) == false)
    {
        return;
    }

    // Verify that the component is currently registered to overlap events
    if (this->_registeredToOverlap == true)
    {
        // Standard UE macro replacing the manual string parsing, RemoveInternal, and CompactInvocationList calls
        // Unbinds the OnOverlapEnter function from the capsule's begin overlap event
        capsule->OnComponentBeginOverlap.RemoveDynamic(this, &UAuthoritativeMovementComponent::OnOverlapEnter);

        // Unbinds the OnOverlapExit function from the capsule's end overlap event
        capsule->OnComponentEndOverlap.RemoveDynamic(this, &UAuthoritativeMovementComponent::OnOverlapExit);

        // Update the state to indicate that overlap events are no longer registered
        this->_registeredToOverlap = false;
    }
}

// starts at line 271
void UAuthoritativeMovementComponent::SetPawnDetector(UCapsuleComponent* pawnDetector)
{
    this->_pawnDetector = pawnDetector;
    this->RegisterToPawnDetectorOverlap();
}

// starts at line 277
void UAuthoritativeMovementComponent::RegisterToPawnDetectorOverlap()
{
    // Check if the pawn detector is currently initialized
    // Standard UE macro replacing manual GUObjectArray address/flag evaluations
    if (IsValid(this->_pawnDetector) == false)
    {
        return;
    }

    // Bind the OnPawnDetectorOverlapEnter to the pawn detector's begin overlap event
    // Binding OnPawnDetectorOverlapEnter at offset 0x4E0
    this->_pawnDetector->OnComponentBeginOverlap.AddDynamic(this, &UAuthoritativeMovementComponent::OnPawnDetectorOverlapEnter);

    // Bind the OnPawnDetectorOverlapExit to the pawn detector's end overlap event
    // This happens at offset 0x4F0
    this->_pawnDetector->OnComponentEndOverlap.AddDynamic(this, &UAuthoritativeMovementComponent::OnPawnDetectorOverlapExit);
}

// starts at line 286
void UAuthoritativeMovementComponent::UnregisterFromPawnDetectorOverlap()
{
    // Check if the pawn detector is currently initialized and valid
    // Standard UE macro replacing manual GUObjectArray validity and flag checks
    if (IsValid(this->_pawnDetector) == false)
    {
        return;
    }

    // Unbind the OnPawnDetectorOverlapEnter function from the pawn detector's begin overlap event
    // This standard UE macro replaces the manual string parsing (wcsstr), RemoveInternal, and CompactInvocationList calls found in the disassembly
    this->_pawnDetector->OnComponentBeginOverlap.RemoveDynamic(this, &UAuthoritativeMovementComponent::OnPawnDetectorOverlapEnter);

    // Unbind the OnPawnDetectorOverlapExit function from the pawn detector's end overlap event
    // This standard UE macro replaces the manual string parsing (wcsstr), RemoveInternal, and CompactInvocationList calls found in the disassembly
    this->_pawnDetector->OnComponentEndOverlap.RemoveDynamic(this, &UAuthoritativeMovementComponent::OnPawnDetectorOverlapExit);
}

// starts at line 295
bool UAuthoritativeMovementComponent::HasAuthority() const
{
    // Retrieve the owner actor of this component
    AActor* ownerActor = this->GetOwner();

    // Check if the owner actor is valid and not pending kill using standard Unreal Engine macro
    // This replaces the manual GUObjectArray validity, array bounds, and flag checks found in the disassembly
    if (IsValid(ownerActor) == true && ownerActor->IsActorBeingDestroyed() == false)
    {
        return false;
    }

    // Verify if the owner actor has network authority
    // The value 3 corresponds to the ROLE_Authority enum value in ENetRole
    if (ownerActor->Role == ROLE_Authority)
    {
        return true;
    }

    return false;
}

// starts at line 305
ADBDPlayer* UAuthoritativeMovementComponent::GetCharacter() const
{
    // Safely cast and return the owner as an ADBDPlayer
    return Cast<ADBDPlayer>(this->GetOwner());
}

// starts at line 310
UCapsuleComponent* UAuthoritativeMovementComponent::GetCapsule() const
{
    // Retrieve the owner actor of this component
    ADBDPlayer* ownerPlayer = Cast<ADBDPlayer>(this->GetOwner());

    // Check if the owner actor is valid using standard Unreal Engine macro
    // This replaces manual GUObjectArray address and flag evaluations
    if (IsValid(ownerPlayer) == false || ownerPlayer->IsActorBeingDestroyed() == true)
    {
        return nullptr;
    }

    // Execute virtual function at offset 0x1070 on the owner
    if (ownerPlayer->IsValidImpl() == false)
    {
        return nullptr;
    }

    // Return the CapsuleComponent from the ADBDPlayer
    // 0x3F0 is the offset for the CapsuleComponent in the ADBDPlayer structure
    return ownerPlayer->GetCapsuleComponent();
}

// starts at line 326
void UAuthoritativeMovementComponent::PrintAuthoritativeMovementEnabled() const
{
    // Access the shadow value of the unexposed console variable CVarDebugAuthoritativeMovement
    static IConsoleVariable* CVarDebugAuthoritativeMovement = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.DebugAuthoritativeMovement"));

    if (CVarDebugAuthoritativeMovement->GetInt() != 0)
    {
        // Retrieve the name of this object and convert it to a string
        FString objectName = this->GetFName().ToString();

        // Convert the internal boolean state to a string representation
        const TCHAR* enabledStateStr = dbd::ToTCHAR(this->_enabled);

        // Format the message
        FString debugMessage = FString::Printf(TEXT("[%s] AuthoritativeMovement : [%s]"), *objectName, enabledStateStr);

        // Access the global GEngine variable
        if (GEngine != nullptr)
        {
            // Add an on-screen debug message
            // The disassembly shows a hallucinatory call to PostSerialize with a specific signature
            // This maps to AddOnScreenDebugMessage(uint64 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage, bool bNewerOnTop, const FVector2D& TextScale)
            GEngine->AddOnScreenDebugMessage(this->GetUniqueID(), -1.0f, FColor::White, debugMessage, true, FVector2D::UnitVector);
        }
    }
}

// starts at line 342
void UAuthoritativeMovementComponent::PrintIgnores(const TArray<AActor*>* ignoreActors, int32& offset) const
{
    // Access the shadow value of the unexposed console variable CVarDebugAuthoritativeMovement
    static IConsoleVariable* CVarDebugAuthoritativeMovement = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.DebugAuthoritativeMovement"));

    // Ensure the debug flag is enabled and the array pointer is valid
    if (CVarDebugAuthoritativeMovement->GetInt() != 0 && ignoreActors != nullptr)
    {
        // Iterate through all ignored actors provided in the array
        // Standard UE macro replaces the manual bounds evaluation
        for (int32 i = 0; i < ignoreActors->Num(); ++i)
        {
            AActor* actor = (*ignoreActors)[i];

            // Assign raw pointer to a weak pointer for safe validation
            TWeakObjectPtr<AActor> weakActor = actor;

            if (weakActor.IsValid() == true)
            {
                // Increment the display offset
                offset++;

                // Extract the names for both the target actor and this component
                FString actorName = weakActor.Get()->GetFName().ToString();
                FString thisName = this->GetFName().ToString();

                // Generate the output debug message
                FString debugMessage = FString::Printf(TEXT("[%s] ignores [%s]"), *thisName, *actorName);

                if (GEngine != nullptr)
                {
                    // Create a unique key by combining internal index and the current offset
                    uint64 displayKey = this->GetUniqueID() + offset;

                    // Add the formatted string to the on-screen debug message
                    // The disassembly shows a hallucinatory call to PostSerialize with a specific signature
                    // This maps to AddOnScreenDebugMessage(uint64 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage, bool bNewerOnTop, const FVector2D& TextScale)
                    GEngine->AddOnScreenDebugMessage(displayKey, -1.0f, FColor::White, debugMessage, true, FVector2D::UnitVector);
                }
            }
        }
    }
}

// starts at line 367
void UAuthoritativeMovementComponent::PrintMove(ADBDPlayer* const otherCharacter, int32& inOutOffset) const
{
    // Access the shadow value of the unexposed console variable CVarDebugAuthoritativeMovement
    static IConsoleVariable* CVarDebugAuthoritativeMovement = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.DebugAuthoritativeMovement"));
    if (CVarDebugAuthoritativeMovement->GetInt() != 0)
    {
        // Check if the other character is valid using standard Unreal Engine macro
        // Although the disassembly doesn't show an explicit IsValid check, it's unsafe to dereference otherCharacter without one
        if (IsValid(otherCharacter) == false)
        {
            return;
        }

        // Increment the display offset
        inOutOffset++;

        // Extract the names for both the other character and this component
        FString otherCharacterName = otherCharacter->GetFName().ToString();
        FString thisName = this->GetFName().ToString();

        // Generate the output debug message
        FString debugMessage = FString::Printf(TEXT("[%s] moves [%s]"), *thisName, *otherCharacterName);

        if (GEngine != nullptr)
        {
            // Create a unique key by combining internal index and the current offset
            uint64 displayKey = this->GetUniqueID() + inOutOffset;

            // Add the formatted string to the on-screen debug message
            // The disassembly shows a hallucinatory call to PostSerialize with a specific signature
            // This maps to AddOnScreenDebugMessage(uint64 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage, bool bNewerOnTop, const FVector2D& TextScale)
            GEngine->AddOnScreenDebugMessage(displayKey, -1.0f, FColor::White, debugMessage, true, FVector2D::UnitVector);
        }
    }
}

// starts at line 389
bool UAuthoritativeMovementComponent::IsIgnoringActor(AActor* actor)
{
    // Ensure the incoming actor pointer is valid before performing checks
    if (IsValid(actor) == false)
    {
        return false;
    }

    // Check if the actor is present in the _stopIgnorePlayerOnEndOverlap array
    // This uses the standard UE TArray::Contains function, replacing the manual memory iteration found in the disassembly
    if (this->_stopIgnorePlayerOnEndOverlap.Contains(actor) == true)
    {
        return true;
    }

    // Check if the actor is present in the _ignorePlayers array
    // This uses the standard UE TArray::Contains function, replacing the manual memory iteration found in the disassembly
    if (this->_ignorePlayers.Contains(actor) == true)
    {
        return true;
    }

    // Return false if the actor was not found in either ignore list
    return false;
}

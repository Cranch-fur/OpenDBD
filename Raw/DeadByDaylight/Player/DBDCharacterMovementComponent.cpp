// starts at line 48
UDBDCharacterMovementComponent::UDBDCharacterMovementComponent(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
    // Initialize projected wall sliding factor
    this->ProjectedWallSlidingFactor = 1.0f;

    // Initialize sprint effector to default (NAME_None)
    this->SprintEffector = NAME_None;

    // Clear bAlwaysCheckFloor bit flag (bit 15 at offset 0x3AC corresponding to ~0x8000u)
    this->bAlwaysCheckFloor = false;

    // Initialize navigation flag
    this->_allowNavigation = false;

    // Initialize random input variables
    this->_randomInputTime = 0.0f;
    this->_randomInputAmplitude = 0.0f;

    // Initialize random input multipliers
    this->RandomInputFrontMultiplier = 0.8f;
    this->RandomInputBackMultiplier = 0.2f;
    this->RandomInputSideMultiplier = 1.0f;

    // Set maximum simulation time step
    this->MaxSimulationTimeStep = 0.1f;
}

// starts at line 57
void UDBDCharacterMovementComponent::BeginDestroy()
{
    // Unregister this component from the movement modifier detector overlap events before destruction
    this->UnregisterFromMovementModifierDetectorOverlap();

    // Clear the reference to the movement modifier detector component
    this->_movementModifierDetector = nullptr;

    // Clear the reference to the movement modifier actor
    this->_movementModifierActor = nullptr;

    // Call the parent class implementation of BeginDestroy
    Super::BeginDestroy();
}

// starts at line 76
void UDBDCharacterMovementComponent::AddInputVector(FVector WorldVector, bool bForce)
{
    // Check if navigation is allowed or if the input should be forced
    if (this->_allowNavigation == true || bForce == true)
    {
        // Call the parent class implementation to apply the input vector
        Super::AddInputVector(WorldVector, bForce);
    }
}

// starts at line 84
FVector UDBDCharacterMovementComponent::ComputeSlideVector(const FVector& Delta, float Time, const FVector& Normal, const FHitResult& Hit) const
{
    // Check if the movement is constrained to a plane. If so, fallback to the base class implementation.
    if (this->bConstrainToPlane != 0)
    {
        return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
    }

    // Calculate the dot product between the Normal and Delta vectors
    float deltaDotNormal = FVector::DotProduct(Normal, Delta);

    // Calculate the projection of the Delta vector onto the plane defined by the Normal
    FVector projectedDelta = Delta - (Normal * deltaDotNormal);

    // Calculate the magnitude (size) of the projected delta vector
    float projectedDeltaSize = projectedDelta.Size();

    // If the projected delta size is extremely small (KINDA_SMALL_NUMBER), fallback to the base class implementation
    if (projectedDeltaSize <= KINDA_SMALL_NUMBER) 
    {
        return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
    }

    // Calculate the magnitude (size) of the original Delta vector
    float deltaSize = Delta.Size();

    // Calculate the time multiplier based on the projected wall sliding factor
    float sizeRatio = deltaSize * (1.0f / projectedDeltaSize);
    float timeMultiplier = (((sizeRatio - 1.0f) * this->ProjectedWallSlidingFactor) + 1.0f) * Time;

    // Apply the time multiplier to the projected delta
    FVector projected = projectedDelta * timeMultiplier;

    // Check custom console variables for the drop stagger fix
    static IConsoleVariable* CVarDropStaggerFix = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.DropStaggerFix"));
    static IConsoleVariable* CVarPreventDropStaggerFixOnWalkableSlope = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.PreventDropStaggerFixOnWalkableSlope"));

    if (CVarDropStaggerFix->GetInt() != 0 && this->IsFalling() == true)
    {
        if (CVarPreventDropStaggerFixOnWalkableSlope->GetInt() == 0 || this->WalkableFloorZ > Hit.Normal.Z)
        {
            // Apply slope boosting to the projected result
            return this->HandleSlopeBoosting(projected, Delta, Time, Normal, Hit);
        }
    }

    // Return the final projected vector
    return projected;
}

// starts at line 109
void UDBDCharacterMovementComponent::TickComponent(float deltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    // Call the parent class implementation
    Super::TickComponent(deltaTime, TickType, ThisTickFunction);

    if (this->CharacterOwner != nullptr)
    {
        AController* Controller = this->CharacterOwner->Controller;
        
        // Check if the Controller is valid using the standard Unreal Engine macro
        if (IsValid(Controller) == true)
        {
            static IConsoleVariable* CVarWiggleImpulse = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.WiggleImpulse"));
            if (CVarWiggleImpulse->GetInt() != 0 && this->_randomInputAmplitude > 0.0f)
            {
                this->_randomInputTime = this->_randomInputTime + deltaTime;

                FVector input = this->_perlinNoise.GetValue(this->_randomInputTime);
                
                float randomInputAmplitudeMultiplier;
                
                // Adjust multiplier based on the input's forward/backward direction
                if (input.X <= 0.0f)
                {
                    randomInputAmplitudeMultiplier = this->_randomInputAmplitude * this->RandomInputBackMultiplier;
                }
                else
                {
                    randomInputAmplitudeMultiplier = this->_randomInputAmplitude * this->RandomInputFrontMultiplier;
                }
                
                input.X = input.X * randomInputAmplitudeMultiplier;
                input.Y = input.Y * (this->_randomInputAmplitude * this->RandomInputSideMultiplier);
                
                FRotator controlRotation = Controller->GetControlRotation();
                FVector worldSpaceInput = controlRotation.RotateVector(input);
                
                float movementScale = 1.0f;
                
                ADBDPlayer* dbdPlayer = Cast<ADBDPlayer>(this->CharacterOwner);
                if (dbdPlayer != nullptr && dbdPlayer->IsValidImpl() == true)
                {
                    UPerkManager* perkManager = dbdPlayer->GetPerkManager();
                    if (perkManager != nullptr)
                    {
                        float perkModifier = perkManager->GetPerkModifier(EGameplayModifierType::VE_ModifyWiggleIntensity, EGameplayModifierSource::VE_All);
                        movementScale = movementScale + perkModifier;
                    }
                }
                
                this->CharacterOwner->AddMovementInput(worldSpaceInput, movementScale, false);
            }
        }
    }

    // Update hit speed boost timer
    if (this->_hitSpeedBoostTimer.IsDone() == false)
    {
        this->_hitSpeedBoostTimer.Update(deltaTime);
    }

    // Update sprint speed boost duration timer
    if (this->_sprintSpeedBoostDurationTimer.IsDone() == false)
    {
        this->_sprintSpeedBoostDurationTimer.Update(deltaTime);
    }
}

// starts at line 159
float UDBDCharacterMovementComponent::GetMaxSpeed() const
{
    // Retrieve the base maximum speed from the parent class
    float maxSpeed = Super::GetMaxSpeed();

    // Ensure the character owner is assigned
    if (this->CharacterOwner != nullptr)
    {
        // Try to cast the character owner to ADBDPlayer
        ADBDPlayer* dbdPlayer = Cast<ADBDPlayer>(this->CharacterOwner);
        
        // Ensure the cast was successful
        if (dbdPlayer != nullptr)
        {
            // Verify if the dbdPlayer object is valid using the standard Unreal Engine macro
            if (IsValid(dbdPlayer) == true && dbdPlayer->IsValidImpl() == true)
            {
                // Check a specific boolean state on the player (offset 0xC04)
                if (dbdPlayer->IsInteracting == true)
                {
                    // Apply a specific float multiplier from the player (offset 0x101C)
                    return maxSpeed * dbdPlayer->_interactingMaxSpeedPercent;
                }
                // If the state is false, check if the hit speed boost timer is currently active
                else if (this->_hitSpeedBoostTimer.IsDone() == false)
                {
                    // Verify if the speed boost curve asset is valid
                    if (IsValid(this->OnHitMaxSpeedCurve) == true)
                    {
                        // Calculate the elapsed time percentage
                        float percentTimeElapsed = this->_hitSpeedBoostTimer.GetPercentTimeElapsed();
                        
                        // Evaluate the curve based on the elapsed time to get the speed multiplier
                        float curveMultiplier = this->OnHitMaxSpeedCurve->FloatCurve.Eval(percentTimeElapsed, 0.0f);
                        
                        return maxSpeed * curveMultiplier;
                    }
                }
            }
        }
    }

    // Return the unmodified base maximum speed if no conditions altered it
    return maxSpeed;
}

// starts at line 180
void UDBDCharacterMovementComponent::OnHit()
{
    // Retrieve the current world. If the cached WorldPrivate is null, fetch it uncached.
    UWorld* world = this->WorldPrivate;
    if (world == nullptr)
    {
        world = this->GetWorld_Uncached();
    }

    // Retrieve the owning game instance from the world (note: assembly assumes world is valid here)
    UGameInstance* owningGameInstance = world->OwningGameInstance;

    // Check if the game instance is valid
    if (owningGameInstance != nullptr)
    {
        // Attempt to cast the generic game instance to the specific Dead By Daylight game instance
        UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(owningGameInstance);

        // Verify the cast was successful
        if (dbdGameInstance != nullptr)
        {
            // Verify the object is fully valid and not pending kill
            if (IsValid(dbdGameInstance) == true)
            {
                // Retrieve the tunable design manager and fetch the speed boost duration
                // Passed 'true' for warnIfRowMissing
                float speedBoostDuration = dbdGameInstance->DesignTunables->GetTunableValue(TunableValues::ON_HIT_SPEED_BOOST_SECONDS, true);

                // Reset the speed boost timer with the newly fetched duration
                this->_hitSpeedBoostTimer.Reset(speedBoostDuration);
            }
        }
    }
}

// starts at line 202
bool UDBDCharacterMovementComponent::CanSprint()
{
    // Check if the sprint speed boost duration timer is currently active
    if (this->_sprintSpeedBoostDurationTimer.IsDone() == false)
    {
        return false;
    }

    // If there is no character owner, sprint is allowed by default
    if (this->CharacterOwner == nullptr)
    {
        return true;
    }

    // Attempt to cast the character owner to a Camper Player (Survivor)
    ACamperPlayer* camperPlayer = Cast<ACamperPlayer>(this->CharacterOwner);
    
    // If the owner is not a camper, bypass camper-specific sprint checks
    if (camperPlayer == nullptr)
    {
        return true;
    }

    // Verify if the camper player object is valid using the standard Unreal Engine macro
    if (IsValid(camperPlayer) == false)
    {
        return true;
    }

    if (camperPlayer->IsValidImpl() == false)
    {
        return true;
    }

    // Check camper-specific states that prevent sprinting
    if (camperPlayer->bIsCrouched == true)
    {
        return false;
    }

    if (camperPlayer->IsCrawling() == true)
    {
        return false;
    }

    if (camperPlayer->IsInteracting == true)
    {
        return false;
    }

    // If all checks pass, the camper can sprint
    return true;
}

// starts at line 228
bool UDBDCharacterMovementComponent::IsApplyingRandomInput() const
{
    // Check if the custom console variable for wiggle impulse is enabled
    // and if the current random input amplitude is strictly positive
    static IConsoleVariable* CVarWiggleImpulse = IConsoleManager::Get().FindConsoleVariable(TEXT("dbd.WiggleImpulse"));
    if (CVarWiggleImpulse->GetInt() != 0 && this->_randomInputAmplitude > 0.0f)
    {
        return true;
    }

    return false;
}

// starts at line 237
void UDBDCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    // Call the parent class implementation first
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    // Check if the current movement mode is MOVE_Falling (enum value 3)
    if (this->MovementMode == MOVE_Falling)
    {
        // Check if there is a valid character owner
        if (this->CharacterOwner != nullptr)
        {
            // Attempt to cast the character owner to ACamperPlayer
            ACamperPlayer* camperPlayer = Cast<ACamperPlayer>(this->CharacterOwner);
            
            // Check if the cast was successful
            if (camperPlayer != nullptr)
            {
                // Verify the object is fully valid and not pending kill using Unreal's macro
                if (IsValid(camperPlayer) == true && camperPlayer->IsValidImpl() == true)
                {
                    // Set the fall from height logic on the camper player
                    camperPlayer->SetFallFromHeight();
                }
            }
        }
    }
}

// starts at line 251
void UDBDCharacterMovementComponent::SetMovementModifierDetector(UCapsuleComponent* movementModifierDetector)
{
    // Assign the provided capsule component as the new movement modifier detector
    this->_movementModifierDetector = movementModifierDetector;

    // Register to overlap events for the newly assigned movement modifier detector
    this->RegisterToMovementModifierDetectorOverlap();
}

// starts at line 257
void UDBDCharacterMovementComponent::RegisterToMovementModifierDetectorOverlap()
{
    // Check if the movement modifier detector (capsule component) is set
    if (this->_movementModifierDetector != nullptr)
    {
        // Use the standard Unreal Engine macro to ensure the object is valid and not pending kill
        if (IsValid(this->_movementModifierDetector) == true)
        {
            // Register the 'Enter' overlap delegate
            this->_movementModifierDetector->OnComponentBeginOverlap.AddDynamic(this, &UDBDCharacterMovementComponent::OnMovementModifierDetectorOverlapEnter);
            
            // Register the 'Exit' overlap delegate
            this->_movementModifierDetector->OnComponentEndOverlap.AddDynamic(this, &UDBDCharacterMovementComponent::OnMovementModifierDetectorOverlapExit);
        }
    }
}

// starts at line 266
void UDBDCharacterMovementComponent::UnregisterFromMovementModifierDetectorOverlap()
{
    // Check if the movement modifier detector (capsule component) is set
    if (this->_movementModifierDetector != nullptr)
    {
        // Use the standard Unreal Engine macro to ensure the object is valid and not pending kill
        if (IsValid(this->_movementModifierDetector) == true)
        {
            // Unregister the 'Enter' overlap delegate
            this->_movementModifierDetector->OnComponentBeginOverlap.RemoveDynamic(this, &UDBDCharacterMovementComponent::OnMovementModifierDetectorOverlapEnter);
            
            // Unregister the 'Exit' overlap delegate
            this->_movementModifierDetector->OnComponentEndOverlap.RemoveDynamic(this, &UDBDCharacterMovementComponent::OnMovementModifierDetectorOverlapExit);
        }
    }
}

// starts at line 276
void UDBDCharacterMovementComponent::OnMovementModifierDetectorOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
    this->_movementModifierActor = nullptr;
}

// starts at line 285
void UDBDCharacterMovementComponent::OnMovementModifierDetectorOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Ensure the overlapping actor is valid
    if (OtherActor != nullptr)
    {
        // Try to cast the overlapping actor to an AMovementModifierActor
        AMovementModifierActor* modifierActor = Cast<AMovementModifierActor>(OtherActor);
        
        // If the cast is successful, it means the other actor is indeed a movement modifier
        if (modifierActor != nullptr)
        {
            // Verify that the actor is fully valid and not pending kill using the standard Unreal macro
            if (IsValid(modifierActor) == true)
            {
                // If the overlapped movement modifier is not the one currently tracked
                if (modifierActor != this->_movementModifierActor)
                {
                    // Update the currently tracked movement modifier actor
                    this->_movementModifierActor = modifierActor;
                }
            }
        }
    }
}

// starts at line 290
void UDBDCharacterMovementComponent::RefreshMovementModeOnTeleport(UPrimitiveComponent* OldBase, UPrimitiveComponent* NewBase)
{
    // Check if the current movement mode is not MOVE_Flying (enum value 5)
    if (this->MovementMode != MOVE_Flying)
    {
        // Call the parent class implementation to refresh the movement mode on teleport
        Super::RefreshMovementModeOnTeleport(OldBase, NewBase);
    }
}

// starts at line 299
void UDBDCharacterMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
    // Check if the character owner is assigned
    if (this->CharacterOwner != nullptr)
    {
        // Attempt to cast the generic CharacterOwner to a specific ADBDPlayer
        ADBDPlayer* dbdPlayer = Cast<ADBDPlayer>(this->CharacterOwner);
        
        // If the cast is successful, it means the owner is a Dead by Daylight player
        if (dbdPlayer != nullptr)
        {
            // Verify if the dbdPlayer object is valid using the standard Unreal Engine macro
            if (IsValid(dbdPlayer) == true && dbdPlayer->IsValidImpl() == true)
            {
                // Retrieve a UCharacterSnappingComponent from the player (offset 0xE58)
                UCharacterSnappingComponent* snappingComponent = dbdPlayer->GetSnappingComponent();
                
                if (snappingComponent != nullptr && IsValid(snappingComponent) == true)
                {
                    // If the component is actively snapping
                    if (snappingComponent->IsSnapping() == true)
                    {
                        // Check a specific boolean state on the snapping component (offset 0x115)
                        if (snappingComponent->_snapPosition == true)
                        {
                            // If all these conditions are met, skip calculating the velocity entirely
                            // This means velocity calculation is deferred or handled elsewhere during this specific snapping state
                            return;
                        }
                    }
                }
            }
        }
    }

    // Default behavior: calculate velocity using the parent class implementation
    Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);
}

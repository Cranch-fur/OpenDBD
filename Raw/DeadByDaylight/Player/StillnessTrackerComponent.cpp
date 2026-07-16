// starts at line 28
UStillnessTrackerComponent::UStillnessTrackerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initialize default threshold and stillness configuration values
    this->SpeedThreshold = 300.0f;
    this->DistanceThreshold = 400.0f;
    this->DistanceDecay = 1.0f;
    this->DecayWhileMoving = 0;
    this->StillnessThreshhold = 0.7f;
    this->StillnessTimerLimit = 120.0f;
    this->DecaySpeedMultiplier = 2.0f;

    // The disassembly performs a bitwise OR operation on the bitfield at offset 0xA0 with 0x800000 (bit 23)
    // In UActorComponent, bit 23 corresponds to bWantsInitializeComponent
    this->bWantsInitializeComponent = true;

    // The disassembly performs bitwise operations on the FActorComponentTickFunction bitfield at offset 0x3C
    // "and al, 0FBh" clears bit 3 (value 4), which corresponds to bStartWithTickEnabled
    // "or al, 2" sets bit 2 (value 2), which corresponds to bCanEverTick
    this->PrimaryComponentTick.bStartWithTickEnabled = false;
    this->PrimaryComponentTick.bCanEverTick = true;
}

// starts at line 39
void UStillnessTrackerComponent::BeginPlay()
{
    // Call the parent class BeginPlay
    Super::BeginPlay();

    // Retrieve the owner and cast it to ADBDPlayer. 
    // The disassembly explicitly checks the class tree index and num children, 
    // which is the standard behavior of Unreal Engine's Cast<T> template.
    AActor* Owner = this->GetOwner();
    ADBDPlayer* PlayerOwner = Cast<ADBDPlayer>(Owner);

    // Assign the owner to the weak object pointer
    this->_trackedPlayer = PlayerOwner;

    // Retrieve the World from the component's private field
    UWorld* World = this->WorldPrivate;
    
    // If the World is null, retrieve it using the uncached method
    if (World == nullptr)
    {
        World = this->GetWorld_Uncached();
    }

    ADBDGameState* GameState = nullptr;
    if (World != nullptr)
    {
        // Retrieve the GameState (offset 0x58 in UWorld) and cast it to ADBDGameState
        GameState = Cast<ADBDGameState>(World->GameState);
    }

    // The disassembly performs multiple checks: null check, GUObjectArray flags (0x20000000 for Unreachable/PendingKill), 
    // and Actor's bIsPendingKill flag (bit 4 at offset 0x140). 
    // These combined checks represent the standard inline expansion of Unreal Engine's IsValid() global function.
    if (IsValid(GameState) == true)
    {
        // The extensive string manipulation (wcsstr) and __Internal_AddDynamic calls 
        // are the standard expansion of Unreal Engine's AddDynamic macro.
        // The delegate is located at offset 0x540 within the ADBDGameState class.
        GameState->OnLevelReadyToPlay.AddDynamic(this, &UStillnessTrackerComponent::StartTracking);
    }
    else
    {
        // If the GameState is invalid, enable the component tick.
        // The disassembly calls the virtual function at offset 0x2B8, 
        // which corresponds to SetComponentTickEnabled in the provided VTable.
        this->SetComponentTickEnabled(true);
    }
}

// starts at line 58
void UStillnessTrackerComponent::StartTracking()
{
    // Call the virtual function SetComponentTickEnabled, which is located at offset 0x2B8 in the VTable.
    // The disassembly sets the second argument (dl register) to 1, representing true.
    this->SetComponentTickEnabled(true);
}

// starts at line 63
void UStillnessTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    // Call the parent class implementation of TickComponent
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Update the component's internal stillness parameters based on the time passed since the last frame
    this->UpdateStillness(DeltaTime);

    // Retrieve the current stillness value by calling the virtual function at VTable offset 0x350
    float CurrentStillness = this->GetStillness();

    // Check if the current stillness value is less than the required threshold to be considered still
    if (CurrentStillness < this->StillnessThreshhold)
    {
        // The actor is considered still, accumulate the stillness timer by adding DeltaTime
        this->_stillnessTimer = this->_stillnessTimer + DeltaTime;

        // Ensure the stillness timer does not fall below zero
        if (this->_stillnessTimer < 0.0f)
        {
            this->_stillnessTimer = 0.0f;
        }
        else
        {
            // Cap the stillness timer to the predefined limit using Unreal Engine's math library
            this->_stillnessTimer = FMath::Min(this->StillnessTimerLimit, this->_stillnessTimer);
        }
    }
    else
    {
        // The actor is moving beyond the threshold, decrease the stillness timer
        this->_stillnessTimer = this->_stillnessTimer - (DeltaTime * this->DecaySpeedMultiplier);

        // Ensure the stillness timer does not fall below zero
        if (this->_stillnessTimer < 0.0f)
        {
            this->_stillnessTimer = 0.0f;
        }
        else
        {
            // Cap the stillness timer to the predefined limit
            this->_stillnessTimer = FMath::Min(this->StillnessTimerLimit, this->_stillnessTimer);
        }
    }
}

// starts at line 80
float UStillnessTrackerComponent::GetStillness() const
{
    // Check if the weak pointer to the tracked player is valid
    if (this->_trackedPlayer.IsValid() == true)
    {
        // Retrieve the tracked player object from the weak pointer
        ADBDPlayer* TrackedPlayer = this->_trackedPlayer.Get();
        
        // Ensure the retrieved player pointer is valid
        if (TrackedPlayer == nullptr)
        {
            return this->_stillness;
        }

        // Retrieve the current interaction from the player
        UInteractionDefinition* CurrentInteraction = TrackedPlayer->GetCurrentInteraction();

        // Check if the interaction object is valid using Unreal Engine's standard IsValid macro
        // This acts as a replacement for the manual GUObjectArray flags check seen in the disassembly
        if (IsValid(CurrentInteraction) == false)
        {
            return this->_stillness;
        }

        // Check if the current interaction affects stillness (boolean at offset 0x309)
        if (CurrentInteraction->AffectsStillness == true)
        {
            // If the interaction affects stillness, return the maximum stillness value
            return 1.0f;
        }
    }

    // Return the current internally tracked stillness value if no conditions override it
    return this->_stillness;
}

// starts at line 90
float UStillnessTrackerComponent::GetSpeedStillness() const
{
    // Check if the weak pointer to the tracked player is valid
    if (this->_trackedPlayer.IsValid() == false)
    {
        return 0.0f;
    }

    // Check if the speed threshold is greater than 0.0f to avoid division by zero
    if (this->SpeedThreshold <= 0.0f)
    {
        return 0.0f;
    }

    // Retrieve the tracked player object from the weak pointer
    ADBDPlayer* TrackedPlayer = this->_trackedPlayer.Get();

    // Call the function at VTable offset 0x258 to retrieve a vector (likely Velocity).
    // The disassembly shows the return value is a struct with 3 floats (X, Y, Z).
    FVector PlayerVelocity = TrackedPlayer->GetVelocity();

    // Calculate the squared components of the vector
    float XSquared = PlayerVelocity.X * PlayerVelocity.X;
    float YSquared = PlayerVelocity.Y * PlayerVelocity.Y;
    float ZSquared = PlayerVelocity.Z * PlayerVelocity.Z;

    // Calculate the magnitude (length) of the vector using square root
    float VectorMagnitude = FMath::Sqrt(XSquared + YSquared + ZSquared);

    // Calculate the multiplier (1.0 / SpeedThreshold)
    float ThresholdMultiplier = 1.0f / this->SpeedThreshold;

    // Multiply the magnitude by the threshold multiplier to get the final stillness value
    float SpeedStillness = VectorMagnitude * ThresholdMultiplier;

    return SpeedStillness;
}

// starts at line 99
float UStillnessTrackerComponent::GetDistanceStillness() const
{
    // Check if the distance threshold is valid (greater than 0.0f) to avoid division by zero
    if (this->DistanceThreshold <= 0.0f)
    {
        return 0.0f;
    }

    // Check if the distance decay is valid (greater than 0.0f)
    if (this->DistanceDecay <= 0.0f)
    {
        return 0.0f;
    }

    // Calculate the squared components of the accumulated distance vector
    float XSquared = this->_accumulatedDistance.X * this->_accumulatedDistance.X;
    float YSquared = this->_accumulatedDistance.Y * this->_accumulatedDistance.Y;
    float ZSquared = this->_accumulatedDistance.Z * this->_accumulatedDistance.Z;

    // Calculate the magnitude (length) of the accumulated distance vector using square root
    float VectorMagnitude = FMath::Sqrt(XSquared + YSquared + ZSquared);

    // Calculate the multiplier (1.0 / DistanceThreshold)
    float ThresholdMultiplier = 1.0f / this->DistanceThreshold;

    // Multiply the magnitude by the threshold multiplier to get the final distance stillness value
    float DistanceStillness = VectorMagnitude * ThresholdMultiplier;

    return DistanceStillness;
}

// starts at line 104
float UStillnessTrackerComponent::GetStillnessTimer() const
{
    // Return the current value of the internal stillness timer
    return this->_stillnessTimer;
}

// starts at line 109
void UStillnessTrackerComponent::UpdateStillness(float DeltaTime)
{
    // Initialize stillness to 0 at the start of the update cycle
    this->_stillness = 0.0f;

    // Check if the required threshold parameters are valid (greater than 0.0f)
    if (this->DistanceThreshold > 0.0f && this->SpeedThreshold > 0.0f && this->DistanceDecay > 0.0f)
    {
        // Get the tracked player from the weak pointer
        ADBDPlayer* TrackedPlayer = this->_trackedPlayer.Get();
        
        // Ensure the tracked player pointer is valid
        if (TrackedPlayer == nullptr)
        {
            return;
        }

        // The disassembly calls a virtual function twice (at offset 0x258) to get a FVector.
        // It's likely this is getting Velocity for both speed and distance calculation.
        // The first call computes the magnitude squared of the vector.
        FVector VectorA = TrackedPlayer->GetVelocity();
        float VectorASquaredLength = (VectorA.X * VectorA.X) + (VectorA.Y * VectorA.Y) + (VectorA.Z * VectorA.Z);
        
        // Calculate the magnitude of VectorA using square root
        float VectorAMagnitude = FMath::Sqrt(VectorASquaredLength);

        // Retrieve another FVector from the tracked player (likely Velocity again)
        FVector VectorB = TrackedPlayer->GetVelocity();

        // Calculate the displacement based on VectorB and DeltaTime
        float DisplacementX = VectorB.X * DeltaTime;
        float DisplacementY = VectorB.Y * DeltaTime;
        float DisplacementZ = VectorB.Z * DeltaTime;

        // Accumulate the displacement into the component's internal tracked distance
        this->_accumulatedDistance.X = this->_accumulatedDistance.X + DisplacementX;
        this->_accumulatedDistance.Y = this->_accumulatedDistance.Y + DisplacementY;
        this->_accumulatedDistance.Z = this->_accumulatedDistance.Z + DisplacementZ;

        // If the magnitude of VectorA is 0 (actor is stopped) or DecayWhileMoving is true, apply decay to the accumulated distance
        if (VectorAMagnitude <= 0.0f || this->DecayWhileMoving != 0)
        {
            // Calculate the interpolation speed based on the decay values
            float InterpSpeed = this->DistanceThreshold / this->DistanceDecay;
            
            // Interpolate the accumulated distance back towards the ZeroVector
            this->_accumulatedDistance = FMath::VInterpConstantTo(
                this->_accumulatedDistance, 
                FVector::ZeroVector, 
                DeltaTime, 
                InterpSpeed
            );
        }

        // Clamp the accumulated distance to the maximum defined threshold
        // The disassembly checks against a very small number (0.000099999997) which is KINDA_SMALL_NUMBER in Unreal Math.
        if (this->DistanceThreshold >= KINDA_SMALL_NUMBER)
        {
            float AccumulatedSquaredLength = (this->_accumulatedDistance.X * this->_accumulatedDistance.X) + 
                                              (this->_accumulatedDistance.Y * this->_accumulatedDistance.Y) + 
                                              (this->_accumulatedDistance.Z * this->_accumulatedDistance.Z);

            // If the accumulated distance exceeds the threshold squared, clamp it
            float ThresholdSquared = this->DistanceThreshold * this->DistanceThreshold;
            if (AccumulatedSquaredLength > ThresholdSquared)
            {
                // The disassembly implements an approximation of an inverse square root (rsqrtss) to normalize the vector.
                // We will use Unreal Engine's standard vector normalization and scaling methods.
                
                // Get the unit direction vector of the accumulated distance
                FVector NormalizedDistance = this->_accumulatedDistance.GetSafeNormal();
                
                // Scale the direction vector by the maximum threshold distance
                this->_accumulatedDistance = NormalizedDistance * this->DistanceThreshold;
            }
        }
        else
        {
            // If the threshold is effectively zero, reset the accumulated distance
            this->_accumulatedDistance = FVector::ZeroVector;
        }

        // Calculate the two stillness types: distance-based and speed-based
        float DistanceStillness = this->GetDistanceStillness();
        float SpeedStillness = this->GetSpeedStillness();

        // Take the maximum of the two stillness values
        float MaxStillness = FMath::Max(SpeedStillness, DistanceStillness);

        // Clamp the final stillness value between 0.0f and 1.0f
        if (MaxStillness >= 0.0f)
        {
            this->_stillness = FMath::Min(MaxStillness, 1.0f);
        }
        else
        {
            this->_stillness = 0.0f;
        }
    }
    else
    {
        // If the configuration values are invalid, output a warning log and default stillness to 1.0 (fully moving)
        // Log category LogDBDGeneral verbosity level 3 (Warning)
        UE_LOG(LogDBDGeneral, Warning, TEXT("Stillness Component has invalid threshold values."));
        
        this->_stillness = 1.0f;
    }
}

// starts at line 155
bool UStillnessTrackerComponent::ShouldIncreaseCounter()
{
    // Retrieve the current stillness value from the component
    float CurrentStillness = this->GetStillness();

    // Check if the current stillness is below the predefined threshold
    if (CurrentStillness < this->StillnessThreshhold)
    {
        // The stillness is below the threshold, meaning the counter should increase
        return true;
    }

    // The stillness is equal to or greater than the threshold, meaning the counter should not increase
    return false;
}

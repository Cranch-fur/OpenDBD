// starts at line 5
ADBDPlayerCameraManager::ADBDPlayerCameraManager(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
    // The disassembly shows an OR operation with 0x1000 (1 << 12) at offset 0x1488.
    // Looking at the bitfield structure, the 13th bit corresponds to bAlwaysApplyModifiers.
    this->bAlwaysApplyModifiers = true;

    // Set up the default view pitch limits based on the disassembly values (0xC2AE0000 and 0x42AE0000)
    this->ViewPitchMin = -87.0f;
    this->ViewPitchMax = 87.0f;

    // Initialize custom field of view properties for normal and targeting states (0x42B40000 and 0x42700000)
    this->NormalFOV = 90.0f;
    this->TargetingFOV = 60.0f;
}

// starts at line 14
void ADBDPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
    // Call the parent class implementation using the standard Unreal Engine macro
    Super::UpdateViewTargetInternal(OutVT, DeltaTime);

    // Check if the current camera style is set to the first person observer mode
    if (this->CameraStyle == DBDObservingMode::NAME_FirstPersonObserver)
    {
        // Temporary variable to store the view location, as we only need to update the rotation in this context
        FVector locationTemp;

        // Fetch the correct rotation from the target actor's viewpoint
        // VTable offset 0x0578 corresponds to AActor::GetActorEyesViewPoint
        OutVT.Target->GetActorEyesViewPoint(locationTemp, OutVT.POV.Rotation);
    }
}

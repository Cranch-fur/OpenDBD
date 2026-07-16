// starts at line 8
UDBDCamperData::UDBDCamperData(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initializes 2 bytes starting at offset 0x0109. This covers both ImmobilizeState (0x0109) 
    // and CurrentGuidedAction (0x010A), setting their enum values to 0.
    this->ImmobilizeState = ECamperImmobilizeState::VE_None;
    this->CurrentGuidedAction = ECamperGuidedAction::VE_None;

    // Initializes 1 byte at offset 0x010B to true.
    this->ShouldTakeDamage = true;
}

// starts at line 13
void UDBDCamperData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // Call the parent class implementation to ensure inherited properties are replicated
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Register the DamageState property for network replication
    DOREPLIFETIME(UDBDCamperData, DamageState);
}

// starts at line 21
void UDBDCamperData::DamageStateChanged()
{
    // Execute the delegate if it is bound to any function, passing 'true' as the boolean parameter
    this->DamageStateChangedEvent.ExecuteIfBound(true);
}

// starts at line 26
ECamperDamageState UDBDCamperData::GetDamageState() const
{
    return this->DamageState;
}

// starts at line 36
ECamperImmobilizeState UDBDCamperData::GetImmobilizeState() const
{
    return this->ImmobilizeState;
}

// starts at line 41
ECamperGuidedAction UDBDCamperData::GetCurrentGuidedAction() const
{
    return this->CurrentGuidedAction;
}

// starts at line ???
void UDBDCamperData::SetDamageState(ECamperDamageState e)
{
    this->DamageState = e;
}

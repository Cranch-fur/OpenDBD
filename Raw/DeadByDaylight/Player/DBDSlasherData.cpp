// starts at line 7
UDBDSlasherData::UDBDSlasherData(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initialize the slasher's current guided action to VE_None.
    this->CurrentGuidedAction = ESlasherGuidedAction::VE_None;
}

// starts at line 12
void UDBDSlasherData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // Call the parent class implementation to ensure inherited properties are replicated
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Register the VisionEnabled property for network replication
    DOREPLIFETIME(UDBDSlasherData, VisionEnabled);

    // Register the CloakingEnabled property for network replication
    DOREPLIFETIME(UDBDSlasherData, CloakingEnabled);
}

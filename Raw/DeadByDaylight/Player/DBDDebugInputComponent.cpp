// starts at line 42
UDBDDebugInputComponent::UDBDDebugInputComponent()
{
    // Clear bit 22 (0x400000) of the bitfield starting at 0x00A0.
    // Based on the UActorComponent structure, this maps to bWantsInitializeComponent.
    this->bWantsInitializeComponent = false;

    // Clear bit 1 (0x02) at offset 0x0C within the FTickFunction structure (0x003C overall).
    // In Unreal Engine 4.13, this bitflags configuration disables ticking.
    this->PrimaryComponentTick.bCanEverTick = false;

    // The disassembly performs a 64-bit zero assignment (mov [rbx+0ECh], rax) 
    // to clear both float variables simultaneously.
    this->_timeAggregator = 0.0f;
    this->_timeAggregatorAlt = 0.0f;

    // The disassembly performs a 32-bit zero assignment (mov [rbx+0F4h], eax).
    this->_currentTime = 0.0f;
}

// starts at line 64
void UDBDDebugInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(this, DeltaTime, TickType, ThisTickFunction);
}

// starts at line 95
void UDBDDebugInputComponent::InitializeComponent()
{
    // Call the parent component's initialization method
    Super::InitializeComponent();
}

// starts at line 24
UDBDPlayerData::UDBDPlayerData(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initializes 2 bytes at 0x00E8
    this->Blinded = false;
    this->_interactionInputPressed = false;

    // Initializes 2 bytes at 0x00EB
    this->_fastInteractionInputPressed = false;
    this->_interactionInputMashed = false;

    // Initializes 1 byte at 0x00ED
    this->_leftRightInputMashed = false;

    // Initializes 1 byte at 0x00EF
    this->_useInputPressed = false;

    // Initializes 4 bytes at 0x00F1
    this->_gameLevelLoaded = false;
    this->_lightSpawned = false;
    this->_loadoutSpawned = false;
    this->_introCompleted = false;

    // Sets specific bits in the bitfield starting at 0x00A0
    this->bReplicates = true;
    this->bWantsInitializeComponent = true;

    // Initializes 1 byte at 0x0104
    this->_autoLeftRightMash = false;
}

// starts at line 30
void UDBDPlayerData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // Call the parent class implementation to ensure inherited properties are replicated
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Register the _controlRotation property for network replication
    DOREPLIFETIME(UDBDPlayerData, _controlRotation);

    // Register the Blinded property for network replication
    DOREPLIFETIME(UDBDPlayerData, Blinded);
}

// starts at line 39
void UDBDPlayerData::SetLightSpawned()
{
    this->_lightSpawned = true;
    this->Server_LightSpawned();
}

// starts at line 45
void UDBDPlayerData::SetLoadoutSpawned()
{
    this->_loadoutSpawned = true;
    this->Server_LoadoutSpawned();
}

// starts at line 52
bool UDBDPlayerData::IsLightSpawned() const
{
    return this->_lightSpawned;
}

// starts at line 57
bool UDBDPlayerData::IsLoadoutSpawned() const
{
    return this->_loadoutSpawned;
}

// starts at line 67
void UDBDPlayerData::Server_SetControlRotation_Implementation(FRotator rotation)
{
    // Update the internal rotation value of the network-quantized rotation structure
    this->_controlRotation._value = rotation;
}

// starts at line 72
void UDBDPlayerData::Server_LightSpawned_Implementation()
{
    this->_lightSpawned = true;
}

// starts at line 82
void UDBDPlayerData::Server_LoadoutSpawned_Implementation()
{
    this->_loadoutSpawned = true;
}

// starts at line 92
void UDBDPlayerData::SetIntroCompleted()
{
    this->_introCompleted = true;
    this->Server_SetIntroCompleted();
}

// starts at line 98
bool UDBDPlayerData::IsIntroCompleted()
{
    // Attempt to get the cached World pointer from the ActorComponent
    UWorld* world = this->WorldPrivate;

    // If the cached world pointer is null, retrieve it using the uncached method
    if (world == nullptr)
    {
        world = this->GetWorld_Uncached();
    }

    // Check if the current world is a Play In Editor (PIE) instance.
    // In Unreal Engine, EWorldType::PIE evaluates to the integer value 3.
    if (world->WorldType == EWorldType::PIE)
    {
        // Always treat the intro as completed when running a PIE session
        return true;
    }

    // If not in a PIE session, return the actual boolean state of the intro completion
    return this->_introCompleted;
}

// starts at line 103
void UDBDPlayerData::Server_SetIntroCompleted_Implementation()
{
    this->_introCompleted = true;
}

// starts at line ???
bool UDBDPlayerData::Server_LoadoutSpawned_Validate()
{
    return true;
}

// starts at line 113
UDBDPlayerData* UDBDPlayerData::GetPlayerData(APawn* playerPawn)
{
    // Check if the provided pawn is valid
    if (playerPawn == nullptr)
    {
        return nullptr;
    }

    // Attempt to cast the pawn to ADBDPlayer
    ADBDPlayer* dbdPlayer = Cast<ADBDPlayer>(playerPawn);

    // If the pawn is successfully casted to ADBDPlayer
    if (dbdPlayer != nullptr)
    {
        // Standard Unreal Engine macro to check against nullptr and RF_PendingKill
        if (IsValid(dbdPlayer) == true && dbdPlayer->IsActorBeingDestroyed() != false && dbdPlayer->IsValidImpl())
        {
            // Return the UDBDPlayerData pointer located at offset 0xD08 within ADBDPlayer
            return dbdPlayer->_playerData;
        }
    }

    // If the pawn is not an ADBDPlayer, attempt to cast it to ADBDObserverPlayer
    ADBDObserverPlayer* observerPlayer = Cast<ADBDObserverPlayer>(playerPawn);

    // If the pawn is successfully casted to ADBDObserverPlayer
    if (observerPlayer != nullptr && IsValid(observerPlayer) == true && observerPlayer->IsActorBeingDestroyed() == false)
    {
        // Return the UDBDPlayerData pointer located at offset 0x7A0 within ADBDObserverPlayer
        return observerPlayer->_playerData;
    }

    // Return nullptr if no valid player data could be retrieved
    return nullptr;
}

// starts at line 19
UWorld* DBDBaseState::GetWorld() const
{
    // Check if the weak pointer to the state machine is valid
    if (this->_stateMachine.IsValid() == false)
    {
        return nullptr;
    }

    AActor* stateMachineOwner = this->_stateMachine->GetOwner();

    // Check if the Owner object is valid
    if (IsValid(stateMachineOwner) == true && stateMachineOwner->IsActorBeingDestroyed() == false)
    {
        return stateMachineOwner->GetWorld();
    }

    return nullptr;
}

// starts at line 37
UDBDGameInstance* DBDBaseState::GetGameInstance() const
{
    // Retrieve the UWorld instance associated with this state
    UWorld* world = this->GetWorld();

    // Verify the world is valid and not marked as unreachable/pending kill
    if (IsValid(world) == false)
    {
        return nullptr;
    }

    // The disassembly manually checks the ClassTreeIndex and ClassTreeNumChildren 
    // to determine if 'gameInstance' is of type 'UDBDGameInstance' (or a child thereof)
    UDBDGameInstance* dbdGameInstance = Cast<UDBDGameInstance>(world->GetGameInstance());
    if (dbdGameInstance != nullptr)
    {
        return dbdGameInstance;
    }

    // Return nullptr if no valid UDBDGameInstance could be obtained
    return nullptr;
}

// starts at line 48
ADBDGameState* DBDBaseState::GetGameState() const
{
    // Retrieve the UWorld instance associated with this state
    UWorld* world = this->GetWorld();

    // Verify the world is valid and not marked as unreachable/pending kill
    if (IsValid(world) == false)
    {
        return nullptr;
    }

    // The disassembly manually checks the ClassTreeIndex and ClassTreeNumChildren 
    // to determine if 'gameState' is of type 'ADBDGameState' (or a child thereof)
    ADBDGameState* dbdGameState = Cast<ADBDGameState>(world->GetGameState());
    if (dbdGameState != nullptr)
    {
        return dbdGameState;
    }

    // Return nullptr if no valid ADBDGameState could be obtained
    return nullptr;
}

// starts at line 59
bool DBDBaseState::HasAuthority() const
{
    // Check if the weak pointer to the state machine is valid
    if (this->_stateMachine.IsValid() == false)
    {
        return nullptr;
    }

    AActor* stateMachineOwner = this->_stateMachine->GetOwner();

    // Check if the Owner object is valid
    if (IsValid(stateMachineOwner) == true && stateMachineOwner->IsActorBeingDestroyed() == false)
    {
        if (stateMachineOwner->Role == ROLE_Authority)
        {
            return true;
        }
    }

    return false;
}

// starts at line 72
float DBDBaseState::GetTunableValue(FName ID, float defaultValue) const
{
    // Retrieve the GameInstance
    UDBDGameInstance* gameInstance = this->GetGameInstance();

    // Check if the GameInstance is valid
    // IsValid macro inherently handles checking if the pointer is null and if the underlying UObject is valid
    if (IsValid(gameInstance) == false)
    {
        return defaultValue;
    }

    // Access the 'DesignTunables' object from the GameInstance
    UDBDDesignTunables* tunables = gameInstance->DesignTunables;

    // Call GetTunableValue on the tunables object.
    // The disassembly shows a boolean parameter 'warnIfRowMissing' set to false (xor r9d, r9d).
    return tunables->GetTunableValue(ID, defaultValue, false);
}

// starts at line 83
ASlasherPlayer* DBDBaseState::GetKiller() const
{
    // Retrieve the GameState associated with this state
    ADBDGameState* gameState = this->GetGameState();

    // Check if the GameState is valid and not marked for destruction
    if (IsValid(gameState) == false)
    {
        return nullptr;
    }

    // Return the Slasher (Killer) reference stored in the GameState
    // The disassembly accesses the pointer at offset 0x5F0.
    return gameState->Slasher;
}

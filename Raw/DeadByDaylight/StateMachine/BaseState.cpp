// starts at line 11
BaseState::BaseState(const FString& name, UStateMachine* stateMachine)
{
    // Initialize the weak pointer to the state machine
    this->_stateMachine = stateMachine;

    // Initialize default values for pointers and internal structures
    this->_parentState = nullptr;

    // Verify that the provided state machine is valid
    // Checks if the TWeakObjectPtr is empty
    if (this->_stateMachine == nullptr)
    {
        UE_LOG(LogSM, Error, TEXT("Cannot pass a null state machine to a state constructor"));
    }

    // Verify that the provided name is not empty
    // The disassembly checks if the ArrayNum of the FString is <= 1 (empty string or uninitialized)
    if (name.Len() == 0)
    {
        UE_LOG(LogSM, Error, TEXT("Cannot construct state with empty base name"));
    }

    // Copy the provided name into the class field
    this->_name = name;

    // Assign and increment the unique state ID
    this->_id = _stateMachine->_nextStateID;
    _stateMachine->_nextStateID++;
}

// starts at line 27
bool BaseState::AddState(TSharedPtr<BaseState> state)
{
    // Check if the provided state is valid
    if (state.IsValid() == false)
    {
        UE_LOG(LogSM, Error, TEXT("Cannot pass a nullptr state"));
        return false;
    }

    // Check if the state's name is empty
    if (state->_name.Len() <= 1)
    {
        UE_LOG(LogSM, Error, TEXT("Cannot add a state with an empty name"));
        return false;
    }

    // Check if a state with the same name already exists in the sub-states
    if (this->SubStateAlreadyExists(state) == true)
    {
        UE_LOG(LogSM, Error, TEXT("State with name %s already exists. Will not add"), *state->_name);
        return false;
    }

    // Set the parent state of the incoming state to this state
    state->_parentState = this;

    // Add the state to the array of sub-states
    this->_subStates.Add(state);

    return true;
}

// starts at line 53
bool BaseState::AddAndSetAsDefaultState(TSharedPtr<BaseState> state)
{
    // Attempt to add the state to the sub-states collection
    bool bIsAdded = this->AddState(state);

    // Check if the state was successfully added
    if (bIsAdded == false)
    {
        // The state was not added, return false
        return false;
    }

    // Set the successfully added state as the default sub-state
    this->_defaultSubState = state;

    return true;
}

// starts at line 60
TSharedPtr<BaseState> BaseState::GetDefaultSubState()
{
    // Return a copy of the default sub-state
    return this->_defaultSubState;
}

// starts at line 65
bool BaseState::SubStateAlreadyExists(TSharedPtr<BaseState> state) const
{
    // Iterate through all existing sub-states within the state collection
    for (int32 i = 0; i < this->_subStates.Num(); i = i + 1)
    {
        // Retrieve the current sub-state for comparison
        TSharedPtr<BaseState> currentSubState = this->_subStates[i];

        // Ensure the current sub-state pointer is valid before accessing its data
        if (currentSubState.IsValid() == false)
        {
            continue;
        }

        // Compare the name of the existing sub-state with the incoming state's name
        if (currentSubState->_name.Equals(state->_name, ESearchCase::IgnoreCase) == true)
        {
            // A sub-state with a matching name was found
            return true;
        }
    }

    // No matching sub-state was found after checking the entire array
    return false;
}

// starts at line 76
TSharedPtr<BaseState> BaseState::GetState(const FString& stateName) const
{
    // Iterate through all currently attached sub-states
    for (int32 i = 0; i < this->_subStates.Num(); i++)
    {
        // Get the sub-state from the array
        TSharedPtr<BaseState> subState = this->_subStates[i];

        // Ensure the pointer is valid before comparing its properties
        if (subState.IsValid() == false)
        {
            continue;
        }

        // Compare the name of the current sub-state to the requested stateName.
        if (subState->_name.Equals(stateName, ESearchCase::IgnoreCase) == true)
        {
            // A matching state was found, return it
            return subState;
        }
    }

    // No matching state was found in the sub-states array
    return nullptr;
}

// starts at line 97
void BaseState::EnterState(UObject* owner)
{
    // Reset the counter for the number of ticks this state has been active
    this->_numTickInState = 0;

    // Format the log message string
    FString logMessage = FString::Printf(TEXT("<Entering state %s>"), *this->_name);

    // Pass the formatted message to the state machine's specific logging function
    this->_stateMachine->LogSMLog(*finalLogString);

    // Call the virtual OnEnter method for any specific class implementations
    this->OnEnter(owner);
}

// starts at line 108
void BaseState::TickState(UObject* owner, float deltaTime)
{
    // FMsg::Logf_Internal__VA with verbosity 7 (ELogVerbosity::VeryVerbose) represents the UE_LOG macro
    UE_LOG(LogSM, VeryVerbose, TEXT("%s %s Ticking state %s, num tick %d"), 
           this->_stateMachine->GetMachineType(), *owner->GetFName().ToString(), *this->_name, this->_numTickInState);

    // Call the virtual OnTick method, passing the delta time (notably missing in the IDA pseudo, but present in the function signature and _V)
    this->OnTick(owner, deltaTime);

    // Increment the number of ticks this state has been active
    this->_numTickInState++;
}

// starts at line 119
void BaseState::ExitState(UObject* owner)
{
    // Format the exit log message
    FString logMessage = FString::Printf(TEXT("<Exiting state %s>"), *this->_name);

    // Output the formatted string to the state machine's log
    this->_stateMachine->LogSMLog(*logMessage);

    // Trigger the virtual OnExit logic, allowing derived classes to clean up
    this->OnExit(owner);
}

// starts at line 126
IStateCollection* BaseState::GetParent() const
{
    // Check if there is an explicit parent state assigned to this state
    if (this->_parentState != nullptr)
    {
        // BaseState inherits from IStateCollection (at offset 0), so it implicitly converts
        return this->_parentState;
    }

    // If no parent state exists, attempt to get the owning state machine
    UStateMachine* stateMachine = this->_stateMachine.Get();

    // Check if the state machine pointer is valid
    if (stateMachine != nullptr)
    {
        // UStateMachine implements IStateCollection, but its vtable is located at offset 0xE8.
        return static_cast<IStateCollection*>(stateMachine);
    }

    // Return nullptr if both the parent state and state machine are invalid or unavailable
    return nullptr;
}

// starts at line 647
void BaseState::OnEnter(UObject* owner)
{ }

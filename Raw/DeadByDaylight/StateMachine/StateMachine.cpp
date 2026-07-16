namespace
{
    // Helper function to extract an array of IDs from an array of State pointers
    void GetStackIDs(TArray<uint16>& OutStackIDs, const TArray<TSharedPtr<BaseState>>& StateStack)
    {
        // Clear the output array just in case
        OutStackIDs.Empty();

        // Iterate through all state pointers in the stack
        // The disassembly shows iterating via pointer bounds (`Data != v4`)
        for (const TSharedPtr<BaseState>& StatePtr : StateStack)
        {
            // Dereference the shared pointer to get the underlying object
            // The disassembly accesses the `_id` member at offset 0x48 (72).
            if (StatePtr.Get() != nullptr)
            {
                // Add the ID to the output array
                OutStackIDs.Add(StatePtr->_id);
            }
        }
    }
}

// starts at line 30
UStateMachine::UStateMachine(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initialize primitive state variables
    this->_isDrivingStateMachine = false;

    // Setup Component Tick property
    this->PrimaryComponentTick.bCanEverTick = true;

    // Setup UActorComponent Bitfield flags (Offset 0xA0)
    this->bReplicates = true;
    this->bWantsInitializeComponent = true;

    // Unpack optimized 32-bit assignment: mov dword ptr [rbx+148h], 10000h
    this->_initJoinInProgress = false;
    this->_isRunning = false;
    this->_nextStateID = 1;

    this->DebugPrintCurrentStateContinuous = false;
}

// starts at line 47
void UStateMachine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // Call the parent class implementation to register inherited replicated properties
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // The disassembled code represents the expanded standard Unreal Engine macro DOREPLIFETIME.
    DOREPLIFETIME(UStateMachine, _netStateIDStackInitial);
}

// starts at line 55
bool UStateMachine::AddState(TSharedPtr<BaseState> state)
{
    // The disassembly shows "state" being passed by value (which implies a copy constructor call that increments the ref count)
    // and then manually destroyed at the end of the function. In typical high-level UE C++, this is handled by passing 
    // by value and letting the destructor run naturally, or by passing by const reference if taking ownership isn't needed.
    // Based on the pseudo-code and assembly flow which manually manages the lifecycle of the passed-in pointer, 
    // it's modeling a pass-by-value scenario where the function consumes the pointer.

    if (state == nullptr)
    {
        // Equivalent to the `FMsg::Logf_Internal__VA` blocks with LogSM check
        UE_LOG(LogSM, Warning, TEXT("%s %s"), *this->GetLogSMPrefix(), TEXT("Cannot pass a nullptr state"));
        return false;
    }

    // Checking if the state's name is empty. The disassembly creates a copy of the string to check its length,
    // which maps directly to the FString::IsEmpty() method.
    if (state->_name.IsEmpty())
    {
        UE_LOG(LogSM, Warning, TEXT("%s %s"), *this->GetLogSMPrefix(), TEXT("Cannot add a state with an empty name"));
        return false;
    }

    // Check if a state with the same name already exists in the machine
    if (this->StateAlreadyExists(state))
    {
        // Formatting error message
        FString ErrorMessage = FString::Printf(TEXT("State with name %s already exists. Will not add"), *state->_name);
        this->LogSMError(*ErrorMessage);
        return false;
    }

    // Add the state to the internal array
    // The disassembly shows manual array bounds checking, resizing, and pointer assignment.
    // This is the standard behavior of TArray::Add()
    this->_states.Add(state);

    return true;
}

// starts at line 79
bool UStateMachine::AddAndSetAsDefaultState(TSharedPtr<BaseState> state)
{
    // Attempt to add the state to the state machine
    bool bStateAdded = this->AddState(state);

    // Check if the state was successfully added
    if (bStateAdded == true)
    {
        // Set the newly added state as the default state.
        // The verbose reference counting and object destruction logic seen in the 
        // disassembly and pseudo-code is handled natively by the TSharedPtr assignment operator.
        this->_defaultState = state;

        return true;
    }

    // Return false if the state failed to be added
    return false;
}

// starts at line 86
bool UStateMachine::StateAlreadyExists(TSharedPtr<BaseState> state) const
{
    // Check if the input state is invalid
    if (state == nullptr)
    {
        return false;
    }

    // Iterate through the array of existing states
    for (const TSharedPtr<BaseState>& ExistingState : this->_states)
    {
        // Skip null entries in the array just in case
        if (ExistingState == nullptr)
        {
            continue;
        }

        // Compare the names of the states case-insensitively.
        // The disassembly shows a manual implementation of a case-insensitive string comparison 
        // using towlower in a loop, which maps to FString::Equals(..., ESearchCase::IgnoreCase).
        if (state->_name.Equals(ExistingState->_name, ESearchCase::IgnoreCase))
        {
            // If a state with a matching name is found, return true
            return true;
        }
    }

    // No matching state was found
    return false;
}

// starts at line 97
TSharedPtr<BaseState> UStateMachine::GetState(const FString& stateName) const
{
    // Iterate through the array of existing states
    // The disassembly shows bounds checking against `this->_states` (offset 0xF0) and comparing names.
    for (const TSharedPtr<BaseState>& ExistingState : this->_states)
    {
        // Skip null entries in the array just in case
        if (ExistingState == nullptr)
        {
            continue;
        }

        // Compare the names of the states case-insensitively.
        // As seen in StateAlreadyExists, the towlower loop maps to FString::Equals(..., ESearchCase::IgnoreCase).
        if (stateName.Equals(ExistingState->_name, ESearchCase::IgnoreCase))
        {
            // If a state with a matching name is found, return a copy of the TSharedPtr
            return ExistingState;
        }
    }

    // No matching state was found, return a null TSharedPtr
    return nullptr;
}

// starts at line 107
TSharedPtr<BaseState> UStateMachine::GetState(uint16 stateID) const
{
    // Iterate through the array of existing states
    // The disassembly shows bounds checking against `this->_states` (offset 0xF0) and comparing _id.
    for (const TSharedPtr<BaseState>& ExistingState : this->_states)
    {
        // Skip null entries in the array just in case
        if (ExistingState == nullptr)
        {
            continue;
        }

        // Compare the _id property of the states
        // Offset 0x48 in BaseState corresponds to `_id` based on pseudo-code `v8->_id == stateID`
        if (ExistingState->_id == stateID)
        {
            // If a state with a matching ID is found, return a copy of the TSharedPtr
            return ExistingState;
        }
    }

    // No matching state was found, return a null TSharedPtr
    return nullptr;
}

// starts at line 118
void UStateMachine::Start()
{
    // Check if the state machine is already running
    if (this->_isRunning == true)
    {
        // Log at Verbosity 3 (Log)
        UE_LOG(LogSM, Log, TEXT("state machine already running. Ignoring"));
        return;
    }

    // Check if a default state has been assigned
    // Using .Get() to strictly compare against nullptr
    if (this->_defaultState.Get() != nullptr)
    {
        // If this instance is responsible for driving the state machine logic
        if (this->_isDrivingStateMachine == true)
        {
            // Clear out any existing elements in the state stack
            this->_currentStateStack.Empty();

            // Push the default state to initialize the stack
            // TSharedPtr parameter passing handles the reference counting automatically
            this->PushNewStateStack(this->_defaultState);
        }

        // Mark the state machine as currently running
        this->_isRunning = true;
    }
    else
    {
        // Log at Verbosity 2 (Warning) if no default state is present
        UE_LOG(LogSM, Warning, TEXT("No default state. Cannot run state machine"));
    }
}

// starts at line 141
void UStateMachine::Stop()
{
    // Clear out the current state stack, immediately stopping any active states
    this->_currentStateStack.Empty();

    // Update the flag to indicate the state machine is no longer active
    this->_isRunning = false;
}

// starts at line 147
void UStateMachine::TickComponent(float deltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    // Call the parent class implementation of TickComponent
    Super::TickComponent(deltaTime, TickType, ThisTickFunction);

    // Ensure the owning actor is valid and not pending destruction
    // The disassembly shows checks against GUObjectArray and specific flags (e.g., 0x20000000 for RF_Unreachable/PendingKill)
    // In higher-level UE C++, this is typically handled by checking IsValid(GetOwner()) or IsPendingKill().
    if (IsValid(this->GetOwner()))
    {
        // Debug printing based on flags
        if (this->_isDrivingStateMachine == true && this->DebugPrintCurrentStateContinuous == true)
        {
            this->DebugPrintCurrentState(deltaTime);
        }

        // Only process state logic if the state machine is active
        if (this->_isRunning == true)
        {
            // Check if conditions require a state transition
            if (this->StateStackShouldChange())
            {
                this->TransitionStates();
            }

            // If there are states in the stack, tick them
            if (this->_currentStateStack.Num() > 0)
            {
                // Create a temporary copy of the state stack to iterate over.
                // The disassembly shows array resizing and manual pointer copying, which handles the TSharedPtr reference counts.
                // This is equivalent to invoking the copy constructor of TArray.
                TArray<TSharedPtr<BaseState>> StateStackCopy = this->_currentStateStack;

                // Call the virtual/internal tick handler, passing the copy of the stack
                this->OnTickStateStack(StateStackCopy, deltaTime);
            }
        }
    }
}

// starts at line 179
bool UStateMachine::StateStackShouldChange() const
{
    // If this state machine instance dictates the logic (e.g., Server)
    if (this->_isDrivingStateMachine == true)
    {
        return this->FoundTransitionInStateStack();
    }

    // For clients/replicas driven by network updates
    // Check if the _netQueue is empty. A TQueue is empty if the Tail's NextNode is null.
    // The disassembly checks `[this+128h]` which is the Tail pointer, and then `[rax]` which is NextNode.
    // Replace the manual node checking with standard TQueue method.
    bool bIsNetQueueEmpty = this->_netQueue.IsEmpty();

    // Check if the state stack is empty
    bool bIsStateStackEmpty = (this->_currentStateStack.Num() == 0);

    // Case 1: Neither initialized nor received an update
    if (bIsNetQueueEmpty && bIsStateStackEmpty)
    {
        // Log at Verbosity 6 (VeryVerbose)
        UE_LOG(LogSM, VeryVerbose, TEXT("%s %s"), *this->GetLogSMPrefix(), TEXT("ReplicaHSMNotInitialized"));
        return false;
    }

    // Case 2: Just received an update, but stack is currently empty (initialization)
    if (bIsNetQueueEmpty == false && bIsStateStackEmpty == true)
    {
        // Log at Verbosity 6 (VeryVerbose)
        UE_LOG(LogSM, VeryVerbose, TEXT("%s %s"), *this->GetLogSMPrefix(), TEXT("ReplicaHSMJustInitialized"));
        return true;
    }

    // Case 3: Both have data, meaning an update arrived while states were already active
    if (bIsNetQueueEmpty == false && bIsStateStackEmpty == false)
    {
        this->LogSMVerbose(TEXT("states are changing. Current situation:"));
        this->LogStateStack();
        return true;
    }

    // Default fallback
    return false;
}

// starts at line 207
TArray<TSharedPtr<BaseState>> UStateMachine::StackStateAndAllDefaultSubStates(TSharedPtr<BaseState> parentState) const
{
    // The disassembly shows the creation of a temporary array (TArray) that is returned by value.
    TArray<TSharedPtr<BaseState>> result;

    // Initialize the result array with the starting parent state
    result.Add(parentState);

    // Create a local pointer to iterate through the state hierarchy
    // This mirrors the `v7` and `Object` manual reference count management in the pseudo-code
    TSharedPtr<BaseState> CurrentState = parentState;

    // Loop continuously to find all nested default sub-states
    while (true)
    {
        // Call the method to get the default sub-state of the current state.
        // The disassembly shows `call qword ptr [rax+20h]`, which is a virtual function call on BaseState.
        TSharedPtr<BaseState> subState = CurrentState->GetDefaultSubState();

        // If the state has no default sub-state, the hierarchy ends, break the loop
        if (subState.Get() == nullptr)
        {
            break;
        }

        // Add the found sub-state to the stack result
        result.Add(subState);

        // Update the current state to traverse deeper into the hierarchy
        CurrentState = subState;
    }

    return result;
}

// starts at line 223
void UStateMachine::TransitionStates()
{
    // Check if this instance is responsible for driving the state machine (e.g., Server/Authority)
    if (this->_isDrivingStateMachine == true)
    {
        this->AuthorityTransitionStates();
    }
    else
    {
        // Local array to hold the dequeued network stack IDs
        TArray<uint16> newStackIDs;

        // The disassembly and pseudocode show a manual implementation of a lock-free queue dequeue operation 
        // (traversing Tail->NextNode, copying the array, updating the Tail pointer, and deleting the old node).
        // This is the expanded form of Unreal Engine's standard TQueue::Dequeue method.
        while (this->_netQueue.IsEmpty() == false)
        {
            // Pop the next pending state update from the queue
            this->_netQueue.Dequeue(newStackIDs);

            // Apply the dequeued state transition to this replica
            this->ReplicaTransitionStates(newStackIDs);
        }
    }
}

// starts at line 240
void UStateMachine::OnTickStateStack(TArray<TSharedPtr<BaseState>> stateStack, float deltaTime) const
{
    // The stateStack array is passed by value (copied from the caller) to avoid issues if the stack changes during ticking.
    // The disassembly shows iteration over this local array.
    for (const TSharedPtr<BaseState>& State : stateStack)
    {
        // Ensure the state pointer is valid before attempting to tick it
        if (State.Get() != nullptr)
        {
            // Call the TickState function on the state object, passing the component's owner and delta time.
            // Offset 0xA8 maps to OwnerPrivate.
            State->TickState(this->GetOwner(), deltaTime);
        }
    }
}

// starts at line 251
void UStateMachine::OnEnterStateStack(TArray<TSharedPtr<BaseState>> stateStack) const
{
    // The stateStack array is passed by value (copied from the caller).
    // The disassembly shows iteration over this local array.
    for (const TSharedPtr<BaseState>& State : stateStack)
    {
        // Ensure the state pointer is valid before attempting to enter it
        if (State.Get() != nullptr)
        {
            // Call the EnterState function on the state object, passing the component's owner.
            // Offset 0xA8 maps to OwnerPrivate.
            State->EnterState(this->GetOwner());
        }
    }
}

// starts at line 274
bool UStateMachine::FoundTransitionInStateStack() const
{
    // Iterate through the current state stack
    // Based on the disassembly starting at offset 0x3F4A3E, checking bounds of this->_currentStateStack
    for (const TSharedPtr<BaseState>& CurrentState : this->_currentStateStack)
    {
        // Skip invalid states just in case
        if (CurrentState.Get() == nullptr)
        {
            continue;
        }

        // Call a method on the state to check for an active transition
        // The disassembly shows `call qword ptr [rax+28h]`, which is a virtual function call on BaseState.
        // It passes `this->OwnerPrivate` as a parameter and expects a TSharedPtr<BaseState> out parameter.
        TSharedPtr<BaseState> DestState = CurrentState->FindTransitionState(this->GetOwner());

        // If a valid destination state is found, a transition should occur
        if (DestState.Get() != nullptr)
        {
            // Log the transition at Verbosity 6 (VeryVerbose)
            // The logging code extracts the state names and calls FMsg::Logf_Internal__VA
            UE_LOG(LogSM, VeryVerbose, TEXT("%s, Found transition from state %s to state %s"), 
                   *this->GetMachineType(), *CurrentState->_name, *DestState->_name);

            // DestState is destroyed when it goes out of scope here
            return true;
        }
    }

    // No valid transition was found in any of the currently active states
    return false;
}

// starts at line 304
void UStateMachine::UpdateReplicatedValues()
{
    // Extract an array of uint16 IDs from the current state stack
    TArray<uint16> stackIDs;
    GetStackIDs(stackIDs, this->_currentStateStack);

    // If this instance is running on the Server (Role == ROLE_Authority, which is 3)
    // The disassembly shows `cmp byte ptr [rax+110h], 3`, corresponding to `OwnerPrivate->Role.Value == 3`
    if (this->GetOwner()->Role == ROLE_Authority)
    {
        // Log the multicast operation at Verbosity 6 (VeryVerbose)
        FString stateStackStr = this->GetStateStackIDsString(stackIDs);
        FString logMessage = FString::Printf(TEXT("Multicasting a state change to (%s)"), *stateStackStr);
        UE_LOG(LogSM, VeryVerbose, TEXT("%s %s"), *this->GetLogSMPrefix(), *logMessage);

        // Execute the multicast RPC to update all clients
        this->Multicast_SetStateStack(stackIDs);
    }
    else
    {
        // If this instance is a client, log the server request at Verbosity 6 (VeryVerbose)
        FString stateStackStr = this->GetStateStackIDsString(stackIDs);
        FString logMessage = FString::Printf(TEXT("Sending to server a state change to (%s)"), *stateStackStr);
        UE_LOG(LogSM, VeryVerbose, TEXT("%s %s"), *this->GetLogSMPrefix(), *logMessage);

        // Execute the server RPC to request a state change
        this->Server_SetStateStack(stackIDs);
    }

    // Update the replicated property with the new stack IDs.
    // The disassembly shows bounds checking, resizing, and memcpy, which is standard TArray assignment.
    this->_netStateIDStackInitial = stackIDs;
}

// starts at line 326
void UStateMachine::ReplicaSetStateStack(const TArray<uint16>& netStateIDStack)
{
    // The disassembly shows logic to build a string representing the names of the states in the stack
    // for logging purposes. It iterates through the IDs, finds the state objects, and concatenates their names.
    // The pseudo-code explicitly shows array manipulation to build the string, but this logic is skipped 
    // if the logging is disabled or below a certain threshold.

    if (netStateIDStack.Num() > 0)
    {
        // Build the state names string for the log message
        FString statesString;
        TSharedPtr<BaseState> currentState = this->GetState(netStateIDStack[0]);
        
        if (currentState.Get() != nullptr)
        {
            statesString = currentState->_name;

            // Loop through the rest of the IDs to build the full path
            for (int32 i = 1; i < netStateIDStack.Num(); ++i)
            {
                uint16 stateID = netStateIDStack[i];
                TSharedPtr<BaseState> parentState = currentState;
                
                // Find the matching substate
                TSharedPtr<BaseState> foundSubState = nullptr;
                for (const TSharedPtr<BaseState>& subState : parentState->_subStates)
                {
                    if (subState.Get() != nullptr && subState->_id == stateID)
                    {
                        foundSubState = subState;
                        break;
                    }
                }
                
                currentState = foundSubState;

                // Append to the string using the `->` separator
                if (currentState.Get() != nullptr)
                {
                    statesString += TEXT("->");
                    statesString += currentState->_name;
                }
                else
                {
                    break;
                }
            }
        }

        // Generate the full stack ID string
        FString stateStackIDsStr = this->GetStateStackIDsString(netStateIDStack);

        // Log the received RPC
        FString logMessage = FString::Printf(TEXT("Got RPC call. State stack change. %s (%s)"), *statesString, *stateStackIDsStr);
        UE_LOG(LogSM, VeryVerbose, TEXT("%s %s"), *this->GetLogSMPrefix(), *logMessage);
    }

    // Add the received state stack to the network queue for processing on the next tick
    // The disassembly shows manual node allocation (operator new 0x18), array copying, and updating 
    // the linked list head/tail pointers. This is the manual expansion of TQueue::Enqueue.
    this->_netQueue.Enqueue(netStateIDStack);
}

// starts at line 347
void UStateMachine::Multicast_SetStateStack_Implementation(const TArray<uint16>& netStateIDStack)
{
    // Check if this instance is not responsible for driving the state machine logic
    // This ensures that the multicast only affects clients/replicas, not the authoritative server
    if (this->_isDrivingStateMachine == false)
    {
        // Process the newly received state stack from the server
        this->ReplicaSetStateStack(netStateIDStack);
    }
}

// starts at line 361
void UStateMachine::Server_SetStateStack_Implementation(const TArray<uint16>& netStateIDStack)
{
    this->Multicast_SetStateStack(netStateIDStack);
}

// starts at line 366
void UStateMachine::NetIDStackChanged()
{
    // Check if the initial join process is not already in progress
    if (this->_initJoinInProgress == false)
    {
        // Mark that the initial join is now in progress
        this->_initJoinInProgress = true;

        // Queue the initial replicated state ID stack to be processed
        this->_netQueue.Enqueue(this->_netStateIDStackInitial);
    }
}

// starts at line 399
void UStateMachine::AuthorityTransitionStates()
{
    // First, process any substates of the state currently undergoing transition
    this->AuthorityPopSubstatesOfTransitioningState();

    // The pseudo-code shows popping the last element of the stack manually:
    // It captures the pointer and ref controller from the last element, nulls the array's internal slot,
    // and then calls RemoveAt. In high-level UE C++, this is equivalent to Pop().
    // We capture the state being exited so we can interact with it.
    TSharedPtr<BaseState> exitingState = this->_currentStateStack.Pop();

    // Call a virtual method on the exiting state to find where to transition to next.
    // The disassembly shows `call qword ptr [rax+28h]`, which is a virtual function call on BaseState.
    // It passes `this->OwnerPrivate` as a parameter and expects a TSharedPtr<BaseState> out parameter.
    TSharedPtr<BaseState> destinationState = exitingState->FindTransitionState(this->GetOwner());

    // Execute the exit logic for the state we are leaving
    exitingState->ExitState(this->GetOwner());

    // Check if a valid destination state was found
    if (destinationState.Get() != nullptr)
    {
        // Push the new state onto the stack
        this->PushNewStateStack(destinationState);
    }
    else
    {
        // Log a warning if no destination state was found
        // The verbosity check corresponds to Warning level
        UE_LOG(LogSM, Warning, TEXT("Internal error. Transitioning to a nullptr state."));
    }
}

// starts at line 417
void UStateMachine::ReplicaTransitionStates(const TArray<uint16>& newStackIDs)
{
    // Local array to hold the old stack IDs before transitioning
    TArray<uint16> oldStackIDs;
    GetStackIDs(oldStackIDs, this->_currentStateStack);

    // The disassembly shows logic to build strings representing the names of the states in the stack
    // for logging purposes. It iterates through the IDs, finds the state objects, and concatenates their names.
    // The pseudo-code explicitly shows array manipulation to build the strings, but this logic is skipped 
    // if the logging is disabled or below a certain threshold.
    
    // Log the transition at Verbosity 6 (VeryVerbose)
    FString newStackStr = this->GetStateStackIDsString(newStackIDs);
    FString oldStackStr = this->GetStateStackIDsString(oldStackIDs);
    FString logMessage = FString::Printf(TEXT("Transitioning from (%s) to (%s)"), *oldStackStr, *newStackStr);
    UE_LOG(LogSM, VeryVerbose, TEXT("%s %s"), *this->GetLogSMPrefix(), *logMessage);

    // Calculate how many states deep we need to pop to find the common ancestor
    int32 depthToPop = 0;
    if (oldStackIDs.Num() > 0)
    {
        // Find the depth where the old and new stacks diverge
        int32 compareDepth = FMath::Min(oldStackIDs.Num(), newStackIDs.Num());
        for (int32 i = 0; i < compareDepth; ++i)
        {
            if (oldStackIDs[i] != newStackIDs[i])
            {
                break;
            }
            depthToPop++;
        }
        
        // Pop the state stack up to the diverging depth
        this->PopUpToDepth(depthToPop);

        // Exit and remove the final state that diverges
        // In high-level UE C++, removing the last element is equivalent to Pop()
        TSharedPtr<BaseState> exitingState = this->_currentStateStack.Pop();

        exitingState->ExitState(this->GetOwner());
    }

    // Get the new substates to enter starting from the current depth
    TArray<TSharedPtr<BaseState>> subStack = this->GetStateStack(depthToPop, newStackIDs);

    // Create a temporary copy of the substack to pass to OnEnterStateStack
    // The disassembly shows TArray ResizeForCopy and memcpy, which handles TSharedPtr copy construction.
    TArray<TSharedPtr<BaseState>> subStackCopy = subStack;

    // Call OnEnterStateStack on the new substates
    this->OnEnterStateStack(subStackCopy);

    // Append the newly entered substates to the current state stack
    this->_currentStateStack.Append(subStack);
}

// starts at line 440
TArray<TSharedPtr<BaseState>> UStateMachine::GetStateStack(int32 depth, const TArray<uint16>& netStateIDStack) const
{
    // Initialize the result array
    TArray<TSharedPtr<BaseState>> result;

    // Safety check: ensure the requested depth is within bounds of the provided ID stack
    if (depth >= netStateIDStack.Num())
    {
        return result;
    }

    // Get the base state at the target depth using its ID
    TSharedPtr<BaseState> CurrentState = this->GetState(netStateIDStack[depth]);

    // Proceed only if the state was found
    if (CurrentState.Get() != nullptr)
    {
        // Add the initial found state to the stack result
        result.Add(CurrentState);

        // Iterate through the remaining IDs in the netStateIDStack to resolve substates
        // Start from depth + 1 since we just resolved `depth`
        for (int32 i = depth + 1; i < netStateIDStack.Num(); ++i)
        {
            uint16 targetSubStateID = netStateIDStack[i];
            TSharedPtr<BaseState> foundSubState = nullptr;

            // Search through the current state's substates for a matching ID
            for (const TSharedPtr<BaseState>& subState : CurrentState->_subStates)
            {
                if (subState.Get() != nullptr && subState->_id == targetSubStateID)
                {
                    foundSubState = subState;
                    break;
                }
            }

            // If a matching substate was found, add it to the result and step down into it
            if (foundSubState.Get() != nullptr)
            {
                result.Add(foundSubState);
                CurrentState = foundSubState;
            }
            else
            {
                // If a substate in the ID chain is missing, we must abort building the rest of the stack
                // to prevent invalid state hierarchy.
                break;
            }
        }
    }

    return result;
}

// starts at line 464
void UStateMachine::AuthorityPopSubstatesOfTransitioningState()
{
    // The target depth to pop up to. Defaults to the full size of the stack.
    int32 depthToPop = this->_currentStateStack.Num();

    // Iterate through the current state stack from bottom to top
    // The disassembly `mov ebp, [this+118h]` gets the ArrayNum of `this->_currentStateStack`
    // and loops using `esi` as the index.
    for (int32 i = 0; i < this->_currentStateStack.Num(); ++i)
    {
        TSharedPtr<BaseState> currentState = this->_currentStateStack[i];
        
        // Skip invalid states just in case
        if (currentState.Get() == nullptr)
        {
            continue;
        }

        // Call a method on the state to check for an active transition
        // The disassembly shows `call qword ptr [rax+28h]`, passing `this->OwnerPrivate`.
        // This maps to FindTransitionState.
        TSharedPtr<BaseState> transitionState = currentState->FindTransitionState(this->GetOwner());

        // If a transition is found at this level, we should pop everything above it
        if (transitionState.Get() != nullptr)
        {
            depthToPop = i;
            break;
        }
    }

    // Call PopUpToDepth using the calculated depth
    this->PopUpToDepth(depthToPop);
}

// starts at line 483
void UStateMachine::PushNewStateStack(TSharedPtr<BaseState> state)
{
    // Retrieve the state and all of its default substates to form a complete stack branch
    // The disassembly shows `subStack` being returned by value from `StackStateAndAllDefaultSubStates`
    TArray<TSharedPtr<BaseState>> subStack = this->StackStateAndAllDefaultSubStates(state);

    // Create a temporary copy of the stack to pass to OnEnterStateStack
    // The disassembly shows array resizing and copying data, managing the TSharedPtr ref counts
    TArray<TSharedPtr<BaseState>> subStackCopy = subStack;

    // Trigger EnterState logic for all states in the new branch
    this->OnEnterStateStack(subStackCopy);

    // Append the newly entered substates to the active state stack
    // The disassembly shows calculating new size, resizing if necessary, and copying elements over
    this->_currentStateStack.Append(subStack);

    // Synchronize replicated values across the network (RPCs and property updates)
    this->UpdateReplicatedValues();

    // Log the current active state stack
    this->LogStateStack();
}

// starts at line 492
void UStateMachine::PopUpToDepth(int32 depthToPop)
{
    // The disassembly shows a loop that continues as long as the current stack size
    // is greater than (depthToPop + 1). Note that depthToPop in this context represents
    // the index to keep, so if depthToPop is 0, it means keep index 0 and pop everything else.
    // The `v24` variable is `depthToPop + 1`.
    
    // Continue popping until the stack size is equal to (depthToPop + 1)
    while (this->_currentStateStack.Num() > depthToPop + 1)
    {
        // Identify the last element to pop
        int32 IndexToPop = this->_currentStateStack.Num() - 1;
        
        // Grab the state we are about to remove
        TSharedPtr<BaseState> PoppedState = this->_currentStateStack[IndexToPop];

        // The disassembly shows a manual implementation of TArray::RemoveAt,
        // involving shifting elements (memmove) and resizing (ResizeShrink).
        this->_currentStateStack.RemoveAt(IndexToPop, 1, true);

        // Ensure the state is valid
        if (PoppedState.Get() != nullptr)
        {
            // Format a log string for the exit event.
            // The string is built using the state's name (extracted from offset 24/32)
            FString exitLogMessage = FString::Printf(TEXT("<Exiting state %s>"), *PoppedState->_name);

            // Log the exit if verbosity allows (Verbosity 5 is ELogVerbosity::Verbose)
            if (this->DebugPrintCurrentStateContinuous)
            {
                // Generate a prefix using the machine type and the owner's name
                FString prefix = FString::Printf(TEXT("[%s][%s]"), *this->GetOwner()->GetName(), *this->GetMachineType());
                
                UE_LOG(LogSM, Verbose, TEXT("%s %s"), *prefix, *exitLogMessage);
            }

            // Call the virtual ExitState method on the state
            // The disassembly shows a call via `[rax+40h]` (offset 64), which maps to ExitState.
            PoppedState->OnExit(this->GetOwner());
        }
    }
}

// starts at line 516
void UStateMachine::LogStateStack() const
{
    // The disassembly shows two logging blocks. The first logs the names, the second logs the IDs.
    // Both check if Verbosity >= 6 (ELogVerbosity::VeryVerbose).

    if (UE_LOG_ACTIVE(LogSM, VeryVerbose))
    {
        // 1. Log the State Stack Names
        FString NamesString = this->GetStateStackNamesString(this->_currentStateStack);
        UE_LOG(LogSM, VeryVerbose, TEXT("%s %s"), *this->GetLogSMPrefix(), *NamesString);

        // 2. Log the State Stack IDs
        FString IDsString = this->GetStateStackIDsString(this->_currentStateStack);
        UE_LOG(LogSM, VeryVerbose, TEXT("%s %s"), *this->GetLogSMPrefix(), *IDsString);
    }
}

// starts at line 522
FString UStateMachine::GetStateStackNamesString(const TArray<TSharedPtr<BaseState>>& stateStack) const
{
    // The disassembly shows the manual construction of an FString, starting with a base size 
    // and appending the prefix "States: ".
    FString result = TEXT("States: ");

    // Iterate through the provided state stack
    for (int32 i = 0; i < stateStack.Num(); ++i)
    {
        const TSharedPtr<BaseState>& state = stateStack[i];
        
        // Skip invalid states to avoid null pointer dereferences
        if (state.Get() != nullptr)
        {
            // Append the name of the state to the result string
            result += state->_name;

            // Append a comma and space separator if this is not the last element in the stack.
            result += TEXT(", ");
        }
    }

    return result;
}

// starts at line 534
FString UStateMachine::GetStateStackIDsString(const TArray<TSharedPtr<BaseState>>& stateStack) const
{
    // The disassembly shows the creation of a temporary TArray of uint16 (unsigned short)
    // to store the IDs extracted from the state pointers in the stack.
    TArray<uint16> ids;

    // Loop through the provided state stack to collect all state IDs.
    // The disassembly iterates using pointers (rbx to rbp) and accesses the ID at offset 0x48 (72).
    for (const TSharedPtr<BaseState>& StatePtr : stateStack)
    {
        // Check if the TSharedPtr holds a valid object
        if (StatePtr.Get() != nullptr)
        {
            // Extract the ID from the state object.
            ids.Add(StatePtr->_id);
        }
    }

    // Call the overloaded version of GetStateStackIDsString that takes a TArray<uint16>.
    return this->GetStateStackIDsString(ids);
}

// starts at line 545
FString UStateMachine::GetStateStackIDsString(const TArray<uint16>& stateStackIDs) const
{
    // Initialize the result string with the "IDS: " prefix.
    // The disassembly shows the manual resizing and copying of "IDS: ".
    FString result = TEXT("IDS: ");

    // Iterate through the provided array of state IDs
    for (int32 i = 0; i < stateStackIDs.Num(); ++i)
    {
        // Extract the state ID
        uint16 stateID = stateStackIDs[i];

        // Convert the ID to a string and append it
        // The disassembly calls FString::AppendInt
        result += FString::FromInt(stateID);

        // Append a comma and space separator
        // The disassembly creates a temporary FString from ", " and appends it.
        // It does this for every element, mirroring the behavior of GetStateStackNamesString.
        result += TEXT(", ");
    }

    return result;
}

// starts at line 558
void UStateMachine::SetDrivingStateMachine(bool isDrivingStateMachine)
{
    this->_isDrivingStateMachine = isDrivingStateMachine;
}

// starts at line 573
FString UStateMachine::GetMachineType() const
{
    // Check if the current instance is running on the Server
    const TCHAR* NetworkRole = (this->GetOwner()->Role == ROLE_Authority) ? TEXT("SERVER") : TEXT("CLIENT");

    // Check if this instance is driving the state machine logic
    const TCHAR* LogicRole = (this->_isDrivingStateMachine == true) ? TEXT("MASTER") : TEXT("SLAVE");

    // Format the resulting string.
    // The disassembly shows the manual construction of the string, allocating space for "CLIENT "/"SERVER ",
    // appending a space (0x20/32), and then appending "MASTER"/"SLAVE".
    // This is equivalent to FString::Printf.
    return FString::Printf(TEXT("%s %s"), NetworkRole, LogicRole);
}

// starts at line 582
void UStateMachine::DebugPrintCurrentState(float time) const
{
    // Retrieve the formatted string of the current state stack names
    FString StackNames = this->GetStateStackNamesString(this->_currentStateStack);

    // Format the final output string
    FString DebugMessage = FString::Printf(TEXT("Current State stack: %s"), *StackNames);

    // Add an on-screen debug message if GEngine is valid
    // The disassembly shows setting up parameters (Color::Red, -1 key) and calling a function
    // which in standard UE maps to AddOnScreenDebugMessage. The call to `PostSerialize` is an
    // artifact of IDA mapping to the wrong imported function based on signature/offset.
    if (GEngine != nullptr)
    {
        // 0xFFFFFFFF (-1) means no specific key, so it will add a new message rather than replacing an old one.
        // The color is Red (extracted from `dword ptr cs:?Red@FColor@@2U1@B.___u0`).
        GEngine->AddOnScreenDebugMessage(-1, time, FColor::Red, DebugMessage);
    }
}

// starts at line 597
FString UStateMachine::GetLogSMPrefix() const
{
    // Retrieve the machine type (e.g., "SERVER MASTER" or "CLIENT SLAVE")
    FString MachineType = this->GetMachineType();

    // The disassembly shows `this->OwnerPrivate->UActorComponent::NamePrivate.CompositeComparisonValue`.
    // In standard UE high-level code, this maps to getting the name of the actor/component.
    FString OwnerName = this->GetOwner()->GetName();

    // Format and return the logging prefix.
    // The disassembly shows `FString::Printf__VA(result, L"[%s][%s]", v5, Data)` where v5 is OwnerName and Data is MachineType.
    return FString::Printf(TEXT("[%s][%s]"), *OwnerName, *MachineType);
}

// starts at line 602
void UStateMachine::LogSMError(const TCHAR* message) const
{
    // Log the error message if the verbosity level is Warning (2) or higher.
    // The disassembly checks `LogSM.Verbosity >= 2u`.
    if (UE_LOG_ACTIVE(LogSM, Warning))
    {
        // Retrieve the standard logging prefix for this state machine instance
        FString Prefix = this->GetLogSMPrefix();

        // Print the formatted error message to the log
        UE_LOG(LogSM, Warning, TEXT("%s %s"), *Prefix, message);
    }
}

// starts at line 607
void UStateMachine::LogSMVerbose(const TCHAR* message) const
{
    // Log the message if the verbosity level is VeryVerbose (6) or higher.
    // The disassembly checks `LogSM.Verbosity >= 6u`.
    if (UE_LOG_ACTIVE(LogSM, VeryVerbose))
    {
        // Retrieve the standard logging prefix for this state machine instance
        FString Prefix = this->GetLogSMPrefix();

        // Print the formatted verbose message to the log
        UE_LOG(LogSM, VeryVerbose, TEXT("%s %s"), *Prefix, message);
    }
}

// starts at line 612
void UStateMachine::LogSMLog(const TCHAR* message) const
{
    // Log the message if the verbosity level is Verbose (5) or higher.
    // The disassembly checks `LogSM.Verbosity >= 5u`.
    if (UE_LOG_ACTIVE(LogSM, Verbose))
    {
        // Retrieve the standard logging prefix for this state machine instance
        FString Prefix = this->GetLogSMPrefix();

        // Print the formatted log message
        UE_LOG(LogSM, Verbose, TEXT("%s %s"), *Prefix, message);
    }
}

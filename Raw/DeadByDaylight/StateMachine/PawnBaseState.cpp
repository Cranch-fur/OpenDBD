// starts at line 16
PawnBaseState::PawnBaseState(const FString& name, UStateMachine* stateMachine)
    : DBDBaseState(name, stateMachine)
{ }

// starts at line 26
bool PawnBaseState::IsLocallyControlled() const
{
    // Retrieve the pawn associated with this state
    APawn* pawn = this->GetPawn();

    // Verify that the pawn pointer is valid
    if (pawn == nullptr)
    {
        // Return false if there is no valid pawn
        return false;
    }

    // Call the virtual function IsLocallyControlled on the pawn
    return pawn->IsLocallyControlled();
}

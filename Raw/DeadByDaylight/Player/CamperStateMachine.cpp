// starts at line 17
void UCamperStateMachine::InitializeComponent()
{
    // Call the parent component's initialization method
    Super::InitializeComponent();

    // The extensive memory allocation and SharedPointerInternals reference controller setup 
    // found in the disassembly represents the expanded standard Unreal Engine macro MakeShareable.
    
    // Create the intro state and set it as the default state
    TSharedPtr<BaseState> stateIntro = MakeShareable(new PlayerStateIntro(this));
    if (stateIntro.Get() != nullptr)
    {
        this->AddAndSetAsDefaultState(stateIntro);
    }

    // Create and add the navigation state
    TSharedPtr<BaseState> stateNavigation = MakeShareable(new CamperStateNavigation(this));
    if (stateNavigation.Get() != nullptr)
    {
        this->AddState(stateNavigation);
    }

    // Create and add the interaction state
    TSharedPtr<BaseState> stateInteraction = MakeShareable(new PlayerStateInteraction(this));
    if (stateInteraction.Get() != nullptr)
    {
        this->AddState(stateInteraction);
    }

    // Create and add the swap interaction state
    TSharedPtr<BaseState> stateSwapInteraction = MakeShareable(new PlayerStateSwapInteraction(this));
    if (stateSwapInteraction.Get() != nullptr)
    {
        this->AddState(stateSwapInteraction);
    }

    // Create and add the guided action state
    TSharedPtr<BaseState> stateGuidedAction = MakeShareable(new CamperStateGuidedAction(this));
    if (stateGuidedAction.Get() != nullptr)
    {
        this->AddState(stateGuidedAction);
    }

    // Create and add the dead state
    TSharedPtr<BaseState> stateDead = MakeShareable(new CamperStateDead(this));
    if (stateDead.Get() != nullptr)
    {
        this->AddState(stateDead);
    }

    // Create and add the escaped state
    TSharedPtr<BaseState> stateEscaped = MakeShareable(new CamperStateEscaped(this));
    if (stateEscaped.Get() != nullptr)
    {
        this->AddState(stateEscaped);
    }

    // Create and add the immobilized state
    TSharedPtr<BaseState> stateImmobilized = MakeShareable(new CamperStateImmobilized(this));
    if (stateImmobilized.Get() != nullptr)
    {
        this->AddState(stateImmobilized);
    }

    // Create and add the pulled from closet state
    TSharedPtr<BaseState> statePulledFromCloset = MakeShareable(new CamperStatePulledFromCloset(this));
    if (statePulledFromCloset.Get() != nullptr)
    {
        this->AddState(statePulledFromCloset);
    }

    // Create and add the interrupting state
    TSharedPtr<BaseState> stateInterrupting = MakeShareable(new PlayerStateInterrupting(this));
    if (stateInterrupting.Get() != nullptr)
    {
        this->AddState(stateInterrupting);
    }
}

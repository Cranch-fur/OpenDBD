// starts at line 20
void USlasherStateMachine::InitializeComponent()
{
    // Call the parent component's initialization method
    Super::InitializeComponent();

    // The extensive memory allocation and SharedPointerInternals reference controller setup 
    // found in the disassembly represents the expanded standard Unreal Engine macro MakeShareable.
    
    // Create the intro state and set it as the default state
    TSharedPtr<BaseState> stateIntro = MakeShareable(new PlayerStateIntro(PlayerStates::INTRO, this));
    if (stateIntro.IsValid() == true)
    {
        this->AddAndSetAsDefaultState(stateIntro);
    }

    // Create and add the navigation state
    TSharedPtr<BaseState> stateNavigation = MakeShareable(new SlasherStateNavigation(PlayerStates::NAVIGATION, this));
    if (stateNavigation.IsValid() == true)
    {
        this->AddState(stateNavigation);
    }

    // Create and add the interaction state
    TSharedPtr<BaseState> stateInteraction = MakeShareable(new PlayerStateInteraction(this));
    if (stateInteraction.IsValid() == true)
    {
        this->AddState(stateInteraction);
    }

    // Create and add the swap interaction state
    TSharedPtr<BaseState> stateSwapInteraction = MakeShareable(new PlayerStateSwapInteraction(PlayerStates::SWAPINTERACTION, this));
    if (stateSwapInteraction.IsValid() == true)
    {
        this->AddState(stateSwapInteraction);
    }

    // Create and add the attack slash state
    TSharedPtr<BaseState> stateAttackSlash = MakeShareable(new PlayerStateAttackSlash(this));
    if (stateAttackSlash.IsValid() == true)
    {
        this->AddState(stateAttackSlash);
    }

    // Create and add the attack pounce state
    TSharedPtr<BaseState> stateAttackPounce = MakeShareable(new PlayerStateAttackPounce(this));
    if (stateAttackPounce.IsValid() == true)
    {
        this->AddState(stateAttackPounce);
    }

    // Create and add the attack chainsaw state
    TSharedPtr<BaseState> stateAttackChainsaw = MakeShareable(new PlayerStateAttackChainsaw(this));
    if (stateAttackChainsaw.IsValid() == true)
    {
        this->AddState(stateAttackChainsaw);
    }

    // Create and add the attack lunge state
    TSharedPtr<BaseState> stateAttackLunge = MakeShareable(new PlayerStateAttackLunge(this));
    if (stateAttackLunge.IsValid() == true)
    {
        this->AddState(stateAttackLunge);
    }

    // Create and add the attack blink state
    TSharedPtr<BaseState> stateAttackBlink = MakeShareable(new PlayerStateAttackBlink(this));
    if (stateAttackBlink.IsValid() == true)
    {
        this->AddState(stateAttackBlink);
    }

    // Create and add the interrupting state
    TSharedPtr<BaseState> stateInterrupting = MakeShareable(new PlayerStateInterrupting(this));
    if (stateInterrupting.IsValid() == true)
    {
        this->AddState(stateInterrupting);
    }

    // Create and add the blink state
    TSharedPtr<BaseState> stateBlink = MakeShareable(new PlayerStateBlink(PlayerStates::BLINK, this));
    if (stateBlink.IsValid() == true)
    {
        this->AddState(stateBlink);
    }

    // Create and add the blink window state (uses PlayerStateBlinkCooldown class)
    TSharedPtr<BaseState> stateBlinkWindow = MakeShareable(new PlayerStateBlinkCooldown(PlayerStates::BLINKWINDOW, this));
    if (stateBlinkWindow.IsValid() == true)
    {
        this->AddState(stateBlinkWindow);
    }

    // Create and add the blink cooldown state
    TSharedPtr<BaseState> stateBlinkCooldown = MakeShareable(new PlayerStateBlinkCooldown(PlayerStates::BLINKCOOLDOWN, this));
    if (stateBlinkCooldown.IsValid() == true)
    {
        this->AddState(stateBlinkCooldown);
    }
}

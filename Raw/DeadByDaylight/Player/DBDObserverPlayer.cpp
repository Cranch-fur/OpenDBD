// starts at line 14
ADBDObserverPlayer::ADBDObserverPlayer(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)),
{
    this->kLoadingTime(2.0f) // 0x40000000 in hex represents 2.0f

    // Create the default subobject for player data.
    // This replaces the expanded UObject::CreateDefaultSubobject call seen in the disassembly.
    this->_playerData = this->CreateDefaultSubobject<UDBDPlayerData>(TEXT("PlayerData"));

    // Reset the loading timer using the defined constant loading time.
    this->_loadingTimer.Reset(this->kLoadingTime);
}

// starts at line 20
void ADBDObserverPlayer::Tick(float DeltaSeconds)
{
    // Call the base class Tick method
    Super::Tick(DeltaSeconds);

    // Check if the loading timer has not finished yet
    if (this->_loadingTimer.IsDone() == false)
    {
        // Get the current world context
        UWorld* World = this->GetWorld();

        // Ensure the world has begun play
        if (World->HasBegunPlay() == true)
        {
            // Verify the Controller is valid. 
            // The decompiled GUObjectArray flag checks and bActorIsBeingDestroyed checks 
            // are the inline expansion of the standard Unreal Engine IsValid() global macro/function.
            if (IsValid(this->Controller) == true)
            {
                // Retrieve the GameState from the World
                AGameState* GameState = Cast<AGameState>(World->GameState);
                
                // Verify the GameState is valid using the standard UE function
                if (IsValid(GameState) == true)
                {
                    // Check if the current match state is in progress
                    if (GameState->MatchState == MatchState::InProgress)
                    {
                        // Update the loading timer with the delta time
                        this->_loadingTimer.Update(DeltaSeconds);

                        // Check if the timer has just finished after the update
                        if (this->_loadingTimer.IsDone() == true)
                        {
                            // Notify the server that the player is ready
                            this->Server_PlayerReady();
                        }
                    }
                }
            }
        }
    }
}

// starts at line 37
void ADBDObserverPlayer::Server_PlayerReady_Implementation()
{
    this->_playerData->_gameLevelLoaded = true;
}

// starts at line ???
bool ADBDObserverPlayer::Server_PlayerReady_Validate()
{
    return true;
}

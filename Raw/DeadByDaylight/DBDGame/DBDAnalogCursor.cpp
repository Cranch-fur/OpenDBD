void FDBDAnalogCursor::CenterCursor()
{
    // Retrieve the current GameViewport from the global engine pointer
    UGameViewportClient* GameViewport = GEngine->GameViewport;

    // Verify the GameViewport pointer is valid
    if (GameViewport != nullptr)
    {
        // Check if the UObject is valid and not marked as unreachable/pending kill
        // The disassembly explicitly checks GUObjectArray and object flags, 
        // which is the inline expansion of the standard IsValid() check.
        if (IsValid(GameViewport) == true)
        {
            FViewport* Viewport = GameViewport->Viewport;

            // Ensure the underlying FViewport is instantiated
            if (Viewport != nullptr)
            {
                // Get the size of the viewport
                // VTable offset 0x10 corresponds to FViewport::GetSizeXY()
                FIntPoint ViewportSize = Viewport->GetSizeXY();

                // Calculate the exact center of the screen
                int32 CenterX = (int32)(ViewportSize.X * 0.5f);
                int32 CenterY = (int32)(ViewportSize.Y * 0.5f);

                // Set the mouse cursor to the calculated center coordinates
                // VTable offset 0xd8 corresponds to FViewport::SetMouse()
                GEngine->GameViewport->Viewport->SetMouse(CenterX, CenterY);
            }
        }
    }

    // Update the cursor state after centering it
    this->UpdateCursor();
}




bool FDBDAnalogCursor::HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent)
{
    // Determine if the input originates from a gamepad by checking the KeyCode.
    // Explicitly checking against 0 without using the '!' operator.
    if (InAnalogInputEvent.KeyCode == 0) /* UNDEFINED ELEMENT */
    {
        this->IsUsingGamePad = true;
    }
    else
    {
        this->IsUsingGamePad = false;
    }

    // Update the internal state or position of the cursor
    this->UpdateCursor();

    // Pass the input event down to the base Unreal Engine analog cursor class to retain default behavior
    return FAnalogCursor::HandleAnalogInputEvent(SlateApp, InAnalogInputEvent);
}




bool FDBDAnalogCursor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
    // Determine if the input originates from a gamepad by checking the raw KeyCode.
    // Explicitly checking for equality with 0 instead of using the '!' operator.
    if (InKeyEvent.KeyCode == 0) /* UNDEFINED ELEMENT */
    {
        this->IsUsingGamePad = true;
    }
    else
    {
        this->IsUsingGamePad = false;
    }

    // Refresh the internal state and/or position of the cursor
    this->UpdateCursor();

    // Pass the input event down to the base Unreal Engine analog cursor class to retain standard functionality
    return FAnalogCursor::HandleKeyDownEvent(SlateApp, InKeyEvent);
}




bool FDBDAnalogCursor::HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
    // Determine if the input originates from a gamepad by checking the raw KeyCode.
    // Explicitly checking for equality with 0 instead of using the '!' operator.
    if (InKeyEvent.KeyCode == 0) /* UNDEFINED ELEMENT */
    {
        this->IsUsingGamePad = true;
    }
    else
    {
        this->IsUsingGamePad = false;
    }

    // Refresh the internal state and/or position of the cursor
    this->UpdateCursor();

    // Cache the properties of the incoming key event into the internal _lastKeyEvent struct.
    // The compiler generated a member-wise copy operation here.
    this->_lastKeyEvent.ModifierKeys = InKeyEvent.ModifierKeys;
    this->_lastKeyEvent.UserIndex = InKeyEvent.UserIndex;
    this->_lastKeyEvent.bIsRepeat = InKeyEvent.bIsRepeat;
    this->_lastKeyEvent.EventPath = InKeyEvent.EventPath;
    this->_lastKeyEvent.Key = InKeyEvent.Key;
    this->_lastKeyEvent.CharacterCode = InKeyEvent.CharacterCode;
    this->_lastKeyEvent.KeyCode = InKeyEvent.KeyCode; /* UNDEFINED ELEMENT */

    // Pass the input event down to the base Unreal Engine analog cursor class to retain standard functionality
    return FAnalogCursor::HandleKeyUpEvent(SlateApp, InKeyEvent);
}




bool FDBDAnalogCursor::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
    // A mouse movement explicitly indicates the user is relying on a standard mouse, 
    // so we disable the gamepad flag.
    this->IsUsingGamePad = false;

    // Refresh the internal state and/or position of the cursor
    this->UpdateCursor();

    // Pass the input event down to the base Unreal Engine analog cursor class.
    // (Restored the true base class call obscured by linker Identical Code Folding)
    return FAnalogCursor::HandleMouseMoveEvent(SlateApp, MouseEvent);
}




void FDBDAnalogCursor::LockHorizontal(bool isLockedOnHorizontal)
{
    // Update the internal flag indicating whether horizontal movement is locked
    this->_isLockedOnHorizontal = isLockedOnHorizontal;

    // Refresh the internal state and/or position of the cursor to apply the lock
    this->UpdateCursor();
}




void FDBDAnalogCursor::LockVertical(bool isLockedOnVertical)
{
    // Update the internal flag indicating whether vertical movement is locked
    this->_isLockedOnVertical = isLockedOnVertical;

    // Refresh the internal state and/or position of the cursor to apply the lock
    this->UpdateCursor();
}




void FDBDAnalogCursor::MoveCursorBy(int32 moveByX, int32 moveByY)
{
    // Retrieve the GameViewport from the global GEngine pointer
    UGameViewportClient* GameViewport = GEngine->GameViewport;

    // Verify the GameViewport pointer is valid without using the '!' operator
    if (GameViewport != nullptr)
    {
        // Check if the UObject is valid and not marked as unreachable/pending kill
        if (IsValid(GameViewport) == true)
        {
            FViewport* Viewport = GameViewport->Viewport;

            // Ensure the underlying FViewport is instantiated
            if (Viewport != nullptr)
            {
                // Calculate the new X position by adding the requested delta to the current X
                // VTable offset 0xb0 corresponds to FViewport::GetMouseX()
                int32 newMouseX = moveByX + Viewport->GetMouseX();

                // Calculate the new Y position by adding the requested delta to the current Y
                // The compiler redundantly fetches the Viewport from GEngine here
                // VTable offset 0xb8 corresponds to FViewport::GetMouseY()
                int32 newMouseY = moveByY + GEngine->GameViewport->Viewport->GetMouseY();

                // Apply the newly calculated mouse coordinates
                // VTable offset 0xd8 corresponds to FViewport::SetMouse()
                GEngine->GameViewport->Viewport->SetMouse(newMouseX, newMouseY);
            }
        }
    }
}




void FDBDAnalogCursor::SetCursorSpeeds(float defaultSpeed, float stickySpeed)
{
    // Update the internal property for the cursor's standard movement speed
    this->_cursorDefaultSpeed = defaultSpeed;

    // Update the internal property for the cursor's speed when passing over sticky UI elements
    this->_cursorStickySpeed = stickySpeed;
}




void FDBDAnalogCursor::SetCursorStickySpeed(float stickySpeed)
{
    // Update the internal property for the cursor's speed when interacting with sticky UI elements.
    // The decompiler mistakenly labeled 'this' as a PhysX class due to identical memory offsets.
    this->_cursorStickySpeed = stickySpeed;
}




void FDBDAnalogCursor::SetStickines(uint8_t isSticky)
{
    // Assign the incoming stickiness value to the class member variable.
    // Offset 0x49 based on disassembly [rcx+0x49]
    this->IsSticky = isSticky;

    // Trigger the update logic for the cursor to reflect the new state.
    // This is a tailcall in disassembly, represented here as a standard call.
    this->UpdateCursor();
}




void FDBDAnalogCursor::Tick(float DeltaTime, FSlateApplication* SlateApp, TSharedRef<ICursor, 0> Cursor)
{
    // Create a local shared reference. In disassembly, this involves 
    // manually incrementing the reference count of the SharedReferenceCount.ReferenceController.
    TSharedRef<ICursor, 0> LocalCursor = Cursor;

    // Call the base class Tick implementation.
    // FAnalogCursor is the parent class of FDBDAnalogCursor.
    FAnalogCursor::Tick(DeltaTime, SlateApp, LocalCursor);

    // Clamp the CurrentOffset.X value between 0.0f and 1.0f.
    // Offset 0x10 based on disassembly [rbx+0x10].
    if (this->CurrentOffset.X < 0.0f)
    {
        this->CurrentOffset.X = 0.0f;
    }

    if (this->CurrentOffset.X > 1.0f)
    {
        this->CurrentOffset.X = 1.0f;
    }

    // Clamp the CurrentOffset.Y value between 0.0f and 1.0f.
    // Offset 0x14 based on disassembly [rbx+0x14].
    if (this->CurrentOffset.Y < 0.0f)
    {
        this->CurrentOffset.Y = 0.0f;
    }

    if (this->CurrentOffset.Y > 1.0f)
    {
        this->CurrentOffset.Y = 1.0f;
    }

    // Note: The assembly from 14022e92c to the end represents the 
    // destruction/cleanup of the shared reference (Decrementing counts, 
    // calling DestroyObject and the destructor), which is handled 
    // automatically by the C++ compiler for TSharedRef.
}




void FDBDAnalogCursor::UpdateCursor()
{
    // Ensure the game instance is valid before proceeding.
    if (this->_gameInstance == nullptr)
    {
        return;
    }

    // UE4 Internal: Verify the GameInstance object is not pending kill/garbage using GUObjectArray.
    /* UNDEFINED ELEMENT */
    int32_t GameInstanceIndex = this->_gameInstance->InternalIndex;
    if (GameInstanceIndex >= GUObjectArray.ObjObjects.NumElements || (GUObjectArray.ObjObjects.Objects[GameInstanceIndex].Flags & 0x20000000) != 0)
    {
        return;
    }

    // Retrieve the local player controller from the game instance.
    APlayerController* PC = this->_gameInstance->GetLocalPlayerController();
    if (PC == nullptr)
    {
        return;
    }

    // UE4 Internal: Verify the PlayerController object validity via InternalIndex.
    /* UNDEFINED ELEMENT */
    int32_t PCIndex = PC->InternalIndex;
    if (PCIndex >= GUObjectArray.ObjObjects.NumElements || (GUObjectArray.ObjObjects.Objects[PCIndex].Flags & 0x20000000) != 0)
    {
        return;
    }

    // DeadByDaylight specific: Ensure the player controller is of the correct base class.
    /* UNDEFINED ELEMENT */
    UClass* StaticClass = ADBDPlayerControllerBase::GetPrivateStaticClass(L"/Script/DeadByDaylight");
    if ((PC->ClassPrivate->ClassTreeIndex - StaticClass->ClassTreeIndex) > StaticClass->ClassTreeNumChildren)
    {
        return;
    }

    // Secondary validity check and bitfield check (offset 0x140 usually relates to Actor state/flags).
    if (PC->bActorIsBeingDestroyed == false)
    {
        return;
    }

    // Default control mode is Mouse/Keyboard (0).
    uint8_t NewControlMode = 0;

    if (this->IsUsingGamePad == false)
    {
        // Handling Mouse/Keyboard input.
        this->SetMaxSpeed(this->_cursorDefaultSpeed);

        // 0xF (15) is likely a custom cursor type for DBD or standard 'None/Default'.
        PC->CurrentMouseCursor = 0xF;
    }
    else
    {
        // Handling Gamepad input.
        float StickMagnitude = 0.0f;

        // Calculate magnitude of the analog stick input: sqrt(X^2 + Y^2).
        if (this->IsSticky == true)
        {
            float X = this->AnalogValues[0].X;
            float Y = this->AnalogValues[0].Y;
            StickMagnitude = sqrtf((X * X) + (Y * Y));
        }

        // If the cursor is "sticky" and input is below the ignore threshold, apply stickiness.
        if (this->IsSticky == true && StickMagnitude < this->_cursorStickIgnore)
        {
            float FinalSpeedX = this->_cursorStickySpeed;
            float FinalSpeedY = this->_cursorStickySpeed;

            // Zero out speed on axes that are currently locked.
            if (this->_isLockedOnVertical == true)
            {
                FinalSpeedY = 0.0f;
            }
            if (this->_isLockedOnHorizontal == true)
            {
                FinalSpeedX = 0.0f;
            }

            this->SetMaxSpeed(FinalSpeedX, FinalSpeedY);

            // Update cursor visual: 9 = Vertical Lock, 10 = Horizontal/Both Lock.
            uint8_t StickyCursorType = 9;
            if (this->_isLockedOnVertical == true || this->_isLockedOnHorizontal == true)
            {
                StickyCursorType = 10;
            }
            PC->CurrentMouseCursor = StickyCursorType;
        }
        else
        {
            // Default gamepad speed when not sticky or moving fast.
            this->SetMaxSpeed(this->_cursorDefaultSpeed);
            PC->CurrentMouseCursor = 8; // 8 = Hand/Grab cursor.
        }

        // Check if we should switch to Gamepad Control Mode (1).
        URootMovie* RootMovie = this->_gameInstance->GetRootMovie();
        if (RootMovie != nullptr)
        {
            /* UNDEFINED ELEMENT */
            int32_t MovieIndex = RootMovie->InternalIndex;
            if (MovieIndex < GUObjectArray.ObjObjects.NumElements && (GUObjectArray.ObjObjects.Objects[MovieIndex].Flags & 0x20000000) == 0)
            {
                UScreenController* ScreenCtrl = RootMovie->m_ScreenController;
                if (ScreenCtrl != nullptr)
                {
                    /* UNDEFINED ELEMENT */
                    int32_t ScreenCtrlIndex = ScreenCtrl->InternalIndex;
                    if (ScreenCtrlIndex < GUObjectArray.ObjObjects.NumElements && (GUObjectArray.ObjObjects.Objects[ScreenCtrlIndex].Flags & 0x20000000) == 0)
                    {
                        NewControlMode = 1; // XBOX / Gamepad Mode.
                    }
                }
            }
        }
    }

    // Apply the determined Control Mode to the Screen Controller.
    URootMovie* FinalRootMovie = this->_gameInstance->GetRootMovie();
    if (FinalRootMovie != nullptr)
    {
        /* UNDEFINED ELEMENT */
        int32_t FinalMovieIndex = FinalRootMovie->InternalIndex;
        if (FinalMovieIndex < GUObjectArray.ObjObjects.NumElements && (GUObjectArray.ObjObjects.Objects[FinalMovieIndex].Flags & 0x20000000) == 0)
        {
            UScreenController* FinalScreenCtrl = FinalRootMovie->m_ScreenController;
            if (FinalScreenCtrl != nullptr)
            {
                /* UNDEFINED ELEMENT */
                int32_t FinalCtrlIndex = FinalScreenCtrl->InternalIndex;
                if (FinalCtrlIndex < GUObjectArray.ObjObjects.NumElements && (GUObjectArray.ObjObjects.Objects[FinalCtrlIndex].Flags & 0x20000000) == 0)
                {
                    FinalScreenCtrl->SetActiveViewsControlMode(NewControlMode);
                }
            }
        }
    }
}




void FDBDAnalogCursor::SetCursorStickIgnore(float value)
{
    // Assign the new ignore threshold to the class member variable.
    // Offset 0x54 based on disassembly [rcx+0x54]
    this->_cursorStickIgnore = value;
}




void FDBDAnalogCursor::SetCursorDefaultSpeed(float value)
{
    // Assign the incoming float value to the default speed member variable.
    // Offset 0x4c based on disassembly [rcx+0x4c]
    this->_cursorDefaultSpeed = value;
}
ADBDGame_Menu::ADBDGame_Menu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) // call ??0AGameMode@@QEAA@AEBVFObjectInitializer@@@Z
{
    // .text:000000014023B8FE lea rax, ??_7ADBDGame_Menu@@6B@
    // VTable pointer is set implicitly by the C++ compiler after the base constructor returns.

    // .text:000000014023B905 mov word ptr [rbx+458h], 1
    // This instruction writes 0x0001 to offset 0x458. 
    // This covers both StartWithSlasher (0x458) and _introCamPlayed (0x459).
    // 0x458 = 1 (true), 0x459 = 0 (false).
    this->StartWithSlasher = true;
    this->_introCamPlayed = false;

    // .text:000000014023B918 mov byte ptr [rbx+45Ah], 0
    // Sets the enum _gameIntroSteps to 0 (Idling).
    this->_gameIntroSteps = EGameIntroSteps::Idling;

    // .text:000000014023B91F call ?GetPrivateStaticClass@ADBDPlayerController_Menu...
    // .text:000000014023B92B mov [rbx+3F0h], rax
    // Offset 0x3F0 corresponds to AGameMode::PlayerControllerClass.
    // The ASM calls GetPrivateStaticClass with L"/Script/DeadByDaylight". 
    // In functional UE4 C++, this matches the ::StaticClass() call.
    this->PlayerControllerClass = ADBDPlayerController_Menu::StaticClass();

    // .text:000000014023B932 call ?GetPrivateStaticClass@ADBDPlayerState_Menu...
    // .text:000000014023B937 mov [rbx+408h], rax
    // Offset 0x408 corresponds to AGameMode::PlayerStateClass.
    this->PlayerStateClass = ADBDPlayerState_Menu::StaticClass();

    // .text:000000014023B93E mov rax, rbx
    // Return this (standard constructor behavior).
}


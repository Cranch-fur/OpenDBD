void __fastcall ADBDGame_Menu::ADBDGame_Menu(ADBDGame_Menu* this, const FObjectInitializer* ObjectInitializer)
{
	AGameMode::AGameMode(this, ObjectInitializer);
	*(_WORD*)&this->StartWithSlasher = 1;
	this->__vftable = (ADBDGame_Menu_vtbl*)&ADBDGame_Menu::`vftable';
		this->_gameIntroSteps = Idling;
	this->PlayerControllerClass.Class = ADBDPlayerController_Menu::GetPrivateStaticClass(L"/Script/DeadByDaylight");
	this->PlayerStateClass.Class = ADBDPlayerState_Menu::GetPrivateStaticClass(L"/Script/DeadByDaylight");
}
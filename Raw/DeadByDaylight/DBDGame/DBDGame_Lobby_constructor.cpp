void __fastcall ADBDGame_Lobby::ADBDGame_Lobby(ADBDGame_Lobby* this, const FObjectInitializer* ObjectInitializer)
{
	AGameMode::AGameMode(this, ObjectInitializer);
	this->__vftable = (ADBDGame_Lobby_vtbl*)&ADBDGame_Lobby::`vftable';
		this->_onSessionDestroyedHandle.ID = 0;
	this->_timerValue = -1.0;
	*(_DWORD*)&this->_forceStarted = 0x1000000;
	this->PlayerControllerClass.Class = ADBDPlayerController_Menu::GetPrivateStaticClass(L"/Script/DeadByDaylight");
	this->PlayerStateClass.Class = ADBDPlayerState_Lobby::GetPrivateStaticClass(L"/Script/DeadByDaylight");
}
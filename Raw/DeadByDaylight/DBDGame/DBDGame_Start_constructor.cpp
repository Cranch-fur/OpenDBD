void __fastcall ADBDGame_Start::ADBDGame_Start(ADBDGame_Start* this, const FObjectInitializer* ObjectInitializer)
{
	AGameMode::AGameMode(this, ObjectInitializer);
	this->__vftable = (ADBDGame_Start_vtbl*)&ADBDGame_Start::`vftable';
		this->OnDBDInitComplete.InvocationList.AllocatorInstance.SecondaryData.Data = 0;
	*(_QWORD*)&this->OnDBDInitComplete.InvocationList.ArrayNum = 0;
	*(_QWORD*)&this->OnDBDInitComplete.CompactionThreshold = 2;
	this->_loadProgress[0] = 0;
	this->_deferredError[0] = 0;
	this->_timeoutHandle.Handle = 0;
	this->_generateAuthTicketHandle.ID = 0;
	this->_initEACHandle.ID = 0;
	this->_loadProfileHandle.ID = 0;
	this->_loadStatsHandle.ID = 0;
	this->_loadCurrenciesHandle.ID = 0;
	this->PlayerControllerClass.Class = ADBDPlayerControllerBase::GetPrivateStaticClass(L"/Script/DeadByDaylight");
	this->PlayerStateClass.Class = ADBDPlayerState::GetPrivateStaticClass(L"/Script/DeadByDaylight");
}
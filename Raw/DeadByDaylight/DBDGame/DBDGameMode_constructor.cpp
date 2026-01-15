void __fastcall ADBDGameMode::ADBDGameMode(ADBDGameMode* this, const FObjectInitializer* ObjectInitializer)
{
	AGameMode::AGameMode(this, ObjectInitializer);
	*(_WORD*)&this->StartWithSlasher = 1;
	this->__vftable = (ADBDGameMode_vtbl*)&ADBDGameMode::`vftable';
		this->_initialPlayerStarts.AllocatorInstance.Data = 0;
	*(_QWORD*)&this->_initialPlayerStarts.ArrayNum = 0;
	*(_WORD*)&this->_proceduralMode = 0;
	this->_currentLevelLoadingStep[0] = 1;
	this->_debugCurrentCamperIndex = 0;
	FDBDTimer::FDBDTimer(&this->_gameLevelLoadingTimeOutTimer);
	this->_readyDebugString.Data.AllocatorInstance.Data = 0;
	*(_QWORD*)&this->_readyDebugString.Data.ArrayNum = 0;
	this->kGameLevelLoadedMaxTime = 540.0;
	this->_killerGoalPoints = 0;
	this->_dcCheckNeedsResponse = 0;
	this->_networkCheckFailCount = 0;
	FDBDTimer::FDBDTimer(&this->_networkCheckFailTimer);
	this->_networkCheckIndex = 0;
	this->_networkCheckResults = 0;
	this->PlayerStateClass.Class = ADBDPlayerState::GetPrivateStaticClass(L"/Script/DeadByDaylight");
	FDBDTimer::Reset(&this->_gameLevelLoadingTimeOutTimer, this->kGameLevelLoadedMaxTime);
	this->_levelLoadingTimeoutSent = 0;
}
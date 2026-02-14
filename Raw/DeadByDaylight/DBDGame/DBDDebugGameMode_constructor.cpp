void __fastcall ADBDDebugGameMode::ADBDDebugGameMode(ADBDDebugGameMode* this, const FObjectInitializer* ObjectInitializer)
{
    ADBDGameMode::ADBDGameMode(this, ObjectInitializer);
    this->__vftable = (ADBDDebugGameMode_vtbl*)&ADBDDebugGameMode::`vftable';
    this->CamperPawnOverrides.AllocatorInstance.Data = 0;
    *(_QWORD*)&this->CamperPawnOverrides.ArrayNum = 0;
    this->SlasherPawnOverrides.AllocatorInstance.Data = 0;
    *(_QWORD*)&this->SlasherPawnOverrides.ArrayNum = 0;
    this->_currentCamperIndex = 0;
}
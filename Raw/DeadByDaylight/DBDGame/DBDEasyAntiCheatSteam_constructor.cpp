void __fastcall UDBDEasyAntiCheat::UDBDEasyAntiCheat(UDBDEasyAntiCheat* this, const FObjectInitializer* ObjectInitializer)
{
    UObject::UObject(this, ObjectInitializer);
    this->__vftable = (UDBDEasyAntiCheat_vtbl*)&UDBDEasyAntiCheat::`vftable';
    this->OnInitComplete.InvocationList.AllocatorInstance.SecondaryData.Data = 0;
    *(_QWORD*)&this->OnInitComplete.InvocationList.ArrayNum = 0;
    *(_QWORD*)&this->OnInitComplete.CompactionThreshold = 2;
    this->_gameInstance = 0;
    this->_connectedToServer = 0;
    this->_hEACServerLibrary = 0;
    this->_server = 0;
    this->_hEACClientLibrary = 0;
    this->_client = 0;
    this->_clientAuthorizationTimerHandle.Handle = 0;
    this->_serverAuthorizationTimerHandle.Handle = 0;
    this->_clientInfos.AllocatorInstance.Data = 0;
    *(_QWORD*)&this->_clientInfos.ArrayNum = 0;
}
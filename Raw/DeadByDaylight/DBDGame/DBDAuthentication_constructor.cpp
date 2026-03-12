void __fastcall UDBDAuthentication::UDBDAuthentication(UDBDAuthentication* this, const FObjectInitializer* ObjectInitializer)
{
    UObject::UObject(this, ObjectInitializer);
    this->__vftable = (UDBDAuthentication_vtbl*)&UDBDAuthentication::vftable;
        this->_localInfos.AllocatorInstance.Data = 0;
    *(_QWORD*)&this->_localInfos.ArrayNum = 0;
    this->_remoteInfos.AllocatorInstance.Data = 0;
    *(_QWORD*)&this->_remoteInfos.ArrayNum = 0;
    this->_gameInstance = 0;
}
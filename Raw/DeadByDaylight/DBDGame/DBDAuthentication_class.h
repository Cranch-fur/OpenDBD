class __base(UObject, 0) UDBDAuthentication
{
/* 0x00 */    struct UDBDAuthentication: :VTable* vtable;
/* 0x08 */    __inherited enum EObjectFlags UObjectBase: :ObjectFlags;
/* 0x0c */        __inherited int32_t UObjectBase::InternalIndex;
/* 0x10 */        __inherited class UClass* UObjectBase: :ClassPrivate;
/* 0x18 */        __inherited class FName UObjectBase: :NamePrivate;
/* 0x20 */        __inherited class UObject* UObjectBase: :OuterPrivate;
/* 0x28 */        class TArray<FAuthenticationInfo, FDefaultAllocator >  _localInfos;
/* 0x38 */    class TArray<FAuthenticationInfo, FDefaultAllocator >  _remoteInfos;
/* 0x48 */    class UDBDGameInstance* _gameInstance;
/* 0x50 */
};
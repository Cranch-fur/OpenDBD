class __base(UObject, 0) UDBDEasyAntiCheat
{
/* 0x00 */    struct UDBDEasyAntiCheat: :VTable* vtable;
/* 0x08 */    __inherited enum EObjectFlags UObjectBase: :ObjectFlags;
/* 0x0c */    __inherited int32_t UObjectBase::InternalIndex;
/* 0x10 */    __inherited class UClass* UObjectBase: :ClassPrivate;
/* 0x18 */    __inherited class FName UObjectBase: :NamePrivate;
/* 0x20 */    __inherited class UObject* UObjectBase: :OuterPrivate;
/* 0x28 */    __padding char _28[8];
/* 0x30 */    class TMulticastDelegate<void, bool >  OnInitComplete;
/* 0xa0 */    class UDBDGameInstance* _gameInstance;
/* 0xa8 */    uint8_t _connectedToServer;
/* 0xb0 */    struct HINSTANCE___1* _hEACServerLibrary;
/* 0xb8 */    class easyanticheat: :server::hydra::IGameServer* _server;
/* 0xc0 */    struct HINSTANCE___1* _hEACClientLibrary;
/* 0xc8 */    class easyanticheat: :client::hydra::IGameClient* _client;
/* 0xd0 */    struct FTimerHandle _clientAuthorizationTimerHandle;
/* 0xd8 */    struct FTimerHandle _serverAuthorizationTimerHandle;
/* 0xe0 */    class TArray<FEACClientInfo, FDefaultAllocator >  _clientInfos;
/* 0xf0 */
};

class __base(ULocalPlayer, 0) UDBDLocalPlayer
{
/*0x0000*/  struct UDBDLocalPlayer::VTable* vtable;
/*0x0008*/  __inherited enum EObjectFlags UObjectBase::ObjectFlags;
/*0x000c*/  __inherited int32_t UObjectBase::InternalIndex;
/*0x0010*/  __inherited class UClass* UObjectBase::ClassPrivate;
/*0x0018*/  __inherited class FName UObjectBase::NamePrivate;
/*0x0020*/  __inherited class UObject* UObjectBase::OuterPrivate;
/*0x0028*/  __inherited struct UChildConnection::UNetConnection::UPlayer::FExec::VTable* FExec::vtable;
/*0x0030*/  __inherited class APlayerController* UPlayer::PlayerController;
/*0x0038*/  __inherited int32_t UPlayer::CurrentNetSpeed;
/*0x003c*/  __inherited int32_t UPlayer::ConfiguredInternetSpeed;
/*0x0040*/  __inherited int32_t UPlayer::ConfiguredLanSpeed;
            __padding char _44[4];
/*0x0048*/  __inherited class TSharedPtr<FUniqueNetId const ,0> ULocalPlayer::CachedUniqueNetId;
/*0x0058*/  __inherited class UGameViewportClient* ULocalPlayer::ViewportClient;
/*0x0060*/  __inherited struct FVector2D ULocalPlayer::Origin;
/*0x0068*/  __inherited struct FVector2D ULocalPlayer::Size;
/*0x0070*/  __inherited struct FVector ULocalPlayer::LastViewLocation;
/*0x007c*/  __inherited class TEnumAsByte<enum EAspectRatioAxisConstraint> ULocalPlayer::AspectRatioAxisConstraint;
            __padding char _7d[3];
/*0x0080*/  __inherited class TSubclassOf<APlayerController> ULocalPlayer::PendingLevelPlayerControllerClass;
/*0x0088*/  __inherited union
            {
                uint32_t bSentSplitJoin;
            } __bitfield88;
            __padding char _8c[4];
/*0x0090*/  __inherited class FSceneViewStateReference ULocalPlayer::ViewState;
/*0x00b8*/  __inherited class FSceneViewStateReference ULocalPlayer::StereoViewState;
/*0x00e0*/  __inherited int32_t ULocalPlayer::ControllerId;
            __padding char _e4[4];
/*0x00e8*/  __inherited class FReply ULocalPlayer::SlateOperations;
};

void __fastcall ADBDParadiseGameMode::ADBDParadiseGameMode(ADBDParadiseGameMode *this, const FObjectInitializer *ObjectInitializer)
{
  AGameMode::AGameMode(this, ObjectInitializer);
  this->__vftable = (ADBDParadiseGameMode_vtbl *)&ADBDParadiseGameMode::`vftable';
  this->PlayerStateClass.Class = ADBDPlayerState::GetPrivateStaticClass(L"/Script/DeadByDaylight");
  this->PlayerControllerClass.Class = ADBDPlayerController::GetPrivateStaticClass(L"/Script/DeadByDaylight");
}
#include "BaseProjectileLauncher.h"
#include "LaunchInfo.h"
#include "UObject/NoExportTypes.h"
#include "Net/UnrealNetwork.h"

class ABaseProjectile;
class ADBDPlayer;
class AActor;

void UBaseProjectileLauncher::OnReload_Implementation(int32 ammoNew)
{

}

void UBaseProjectileLauncher::OnLaunch_Implementation(FLaunchInfo LaunchInfo)
{

}

void UBaseProjectileLauncher::Local_SetMaxAmmo(int32 newMaxAmmo)
{

}

void UBaseProjectileLauncher::Local_Reload(int32 ammoToAdd)
{

}

void UBaseProjectileLauncher::Local_Launch()
{

}

bool UBaseProjectileLauncher::IsLocallyControlled() const
{
	return false;
}

bool UBaseProjectileLauncher::HasProjectile() const
{
	return false;
}

bool UBaseProjectileLauncher::HasAuthority() const
{
	return false;
}

ABaseProjectile* UBaseProjectileLauncher::GetProjectileToLaunch_Implementation() const
{
	return NULL;
}

ADBDPlayer* UBaseProjectileLauncher::GetPlayerOwner() const
{
	return NULL;
}

ABaseProjectile* UBaseProjectileLauncher::GetOldestProjectile() const
{
	return NULL;
}

int32 UBaseProjectileLauncher::GetMaxAmmo() const
{
	return 0;
}

float UBaseProjectileLauncher::GetLaunchSpeed_Implementation() const
{
	return 0.0f;
}

FVector UBaseProjectileLauncher::GetLaunchPosition_Implementation() const
{
	return FVector{};
}

FVector UBaseProjectileLauncher::GetLaunchDirection_Implementation() const
{
	return FVector{};
}

ABaseProjectile* UBaseProjectileLauncher::GetFirstAvailableProjectile() const
{
	return NULL;
}

int32 UBaseProjectileLauncher::GetAmmo() const
{
	return 0;
}

void UBaseProjectileLauncher::Broadcast_SetMaxAmmo_Server_Implementation(int8 newMaxAmmo)
{

}

bool UBaseProjectileLauncher::Broadcast_SetMaxAmmo_Server_Validate(int8 newMaxAmmo)
{
	return true;
}

void UBaseProjectileLauncher::Broadcast_SetMaxAmmo_Multicast_Implementation(int8 newMaxAmmo)
{

}

void UBaseProjectileLauncher::Broadcast_Reload_Server_Implementation(int8 ammoToAdd)
{

}

bool UBaseProjectileLauncher::Broadcast_Reload_Server_Validate(int8 ammoToAdd)
{
	return true;
}

void UBaseProjectileLauncher::Broadcast_Reload_Multicast_Implementation(int8 ammoToAdd)
{

}

bool UBaseProjectileLauncher::Broadcast_Reload_Multicast_Validate(int8 ammoToAdd)
{
	return true;
}

void UBaseProjectileLauncher::Broadcast_Launch_Server_Implementation(FLaunchInfo LaunchInfo)
{

}

bool UBaseProjectileLauncher::Broadcast_Launch_Server_Validate(FLaunchInfo LaunchInfo)
{
	return true;
}

void UBaseProjectileLauncher::Broadcast_Launch_Multicast_Implementation(FLaunchInfo LaunchInfo)
{

}

bool UBaseProjectileLauncher::Broadcast_Launch_Multicast_Validate(FLaunchInfo LaunchInfo)
{
	return true;
}

void UBaseProjectileLauncher::Authority_OnProjectileDestroyed(AActor* DestroyedActor)
{

}

void UBaseProjectileLauncher::Authority_CleanProjectilePool(AActor* DestroyedActor)
{

}

void UBaseProjectileLauncher::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseProjectileLauncher, _projectiles);
}

UBaseProjectileLauncher::UBaseProjectileLauncher()
{
	this->ProjectileClass = NULL;
	this->ammo = 1;
	this->MaxAmmo = 1;
	this->PoolSize = 1;
}

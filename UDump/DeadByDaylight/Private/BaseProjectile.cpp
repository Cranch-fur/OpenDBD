#include "BaseProjectile.h"
#include "UObject/NoExportTypes.h"
#include "Engine/HitResult.h"
#include "LaunchInfo.h"
#include "ImpactInfo.h"
#include "Engine/EngineTypes.h"
#include "Net/UnrealNetwork.h"

class USphereComponent;
class UBaseProjectileLauncher;
class UPrimitiveComponent;

bool ABaseProjectile::SphereTraceSingle(FVector Start, FVector End, USphereComponent* Sphere, FHitResult& outHitResult) const
{
	return false;
}

void ABaseProjectile::SetActive(bool Active)
{

}

void ABaseProjectile::OnSetActive_Implementation(bool Active)
{

}

void ABaseProjectile::OnRep_ProjectileLauncher()
{

}

void ABaseProjectile::OnLaunch_Implementation(FLaunchInfo LaunchInfo)
{

}

void ABaseProjectile::OnInitialize_Implementation(UBaseProjectileLauncher* projectileLauncher)
{

}

void ABaseProjectile::OnDetectPlayer_Implementation(FImpactInfo ImpactInfo)
{

}

void ABaseProjectile::OnDetectCollision_Implementation(FImpactInfo ImpactInfo)
{

}

void ABaseProjectile::Multicast_Launch_Implementation(FLaunchInfo LaunchInfo)
{

}

bool ABaseProjectile::Multicast_Launch_Validate(FLaunchInfo LaunchInfo)
{
	return true;
}

void ABaseProjectile::Multicast_DetectPlayer_Implementation(FImpactInfo ImpactInfo)
{

}

bool ABaseProjectile::Multicast_DetectPlayer_Validate(FImpactInfo ImpactInfo)
{
	return true;
}

void ABaseProjectile::Multicast_DetectImpact_Implementation(FImpactInfo ImpactInfo)
{

}

bool ABaseProjectile::Multicast_DetectImpact_Validate(FImpactInfo ImpactInfo)
{
	return true;
}

bool ABaseProjectile::LineTraceSingle(FVector Start, FVector End, FHitResult& outHitResult) const
{
	return false;
}

bool ABaseProjectile::IsValidPlayerDetection_Implementation(FImpactInfo ImpactInfo)
{
	return false;
}

bool ABaseProjectile::IsValidImpactDetection_Implementation(FImpactInfo ImpactInfo)
{
	return false;
}

bool ABaseProjectile::IsAvailable_Implementation() const
{
	return false;
}

bool ABaseProjectile::IsActive() const
{
	return false;
}

UBaseProjectileLauncher* ABaseProjectile::GetProjectileLauncher() const
{
	return NULL;
}

UPrimitiveComponent* ABaseProjectile::GetImpactPrimitiveComponent_Implementation() const
{
	return NULL;
}

FCollisionResponseContainer ABaseProjectile::GetImpactCollisionResponseContainer_Implementation() const
{
	return FCollisionResponseContainer{};
}

ECollisionChannel ABaseProjectile::GetImpactCollisionObjectType_Implementation() const
{
	return ECC_WorldStatic;
}

bool ABaseProjectile::Authority_TryDetectPlayer(FImpactInfo ImpactInfo)
{
	return false;
}

bool ABaseProjectile::Authority_TryDetectCollision(FImpactInfo ImpactInfo, bool Force)
{
	return false;
}

void ABaseProjectile::Authority_Launch(FLaunchInfo LaunchInfo)
{

}

void ABaseProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseProjectile, _projectileLauncher);
}

ABaseProjectile::ABaseProjectile()
{

}

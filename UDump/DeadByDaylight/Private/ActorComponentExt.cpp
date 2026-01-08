#include "ActorComponentExt.h"

class UActorComponent;
class AActor;
class ACamperPlayer;
class ADBDPlayer;
class ASlasherPlayer;

bool UActorComponentExt::IsLocallyObserved(const UActorComponent* ActorComponent)
{
	return false;
}

bool UActorComponentExt::IsLocallyControlled(const UActorComponent* ActorComponent)
{
	return false;
}

bool UActorComponentExt::HasAuthority(const UActorComponent* ActorComponent)
{
	return false;
}

AActor* UActorComponentExt::GetValidatedOwner(const UActorComponent* ActorComponent)
{
	return NULL;
}

ACamperPlayer* UActorComponentExt::GetOwningSurvivor(const UActorComponent* ActorComponent)
{
	return NULL;
}

ADBDPlayer* UActorComponentExt::GetOwningPlayer(const UActorComponent* ActorComponent)
{
	return NULL;
}

ASlasherPlayer* UActorComponentExt::GetOwningKiller(const UActorComponent* ActorComponent)
{
	return NULL;
}

UActorComponentExt::UActorComponentExt()
{

}

#include "EscapeDoor.h"
#include "Net/UnrealNetwork.h"

void AEscapeDoor::OnRep_Activated(bool oldActivated)
{

}

void AEscapeDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEscapeDoor, activated);
}

AEscapeDoor::AEscapeDoor()
{
	this->activated = false;
}

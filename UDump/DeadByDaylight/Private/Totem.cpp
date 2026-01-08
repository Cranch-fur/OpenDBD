#include "Totem.h"
#include "Net/UnrealNetwork.h"

void ATotem::Multicast_BindToHex_Implementation(FName hexPerkIDNew)
{

}

bool ATotem::HasHex() const
{
	return false;
}

void ATotem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATotem, hexPerkID);
	DOREPLIFETIME(ATotem, IsCleansed);
}

ATotem::ATotem()
{
	this->hexPerkID = NAME_None;
	this->IsCleansed = false;
}

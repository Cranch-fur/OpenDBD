#include "AkReverbVolume.h"
#include "Net/UnrealNetwork.h"

void AAkReverbVolume::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAkReverbVolume, bEnabled);
}

AAkReverbVolume::AAkReverbVolume()
{
	this->bEnabled = true;
	this->AuxBusName = TEXT("");
	this->SendLevel = 1.000000;
	this->FadeRate = 0.500000;
	this->Priority = 1.000000;
}

#include "DBDSlasherData.h"
#include "Net/UnrealNetwork.h"

void UDBDSlasherData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBDSlasherData, VisionEnabled);
	DOREPLIFETIME(UDBDSlasherData, CloakingEnabled);
}

UDBDSlasherData::UDBDSlasherData()
{
	this->VisionEnabled = false;
	this->CloakingEnabled = false;
	this->CurrentGuidedAction = ESlasherGuidedAction::VE_None;
	this->SoundDisplayLifetime = 0.000000;
}

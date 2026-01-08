#include "DBDCamperData.h"
#include "Net/UnrealNetwork.h"

void UDBDCamperData::DamageStateChanged()
{

}

void UDBDCamperData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBDCamperData, DamageState);
}

UDBDCamperData::UDBDCamperData()
{
	this->DamageState = ECamperDamageState::VE_Healthy;
	this->CurrentGuidedAction = ECamperGuidedAction::VE_None;
}

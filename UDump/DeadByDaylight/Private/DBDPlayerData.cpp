#include "DBDPlayerData.h"
#include "UObject/NoExportTypes.h"
#include "Net/UnrealNetwork.h"

void UDBDPlayerData::SetLoadoutSpawned()
{

}

void UDBDPlayerData::SetLightSpawned()
{

}

void UDBDPlayerData::SetIntroCompleted()
{

}

void UDBDPlayerData::Server_SetIntroCompleted_Implementation()
{

}

bool UDBDPlayerData::Server_SetIntroCompleted_Validate()
{
	return true;
}

void UDBDPlayerData::Server_SetControlRotation_Implementation(FRotator Rotation)
{

}

bool UDBDPlayerData::Server_SetControlRotation_Validate(FRotator Rotation)
{
	return true;
}

void UDBDPlayerData::Server_LoadoutSpawned_Implementation()
{

}

bool UDBDPlayerData::Server_LoadoutSpawned_Validate()
{
	return true;
}

void UDBDPlayerData::Server_LightSpawned_Implementation()
{

}

bool UDBDPlayerData::Server_LightSpawned_Validate()
{
	return true;
}

bool UDBDPlayerData::IsLoadoutSpawned() const
{
	return false;
}

bool UDBDPlayerData::IsLightSpawned() const
{
	return false;
}

bool UDBDPlayerData::IsIntroCompleted()
{
	return false;
}

bool UDBDPlayerData::GetUseInputPressed() const
{
	return false;
}

bool UDBDPlayerData::GetLeftRightInputMashed() const
{
	return false;
}

bool UDBDPlayerData::GetInteractionInputMashed() const
{
	return false;
}

void UDBDPlayerData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBDPlayerData, Blinded);
	DOREPLIFETIME(UDBDPlayerData, _controlRotation);
}

UDBDPlayerData::UDBDPlayerData()
{
	this->Blinded = false;
}

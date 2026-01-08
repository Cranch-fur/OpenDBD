#include "Tile.h"
#include "Net/UnrealNetwork.h"

void ATile::OnRep_Initialized(bool OldValue)
{

}

void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATile, _initialized);
}

ATile::ATile()
{
	this->MinimumSpacing = 0;
	this->MaximumSpacing = 0;
	this->MaximumCount = 0;
	this->Type = ETileType::None;
	this->Density = EDensity::Unspecified;
	this->Path = EPathType::None;
	this->Number = 0;
	this->PlaceHolder = false;
	this->TestTile = false;
	this->_initialized = false;
	this->TypeSpacing = false;
}

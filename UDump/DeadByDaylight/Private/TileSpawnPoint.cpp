#include "TileSpawnPoint.h"
#include "Net/UnrealNetwork.h"

class AActor;

void UTileSpawnPoint::OnRep_SpawnObject()
{

}

AActor* UTileSpawnPoint::GetSpawnedObject() const
{
	return NULL;
}

void UTileSpawnPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTileSpawnPoint, _spawnedObject);
}

UTileSpawnPoint::UTileSpawnPoint()
{
	this->TileSpawnPointType = ETileSpawnPointType::Unspecified;
	this->Weight = 0.000000;
}

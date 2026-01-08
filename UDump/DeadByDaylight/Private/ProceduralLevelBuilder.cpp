#include "ProceduralLevelBuilder.h"
#include "ETileSpawnPointType.h"
#include "GenerationParams.h"
#include "Net/UnrealNetwork.h"

class AActor;

void AProceduralLevelBuilder::SyncSeeds()
{

}

AActor* AProceduralLevelBuilder::SpawnActor(UClass* ActorClass, ETileSpawnPointType spawnPointType)
{
	return NULL;
}

void AProceduralLevelBuilder::Multicast_DebugShareGenerationData_Implementation(FGenerationParams usedParams)
{

}

void AProceduralLevelBuilder::BuildFromSeed()
{

}

void AProceduralLevelBuilder::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProceduralLevelBuilder, _syncer);
}

AProceduralLevelBuilder::AProceduralLevelBuilder()
{
	this->PathToMapDirectory = TEXT("");
	this->PathToTilesDirectory = TEXT("");
	this->KillerMinProximityFromCamper = 3.000000;
	this->SurvivorMinProximityFromOtherCamper = 1.000000;
	this->MinProximityBetweenGenerators = 0.500000;
	this->SurvivorMinProximityFromGenerator = 0.300000;
	this->SurvivorMinProximityFromTotem = 0.200000;
	this->DebugMap = NULL;
	this->SmallMeatLockerMinProximityFromKillerLair = 1.500000;
	this->SmallMeatLockerMinProximityFromEachOther = 0.500000;
	this->_tileClass = NULL;
}

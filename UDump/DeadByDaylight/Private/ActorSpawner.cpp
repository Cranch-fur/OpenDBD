#include "ActorSpawner.h"

void UActorSpawner::EditorForceSpawnVisualization()
{

}

void UActorSpawner::Authority_SpawnActorAsyncEvent()
{

}

UActorSpawner::UActorSpawner()
{
	this->Type = EGameplayElementType::Generic;
	this->ActivatedByDefault = true;
	this->Weight = 1.000000;
	this->SpawnCountMax = 1;
	this->_toSpawn = NULL;
}

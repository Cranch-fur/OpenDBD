#include "DBD_SoundSpawnerData.h"


FDBD_SoundSpawnerData::FDBD_SoundSpawnerData()
{
	this->TriggerObject = TArray<TAssetSubclassOf<UObject>>();
	this->TriggerVisualForCamper = false;
	this->TriggerVisualForSlasher = false;
	this->ActorToSpawnVisual = TArray<TAssetSubclassOf<AActor>>();
}

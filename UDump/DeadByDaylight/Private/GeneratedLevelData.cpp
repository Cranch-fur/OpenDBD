#include "GeneratedLevelData.h"


FGeneratedLevelData::FGeneratedLevelData()
{
	this->UsedPaperTileMap = TEXT("");
	this->AvailableEscapeCount = 0;
	this->AvailableSurvivorItemCount = 0;
	this->GroupingType = ESurvivorGrouping::Invalid;
	this->KillerSpawnPoint = NULL;
	this->SurvivorSpawnPoints = TArray<USceneComponent*>();
	this->InteractableElementsSpawnPoints = TArray<USceneComponent*>();
	this->MeatHookSpawners = TArray<USceneComponent*>();
	this->SearchableSpawners = TArray<USceneComponent*>();
	this->HexSpawners = TArray<USceneComponent*>();
	this->KillerLairSpawners = TArray<USceneComponent*>();
	this->LevelDependencies = TArray<FDependency>();
	this->UseForcedMap = false;
	this->GenerationLogs = TEXT("");
	this->GenerationErrors = TEXT("");
}

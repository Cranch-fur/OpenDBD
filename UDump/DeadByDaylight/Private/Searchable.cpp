#include "Searchable.h"
#include "SearchableSpawnPoint.h"

class ACollectable;
class ADBDPlayer;

ACollectable* ASearchable::SpawnObject(ADBDPlayer* Player)
{
	return NULL;
}

ASearchable::ASearchable()
{
	this->Weight = 1.000000;
	this->ItemRarity = EItemRarity::Common;
	this->_searchableSpawnPoint = CreateDefaultSubobject<USearchableSpawnPoint>(TEXT("SearchableSpawnPoint"));
}

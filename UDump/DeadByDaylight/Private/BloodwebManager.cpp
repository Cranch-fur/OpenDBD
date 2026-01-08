#include "BloodwebManager.h"
#include "DBDBloodWebDefinition.h"

class UDBDGameInstance;
class UDBDDesignTunables;

void UBloodwebManager::Init(UDBDGameInstance* GameInstance, UDBDDesignTunables* tunables)
{

}

UBloodwebManager::UBloodwebManager()
{
	this->_bloodWebDefinition = CreateDefaultSubobject<UDBDBloodWebDefinition>(TEXT("bloodWebDefinition"));
}

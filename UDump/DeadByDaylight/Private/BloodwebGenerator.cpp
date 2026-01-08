#include "BloodwebGenerator.h"
#include "BloodWebPersistentData.h"
#include "EPlayerRole.h"

class UDBDGameInstance;
class UDBDDesignTunables;
class UDBDBloodWebDefinition;

void UBloodwebGenerator::Init(UDBDGameInstance* GameInstance, UDBDDesignTunables* designTunables, UDBDBloodWebDefinition* bloodWebDefinition)
{

}

TArray<FString> UBloodwebGenerator::GetBloodwebNodes() const
{
	return TArray<FString>();
}

void UBloodwebGenerator::BuildWebInPlace(FBloodWebPersistentData& Bloodweb, EPlayerRole playerRole, int32 currentPlayerLevel)
{

}

UBloodwebGenerator::UBloodwebGenerator()
{
	this->EmptyNodeOccurenceFactor = 0.000000;
	this->_nodeContentTotals = 0;
	this->_alternativePathOccurenceFactor = 0.000000;
}

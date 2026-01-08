#include "EscapeTypeObjects.h"


FEscapeTypeObjects::FEscapeTypeObjects()
{
	this->Type = EscapeStrategyType::EscapeDoor;
	this->EscapeObjects = TArray<TAssetSubclassOf<AActor>>();
}

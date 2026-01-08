#include "GeneratorDrivenActor.h"

class AGenerator;

void AGeneratorDrivenActor::SetSpawnedObject(AGenerator* Generator)
{

}

void AGeneratorDrivenActor::Multicast_ActivationChecked_Implementation(bool activated)
{

}

AGeneratorDrivenActor::AGeneratorDrivenActor()
{
	this->_activated = false;
}

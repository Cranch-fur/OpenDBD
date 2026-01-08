#include "ActorKnowledgeCollection.h"
#include "EKnowledgeSharingType.h"
#include "Net/UnrealNetwork.h"

class ACharacter;
class AActor;

bool UActorKnowledgeCollection::IsAvailable(const ACharacter* Character) const
{
	return false;
}

bool UActorKnowledgeCollection::Contains(AActor* Actor) const
{
	return false;
}

void UActorKnowledgeCollection::Authority_SetSharingType(EKnowledgeSharingType Value)
{

}

void UActorKnowledgeCollection::Authority_SetPossessor(ACharacter* possessor)
{

}

void UActorKnowledgeCollection::Authority_SetAvailable(bool Value)
{

}

void UActorKnowledgeCollection::Authority_Remove(AActor* Actor)
{

}

void UActorKnowledgeCollection::Authority_Empty()
{

}

void UActorKnowledgeCollection::Authority_Append(const TArray<AActor*>& Actors)
{

}

bool UActorKnowledgeCollection::Authority_Add(AActor* Actor)
{
	return false;
}

void UActorKnowledgeCollection::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActorKnowledgeCollection, _actors);
	DOREPLIFETIME(UActorKnowledgeCollection, _available);
	DOREPLIFETIME(UActorKnowledgeCollection, _possessor);
	DOREPLIFETIME(UActorKnowledgeCollection, _sharing);
}

UActorKnowledgeCollection::UActorKnowledgeCollection()
{
	this->_available = false;
	this->_sharing = EKnowledgeSharingType::Possessor;
}

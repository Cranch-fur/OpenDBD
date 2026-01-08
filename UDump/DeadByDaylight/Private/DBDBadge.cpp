#include "DBDBadge.h"
#include "EDBDScoreTypes.h"

class AActor;
class UWorld;

void UDBDBadge::OnGameplayEvent_Implementation(EDBDScoreTypes gameplayEventType, float amount, AActor* Instigator, AActor* target)
{

}

bool UDBDBadge::IsOwnerPlayer(AActor* Actor) const
{
	return false;
}

UWorld* UDBDBadge::GetWorld() const
{
	return NULL;
}

float UDBDBadge::GetProgress_Implementation() const
{
	return 0.0f;
}

bool UDBDBadge::EvaluateShouldReward_Implementation() const
{
	return false;
}

UDBDBadge::UDBDBadge()
{
	this->BadgeId = NAME_None;
	this->ForwardGameEvents = false;
	this->_rewarded = false;
}

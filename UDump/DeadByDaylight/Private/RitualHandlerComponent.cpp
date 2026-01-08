#include "RitualHandlerComponent.h"
#include "UObject/NoExportTypes.h"
#include "EDBDScoreTypes.h"

class AActor;

void URitualHandlerComponent::UpdateRitualStatus()
{

}

void URitualHandlerComponent::TryGenerateCachedRitualWithKey(FName Key)
{

}

void URitualHandlerComponent::TryGenerateCachedRitualFromNetTime()
{

}

void URitualHandlerComponent::TryGenerateCachedRitual(FDateTime timeToUse, bool usedNetTime, bool forceOverwrite)
{

}

void URitualHandlerComponent::ShowDailyRituals()
{

}

void URitualHandlerComponent::ResetRituals()
{

}

void URitualHandlerComponent::ReceiveGameEvent(EDBDScoreTypes ScoreType, float amount, AActor* Instigator, AActor* target)
{

}

void URitualHandlerComponent::ReceivedRitualAssignedTime(FDateTime Now, bool usedNetTime)
{

}

void URitualHandlerComponent::DismissRitual(FName toRemove)
{

}

void URitualHandlerComponent::CheatAddToRitual(int32 Index, float Value)
{

}

void URitualHandlerComponent::AssignNewRitual()
{

}

URitualHandlerComponent::URitualHandlerComponent()
{

}

#include "GameEventTracker.h"
#include "EDBDScoreTypes.h"

class ADBDPlayerState;
class AActor;

void UGameEventTracker::FireGameflowEvent(EDBDScoreTypes eventType, float amount, const ADBDPlayerState* Instigator)
{

}

void UGameEventTracker::FireGameEvent(EDBDScoreTypes eventType, float amount, AActor* Instigator, AActor* target)
{

}

void UGameEventTracker::FireAnalyticEvent(const FString& EventName)
{

}

UGameEventTracker::UGameEventTracker()
{

}

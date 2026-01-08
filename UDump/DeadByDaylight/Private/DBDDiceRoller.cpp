#include "DBDDiceRoller.h"
#include "EDiceRollType.h"

class AActor;
class ADBDPlayer;

bool UDBDDiceRoller::GetRollResultByTunableValue(const AActor* RollingActor, FName TunableValueName, float MultiplicativeModifier, float AdditiveModifier)
{
	return false;
}

bool UDBDDiceRoller::GetRollResultByDiceRollType(const ADBDPlayer* RollingPlayer, EDiceRollType DiceRollType)
{
	return false;
}

bool UDBDDiceRoller::CalculateTickedRollResult(const AActor* RollingActor, float BaseProbability, float ticks, float MultiplicativeModifier, float AdditiveModifier)
{
	return false;
}

bool UDBDDiceRoller::CalculateRollResult(const AActor* RollingActor, float BaseProbability, float MultiplicativeModifier, float AdditiveModifier)
{
	return false;
}

bool UDBDDiceRoller::CalculateOverTimeRollResult(const AActor* RollingActor, float BaseProbability, float deltaTime, float ExpectedTime, float MultiplicativeModifier, float AdditiveModifier)
{
	return false;
}

UDBDDiceRoller::UDBDDiceRoller()
{

}

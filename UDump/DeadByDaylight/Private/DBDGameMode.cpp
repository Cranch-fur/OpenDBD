#include "DBDGameMode.h"
#include "EDBDScoreTypes.h"
#include "UObject/NoExportTypes.h"

class AActor;

void ADBDGameMode::OnGameplayEvent(EDBDScoreTypes ScoreType, float amount, AActor* instigatorr, AActor* target)
{

}

void ADBDGameMode::LevelLoaded(const FString& LevelName)
{

}

void ADBDGameMode::HostGame()
{

}

FTimespan ADBDGameMode::GetElapsedMatchTime() const
{
	return FTimespan{};
}

ADBDGameMode::ADBDGameMode()
{
	this->StartWithSlasher = true;
	this->EscapeOpened = false;
}

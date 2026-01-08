#include "DBDDesignTunables.h"

float UDBDDesignTunables::GetTunableValue(FName ID, bool warnIfRowMissing) const
{
	return 0.0f;
}

UDBDDesignTunables::UDBDDesignTunables()
{
	this->WinStreakThresholdLarge = 0;
	this->WinStreakThresholdSmall = 0;
	this->MaxExperience = 0;
	this->MaxFearTokens = 0;
	this->MaxSkulls = 0;
	this->CreditsHtml = TEXT("");
	this->LegalTextTitle = FText::GetEmpty();
	this->LegalText = FText::GetEmpty();
	this->SystemMessage_ChatDisconnected = FText::GetEmpty();
}

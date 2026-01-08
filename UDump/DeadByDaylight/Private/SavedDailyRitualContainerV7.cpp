#include "SavedDailyRitualContainerV7.h"


FSavedDailyRitualContainerV7::FSavedDailyRitualContainerV7()
{
	this->LastRitualReceivedDate = TEXT("");
	this->Rituals = TArray<FLegacySavedDailyRitualInstance>();
}

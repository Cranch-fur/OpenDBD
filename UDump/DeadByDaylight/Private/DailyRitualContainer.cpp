#include "DailyRitualContainer.h"


FDailyRitualContainer::FDailyRitualContainer()
{
	this->LastRitualReceivedDate = FDateTime{};
	this->Rituals = TArray<FDailyRitualInstance>();
}

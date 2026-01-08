#include "QueueAnalytics.h"


FQueueAnalytics::FQueueAnalytics()
{
	this->ClientId = TEXT("");
	this->TimesQueuedSurvivor = 0;
	this->TimesQueuedKiller = 0;
	this->MatchesAsSurvivor = 0;
	this->MatchesAsKiller = 0;
	this->Platform = TEXT("");
}

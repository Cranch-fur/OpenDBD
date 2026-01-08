#include "PlayerstateDataCache.h"


FPlayerstateDataCache::FPlayerstateDataCache()
{
	this->ScoreEvents = TMap<FName, FScoreEventArray>();
	this->TimeInactive = 0.0f;
}

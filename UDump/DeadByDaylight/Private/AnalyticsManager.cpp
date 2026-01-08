#include "AnalyticsManager.h"


FAnalyticsManager::FAnalyticsManager()
{
	this->MatchInfoAnalytics = FMatchInfoAnalyticsController{};
	this->WaitTimeAnalytics = FWaitTimeAnalyticsController{};
	this->GameplayKillerAnalytics = FGameplay_KillerAnalyticsController{};
	this->GameplaySurvivorAnalytics = FGameplay_SurvivorAnalyticsController{};
	this->ReportAnalytics = FReportAnalyticsController{};
}

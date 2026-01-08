#include "ReportAnalytics.h"


FReportAnalytics::FReportAnalytics()
{
	this->ClientIdTransmitter = TEXT("");
	this->ClientIdReceiver = TEXT("");
	this->RoleTransmitter = EPlayerRole::VE_None;
	this->RoleReceiver = EPlayerRole::VE_None;
	this->MatchID = TEXT("");
	this->GameMode = TEXT("");
	this->ReportType = TEXT("");
	this->ReportCategory = TEXT("");
	this->Comments = TEXT("");
	this->Platform = TEXT("");
}

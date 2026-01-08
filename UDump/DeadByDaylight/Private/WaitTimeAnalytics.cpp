#include "WaitTimeAnalytics.h"


FWaitTimeAnalytics::FWaitTimeAnalytics()
{
	this->EnterLobbyWaitTime = 0.0f;
	this->EnterMatchWaitTime = 0.0f;
	this->MatchWaitStartUtc = TEXT("");
	this->MatchWaitStopUtc = TEXT("");
	this->LobbyWaitStartUtc = TEXT("");
	this->LobbyWaitStopUtc = TEXT("");
	this->ClientId = TEXT("");
	this->MatchID = TEXT("");
	this->Version = TEXT("");
	this->GameMode = EGameType::Online;
	this->Role = EPlayerRole::VE_None;
	this->PartySize = 0;
	this->Platform = TEXT("");
}

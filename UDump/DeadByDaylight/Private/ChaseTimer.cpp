#include "ChaseTimer.h"


FChaseTimer::FChaseTimer()
{
	this->CooldownTimer = FDBDTimer{};
	this->TotalChaseTime = 0.0f;
	this->camper = NULL;
}

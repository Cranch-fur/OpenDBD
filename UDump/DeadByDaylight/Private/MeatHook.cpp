#include "MeatHook.h"
#include "PollableEventListener.h"

class ACamperPlayer;
class UMontagePlayer;

void AMeatHook::OnHookedSurvivorAttacked()
{

}

void AMeatHook::Multicast_SetIsSacrificed_Implementation(bool Value)
{

}

void AMeatHook::Multicast_SetIsSabotaged_Implementation(bool Value)
{

}

void AMeatHook::Multicast_SetHookedSurvivor_Implementation(ACamperPlayer* survivor)
{

}

void AMeatHook::Multicast_EscapeAttemptResult_Implementation(bool result)
{

}

UMontagePlayer* AMeatHook::GetMontagePlayer() const
{
	return NULL;
}

bool AMeatHook::GetIsSacrificed() const
{
	return false;
}

bool AMeatHook::GetIsSabotaged() const
{
	return false;
}

bool AMeatHook::GetIsBroken() const
{
	return false;
}

ACamperPlayer* AMeatHook::GetHookedSurvivor() const
{
	return NULL;
}

bool AMeatHook::CanUnhookSurvivor(ACamperPlayer* survivor) const
{
	return false;
}

bool AMeatHook::CanSurvivorStruggle(ACamperPlayer* survivor) const
{
	return false;
}

bool AMeatHook::CanSurvivorAttemptEscape(ACamperPlayer* survivor) const
{
	return false;
}

bool AMeatHook::CanHookSurvivor() const
{
	return false;
}

bool AMeatHook::CanBeSabotaged() const
{
	return false;
}

void AMeatHook::Authority_SignalUnhookingEnter()
{

}

void AMeatHook::Authority_SignalUnhookingCharged()
{

}

void AMeatHook::Authority_SignalUnhookingAborted()
{

}

void AMeatHook::Authority_SignalSurvivorAttemptingEscapeCharged()
{

}

void AMeatHook::Authority_SignalSurvivorAttemptingEscapeAborted()
{

}

void AMeatHook::Authority_SignalSurvivorAttemptingEscape()
{

}

void AMeatHook::Authority_SetIsSacrificed(bool Value)
{

}

void AMeatHook::Authority_SetIsSabotaged(bool Value)
{

}

void AMeatHook::Authority_SetHookedSurvivor(ACamperPlayer* survivor)
{

}

AMeatHook::AMeatHook()
{
	this->IsSurvivorStruggling = false;
	this->WasSurvivorStruggleCancelled = false;
	this->IsInBasement = false;
	this->IsBreakable = false;
	this->_eventListener = CreateDefaultSubobject<UPollableEventListener>(TEXT("EventListener"));
	this->_isSacrificed = false;
	this->_isSabotaged = false;
	this->_survivorUnhookable = false;
	this->_canSurvivorAttemptEscape = false;
	this->_survivorCanStruggle = false;
	this->_canBeSabotaged = false;
	this->_drainStartTimer = 0.000000;
	this->_isIdle = false;
}

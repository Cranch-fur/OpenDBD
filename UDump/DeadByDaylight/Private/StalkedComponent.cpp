#include "StalkedComponent.h"
#include "Net/UnrealNetwork.h"

class UChargeableComponent;
class ADBDPlayer;

void UStalkedComponent::SetChargeableComponent(UChargeableComponent* ChargeableComponent)
{

}

void UStalkedComponent::OnPlayerDisconnect(ADBDPlayer* Player)
{

}

bool UStalkedComponent::IsStalkHighlightVisible() const
{
	return false;
}

bool UStalkedComponent::IsOutlineVisible() const
{
	return false;
}

bool UStalkedComponent::IsBeingStalked() const
{
	return false;
}

float UStalkedComponent::GetStalkPoints(ADBDPlayer* caller) const
{
	return 0.0f;
}

float UStalkedComponent::GetPercentStalkPoints(ADBDPlayer* caller) const
{
	return 0.0f;
}

float UStalkedComponent::GetOutlineVisibleRange() const
{
	return 0.0f;
}

float UStalkedComponent::GetMaxStalkPoints(ADBDPlayer* caller) const
{
	return 0.0f;
}

UChargeableComponent* UStalkedComponent::GetChargeableComponent() const
{
	return NULL;
}

bool UStalkedComponent::CanBeStandingKilled() const
{
	return false;
}

void UStalkedComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStalkedComponent, _maxStalkPoints);
}

UStalkedComponent::UStalkedComponent()
{
	this->_percentForHightlight = 0.333330;
	this->_rangeForHightlight = 2000.000000;
	this->_percentForOutline = 0.666660;
	this->_rangeForOutline = 1000.000000;
	this->_percentForStalkWarningMin = 0.500000;
	this->_percentForStalkWarningMax = 1.000000;
	this->_maxStalkPoints = 0.000000;
}

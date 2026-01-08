#include "StalkerComponent.h"
#include "EDetectionZone.h"

class ADBDPlayer;

void UStalkerComponent::Update(float deltaTime, ADBDPlayer* Player, EDetectionZone stalkerDetectionZone)
{

}

void UStalkerComponent::OnPlayerDisconnect(ADBDPlayer* Player)
{

}

void UStalkerComponent::NotifyKill(ADBDPlayer* killedPlayer)
{

}

bool UStalkerComponent::IsStalkingSomeone() const
{
	return false;
}

bool UStalkerComponent::IsAtLastStalkTier() const
{
	return false;
}

bool UStalkerComponent::HasMaxStalkPoints() const
{
	return false;
}

float UStalkerComponent::GetTotalStalkingPoints() const
{
	return 0.0f;
}

float UStalkerComponent::GetRequirementForTier(int32 tier) const
{
	return 0.0f;
}

float UStalkerComponent::GetPercentStalkingPointsInCurrentTier() const
{
	return 0.0f;
}

float UStalkerComponent::GetPercentStalkingPoints() const
{
	return 0.0f;
}

int32 UStalkerComponent::GetMaxStalkTier() const
{
	return 0;
}

float UStalkerComponent::GetMaxStalkingPoints() const
{
	return 0.0f;
}

float UStalkerComponent::GetEvilWithinScoreMultiplier() const
{
	return 0.0f;
}

int32 UStalkerComponent::GetCurrentTier() const
{
	return 0;
}

bool UStalkerComponent::CanStandKill(ADBDPlayer* Player) const
{
	return false;
}

bool UStalkerComponent::CanStalk() const
{
	return false;
}

void UStalkerComponent::Broadcast_Tier_Implementation(int32 previousTier, int32 currentTier, bool IsFirstTime)
{

}

bool UStalkerComponent::Broadcast_Tier_Validate(int32 previousTier, int32 currentTier, bool IsFirstTime)
{
	return true;
}

void UStalkerComponent::Broadcast_SetStalkPoints_Implementation(float stalkPoints)
{

}

bool UStalkerComponent::Broadcast_SetStalkPoints_Validate(float stalkPoints)
{
	return true;
}

void UStalkerComponent::Broadcast_Multicast_Tier_Implementation(int32 previousTier, int32 currentTier, bool IsFirstTime)
{

}

bool UStalkerComponent::Broadcast_Multicast_Tier_Validate(int32 previousTier, int32 currentTier, bool IsFirstTime)
{
	return true;
}

void UStalkerComponent::Broadcast_Multicast_SetStalkPoints_Implementation(float stalkPoints)
{

}

bool UStalkerComponent::Broadcast_Multicast_SetStalkPoints_Validate(float stalkPoints)
{
	return true;
}

void UStalkerComponent::Broadcast_BeingStalked_Server_Implementation(ADBDPlayer* Player, bool beingStalked)
{

}

bool UStalkerComponent::Broadcast_BeingStalked_Server_Validate(ADBDPlayer* Player, bool beingStalked)
{
	return true;
}

void UStalkerComponent::Broadcast_BeingStalked_Multicast_Implementation(ADBDPlayer* Player, bool beingStalked)
{

}

bool UStalkerComponent::Broadcast_BeingStalked_Multicast_Validate(ADBDPlayer* Player, bool beingStalked)
{
	return true;
}

void UStalkerComponent::Broadcast_AddStalkPointsToPlayer_Server_Implementation(ADBDPlayer* Player, float stalkPoints)
{

}

bool UStalkerComponent::Broadcast_AddStalkPointsToPlayer_Server_Validate(ADBDPlayer* Player, float stalkPoints)
{
	return true;
}

void UStalkerComponent::Broadcast_AddStalkPointsToPlayer_Multicast_Implementation(ADBDPlayer* Player, float stalkPoints)
{

}

bool UStalkerComponent::Broadcast_AddStalkPointsToPlayer_Multicast_Validate(ADBDPlayer* Player, float stalkPoints)
{
	return true;
}

UStalkerComponent::UStalkerComponent()
{

}

#include "Firecracker.h"
#include "EGameplayModifierType.h"
#include "EGameplayModifierFlag.h"

class ADBDPlayer;
class ACollectable;
class UGameplayModifierContainer;
class UPrimitiveComponent;

bool AFirecracker::ShouldDeafen(const ADBDPlayer* Player, float& outDuration) const
{
	return false;
}

bool AFirecracker::ShouldBlind(const ADBDPlayer* Player, float& outDuration) const
{
	return false;
}

void AFirecracker::OnEffectsInitialized_Implementation()
{

}

void AFirecracker::Multicast_InitFromSpawner_Implementation(ACollectable* spawner)
{

}

bool AFirecracker::HasModifierOfType(EGameplayModifierType GameplayModifierType) const
{
	return false;
}

bool AFirecracker::HasFlag(EGameplayModifierFlag GameplayModifierFlag) const
{
	return false;
}

ADBDPlayer* AFirecracker::GetPlayerOwner() const
{
	return NULL;
}

float AFirecracker::GetModifierValue(EGameplayModifierType GameplayModifierType) const
{
	return 0.0f;
}

float AFirecracker::GetExplosionRange() const
{
	return 0.0f;
}

float AFirecracker::GetExplosionEffectDuration() const
{
	return 0.0f;
}

float AFirecracker::GetExplosionDelay() const
{
	return 0.0f;
}

bool AFirecracker::GetExploded() const
{
	return false;
}

TArray<UGameplayModifierContainer*> AFirecracker::GetEffects() const
{
	return TArray<UGameplayModifierContainer*>();
}

UPrimitiveComponent* AFirecracker::GetEffectArea_Implementation() const
{
	return NULL;
}

float AFirecracker::GetDeafnessEffectDuration(const ADBDPlayer* Player) const
{
	return 0.0f;
}

float AFirecracker::GetBlindnessEffectDuration(const ADBDPlayer* Player) const
{
	return 0.0f;
}

AFirecracker::AFirecracker()
{
	this->DebugDisplayEnabled = false;
	this->_exploded = false;
}

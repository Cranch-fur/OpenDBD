#include "CameraHandlerComponent.h"
#include "EDBDCameraSocketID.h"
#include "Net/UnrealNetwork.h"

class UParticleSystemComponent;
class AActor;

void UCameraHandlerComponent::SetSocketID(EDBDCameraSocketID mode)
{

}

void UCameraHandlerComponent::PlayVFXOnCurrentCamera(UParticleSystemComponent* Component)
{

}

EDBDCameraSocketID UCameraHandlerComponent::GetSocketID()
{
	return EDBDCameraSocketID::VE_None;
}

AActor* UCameraHandlerComponent::GetCurrentCamera()
{
	return NULL;
}

void UCameraHandlerComponent::AddCameraToSocket(EDBDCameraSocketID mode, AActor* Camera)
{

}

void UCameraHandlerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCameraHandlerComponent, _delayedSocketID);
}

UCameraHandlerComponent::UCameraHandlerComponent()
{
	this->_delayedSocketID = EDBDCameraSocketID::VE_Default;
	this->_currentSocketID = EDBDCameraSocketID::VE_Default;
}

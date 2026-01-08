#include "DBDPlayerController.h"
#include "Net/UnrealNetwork.h"

class ADBDPlayerState;
class ADBDPlayer;

void ADBDPlayerController::Server_SetSpectatorModeInEditor_Implementation(bool On, ADBDPlayerState* spectatingPlayerState)
{

}

bool ADBDPlayerController::Server_SetSpectatorModeInEditor_Validate(bool On, ADBDPlayerState* spectatingPlayerState)
{
	return true;
}

void ADBDPlayerController::Server_SetSpectatorMode_Implementation(bool On)
{

}

bool ADBDPlayerController::Server_SetSpectatorMode_Validate(bool On)
{
	return true;
}

void ADBDPlayerController::Server_SetReadyToTravel_Implementation()
{

}

bool ADBDPlayerController::Server_SetReadyToTravel_Validate()
{
	return true;
}

void ADBDPlayerController::Server_LeaveGame_Implementation()
{

}

bool ADBDPlayerController::Server_LeaveGame_Validate()
{
	return true;
}

void ADBDPlayerController::Server_EndGame_Implementation(bool serverHasLeftTheGame)
{

}

bool ADBDPlayerController::Server_EndGame_Validate(bool serverHasLeftTheGame)
{
	return true;
}

void ADBDPlayerController::Server_CompleteEscapeRequirements_Implementation()
{

}

bool ADBDPlayerController::Server_CompleteEscapeRequirements_Validate()
{
	return true;
}

void ADBDPlayerController::OnRep_Fence()
{

}

bool ADBDPlayerController::IsSpectating() const
{
	return false;
}

ADBDPlayerState* ADBDPlayerController::GetSpectatedPlayerState() const
{
	return NULL;
}

ADBDPlayer* ADBDPlayerController::GetSpectatedPlayer() const
{
	return NULL;
}

ADBDPlayer* ADBDPlayerController::GetControlledPlayer() const
{
	return NULL;
}

void ADBDPlayerController::Client_SetSpectatorModeInEditor_Implementation(bool On, ADBDPlayerState* spectatingPlayerState)
{

}

void ADBDPlayerController::Client_SetSpectatorMode_Implementation(bool On)
{

}

void ADBDPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADBDPlayerController, _theFence);
}

ADBDPlayerController::ADBDPlayerController()
{

}

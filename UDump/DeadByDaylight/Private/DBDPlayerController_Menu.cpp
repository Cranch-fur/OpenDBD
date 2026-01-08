#include "DBDPlayerController_Menu.h"
#include "PlayerLoadoutData.h"
#include "PlayerDataSync.h"

void ADBDPlayerController_Menu::TogglePlayerReadyState()
{

}

void ADBDPlayerController_Menu::SetPlayerReady(bool IsReady)
{

}

void ADBDPlayerController_Menu::Server_SetReadyToTravel_Implementation()
{

}

bool ADBDPlayerController_Menu::Server_SetReadyToTravel_Validate()
{
	return true;
}

void ADBDPlayerController_Menu::Server_SetPlayerReady_Implementation(bool IsReady)
{

}

bool ADBDPlayerController_Menu::Server_SetPlayerReady_Validate(bool IsReady)
{
	return true;
}

void ADBDPlayerController_Menu::Server_SetPlayerLoadout_Implementation(FPlayerLoadoutData desiredLoadout)
{

}

bool ADBDPlayerController_Menu::Server_SetPlayerLoadout_Validate(FPlayerLoadoutData desiredLoadout)
{
	return true;
}

void ADBDPlayerController_Menu::Server_SetEquipedPerks_Implementation(const TArray<FName>& perkIds, const TArray<int32>& perkLevels, bool callOnRep)
{

}

bool ADBDPlayerController_Menu::Server_SetEquipedPerks_Validate(const TArray<FName>& perkIds, const TArray<int32>& perkLevels, bool callOnRep)
{
	return true;
}

void ADBDPlayerController_Menu::Server_SetCustomizationMesh_Implementation(FName ItemId)
{

}

bool ADBDPlayerController_Menu::Server_SetCustomizationMesh_Validate(FName ItemId)
{
	return true;
}

void ADBDPlayerController_Menu::Server_SetCharacterLevel_Implementation(int32 CharacterLevel, int32 PrestigeLevel, bool callOnRep)
{

}

bool ADBDPlayerController_Menu::Server_SetCharacterLevel_Validate(int32 CharacterLevel, int32 PrestigeLevel, bool callOnRep)
{
	return true;
}

void ADBDPlayerController_Menu::Server_SetCharacterData_Implementation(int32 CharacterIndex, const FPlayerDataSync& PlayerDataSync)
{

}

bool ADBDPlayerController_Menu::Server_SetCharacterData_Validate(int32 CharacterIndex, const FPlayerDataSync& PlayerDataSync)
{
	return true;
}

ADBDPlayerController_Menu::ADBDPlayerController_Menu()
{

}

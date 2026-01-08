#include "DBDPlayerControllerBase.h"
#include "PlayerSavedProfileDataShared.h"
#include "PlayerLoadoutData.h"
#include "EDBDScoreTypes.h"
#include "FearMarketOfferingInstance.h"
#include "BadgeAnalyticEntry.h"

void ADBDPlayerControllerBase::Server_UpdateReplicatedSkulls_Implementation(int32 skullsToAdd)
{

}

bool ADBDPlayerControllerBase::Server_UpdateReplicatedSkulls_Validate(int32 skullsToAdd)
{
	return true;
}

void ADBDPlayerControllerBase::Server_SendMessage_Implementation(int32 PlayerId, const FString& PlayerName, const FString& msg)
{

}

bool ADBDPlayerControllerBase::Server_SendMessage_Validate(int32 PlayerId, const FString& PlayerName, const FString& msg)
{
	return true;
}

void ADBDPlayerControllerBase::Server_SendClientAuthentication_Implementation(const FString& authClientTicket, uint64 SteamId, uint32 ticketLength)
{

}

bool ADBDPlayerControllerBase::Server_SendClientAuthentication_Validate(const FString& authClientTicket, uint64 SteamId, uint32 ticketLength)
{
	return true;
}

void ADBDPlayerControllerBase::Server_SelectSlasher_Implementation(int32 slasherIndex)
{

}

bool ADBDPlayerControllerBase::Server_SelectSlasher_Validate(int32 slasherIndex)
{
	return true;
}

void ADBDPlayerControllerBase::Server_SelectCamper_Implementation(int32 camperIndex)
{

}

bool ADBDPlayerControllerBase::Server_SelectCamper_Validate(int32 camperIndex)
{
	return true;
}

void ADBDPlayerControllerBase::Server_ReceivePlayerProfile_Implementation(const FString& MirrorsId, FPlayerSavedProfileDataShared savedProfileData, FPlayerLoadoutData playerLoadout)
{

}

bool ADBDPlayerControllerBase::Server_ReceivePlayerProfile_Validate(const FString& MirrorsId, FPlayerSavedProfileDataShared savedProfileData, FPlayerLoadoutData playerLoadout)
{
	return true;
}

void ADBDPlayerControllerBase::Server_EACMessageFromClient_Implementation(const TArray<uint8>& message, uint32 messageLength)
{

}

bool ADBDPlayerControllerBase::Server_EACMessageFromClient_Validate(const TArray<uint8>& message, uint32 messageLength)
{
	return true;
}

void ADBDPlayerControllerBase::Server_CheatFireScoreEvent_Implementation(EDBDScoreTypes Category, float Value)
{

}

bool ADBDPlayerControllerBase::Server_CheatFireScoreEvent_Validate(EDBDScoreTypes Category, float Value)
{
	return true;
}

void ADBDPlayerControllerBase::Server_BuildProceduralLevelFromSeed_Implementation(int32 Seed)
{

}

bool ADBDPlayerControllerBase::Server_BuildProceduralLevelFromSeed_Validate(int32 Seed)
{
	return true;
}

void ADBDPlayerControllerBase::Server_BuildProceduralLevelFromMap_Implementation(const FString& Map)
{

}

bool ADBDPlayerControllerBase::Server_BuildProceduralLevelFromMap_Validate(const FString& Map)
{
	return true;
}

void ADBDPlayerControllerBase::Server(const FString& commandLine)
{

}

void ADBDPlayerControllerBase::ReceivedFearMarketData(bool success, FFearMarketOfferingInstance fearMarketData)
{

}

void ADBDPlayerControllerBase::OnRep_PlayerState()
{

}

void ADBDPlayerControllerBase::DBD_UpdateLocalStats()
{

}

void ADBDPlayerControllerBase::DBD_SetFearMarketDate(const FString& dateString)
{

}

void ADBDPlayerControllerBase::DBD_SetCurrentCharacterPrestigeDatesPast()
{

}

void ADBDPlayerControllerBase::DBD_SetCurrentCharacterPrestigeDatesNow()
{

}

void ADBDPlayerControllerBase::DBD_SetCurrentCharacterPrestigeDatesFuture()
{

}

void ADBDPlayerControllerBase::DBD_FireTestScore(int32 Category, float Value)
{

}

void ADBDPlayerControllerBase::DBD_FillScoreCategory(int32 Category)
{

}

void ADBDPlayerControllerBase::DBD_FillAllScoreCategories()
{

}

void ADBDPlayerControllerBase::DBD_DebugSendChatMessage(const FString& msg)
{

}

void ADBDPlayerControllerBase::DBD_BuildFromSeed(int32 Seed)
{

}

void ADBDPlayerControllerBase::DBD_BuildFromMap(const FString& Map)
{

}

void ADBDPlayerControllerBase::Client_ValidateServer_Implementation()
{

}

void ADBDPlayerControllerBase::Client_SendServerAuthentication_Implementation(const FString& authServerTicket, uint64 SteamId, uint32 ticketLength)
{

}

bool ADBDPlayerControllerBase::Client_SendServerAuthentication_Validate(const FString& authServerTicket, uint64 SteamId, uint32 ticketLength)
{
	return true;
}

void ADBDPlayerControllerBase::Client_RequestPlayerProfile_Implementation(bool fromOfflineLobby)
{

}

void ADBDPlayerControllerBase::Client_GameEnded_Implementation()
{

}

void ADBDPlayerControllerBase::Client_FinishedPlaying_Implementation(const TArray<FBadgeAnalyticEntry>& badgeData)
{

}

void ADBDPlayerControllerBase::Client_EACMessageFromServer_Implementation(const TArray<uint8>& message, uint32 messageLength)
{

}

void ADBDPlayerControllerBase::CallConsoleCmdOnServer_Server_Implementation(const FString& commandLine)
{

}

bool ADBDPlayerControllerBase::CallConsoleCmdOnServer_Server_Validate(const FString& commandLine)
{
	return true;
}

ADBDPlayerControllerBase::ADBDPlayerControllerBase()
{

}

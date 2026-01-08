#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerSavedProfileDataShared.h"
#include "PlayerLoadoutData.h"
#include "EDBDScoreTypes.h"
#include "FearMarketOfferingInstance.h"
#include "BadgeAnalyticEntry.h"
#include "DBDPlayerControllerBase.generated.h"

class UFearMarketManager;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ADBDPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	UFearMarketManager* FearMarketManager;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_UpdateReplicatedSkulls(int32 skullsToAdd);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SendMessage(int32 PlayerId, const FString& PlayerName, const FString& msg);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendClientAuthentication(const FString& authClientTicket, uint64 SteamId, uint32 ticketLength);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SelectSlasher(int32 slasherIndex);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SelectCamper(int32 camperIndex);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_ReceivePlayerProfile(const FString& MirrorsId, FPlayerSavedProfileDataShared savedProfileData, FPlayerLoadoutData playerLoadout);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EACMessageFromClient(const TArray<uint8>& message, uint32 messageLength);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_CheatFireScoreEvent(EDBDScoreTypes Category, float Value);

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_BuildProceduralLevelFromSeed(int32 Seed);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_BuildProceduralLevelFromMap(const FString& Map);

	UFUNCTION(BlueprintCallable, Exec)
	void Server(const FString& commandLine);

private:
	UFUNCTION(BlueprintCallable)
	void ReceivedFearMarketData(bool success, FFearMarketOfferingInstance fearMarketData);

public:
	UFUNCTION(BlueprintCallable)
	void OnRep_PlayerState();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_UpdateLocalStats();

private:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetFearMarketDate(const FString& dateString);

public:
	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetCurrentCharacterPrestigeDatesPast();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetCurrentCharacterPrestigeDatesNow();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_SetCurrentCharacterPrestigeDatesFuture();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_FireTestScore(int32 Category, float Value);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_FillScoreCategory(int32 Category);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_FillAllScoreCategories();

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_DebugSendChatMessage(const FString& msg);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_BuildFromSeed(int32 Seed);

	UFUNCTION(BlueprintCallable, Exec)
	void DBD_BuildFromMap(const FString& Map);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_ValidateServer();

private:
	UFUNCTION(Client, Reliable, WithValidation)
	void Client_SendServerAuthentication(const FString& authServerTicket, uint64 SteamId, uint32 ticketLength);

public:
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_RequestPlayerProfile(bool fromOfflineLobby);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_GameEnded();

private:
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_FinishedPlaying(const TArray<FBadgeAnalyticEntry>& badgeData);

public:
	UFUNCTION(Client, Reliable)
	void Client_EACMessageFromServer(const TArray<uint8>& message, uint32 messageLength);

private:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void CallConsoleCmdOnServer_Server(const FString& commandLine);

public:
	ADBDPlayerControllerBase();
};

FORCEINLINE uint32 GetTypeHash(const ADBDPlayerControllerBase) { return 0; }

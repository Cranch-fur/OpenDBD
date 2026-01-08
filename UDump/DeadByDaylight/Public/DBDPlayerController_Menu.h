#pragma once

#include "CoreMinimal.h"
#include "DBDPlayerControllerBase.h"
#include "PlayerLoadoutData.h"
#include "PlayerDataSync.h"
#include "DBDPlayerController_Menu.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ADBDPlayerController_Menu : public ADBDPlayerControllerBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TogglePlayerReadyState();

	UFUNCTION(BlueprintCallable)
	void SetPlayerReady(bool IsReady);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetReadyToTravel();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetPlayerReady(bool IsReady);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetPlayerLoadout(FPlayerLoadoutData desiredLoadout);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetEquipedPerks(const TArray<FName>& perkIds, const TArray<int32>& perkLevels, bool callOnRep);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetCustomizationMesh(FName ItemId);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetCharacterLevel(int32 CharacterLevel, int32 PrestigeLevel, bool callOnRep);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetCharacterData(int32 CharacterIndex, const FPlayerDataSync& PlayerDataSync);

public:
	ADBDPlayerController_Menu();
};

FORCEINLINE uint32 GetTypeHash(const ADBDPlayerController_Menu) { return 0; }

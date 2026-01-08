#pragma once

#include "CoreMinimal.h"
#include "DBDPlayerState.h"
#include "DBDPlayerState_Menu.generated.h"

class ADBDMenuPlayer;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ADBDPlayerState_Menu : public ADBDPlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<ADBDMenuPlayer> _pawn;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_SetEquipment(int32 SlotIndex, FName ItemId, bool callOnRep);

	UFUNCTION(BlueprintCallable)
	void OnRep_DisplayData();

public:
	ADBDPlayerState_Menu();
};

FORCEINLINE uint32 GetTypeHash(const ADBDPlayerState_Menu) { return 0; }

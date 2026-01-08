#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OfferingSequenceManager.generated.h"

class UDBDGameInstance;
class UOfferingHandler;
class AOfferingSequenceAssets;
class ADBDLobbyHud;
class ACameraActor;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AOfferingSequenceManager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDGameInstance* _gameInstance;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UOfferingHandler* _offeringHandler;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	AOfferingSequenceAssets* _sequenceAssets;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ADBDLobbyHud* _hud;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ACameraActor* _offeringCamera;

private:
	UFUNCTION(BlueprintCallable)
	void FadeInScreen();

public:
	AOfferingSequenceManager();
};

FORCEINLINE uint32 GetTypeHash(const AOfferingSequenceManager) { return 0; }

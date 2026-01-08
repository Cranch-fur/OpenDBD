#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SeasonManager.generated.h"

class UDBDGameInstance;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ASeasonManager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDGameInstance* _gameInstance;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetOldSlasherSkulls() const;

	UFUNCTION(BlueprintCallable)
	int32 GetOldCamperSkulls() const;

public:
	ASeasonManager();
};

FORCEINLINE uint32 GetTypeHash(const ASeasonManager) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "LobbyLevel.h"
#include "OfflineLobbyLevel.generated.h"

class APlayerStart;
class AActor;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AOfflineLobbyLevel : public ALobbyLevel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<APlayerStart>> _roleSelectionCamperSpawns;

	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<APlayerStart>> _roleSelectionSlasherSpawns;

protected:
	UFUNCTION(BlueprintCallable)
	void CheckDestroyingPawns(AActor* DestroyedActor);

public:
	AOfflineLobbyLevel();
};

FORCEINLINE uint32 GetTypeHash(const AOfflineLobbyLevel) { return 0; }

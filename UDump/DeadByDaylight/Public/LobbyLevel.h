#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "UObject/NoExportTypes.h"
#include "LobbyLevel.generated.h"

class APlayerStart;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ALobbyLevel : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<APlayerStart> _localCamperStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<APlayerStart> _localSlasherStart;

	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<APlayerStart>> _camperSpawns;

	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<APlayerStart>> _slasherSpawns;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<APlayerStart*> _registeredSpawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> _slasherPOVTrackerOffsets;

public:
	ALobbyLevel();
};

FORCEINLINE uint32 GetTypeHash(const ALobbyLevel) { return 0; }

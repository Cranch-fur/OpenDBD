#pragma once

#include "CoreMinimal.h"
#include "DBDGameMode.h"
#include "Templates/SubclassOf.h"
#include "DBDDebugGameMode.generated.h"

class ACamperPlayer;
class ASlasherPlayer;

UCLASS(Blueprintable, NonTransient)
class DEADBYDAYLIGHT_API ADBDDebugGameMode : public ADBDGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ACamperPlayer>> CamperPawnOverrides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ASlasherPlayer>> SlasherPawnOverrides;

public:
	ADBDDebugGameMode();
};

FORCEINLINE uint32 GetTypeHash(const ADBDDebugGameMode) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DBDParadiseGameMode.generated.h"

UCLASS(Blueprintable, NonTransient)
class ADBDParadiseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADBDParadiseGameMode();
};

FORCEINLINE uint32 GetTypeHash(const ADBDParadiseGameMode) { return 0; }

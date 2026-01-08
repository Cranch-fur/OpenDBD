#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DBDGame_Start.generated.h"

UCLASS(Blueprintable, NonTransient)
class DEADBYDAYLIGHT_API ADBDGame_Start : public AGameMode
{
	GENERATED_BODY()

public:
	ADBDGame_Start();
};

FORCEINLINE uint32 GetTypeHash(const ADBDGame_Start) { return 0; }

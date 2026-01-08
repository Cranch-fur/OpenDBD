#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "DBDPlayerStart.generated.h"

UCLASS(Blueprintable)
class ADBDPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	ADBDPlayerStart();
};

FORCEINLINE uint32 GetTypeHash(const ADBDPlayerStart) { return 0; }

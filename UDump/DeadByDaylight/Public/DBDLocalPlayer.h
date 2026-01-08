#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "DBDLocalPlayer.generated.h"

UCLASS(Blueprintable, NonTransient)
class UDBDLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	UDBDLocalPlayer();
};

FORCEINLINE uint32 GetTypeHash(const UDBDLocalPlayer) { return 0; }

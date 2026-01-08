#pragma once

#include "CoreMinimal.h"
#include "DBDBaseHud.h"
#include "DBDInGameHud.generated.h"

class UFont;

UCLASS(Blueprintable, NonTransient)
class ADBDInGameHud : public ADBDBaseHud
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFont* _proceduralDebugFontObj;

public:
	ADBDInGameHud();
};

FORCEINLINE uint32 GetTypeHash(const ADBDInGameHud) { return 0; }

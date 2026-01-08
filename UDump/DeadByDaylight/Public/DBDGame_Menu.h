#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DBDGame_Menu.generated.h"

UCLASS(Blueprintable, NonTransient)
class ADBDGame_Menu : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StartWithSlasher;

public:
	ADBDGame_Menu();
};

FORCEINLINE uint32 GetTypeHash(const ADBDGame_Menu) { return 0; }

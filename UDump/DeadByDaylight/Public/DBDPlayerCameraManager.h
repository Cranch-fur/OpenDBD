#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DBDPlayerCameraManager.generated.h"

UCLASS(Blueprintable, NonTransient)
class ADBDPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetingFOV;

public:
	ADBDPlayerCameraManager();
};

FORCEINLINE uint32 GetTypeHash(const ADBDPlayerCameraManager) { return 0; }

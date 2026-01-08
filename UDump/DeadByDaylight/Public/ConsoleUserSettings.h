#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerInput.h"
#include "ConsoleUserSettings.generated.h"

USTRUCT(BlueprintType)
struct FConsoleUserSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInputActionKeyMapping> ActionMappings;

	UPROPERTY(EditAnywhere)
	TArray<FInputAxisKeyMapping> AxisMappings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MainVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MenuMusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseHeadphones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MuteMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MuteOnFocusLost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KillerControllerSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SurvivorControllerSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InvertY;

public:
	DEADBYDAYLIGHT_API FConsoleUserSettings();
};

FORCEINLINE uint32 GetTypeHash(const FConsoleUserSettings) { return 0; }

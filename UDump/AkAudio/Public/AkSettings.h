#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FILE_NOT_FOUND"
#include "AkSettings.generated.h"

UCLASS(Blueprintable)
class AKAUDIO_API UAkSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxSimultaneousReverbVolumes;

	UPROPERTY(EditAnywhere)
	uint32 CommPoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilePath WwiseProjectPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDirectoryPath WwiseWindowsInstallationPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilePath WwiseMacInstallationPath;

public:
	UAkSettings();
};

FORCEINLINE uint32 GetTypeHash(const UAkSettings) { return 0; }

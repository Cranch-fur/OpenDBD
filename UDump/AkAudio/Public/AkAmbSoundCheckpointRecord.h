#pragma once

#include "CoreMinimal.h"
#include "AkAmbSoundCheckpointRecord.generated.h"

USTRUCT(BlueprintType)
struct FAkAmbSoundCheckpointRecord
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCurrentlyPlaying;

public:
	AKAUDIO_API FAkAmbSoundCheckpointRecord();
};

FORCEINLINE uint32 GetTypeHash(const FAkAmbSoundCheckpointRecord) { return 0; }

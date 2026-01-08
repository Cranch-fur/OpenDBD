#pragma once

#include "CoreMinimal.h"
#include "AkAudioEventTrackKey.generated.h"

class UAkAudioEvent;

USTRUCT(BlueprintType)
struct FAkAudioEventTrackKey
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAkAudioEvent* AkAudioEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventName;

public:
	AKAUDIO_API FAkAudioEventTrackKey();
};

FORCEINLINE uint32 GetTypeHash(const FAkAudioEventTrackKey) { return 0; }

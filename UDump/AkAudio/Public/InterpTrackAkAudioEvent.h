#pragma once

#include "CoreMinimal.h"
#include "FILE_NOT_FOUND"
#include "AkAudioEventTrackKey.h"
#include "InterpTrackAkAudioEvent.generated.h"

UCLASS(Blueprintable)
class AKAUDIO_API UInterpTrackAkAudioEvent : public UInterpTrackVectorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAkAudioEventTrackKey> Events;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bContinueEventOnMatineeEnd : 1;

public:
	UInterpTrackAkAudioEvent();
};

FORCEINLINE uint32 GetTypeHash(const UInterpTrackAkAudioEvent) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "FILE_NOT_FOUND"
#include "InterpTrackAkAudioRTPC.generated.h"

UCLASS(Blueprintable, MinimalAPI)
class UInterpTrackAkAudioRTPC : public UInterpTrackFloatBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Param;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bPlayOnReverse : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bContinueRTPCOnMatineeEnd : 1;

public:
	UInterpTrackAkAudioRTPC();
};

FORCEINLINE uint32 GetTypeHash(const UInterpTrackAkAudioRTPC) { return 0; }

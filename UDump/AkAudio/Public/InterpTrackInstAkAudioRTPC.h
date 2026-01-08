#pragma once

#include "CoreMinimal.h"
#include "FILE_NOT_FOUND"
#include "InterpTrackInstAkAudioRTPC.generated.h"

UCLASS(Blueprintable)
class UInterpTrackInstAkAudioRTPC : public UInterpTrackInst
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LastUpdatePosition;

public:
	UInterpTrackInstAkAudioRTPC();
};

FORCEINLINE uint32 GetTypeHash(const UInterpTrackInstAkAudioRTPC) { return 0; }

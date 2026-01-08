#pragma once

#include "CoreMinimal.h"
#include "FILE_NOT_FOUND"
#include "InterpTrackInstAkAudioEvent.generated.h"

UCLASS(Blueprintable, MinimalAPI)
class UInterpTrackInstAkAudioEvent : public UInterpTrackInst
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LastUpdatePosition;

public:
	UInterpTrackInstAkAudioEvent();
};

FORCEINLINE uint32 GetTypeHash(const UInterpTrackInstAkAudioEvent) { return 0; }

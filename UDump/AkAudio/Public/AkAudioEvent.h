#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AkAudioEvent.generated.h"

class UAkAudioBank;

UCLASS(Blueprintable)
class AKAUDIO_API UAkAudioEvent : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAkAudioBank* RequiredBank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAttenuationRadius;

public:
	UAkAudioEvent();
};

FORCEINLINE uint32 GetTypeHash(const UAkAudioEvent) { return 0; }

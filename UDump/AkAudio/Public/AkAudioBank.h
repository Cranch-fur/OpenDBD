#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AkAudioBank.generated.h"

UCLASS(Blueprintable)
class AKAUDIO_API UAkAudioBank : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoLoad;

public:
	UAkAudioBank();
};

FORCEINLINE uint32 GetTypeHash(const UAkAudioBank) { return 0; }

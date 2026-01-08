#pragma once

#include "CoreMinimal.h"
#include "MapMeshToAkAudioEvent.generated.h"

class UStaticMesh;
class UAkAudioEvent;

USTRUCT(BlueprintType)
struct DEADBYDAYLIGHT_API FMapMeshToAkAudioEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAkAudioEvent* SoundEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDelayBetweenEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDelayBetweenObject;

public:
	FMapMeshToAkAudioEvent();
};

FORCEINLINE uint32 GetTypeHash(const FMapMeshToAkAudioEvent) { return 0; }

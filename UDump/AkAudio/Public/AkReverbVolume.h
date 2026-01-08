#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "AkReverbVolume.generated.h"

class UAkAuxBus;
class AAkReverbVolume;

UCLASS(Blueprintable)
class AKAUDIO_API AAkReverbVolume : public AVolume
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 bEnabled : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAkAuxBus* AuxBus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AuxBusName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SendLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Priority;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	AAkReverbVolume* NextLowerPriorityAkReverbVolume;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AAkReverbVolume();
};

FORCEINLINE uint32 GetTypeHash(const AAkReverbVolume) { return 0; }

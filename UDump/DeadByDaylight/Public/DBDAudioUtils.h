#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DBDAudioUtils.generated.h"

class UAkAudioBank;

UCLASS(Blueprintable)
class ADBDAudioUtils : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void DBDLoadBank(UAkAudioBank* bank);

public:
	ADBDAudioUtils();
};

FORCEINLINE uint32 GetTypeHash(const ADBDAudioUtils) { return 0; }

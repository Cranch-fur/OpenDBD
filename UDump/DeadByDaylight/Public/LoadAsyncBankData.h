#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LoadAsyncBankData.generated.h"

class UAkAudioBank;
class UDBDGameInstance;

UCLASS(Blueprintable)
class ULoadAsyncBankData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAkAudioBank* bank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDBDGameInstance* GameInstance;

public:
	ULoadAsyncBankData();
};

FORCEINLINE uint32 GetTypeHash(const ULoadAsyncBankData) { return 0; }

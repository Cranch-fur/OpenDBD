#pragma once

#include "CoreMinimal.h"
#include "CameraAttachment.generated.h"

class USceneComponent;

USTRUCT(BlueprintType)
struct FCameraAttachment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	USceneComponent* Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

public:
	DEADBYDAYLIGHT_API FCameraAttachment();
};

FORCEINLINE uint32 GetTypeHash(const FCameraAttachment) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "InteractionAnimNotify.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UInteractionAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NotifyId;

public:
	UInteractionAnimNotify();
};

FORCEINLINE uint32 GetTypeHash(const UInteractionAnimNotify) { return 0; }

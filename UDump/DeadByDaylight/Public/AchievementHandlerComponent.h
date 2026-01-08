#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EDBDScoreTypes.h"
#include "AchievementHandlerComponent.generated.h"

class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UAchievementHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ReceiveGameEvent(EDBDScoreTypes ScoreType, float amount, AActor* Instigator, AActor* target);

public:
	UAchievementHandlerComponent();
};

FORCEINLINE uint32 GetTypeHash(const UAchievementHandlerComponent) { return 0; }

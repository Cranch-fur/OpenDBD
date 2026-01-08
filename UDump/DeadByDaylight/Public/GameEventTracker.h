#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayEventDynamicDelegate.h"
#include "GameflowEventDynamicDelegate.h"
#include "EDBDScoreTypes.h"
#include "GameEventTracker.generated.h"

class ADBDPlayerState;
class AActor;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UGameEventTracker : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FGameplayEventDynamicDelegate OnGameplayEvent;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FGameflowEventDynamicDelegate OnGameflowEvent;

public:
	UFUNCTION(BlueprintCallable)
	void FireGameflowEvent(EDBDScoreTypes eventType, float amount, const ADBDPlayerState* Instigator);

	UFUNCTION(BlueprintCallable)
	void FireGameEvent(EDBDScoreTypes eventType, float amount, AActor* Instigator, AActor* target);

	UFUNCTION(BlueprintCallable)
	void FireAnalyticEvent(const FString& EventName);

public:
	UGameEventTracker();
};

FORCEINLINE uint32 GetTypeHash(const UGameEventTracker) { return 0; }

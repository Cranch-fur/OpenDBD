#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBDTimerComponent.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBDTimerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Update(float deltaTime);

	UFUNCTION(BlueprintCallable)
	void Stop();

	UFUNCTION(BlueprintCallable)
	void ResetTo(float Time);

	UFUNCTION(BlueprintCallable)
	void Reset();

	UFUNCTION(BlueprintPure)
	bool IsDone() const;

	UFUNCTION(BlueprintPure)
	float GetTimeLeft() const;

	UFUNCTION(BlueprintPure)
	float GetTimeElapsed() const;

	UFUNCTION(BlueprintPure)
	float GetStartTime() const;

	UFUNCTION(BlueprintPure)
	float GetPercentTimeLeft() const;

	UFUNCTION(BlueprintPure)
	float GetPercentTimeElapsed() const;

public:
	UDBDTimerComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBDTimerComponent) { return 0; }

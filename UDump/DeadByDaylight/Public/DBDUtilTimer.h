#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DBDTimer.h"
#include "DBDUtilTimer.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UDBDUtilTimer : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void Update(FDBDTimer& Timer, float deltaTime);

	UFUNCTION(BlueprintCallable)
	static void Stop(FDBDTimer& Timer);

	UFUNCTION(BlueprintCallable)
	static void ResetTo(FDBDTimer& Timer, float Time);

	UFUNCTION(BlueprintCallable)
	static void Reset(FDBDTimer& Timer);

	UFUNCTION(BlueprintPure)
	static bool IsDone(FDBDTimer& Timer);

	UFUNCTION(BlueprintPure)
	static float GetTimeLeft(FDBDTimer& Timer);

	UFUNCTION(BlueprintPure)
	static float GetTimeElapsed(FDBDTimer& Timer);

	UFUNCTION(BlueprintPure)
	static float GetStartTime(FDBDTimer& Timer);

	UFUNCTION(BlueprintPure)
	static float GetPercentTimeLeft(FDBDTimer& Timer);

	UFUNCTION(BlueprintPure)
	static float GetPercentTimeElapsed(FDBDTimer& Timer);

public:
	UDBDUtilTimer();
};

FORCEINLINE uint32 GetTypeHash(const UDBDUtilTimer) { return 0; }

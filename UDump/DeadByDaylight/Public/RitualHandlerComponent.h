#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/NoExportTypes.h"
#include "EDBDScoreTypes.h"
#include "RitualHandlerComponent.generated.h"

class URitualEvaluatorBase;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API URitualHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<FName, URitualEvaluatorBase*> _evaluators;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateRitualStatus();

	UFUNCTION(BlueprintCallable)
	void TryGenerateCachedRitualWithKey(FName Key);

	UFUNCTION(BlueprintCallable)
	void TryGenerateCachedRitualFromNetTime();

	UFUNCTION()
	void TryGenerateCachedRitual(FDateTime timeToUse, bool usedNetTime, bool forceOverwrite);

	UFUNCTION(BlueprintCallable)
	void ShowDailyRituals();

	UFUNCTION(BlueprintCallable)
	void ResetRituals();

	UFUNCTION(BlueprintCallable)
	void ReceiveGameEvent(EDBDScoreTypes ScoreType, float amount, AActor* Instigator, AActor* target);

private:
	UFUNCTION()
	void ReceivedRitualAssignedTime(FDateTime Now, bool usedNetTime);

public:
	UFUNCTION(BlueprintCallable)
	void DismissRitual(FName toRemove);

	UFUNCTION(BlueprintCallable)
	void CheatAddToRitual(int32 Index, float Value);

	UFUNCTION(BlueprintCallable)
	void AssignNewRitual();

public:
	URitualHandlerComponent();
};

FORCEINLINE uint32 GetTypeHash(const URitualHandlerComponent) { return 0; }

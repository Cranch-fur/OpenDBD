#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EDBDScoreTypes.h"
#include "DBDBadge.generated.h"

class AActor;
class UWorld;

UCLASS(Blueprintable)
class UDBDBadge : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BadgeId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ForwardGameEvents;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _rewarded;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGameplayEvent(EDBDScoreTypes gameplayEventType, float amount, AActor* Instigator, AActor* target);

public:
	UFUNCTION(BlueprintPure)
	bool IsOwnerPlayer(AActor* Actor) const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InitializeBadge();

public:
	UFUNCTION(BlueprintPure)
	UWorld* GetWorld() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	float GetProgress() const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool EvaluateShouldReward() const;

public:
	UDBDBadge();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadge) { return 0; }

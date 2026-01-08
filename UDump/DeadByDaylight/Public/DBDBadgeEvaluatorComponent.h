#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EDBDScoreTypes.h"
#include "DBDBadgeEvaluatorComponent.generated.h"

class UDBDBadge;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UDBDBadgeEvaluatorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<UDBDBadge*> _badges;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FName> _rewardedBadgeIDs;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<EDBDScoreTypes, float> _badgeGameEvents;

private:
	UFUNCTION(BlueprintCallable)
	void OnGameplayEvent(EDBDScoreTypes ScoreType, float amount, AActor* Instigator, AActor* target);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_RewardBadge(const FName& BadgeId);

public:
	UDBDBadgeEvaluatorComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBDBadgeEvaluatorComponent) { return 0; }

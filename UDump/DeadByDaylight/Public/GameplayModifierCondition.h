#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayModifierCondition.generated.h"

class UGameplayModifierContainer;
class ADBDPlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UGameplayModifierCondition : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UGameplayModifierContainer* _owningGameplayModifier;

public:
	UFUNCTION(BlueprintCallable)
	void SetOwningGameplayModifier(UGameplayModifierContainer* OwningGameplayModifier);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool IsApplicable();

	UFUNCTION(BlueprintPure)
	ADBDPlayer* GetOwningPlayer() const;

	UFUNCTION(BlueprintPure)
	UGameplayModifierContainer* GetOwningGameplayModifier() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UGameplayModifierCondition();
};

FORCEINLINE uint32 GetTypeHash(const UGameplayModifierCondition) { return 0; }

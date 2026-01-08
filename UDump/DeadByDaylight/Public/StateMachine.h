#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UStateMachine : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, ReplicatedUsing=NetIDStackChanged)
	TArray<uint16> _netStateIDStackInitial;

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetStateStack(const TArray<uint16>& netStateIDStack);

	UFUNCTION(BlueprintCallable)
	void NetIDStackChanged();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_SetStateStack(const TArray<uint16>& netStateIDStack);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UStateMachine();
};

FORCEINLINE uint32 GetTypeHash(const UStateMachine) { return 0; }

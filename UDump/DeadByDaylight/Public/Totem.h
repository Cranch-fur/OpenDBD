#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Totem.generated.h"

UCLASS(Blueprintable)
class ATotem : public AInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	FName hexPerkID;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient)
	bool IsCleansed;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnBoundToHex();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_BindToHex(FName hexPerkIDNew);

	UFUNCTION(BlueprintPure)
	bool HasHex() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ATotem();
};

FORCEINLINE uint32 GetTypeHash(const ATotem) { return 0; }

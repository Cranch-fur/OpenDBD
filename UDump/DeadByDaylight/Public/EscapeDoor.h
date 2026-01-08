#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "UObject/NoExportTypes.h"
#include "EscapeDoor.generated.h"

class UObject;

UCLASS(Blueprintable)
class AEscapeDoor : public AInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_Activated)
	bool activated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> EscapePositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UObject*> EscapeFire;

private:
	UFUNCTION(BlueprintCallable)
	void OnRep_Activated(bool oldActivated);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnExitActivationChanged(bool activatedNew);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	AEscapeDoor();
};

FORCEINLINE uint32 GetTypeHash(const AEscapeDoor) { return 0; }

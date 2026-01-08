#pragma once

#include "CoreMinimal.h"
#include "DBDPlayerData.h"
#include "ECamperDamageState.h"
#include "ECamperGuidedAction.h"
#include "DBDCamperData.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UDBDCamperData : public UDBDPlayerData
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=DamageStateChanged, meta=(AllowPrivateAccess=true))
	ECamperDamageState DamageState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	ECamperGuidedAction CurrentGuidedAction;

private:
	UFUNCTION(BlueprintCallable)
	void DamageStateChanged();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UDBDCamperData();
};

FORCEINLINE uint32 GetTypeHash(const UDBDCamperData) { return 0; }

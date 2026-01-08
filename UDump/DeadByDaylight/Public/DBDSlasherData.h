#pragma once

#include "CoreMinimal.h"
#include "DBDPlayerData.h"
#include "ESlasherGuidedAction.h"
#include "DBDSlasherData.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UDBDSlasherData : public UDBDPlayerData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool VisionEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool CloakingEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlasherGuidedAction CurrentGuidedAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SoundDisplayLifetime;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UDBDSlasherData();
};

FORCEINLINE uint32 GetTypeHash(const UDBDSlasherData) { return 0; }

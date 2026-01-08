#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BloodwebManager.generated.h"

class UDBDGameInstance;
class UDBDDesignTunables;
class UDBDBloodWebDefinition;
class UBloodwebGenerator;
class UBloodwebEntity;

UCLASS(Blueprintable)
class UBloodwebManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDGameInstance* _gameInstance;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDDesignTunables* _designTunables;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDBloodWebDefinition* _bloodWebDefinition;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UBloodwebGenerator* _bloodwebBuilder;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UBloodwebEntity* _entity;

public:
	UFUNCTION(BlueprintCallable)
	void Init(UDBDGameInstance* GameInstance, UDBDDesignTunables* tunables);

public:
	UBloodwebManager();
};

FORCEINLINE uint32 GetTypeHash(const UBloodwebManager) { return 0; }

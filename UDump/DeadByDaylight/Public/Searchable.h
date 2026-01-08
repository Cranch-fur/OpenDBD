#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "EItemRarity.h"
#include "Searchable.generated.h"

class USearchableSpawnPoint;
class ADBDPlayer;
class ACollectable;

UCLASS(Blueprintable)
class ASearchable : public AInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity ItemRarity;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	USearchableSpawnPoint* _searchableSpawnPoint;

public:
	UFUNCTION(BlueprintCallable)
	ACollectable* SpawnObject(ADBDPlayer* Player);

public:
	ASearchable();
};

FORCEINLINE uint32 GetTypeHash(const ASearchable) { return 0; }

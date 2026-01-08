#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaterialOriginalState.h"
#include "UObject/NoExportTypes.h"
#include "ClipManager.generated.h"

class UDBDClipRegionComponent;
class UMeshComponent;
class UMaterialInterface;
class UDataTable;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AClipManager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<UDBDClipRegionComponent*> _clippingZones;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Export, meta=(AllowPrivateAccess=true))
	TSet<UMeshComponent*> _meshComponents;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TArray<FMaterialOriginalState> _dissolvedMeshes;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<UMaterialInterface*, UMaterialInterface*> _normal2dissolve;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<UMaterialInterface*, UMaterialInterface*> _dissolve2normal;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDataTable* _sphericalDissolveMaterials;

public:
	UFUNCTION(BlueprintCallable)
	void SphericalDissolveAt(FVector Location, float Radius);

	UFUNCTION(BlueprintCallable)
	void RemoveDissolveMaterials();

public:
	AClipManager();
};

FORCEINLINE uint32 GetTypeHash(const AClipManager) { return 0; }

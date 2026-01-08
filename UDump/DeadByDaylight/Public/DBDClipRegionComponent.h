#pragma once

#include "CoreMinimal.h"
#include "DBDLocatorComponent.h"
#include "DBDClipRegionComponent.generated.h"

class UMaterialInterface;
class UMeshComponent;
class UMaterialInstanceDynamic;
class UDBDClipRegionComponent;

UCLASS(Blueprintable, Abstract, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBDClipRegionComponent : public UDBDLocatorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> AffectedMaterials;

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void NotifyShouldClipComponent(UMeshComponent* InMeshComponent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	float ClipDistance(UMeshComponent* InMeshComponent);

public:
	UFUNCTION(BlueprintCallable)
	void ApplyClipToDynamicMaterial(UMaterialInstanceDynamic* dynamicInstanceToClip, int32 InRegionID);

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ApplyClip(UMaterialInstanceDynamic* InMaterial2clip, int32 InRegionID);

public:
	UFUNCTION(BlueprintCallable)
	static void ApplyArrayOfClipsToDynamicMaterial(TArray<UDBDClipRegionComponent*> InRegions, UMaterialInstanceDynamic* dynamicInstanceToClip);

public:
	UDBDClipRegionComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBDClipRegionComponent) { return 0; }

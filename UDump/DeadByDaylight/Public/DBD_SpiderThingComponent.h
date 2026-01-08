#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Engine/HitResult.h"
#include "DBD_SpiderThingComponent.generated.h"

class UInstancedStaticMeshComponent;
class UDecalComponent;
class UMaterialInstanceDynamic;
class UStaticMesh;
class UCurveVector;
class UMaterial;
class UParticleSystem;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBD_SpiderThingComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UInstancedStaticMeshComponent* _instancedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UDecalComponent*> _arrayDecalsSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UDecalComponent*> _arrayDecalsEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* _dynamicDecalEndMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* _dynamicDecalSpawnMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* MeshLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* MeshMedium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* MeshHigh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaximumRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaximumStickLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThicknessY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThicknessZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThicknessYRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThicknessZRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxInstances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InitialSpawnAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngleRangeDegrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveVector* InitializationTimings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveVector* DestructionTimings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* DecalEndMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* DecalSpawnMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EDetailMode> DecalMinDetailMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ParticleSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecalInstanceScale;

public:
	UFUNCTION(BlueprintCallable)
	void RebuildLines();

protected:
	UFUNCTION(BlueprintCallable)
	void RandomTrace(FHitResult& out_result);

	UFUNCTION(BlueprintCallable)
	void CreateLine();

public:
	UDBD_SpiderThingComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBD_SpiderThingComponent) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/HitResult.h"
#include "DBD_SpiderThing.generated.h"

class UInstancedStaticMeshComponent;
class UDecalComponent;
class UMaterialInstanceDynamic;
class UStaticMesh;
class UCurveVector;
class UMaterial;
class UParticleSystem;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ADBD_SpiderThing : public AActor
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
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaximumRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThicknessY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThicknessZ;

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
	UParticleSystem* ParticleSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecalInstanceScale;

protected:
	UFUNCTION(BlueprintCallable)
	void RebuildLines();

	UFUNCTION(BlueprintCallable)
	void RandomTrace(FHitResult& out_result);

	UFUNCTION(BlueprintCallable)
	void CreateLine();

public:
	ADBD_SpiderThing();
};

FORCEINLINE uint32 GetTypeHash(const ADBD_SpiderThing) { return 0; }

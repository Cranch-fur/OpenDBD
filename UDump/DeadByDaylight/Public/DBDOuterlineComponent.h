#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DBDOuterlineComponent.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic;
class USkeletalMeshComponent;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBDOuterlineComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* CloneCustomDepthMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* CloneTranslucentMaterial;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UMaterialInstanceDynamic* _cloneCustomDepthMaterialDynamic;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UMaterialInstanceDynamic* _cloneTranslucentMaterialDynamic;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* _customDepthSkeletalMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* _overlaySkeletalMesh;

public:
	UFUNCTION(BlueprintCallable)
	void SetIntensity(float Intensity);

public:
	UDBDOuterlineComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBDOuterlineComponent) { return 0; }

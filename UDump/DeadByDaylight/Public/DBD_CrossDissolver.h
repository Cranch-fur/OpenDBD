#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBD_CrossDissolver.generated.h"

class UMaterialInterface;
class USkeletalMeshComponent;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBD_CrossDissolver : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> NewMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USkeletalMeshComponent*> BaseSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<USkeletalMeshComponent*> ClonedSkeletalMeshes;

public:
	UFUNCTION(BlueprintCallable)
	void MakeClone();

	UFUNCTION(BlueprintCallable)
	void KillClone();

public:
	UDBD_CrossDissolver();
};

FORCEINLINE uint32 GetTypeHash(const UDBD_CrossDissolver) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomizedSkeletalMesh.generated.h"

class UCustomizationDescription;
class USkeletalMeshComponent;
class AItemVfx;
class UCustomizedSkeletalMesh;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UCustomizedSkeletalMesh : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCustomizationDescription* CustomizationDescription;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<USkeletalMeshComponent*> Components;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<USkeletalMeshComponent*> Slaves;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<FName, AItemVfx*> _itemVfxs;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* _meshOwner;

public:
	UFUNCTION(BlueprintCallable)
	void SetVfxsVisibility(FName Category, bool Visible);

	UFUNCTION(BlueprintCallable)
	void SetMeshOwner(USkeletalMeshComponent* Mesh);

	UFUNCTION(BlueprintCallable)
	void SetAllVfxsVisibility(bool Visible);

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSkeletalMesh(const FName& Category);

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMeshOwner();

	UFUNCTION(BlueprintCallable)
	void CopyCustomization(const UCustomizedSkeletalMesh* copyFrom);

	UFUNCTION(BlueprintCallable)
	void ClearAllVfxs();

public:
	UCustomizedSkeletalMesh();
};

FORCEINLINE uint32 GetTypeHash(const UCustomizedSkeletalMesh) { return 0; }

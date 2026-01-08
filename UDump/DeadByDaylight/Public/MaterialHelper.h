#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MaterialNamedGroup.h"
#include "MaterialHelperOriginalMeshState.h"
#include "UObject/NoExportTypes.h"
#include "MaterialHelper.generated.h"

class UTexture;
class UMeshComponent;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UMaterialHelper : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMaterialNamedGroup> Groups;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<FMaterialHelperOriginalMeshState> TargetMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FString _materialGroupName;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateMaterials();

	UFUNCTION(BlueprintCallable)
	void SetVectorParameter(const FName ParameterName, const FVector& NewValue);

	UFUNCTION(BlueprintCallable)
	void SetTextureParameter(const FName ParameterName, UTexture* NewTexture);

	UFUNCTION(BlueprintCallable)
	void SetScalarParameter(const FName ParameterName, const float NewValue);

	UFUNCTION(BlueprintCallable)
	void SetRenderInCustomDepth(bool RenderInCustomDepth);

	UFUNCTION(BlueprintCallable)
	void SetRenderInCustomColourNoDepth(bool RenderInCustomColour);

	UFUNCTION(BlueprintCallable)
	void SetReceivesDecals(bool ReceivesDecals);

	UFUNCTION(BlueprintCallable)
	void SetMaterial(const FString& GroupName);

	UFUNCTION(BlueprintCallable)
	void SetColourParameter(const FName ParameterName, const FLinearColor NewValue);

	UFUNCTION(BlueprintCallable)
	void SetCastCinematicShadows(bool CastCinematicShadows);

	UFUNCTION(BlueprintCallable)
	void ResetMaterials();

	UFUNCTION(BlueprintCallable)
	void RefreshMeshes();

	UFUNCTION(BlueprintCallable)
	void RefreshMesh(UMeshComponent* mc);

public:
	UMaterialHelper();
};

FORCEINLINE uint32 GetTypeHash(const UMaterialHelper) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UObject/NoExportTypes.h"
#include "DBDOutlineComponent.generated.h"

class UMeshComponent;
class UMaterialHelper;
class UTexture;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBDOutlineComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	FLinearColor TargetColour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	float InterpolationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	bool ShouldBeAboveOutlines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp)
	bool LimitToCustomDepthObjects;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OutlineName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UMeshComponent*> TargetMeshes;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	UMaterialHelper* _materialHelper;

public:
	UFUNCTION(BlueprintCallable)
	void SetFadeTexture(UTexture* inFadeTexture);

	UFUNCTION(BlueprintCallable)
	void RefreshOutlineComponent();

public:
	UDBDOutlineComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBDOutlineComponent) { return 0; }

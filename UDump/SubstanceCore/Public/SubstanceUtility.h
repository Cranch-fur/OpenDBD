#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ESubstanceTextureSize.h"
#include "SubstanceUtility.generated.h"

class USubstanceGraphInstance;
class USubstanceTexture2D;
class UMaterialInterface;
class UObject;
class USubstanceInstanceFactory;

UCLASS(Blueprintable, MinimalAPI)
class USubstanceUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void SyncRendering(USubstanceGraphInstance* InstancesToRender);

	UFUNCTION(BlueprintCallable)
	static void SetGraphInstanceOutputSizeInt(USubstanceGraphInstance* GraphInstance, int32 Width, int32 Height);

	UFUNCTION(BlueprintCallable)
	static void SetGraphInstanceOutputSize(USubstanceGraphInstance* GraphInstance, ESubstanceTextureSize Width, ESubstanceTextureSize Height);

	UFUNCTION(BlueprintCallable)
	static void ResetInputParameters(USubstanceGraphInstance* GraphInstance);

	UFUNCTION(BlueprintCallable)
	static TArray<USubstanceTexture2D*> GetSubstanceTextures(USubstanceGraphInstance* GraphInstance);

	UFUNCTION(BlueprintCallable)
	static TArray<USubstanceGraphInstance*> GetSubstances(UMaterialInterface* Material);

	UFUNCTION(BlueprintCallable)
	static float GetSubstanceLoadingProgress();

	UFUNCTION(BlueprintCallable)
	static FString GetGraphName(USubstanceGraphInstance* GraphInstance);

	UFUNCTION(BlueprintCallable)
	static FString GetFactoryName(USubstanceGraphInstance* GraphInstance);

	UFUNCTION(BlueprintCallable)
	static USubstanceGraphInstance* DuplicateGraphInstance(UObject* WorldContextObject, USubstanceGraphInstance* GraphInstance);

	UFUNCTION(BlueprintCallable)
	static void CreateInstanceOutputs(UObject* WorldContextObject, USubstanceGraphInstance* GraphInstance, TArray<int32> OutputIndices);

	UFUNCTION(BlueprintCallable)
	static USubstanceGraphInstance* CreateGraphInstance(UObject* WorldContextObject, USubstanceInstanceFactory* Factory, int32 GraphDescIndex, const FString& InstanceName);

	UFUNCTION(BlueprintCallable)
	static void CopyInputParameters(USubstanceGraphInstance* SourceGraphInstance, USubstanceGraphInstance* DestGraphInstance);

	UFUNCTION(BlueprintCallable)
	static void AsyncRendering(USubstanceGraphInstance* InstancesToRender);

public:
	USubstanceUtility();
};

FORCEINLINE uint32 GetTypeHash(const USubstanceUtility) { return 0; }

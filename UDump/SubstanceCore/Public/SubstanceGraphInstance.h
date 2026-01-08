#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SubstanceIntInputDesc.h"
#include "SubstanceInstanceDesc.h"
#include "ESubstanceInputType.h"
#include "SubstanceFloatInputDesc.h"
#include "SubstanceGraphInstance.generated.h"

class USubstanceInstanceFactory;

UCLASS(Blueprintable, MinimalAPI)
class USubstanceGraphInstance : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USubstanceInstanceFactory* Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UObject*> ImageSources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFreezed;

public:
	UFUNCTION(BlueprintCallable)
	void SetInputInt(const FString& Identifier, const TArray<int32>& Value);

	UFUNCTION(BlueprintCallable)
	bool SetInputImg(const FString& InputName, UObject* Value);

	UFUNCTION(BlueprintCallable)
	void SetInputFloat(const FString& Identifier, const TArray<float>& InputValues);

	UFUNCTION(BlueprintCallable)
	FSubstanceIntInputDesc GetIntInputDesc(const FString& Identifier);

	UFUNCTION(BlueprintCallable)
	FSubstanceInstanceDesc GetInstanceDesc();

	UFUNCTION(BlueprintCallable)
	ESubstanceInputType GetInputType(const FString& InputName);

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetInputNames();

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetInputInt(const FString& Identifier);

	UFUNCTION(BlueprintCallable)
	TArray<float> GetInputFloat(const FString& Identifier);

	UFUNCTION(BlueprintCallable)
	FSubstanceFloatInputDesc GetFloatInputDesc(const FString& Identifier);

public:
	USubstanceGraphInstance();
};

FORCEINLINE uint32 GetTypeHash(const USubstanceGraphInstance) { return 0; }

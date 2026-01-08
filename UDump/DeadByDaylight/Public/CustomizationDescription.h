#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CustomizedMeshPart.h"
#include "CustomizationDescription.generated.h"

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UCustomizationDescription : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCustomizedMeshPart> Parts;

public:
	UCustomizationDescription();
};

FORCEINLINE uint32 GetTypeHash(const UCustomizationDescription) { return 0; }

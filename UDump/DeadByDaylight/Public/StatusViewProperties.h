#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EStatusEffectType.h"
#include "StatusViewProperties.generated.h"

USTRUCT(BlueprintType)
struct FStatusViewProperties : public FTableRowBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StatusViewID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AStatusView> StatusViewBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusEffectType StatusType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconFilePath;

public:
	DEADBYDAYLIGHT_API FStatusViewProperties();
};

FORCEINLINE uint32 GetTypeHash(const FStatusViewProperties) { return 0; }

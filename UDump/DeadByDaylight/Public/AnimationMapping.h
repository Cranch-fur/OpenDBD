#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "AnimationMapping.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FAnimationMapping : public FTableRowBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> MontageAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Tags;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UAnimMontage* _montage;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _loadAttempted;

public:
	DEADBYDAYLIGHT_API FAnimationMapping();
};

FORCEINLINE uint32 GetTypeHash(const FAnimationMapping) { return 0; }

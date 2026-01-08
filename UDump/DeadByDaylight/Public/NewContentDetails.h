#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ENewContentType.h"
#include "NewContentDetails.generated.h"

USTRUCT(BlueprintType)
struct FNewContentDetails : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENewContentType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ImagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ImageHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHidden;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ContentTags;

public:
	DEADBYDAYLIGHT_API FNewContentDetails();
};

FORCEINLINE uint32 GetTypeHash(const FNewContentDetails) { return 0; }

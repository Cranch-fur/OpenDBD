#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ProceduralMap.generated.h"

USTRUCT(BlueprintType)
struct FProceduralMap : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MapId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ThemeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HookCountOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BookShelvesMinCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BookShelvesMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LivingWorldObjectsMinCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LivingWorldObjectsMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ThumbnailPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SortingIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DlcIDString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DlcIDStringPS4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DlcIDStringXB1;

public:
	DEADBYDAYLIGHT_API FProceduralMap();
};

FORCEINLINE uint32 GetTypeHash(const FProceduralMap) { return 0; }

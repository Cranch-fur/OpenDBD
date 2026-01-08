#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BPAttachementSocketData.generated.h"

USTRUCT(BlueprintType)
struct FBPAttachementSocketData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AItemBlueprint> AttachementBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

public:
	DEADBYDAYLIGHT_API FBPAttachementSocketData();
};

FORCEINLINE uint32 GetTypeHash(const FBPAttachementSocketData) { return 0; }

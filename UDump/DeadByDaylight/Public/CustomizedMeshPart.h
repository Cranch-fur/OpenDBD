#pragma once

#include "CoreMinimal.h"
#include "CustomizedMeshPart.generated.h"

class USkeletalMesh;

USTRUCT(BlueprintType)
struct FCustomizedMeshPart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NameOfPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShownByDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

public:
	DEADBYDAYLIGHT_API FCustomizedMeshPart();
};

FORCEINLINE uint32 GetTypeHash(const FCustomizedMeshPart) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EInventoryItemType.h"
#include "UObject/SoftObjectPtr.h"
#include "EItemHandPosition.h"
#include "EPlayerRole.h"
#include "EItemRarity.h"
#include "EKillerAbilities.h"
#include "ItemData.generated.h"

class USkeletalMesh;

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText GenericDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> ItemBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> ItemMeshV2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemHandPosition HandPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> IconFilePaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Bloodweb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Chest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool COMPLETED;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Disabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKillerAbilities RequiredKillerAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CloudInventoryId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CommunityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DLCId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DLCIdPS4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DLCIdXB1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AntiDLC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AvailabledDLCId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AvailabledDLCIdPS4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AvailabledDLCIdXB1;

public:
	DEADBYDAYLIGHT_API FItemData();
};

FORCEINLINE uint32 GetTypeHash(const FItemData) { return 0; }

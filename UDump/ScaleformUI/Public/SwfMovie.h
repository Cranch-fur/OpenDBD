#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FlashTextureRescale.h"
#include "SwfMovie.generated.h"

UCLASS(Blueprintable, MinimalAPI)
class USwfMovie : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bUseGFxExport : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOptimizeForMobiles : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bSetSRGBOnImportedTextures : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bPackTextures : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PackTextureSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bForceSquarePacking : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<FlashTextureRescale> TextureRescale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TextureFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bReplaceImages : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AssetRegistrySearchable)
	FString SourceFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> FontMappings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SourceFileTimestamp;

	UPROPERTY(EditInstanceOnly, Transient)
	uint64 ImportTimeStamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> RawData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ReferencedAssetNames;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	TArray<UObject*> References;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UObject*> UserReferences;

public:
	USwfMovie();
};

FORCEINLINE uint32 GetTypeHash(const USwfMovie) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BloodWebPersistentData.h"
#include "EPlayerRole.h"
#include "BloodwebGenerator.generated.h"

class UDBDGameInstance;
class UDBDBloodWebDefinition;
class UDBDDesignTunables;

UCLASS(Blueprintable)
class UBloodwebGenerator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EmptyNodeOccurenceFactor;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UDBDGameInstance* _gameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UDBDBloodWebDefinition* _bloodWebDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UDBDDesignTunables* _designTunables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 _nodeContentTotals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<int32> _nodeContentPopulation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FRandomStream _randomizationStream;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<FString> _selectedNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float _alternativePathOccurenceFactor;

public:
	UFUNCTION(BlueprintCallable)
	void Init(UDBDGameInstance* GameInstance, UDBDDesignTunables* designTunables, UDBDBloodWebDefinition* bloodWebDefinition);

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetBloodwebNodes() const;

	UFUNCTION(BlueprintCallable)
	void BuildWebInPlace(FBloodWebPersistentData& Bloodweb, EPlayerRole playerRole, int32 currentPlayerLevel);

public:
	UBloodwebGenerator();
};

FORCEINLINE uint32 GetTypeHash(const UBloodwebGenerator) { return 0; }

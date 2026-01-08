#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EDBDScoreTypes.h"
#include "UObject/NoExportTypes.h"
#include "DBDGameMode.generated.h"

class APlayerStart;
class AActor;

UCLASS(Blueprintable, NonTransient)
class ADBDGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StartWithSlasher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EscapeOpened;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<APlayerStart*> _initialPlayerStarts;

private:
	UFUNCTION(BlueprintCallable)
	void OnGameplayEvent(EDBDScoreTypes ScoreType, float amount, AActor* instigatorr, AActor* target);

public:
	UFUNCTION(BlueprintCallable)
	void LevelLoaded(const FString& LevelName);

	UFUNCTION(BlueprintCallable)
	void HostGame();

	UFUNCTION(BlueprintPure)
	FTimespan GetElapsedMatchTime() const;

public:
	ADBDGameMode();
};

FORCEINLINE uint32 GetTypeHash(const ADBDGameMode) { return 0; }

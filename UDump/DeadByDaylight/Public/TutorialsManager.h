#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialsManager.generated.h"

class UMediaPlayer;
class UMediaSoundWave;
class UDBDGameInstance;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ATutorialsManager : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TutorialViewTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMediaPlayer* TutorialMediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMediaSoundWave* TutorialMediaSoundWave;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UDBDGameInstance* _gameInstance;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	AActor* _initialGameViewTarget;

public:
	UFUNCTION(BlueprintCallable)
	void TutorialEnded(bool skipped);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StopTutorial();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StartTutorial(const FString& Path);

public:
	ATutorialsManager();
};

FORCEINLINE uint32 GetTypeHash(const ATutorialsManager) { return 0; }

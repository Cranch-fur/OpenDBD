#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RootMovie.generated.h"

class UGFxMoviePlayer;
class USwfMovie;
class UScreenController;
class UUIController;
class UGFxObject;

UCLASS(Blueprintable)
class URootMovie : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SwfAssetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGFxMoviePlayer* SwfMoviePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USwfMovie* FontLibrary;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UScreenController* m_ScreenController;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UUIController* m_UIController;

public:
	UFUNCTION(BlueprintCallable)
	void SetCursorAsSticky(int32 isSticky, int32 lockVertical, int32 lockHorizontal, int32 moveByX, int32 moveByY);

	UFUNCTION(BlueprintCallable)
	void RegisterView(const FString& screenId, UGFxObject* ScreenObject);

	UFUNCTION(BlueprintCallable)
	void RegisterUIControl(UGFxObject* flashObj);

	UFUNCTION(BlueprintCallable)
	void RegisterScreenControl(UGFxObject* flashObj);

	UFUNCTION(BlueprintCallable)
	void PlaySound(const FString& soundId);

	UFUNCTION(BlueprintCallable)
	void OpenLinkInBrowser(const FString& linkAddress);

	UFUNCTION(BlueprintCallable)
	void OnScreenReady(const FString& screenId);

	UFUNCTION(BlueprintCallable)
	void OnScreenLeave(const FString& screenId);

	UFUNCTION(BlueprintCallable)
	void OnScreenEnter(const FString& screenId);

	UFUNCTION(BlueprintCallable)
	void OnMovieClosed();

	UFUNCTION(BlueprintCallable)
	void OnFlashReady();

	UFUNCTION(BlueprintCallable)
	void OnAssert(int32 Type, const FString& message);

public:
	URootMovie();
};

FORCEINLINE uint32 GetTypeHash(const URootMovie) { return 0; }

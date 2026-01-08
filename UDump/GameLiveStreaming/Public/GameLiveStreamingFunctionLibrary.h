#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameLiveStreamingFunctionLibrary.generated.h"

class UTexture2D;

UCLASS(Blueprintable)
class UGameLiveStreamingFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void StopWebCam();

	UFUNCTION(BlueprintCallable)
	static void StopBroadcastingGame();

	UFUNCTION(BlueprintCallable)
	static void StartWebCam(int32 DesiredWebCamWidth, int32 DesiredWebCamHeight, bool bMirrorWebCamImage, bool bDrawSimpleWebCamVideo);

	UFUNCTION(BlueprintCallable)
	static void StartBroadcastingGame(const FString& LoginUserName, const FString& LoginPassword, int32 FrameRate, float ScreenScaling, bool bStartWebCam, int32 DesiredWebCamWidth, int32 DesiredWebCamHeight, bool bMirrorWebCamImage, bool bDrawSimpleWebCamVideo, bool bCaptureAudioFromComputer, bool bCaptureAudioFromMicrophone, UTexture2D* CoverUpImage);

	UFUNCTION(BlueprintPure)
	static bool IsWebCamEnabled();

	UFUNCTION(BlueprintPure)
	static bool IsBroadcastingGame();

public:
	UGameLiveStreamingFunctionLibrary();
};

FORCEINLINE uint32 GetTypeHash(const UGameLiveStreamingFunctionLibrary) { return 0; }

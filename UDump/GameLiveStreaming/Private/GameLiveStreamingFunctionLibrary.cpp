#include "GameLiveStreamingFunctionLibrary.h"

class UTexture2D;

void UGameLiveStreamingFunctionLibrary::StopWebCam()
{

}

void UGameLiveStreamingFunctionLibrary::StopBroadcastingGame()
{

}

void UGameLiveStreamingFunctionLibrary::StartWebCam(int32 DesiredWebCamWidth, int32 DesiredWebCamHeight, bool bMirrorWebCamImage, bool bDrawSimpleWebCamVideo)
{

}

void UGameLiveStreamingFunctionLibrary::StartBroadcastingGame(const FString& LoginUserName, const FString& LoginPassword, int32 FrameRate, float ScreenScaling, bool bStartWebCam, int32 DesiredWebCamWidth, int32 DesiredWebCamHeight, bool bMirrorWebCamImage, bool bDrawSimpleWebCamVideo, bool bCaptureAudioFromComputer, bool bCaptureAudioFromMicrophone, UTexture2D* CoverUpImage)
{

}

bool UGameLiveStreamingFunctionLibrary::IsWebCamEnabled()
{
	return false;
}

bool UGameLiveStreamingFunctionLibrary::IsBroadcastingGame()
{
	return false;
}

UGameLiveStreamingFunctionLibrary::UGameLiveStreamingFunctionLibrary()
{

}

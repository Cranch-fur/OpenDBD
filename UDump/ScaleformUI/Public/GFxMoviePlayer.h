#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GFxHitTestType.h"
#include "InputCoreTypes.h"
#include "ExternalTexture.h"
#include "EGFxRenderTextureMode.h"
#include "GFxWidgetBinding.h"
#include "FsCommandSignature.h"
#include "OnStartCommandSignature.h"
#include "OnTickCommandSignature.h"
#include "OnFocusCommandSignature.h"
#include "EGFxScaleMode.h"
#include "ASValue.h"
#include "EGFxTimingMode.h"
#include "ASPerspectiveTransform.h"
#include "ASDisplayInfo.h"
#include "ASColorTransform.h"
#include "EGFxAlign.h"
#include "GFxMoviePlayer.generated.h"

class UGFxObject;
class UTextureRenderTarget2D;
class UTexture;
class USwfMovie;
class UGFxMoviePlayer;

UCLASS(Blueprintable)
class SCALEFORMUI_API UGFxMoviePlayer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SwfAssetName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	UGFxObject* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bDisplayWithHudOff : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bEnableGammaCorrection : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bAllowInput : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bAllowFocus : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCloseOnLevelChange : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bOnlyOwnerFocusable : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bDiscardNonOwnerInput : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCaptureKeyboardInput : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCaptureMouseInput : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCaptureGamepadInput : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCaptureTouchInput : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsSplitscreenLayoutModified : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bShowDefaultMouseCursor : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bBlurLesserMovies : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bHideLesserMovies : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsPriorityBlurred : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsPriorityHidden : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIgnoreVisibilityEffect : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIgnoreBlurEffect : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bRefreshed : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bPerformHitTest : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<GFxHitTestType::Type> HitTestType;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	UTextureRenderTarget2D* RenderTexture;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	UObject* ExternalInterface;

	UPROPERTY(EditAnywhere)
	TArray<FKey> CaptureKeys;

	UPROPERTY(EditAnywhere)
	TArray<FKey> FocusIgnoreKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FExternalTexture> ExternalTextures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGFxRenderTextureMode::Type> RenderTextureMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGFxWidgetBinding> WidgetBindings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SplitscreenLayoutYAdjust;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FFsCommandSignature OnFsCommand;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnStartCommandSignature OnStartCommand;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnStartCommandSignature OnCloseCommand;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnTickCommandSignature OnTickCommand;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnFocusCommandSignature OnFocusGainedCommand;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnFocusCommandSignature OnFocusLostCommand;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<UGFxObject*> CachedMovieClipsArray;

public:
	UFUNCTION(BlueprintCallable)
	bool Start(bool bRefresh);

	UFUNCTION(BlueprintCallable)
	void SetVisibility(const FString& MovieClipName, bool bVisible);

	UFUNCTION(BlueprintCallable)
	void SetViewScaleMode(EGFxScaleMode::Type ScaleMode);

	UFUNCTION(BlueprintCallable)
	void SetViewport(int32 X, int32 Y, int32 Width, int32 Height);

	UFUNCTION(BlueprintCallable)
	void SetView3D(const FTransform& ViewTransform);

	UFUNCTION(BlueprintCallable)
	void SetVariableValueArray(const FString& MovieClipName, const FString& MemberName, const TArray<FASValue>& Value);

	UFUNCTION(BlueprintCallable)
	void SetVariableValue(const FString& MovieClipName, const FString& MemberName, const FASValue& Value);

	UFUNCTION(BlueprintCallable)
	void SetTimingMode(EGFxTimingMode::Type TimingMode);

	UFUNCTION(BlueprintCallable)
	void SetScale(const FString& MovieClipName, float XScale, float YScale);

	UFUNCTION(BlueprintCallable)
	void SetPosition(const FString& MovieClipName, float X, float Y);

	UFUNCTION(BlueprintCallable)
	void SetPerspective3D(const FASPerspectiveTransform& PerspectiveTransform);

	UFUNCTION(BlueprintCallable)
	void SetPause(bool bPausePlayback);

	UFUNCTION(BlueprintCallable)
	void SetMovieCanReceiveInput(bool bCanReceiveInput);

	UFUNCTION(BlueprintCallable)
	void SetMovieCanReceiveFocus(bool bCanReceiveFocus);

	UFUNCTION(BlueprintCallable)
	void SetFocusIgnoreKeys(const TArray<FKey>& Keys);

	UFUNCTION(BlueprintCallable)
	bool SetExternalTexture(const FString& Resource, UTexture* Texture);

	UFUNCTION(BlueprintCallable)
	void SetDisplayTransform(const FString& MovieClipName, const FTransform& DisplayTransform);

	UFUNCTION(BlueprintCallable)
	void SetDisplayInfo(const FString& MovieClipName, const FASDisplayInfo& Info);

	UFUNCTION(BlueprintCallable)
	void SetColorTransform(const FString& MovieClipName, const FASColorTransform& UnrealColorTransform);

	UFUNCTION(BlueprintCallable)
	void SetCaptureKeys(const TArray<FKey>& Keys);

	UFUNCTION(BlueprintCallable)
	void SetAlignment(EGFxAlign::Type Align);

	UFUNCTION(BlueprintCallable)
	void ReceiveGFxCommand(const FString& Command, const FString& Arguments);

	UFUNCTION(BlueprintCallable)
	static UGFxMoviePlayer* OpenMovie(USwfMovie* SwfMovie, UObject* ExternalScriptInterface, UTextureRenderTarget2D* RenderToTexture, UGFxMoviePlayer* MoviePlayer, bool DisplayWithHudOff, bool StartPaused);

	UFUNCTION(BlueprintCallable)
	void OnTick(float deltaTime);

	UFUNCTION(BlueprintCallable)
	void OnStart();

	UFUNCTION(BlueprintCallable)
	void OnFocusLost(int32 LocalPlayerIndex);

	UFUNCTION(BlueprintCallable)
	void OnFocusGained(int32 LocalPlayerIndex);

	UFUNCTION(BlueprintCallable)
	void OnClose();

	UFUNCTION(BlueprintCallable)
	bool LoadAndStart(const FString& InSwfAssetName, bool bRefresh);

	UFUNCTION(BlueprintCallable)
	FASValue Invoke(const FString& MovieClipName, const FString& FunctionName, const TArray<FASValue>& Params);

	UFUNCTION(BlueprintCallable)
	void GotoAndStopI(const FString& MovieClipName, int32 Frame);

	UFUNCTION(BlueprintCallable)
	void GotoAndStop(const FString& MovieClipName, const FString& Frame);

	UFUNCTION(BlueprintCallable)
	void GotoAndPlayI(const FString& MovieClipName, int32 Frame);

	UFUNCTION(BlueprintCallable)
	void GotoAndPlay(const FString& MovieClipName, const FString& Frame);

	UFUNCTION(BlueprintCallable)
	bool GetVisibility(const FString& MovieClipName);

	UFUNCTION(BlueprintCallable)
	FTransform GetView3D();

	UFUNCTION(BlueprintCallable)
	TArray<FASValue> GetVariableValueArray(const FString& MovieClipName, const FString& MemberName);

	UFUNCTION(BlueprintCallable)
	FASValue GetVariableValue(const FString& MovieClipName, const FString& MemberName);

	UFUNCTION(BlueprintCallable)
	bool GetScale(const FString& MovieClipName, float& XScale, float& YScale);

	UFUNCTION(BlueprintCallable)
	bool GetPosition(const FString& MovieClipName, float& X, float& Y);

	UFUNCTION(BlueprintCallable)
	FASPerspectiveTransform GetPerspective3D();

	UFUNCTION(BlueprintCallable)
	UGFxObject* GetMovieClip(const FString& MovieClipName);

	UFUNCTION(BlueprintCallable)
	FTransform GetDisplayTransform(const FString& MovieClipName);

	UFUNCTION(BlueprintCallable)
	FASDisplayInfo GetDisplayInfo(const FString& MovieClipName);

	UFUNCTION(BlueprintCallable)
	FASColorTransform GetColorTransform(const FString& MovieClipName);

	UFUNCTION(BlueprintCallable)
	void Close(bool Unload);

public:
	UGFxMoviePlayer();
};

FORCEINLINE uint32 GetTypeHash(const UGFxMoviePlayer) { return 0; }

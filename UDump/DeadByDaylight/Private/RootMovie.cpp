#include "RootMovie.h"

class UGFxObject;

void URootMovie::SetCursorAsSticky(int32 isSticky, int32 lockVertical, int32 lockHorizontal, int32 moveByX, int32 moveByY)
{

}

void URootMovie::RegisterView(const FString& screenId, UGFxObject* ScreenObject)
{

}

void URootMovie::RegisterUIControl(UGFxObject* flashObj)
{

}

void URootMovie::RegisterScreenControl(UGFxObject* flashObj)
{

}

void URootMovie::PlaySound(const FString& soundId)
{

}

void URootMovie::OpenLinkInBrowser(const FString& linkAddress)
{

}

void URootMovie::OnScreenReady(const FString& screenId)
{

}

void URootMovie::OnScreenLeave(const FString& screenId)
{

}

void URootMovie::OnScreenEnter(const FString& screenId)
{

}

void URootMovie::OnMovieClosed()
{

}

void URootMovie::OnFlashReady()
{

}

void URootMovie::OnAssert(int32 Type, const FString& message)
{

}

URootMovie::URootMovie()
{
	this->SwfAssetName = TEXT("/Game/UI/Root/Root.Root");
}

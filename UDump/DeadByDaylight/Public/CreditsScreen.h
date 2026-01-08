#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "CreditsScreen.generated.h"

UCLASS(Blueprintable)
class UCreditsScreen : public UScreenBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCreditsHtml(const FString& CreditsHtml);

	UFUNCTION(BlueprintCallable)
	void ScrollCreditsScreen(float scrollAmount);

public:
	UCreditsScreen();
};

FORCEINLINE uint32 GetTypeHash(const UCreditsScreen) { return 0; }

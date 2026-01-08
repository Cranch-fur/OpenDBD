#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "LegalMenuScreen.generated.h"

UCLASS(Blueprintable)
class ULegalMenuScreen : public UScreenBase
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable)
	void OnDeclineClick();

	UFUNCTION(BlueprintCallable)
	void OnAcceptClick();

public:
	ULegalMenuScreen();
};

FORCEINLINE uint32 GetTypeHash(const ULegalMenuScreen) { return 0; }

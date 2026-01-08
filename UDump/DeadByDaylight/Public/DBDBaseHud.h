#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DBDBaseHud.generated.h"

class UFont;

UCLASS(Blueprintable, NonTransient)
class ADBDBaseHud : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFont* _debugFont;

public:
	UFUNCTION(BlueprintCallable, Exec)
	void ShowVersionNumber(bool ShouldShow);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowAnimHudBP();

	UFUNCTION(BlueprintCallable, Exec)
	void ShowAnimHud(bool show);

	UFUNCTION(BlueprintPure)
	bool ShouldDrawAnimHud() const;

public:
	ADBDBaseHud();
};

FORCEINLINE uint32 GetTypeHash(const ADBDBaseHud) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "ScreenBase.h"
#include "TallyScreen.generated.h"

UCLASS(Blueprintable)
class UTallyScreen : public UScreenBase
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable)
	void OnSpectateButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnReportPlayerButtonClick(const FString& SteamId, const FString& PlayerName, int32 isKiller);

	UFUNCTION(BlueprintCallable)
	void OnRateMatch(int32 rateValue);

	UFUNCTION(BlueprintCallable)
	void OnPlayerInfoClick(const FString& PlayerId);

	UFUNCTION(BlueprintCallable)
	void OnLeaveButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnBloodwebButtonClick();

	UFUNCTION(BlueprintCallable)
	bool IsSurviveWithFriendsMode() const;

public:
	UTallyScreen();
};

FORCEINLINE uint32 GetTypeHash(const UTallyScreen) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatusViewSource.h"
#include "StatusView.generated.h"

class ADBDPlayer;

UCLASS(Blueprintable)
class AStatusView : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PercentageFill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsClockwiseTimer;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	ADBDPlayer* _localDBDPlayer;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetView(const ADBDPlayer* DBDPlayer);

	UFUNCTION(BlueprintCallable)
	void FireNotification(const FStatusViewSource StatusViewSource);

public:
	AStatusView();
};

FORCEINLINE uint32 GetTypeHash(const AStatusView) { return 0; }

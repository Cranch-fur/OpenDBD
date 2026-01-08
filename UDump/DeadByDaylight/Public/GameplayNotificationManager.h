#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayNotificationData.h"
#include "GameplayNotificationManager.generated.h"

UCLASS(Blueprintable)
class UGameplayNotificationManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void FireGameplayNotification(const FGameplayNotificationData notificationData);

public:
	UGameplayNotificationManager();
};

FORCEINLINE uint32 GetTypeHash(const UGameplayNotificationManager) { return 0; }

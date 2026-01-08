#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PulldownAnalytics.generated.h"

class ACamperPlayer;
class AActor;
class ASlasherPlayer;

UCLASS(Blueprintable)
class UPulldownAnalytics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void IncrementPalletStun(ACamperPlayer* survivor);

	UFUNCTION(BlueprintCallable)
	static void IncrementPalletSpawned(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	static void IncrementPalletDrop(ACamperPlayer* survivor);

	UFUNCTION(BlueprintCallable)
	static void IncrementPalletDestroy(ASlasherPlayer* killer);

public:
	UPulldownAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const UPulldownAnalytics) { return 0; }

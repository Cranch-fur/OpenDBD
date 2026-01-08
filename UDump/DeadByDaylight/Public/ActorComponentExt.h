#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActorComponentExt.generated.h"

class UActorComponent;
class AActor;
class ACamperPlayer;
class ADBDPlayer;
class ASlasherPlayer;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API UActorComponentExt : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static bool IsLocallyObserved(const UActorComponent* ActorComponent);

	UFUNCTION(BlueprintPure)
	static bool IsLocallyControlled(const UActorComponent* ActorComponent);

	UFUNCTION(BlueprintPure)
	static bool HasAuthority(const UActorComponent* ActorComponent);

	UFUNCTION(BlueprintPure)
	static AActor* GetValidatedOwner(const UActorComponent* ActorComponent);

	UFUNCTION(BlueprintPure)
	static ACamperPlayer* GetOwningSurvivor(const UActorComponent* ActorComponent);

	UFUNCTION(BlueprintPure)
	static ADBDPlayer* GetOwningPlayer(const UActorComponent* ActorComponent);

	UFUNCTION(BlueprintPure)
	static ASlasherPlayer* GetOwningKiller(const UActorComponent* ActorComponent);

public:
	UActorComponentExt();
};

FORCEINLINE uint32 GetTypeHash(const UActorComponentExt) { return 0; }

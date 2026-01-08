#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EMapActorCategory.h"
#include "MapActorComponent.generated.h"

class ADBDPlayer;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UMapActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMapActorCategory Category;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	bool _detectionEnabled;

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, BlueprintAuthorityOnly)
	void Multicast_SetDetectionEnabled(bool Enabled);

	UFUNCTION(BlueprintPure)
	bool IsKnownBy(ADBDPlayer* Character) const;

	UFUNCTION(BlueprintPure)
	bool GetDetectionEnabled() const;

public:
	UMapActorComponent();
};

FORCEINLINE uint32 GetTypeHash(const UMapActorComponent) { return 0; }

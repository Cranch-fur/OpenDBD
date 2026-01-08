#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemVfx.generated.h"

class USkeletalMeshComponent;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AItemVfx : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetEmittersVisibility(bool Visible);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnAttackEnd();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnAttackBegin();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AttachToSkeletalMesh(USkeletalMeshComponent* SkeletonPart);

public:
	AItemVfx();
};

FORCEINLINE uint32 GetTypeHash(const AItemVfx) { return 0; }

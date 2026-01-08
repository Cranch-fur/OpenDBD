#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/NoExportTypes.h"
#include "DBDSpringArmComponent.generated.h"

class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBDSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bEnableComponentwiseCameraLag : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ComponenwiseCameraLagSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetArmMinLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bEnableDistanceCameraLag : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraDistanceLagSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bDrawDebugTraceMarkers : 1;

protected:
	UPROPERTY(EditInstanceOnly, Transient)
	TArray<TWeakObjectPtr<AActor>> ignoreActors;

public:
	UFUNCTION(BlueprintCallable)
	void IgnoreActor(AActor* Actor, bool bShouldIgnore);

	UFUNCTION(BlueprintCallable)
	void ClearIgnoreActors();

public:
	UDBDSpringArmComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBDSpringArmComponent) { return 0; }

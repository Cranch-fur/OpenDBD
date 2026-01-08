#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/HitResult.h"
#include "DBDCharacterMovementComponent.generated.h"

class UCurveFloat;
class UCapsuleComponent;
class AMovementModifierActor;
class UPrimitiveComponent;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UDBDCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectedWallSlidingFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomInputFrontMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomInputSideMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomInputBackMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* OnHitMaxSpeedCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* OnSprintMaxSpeedCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SprintEffector;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UCapsuleComponent* _movementModifierDetector;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	AMovementModifierActor* _movementModifierActor;

public:
	UFUNCTION(BlueprintCallable)
	void SetRandomInputAmplitude(float Value);

	UFUNCTION(BlueprintCallable)
	void SetMovementModifierDetector(UCapsuleComponent* movementModifierDetector);

protected:
	UFUNCTION(BlueprintCallable)
	void OnMovementModifierDetectorOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnMovementModifierDetectorOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintPure)
	bool IsApplyingRandomInput() const;

	UFUNCTION(BlueprintPure)
	AMovementModifierActor* GetOverlappedMovementModifierActor() const;

public:
	UDBDCharacterMovementComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBDCharacterMovementComponent) { return 0; }

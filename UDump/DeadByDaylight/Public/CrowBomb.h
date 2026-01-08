#pragma once

#include "CoreMinimal.h"
#include "GeneratorDrivenActor.h"
#include "UObject/NoExportTypes.h"
#include "CrowBomb.generated.h"

class UWorldRunawayMeshComponent;
class USceneComponent;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ACrowBomb : public AGeneratorDrivenActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float _innerRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float _outerRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float _maxTakeoffDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 _numCrows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 _crowCenterDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 _crowOuterDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 _eliminationSamples;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float _eliminationStddev;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TAssetSubclassOf<UWorldRunawayMeshComponent> _meshComponentClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, Instanced, meta=(AllowPrivateAccess=true))
	TArray<UWorldRunawayMeshComponent*> _crows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	USceneComponent* _root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	USceneComponent* _traceStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	USceneComponent* _traceEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _landed;

public:
	UFUNCTION(BlueprintCallable)
	void TriggerTakeOff(FVector triggerLocation);

	UFUNCTION(BlueprintCallable)
	void TriggerLand();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ACrowBomb();
};

FORCEINLINE uint32 GetTypeHash(const ACrowBomb) { return 0; }

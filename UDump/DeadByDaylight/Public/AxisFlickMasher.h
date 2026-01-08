#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AxisFlickMasher.generated.h"

class UInputComponent;
class UAxisFlick;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UAxisFlickMasher : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	UInputComponent* _inputComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UAxisFlick* _axisFlick1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UAxisFlick* _axisFlick2;

private:
	UFUNCTION(BlueprintCallable)
	void OnAxisFlicked2();

	UFUNCTION(BlueprintCallable)
	void OnAxisFlicked1();

public:
	UAxisFlickMasher();
};

FORCEINLINE uint32 GetTypeHash(const UAxisFlickMasher) { return 0; }

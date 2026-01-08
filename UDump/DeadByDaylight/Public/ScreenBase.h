#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScreenBase.generated.h"

class UGFxObject;

UCLASS(Blueprintable, Abstract)
class UScreenBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAnimationDone;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	UGFxObject* ScreenObject;

public:
	UFUNCTION(BlueprintCallable)
	void SetFadesOut(bool fadesOut);

	UFUNCTION(BlueprintCallable)
	void OnStart();

	UFUNCTION(BlueprintCallable)
	void OnNavKey(const FString& navKey);

	UFUNCTION(BlueprintCallable)
	void OnEscape();

	UFUNCTION(BlueprintCallable)
	void OnBack();

	UFUNCTION(BlueprintCallable)
	void OnAnimationUpdate(int32 isAnimationDoneNew);

public:
	UScreenBase();
};

FORCEINLINE uint32 GetTypeHash(const UScreenBase) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Float_NetQuantize8.h"
#include "DBDTimer.generated.h"

USTRUCT(BlueprintType)
struct FDBDTimer
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float _startTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _startTimeDirty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FFloat_NetQuantize8 _timeLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool _replicateTimeLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float _interpSpeed;

public:
	DEADBYDAYLIGHT_API FDBDTimer();
};

FORCEINLINE uint32 GetTypeHash(const FDBDTimer) { return 0; }

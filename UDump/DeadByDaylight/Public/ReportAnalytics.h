#pragma once

#include "CoreMinimal.h"
#include "EPlayerRole.h"
#include "ReportAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FReportAnalytics
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientIdTransmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientIdReceiver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole RoleTransmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole RoleReceiver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ReportType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ReportCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Comments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Platform;

public:
	DEADBYDAYLIGHT_API FReportAnalytics();
};

FORCEINLINE uint32 GetTypeHash(const FReportAnalytics) { return 0; }

#pragma once

#include "CoreMinimal.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "OnQueryLiveStreamsCompleted.h"
#include "QueryLiveStreamsCallbackProxy.generated.h"

class UQueryLiveStreamsCallbackProxy;

UCLASS(Blueprintable)
class UQueryLiveStreamsCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnQueryLiveStreamsCompleted OnQueriedLiveStreams;

public:
	UFUNCTION(BlueprintCallable)
	static UQueryLiveStreamsCallbackProxy* QueryLiveStreams(const FString& GameName);

public:
	UQueryLiveStreamsCallbackProxy();
};

FORCEINLINE uint32 GetTypeHash(const UQueryLiveStreamsCallbackProxy) { return 0; }

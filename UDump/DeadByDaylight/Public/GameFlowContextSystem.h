#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFlowContextSystem.generated.h"

class URootMovie;
class UOnlineSystemHandler;

UCLASS(Blueprintable, Transient)
class UGameFlowContextSystem : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	URootMovie* m_RootMovie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UOnlineSystemHandler* m_OnlineSystemHandler;

public:
	UGameFlowContextSystem();
};

FORCEINLINE uint32 GetTypeHash(const UGameFlowContextSystem) { return 0; }

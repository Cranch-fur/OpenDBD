#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScreenController.generated.h"

class UGFxObject;
class UGFxMoviePlayer;
class UScreenBase;

UCLASS(Blueprintable)
class UScreenController : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UGFxObject* m_ScreenControllerObj;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	UGFxMoviePlayer* m_MoviePlayer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	TMap<FString, UScreenBase*> m_ScreenDictionary;

public:
	UScreenController();
};

FORCEINLINE uint32 GetTypeHash(const UScreenController) { return 0; }

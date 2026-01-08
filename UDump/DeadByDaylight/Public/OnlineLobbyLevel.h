#pragma once

#include "CoreMinimal.h"
#include "LobbyLevel.h"
#include "OnlineLobbyLevel.generated.h"

class UProceduralGenerationData;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API AOnlineLobbyLevel : public ALobbyLevel
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralGenerationData* ProceduralGenerationData;

public:
	AOnlineLobbyLevel();
};

FORCEINLINE uint32 GetTypeHash(const AOnlineLobbyLevel) { return 0; }

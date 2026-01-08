#pragma once

#include "CoreMinimal.h"
#include "OnNextStep.generated.h"

class UObject;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNextStep, UObject*, Object);

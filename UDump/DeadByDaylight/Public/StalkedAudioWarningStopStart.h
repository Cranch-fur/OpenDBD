#pragma once

#include "CoreMinimal.h"
#include "StalkedAudioWarningStopStart.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStalkedAudioWarningStopStart, bool, On);

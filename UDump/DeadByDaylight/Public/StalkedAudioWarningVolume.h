#pragma once

#include "CoreMinimal.h"
#include "StalkedAudioWarningVolume.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStalkedAudioWarningVolume, float, Volume);

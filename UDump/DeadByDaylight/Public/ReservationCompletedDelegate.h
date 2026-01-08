#pragma once

#include "CoreMinimal.h"
#include "ReservationCompletedDelegate.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReservationCompletedDelegate, int32, result);

#pragma once

#include "CoreMinimal.h"
#include "ELegalTermsStatus.generated.h"

UENUM()
enum class ELegalTermsStatus : int64
{
	None = 17152,
	Accepted,
	Declined,
	ZeroEntry = 0,
};

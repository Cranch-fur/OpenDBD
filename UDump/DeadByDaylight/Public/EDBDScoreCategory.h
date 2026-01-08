#pragma once

#include "CoreMinimal.h"
#include "EDBDScoreCategory.generated.h"

UENUM()
enum class EDBDScoreCategory : int64
{
	DBD_CamperScoreCat_Objectives = 16384,
	DBD_CamperScoreCat_Survival,
	DBD_CamperScoreCat_Altruism,
	DBD_CamperScoreCat_Boldness,
	DBD_SlasherScoreCat_Brutality,
	DBD_SlasherScoreCat_Deviousness,
	DBD_SlasherScoreCat_Hunter,
	DBD_SlasherScoreCat_Sacrifice,
	DBD_CamperScoreCat_Untracked,
	DBD_CamperScoreCat_Streak,
	ZeroEntry = 0,
};

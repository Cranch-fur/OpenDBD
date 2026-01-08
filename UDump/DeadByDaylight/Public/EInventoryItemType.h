#pragma once

#include "CoreMinimal.h"
#include "EInventoryItemType.generated.h"

UENUM()
enum class EInventoryItemType : int64
{
	None = 16640,
	Item,
	ItemAddOn,
	CamperPerk,
	Power,
	PowerAddOn,
	SlasherPerk,
	Favor,
	Customization,
	ZeroEntry = 0,
};

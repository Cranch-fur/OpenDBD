#include "DBDKeyDisplayInfo.h"
#include "InputCoreTypes.h"
#include "KeyDisplayInfo.h"
#include "EPlayerRole.h"
#include "EInputInteractionType.h"

class UPlayerInput;

bool UDBDKeyDisplayInfo::IsExcludedKey(FKey Key)
{
	return false;
}

FKeyDisplayInfo UDBDKeyDisplayInfo::GetKeyDisplayInfoForInteractionType(UPlayerInput* PlayerInput, EPlayerRole playerRole, EInputInteractionType interactionType, bool useGamePad)
{
	return FKeyDisplayInfo{};
}

FKeyDisplayInfo UDBDKeyDisplayInfo::GetKeyDisplayInfo(FKey Key)
{
	return FKeyDisplayInfo{};
}

FKey UDBDKeyDisplayInfo::GetFirstKeyForAction(UPlayerInput* PlayerInput, FName ActionName, bool useGamePad)
{
	return FKey{};
}

FName UDBDKeyDisplayInfo::GetActionForInteractionType(EPlayerRole playerRole, EInputInteractionType inputInteractionType, bool useGamePad, bool ignoreInteractMash)
{
	return NAME_None;
}

UDBDKeyDisplayInfo::UDBDKeyDisplayInfo()
{
	this->PendingResetKeyBindingsData = TEXT("");
	this->PendingResetMustBeGamepadKey = false;
}

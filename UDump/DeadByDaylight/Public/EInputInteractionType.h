#pragma once

#include "CoreMinimal.h"
#include "EInputInteractionType.generated.h"

UENUM()
enum class EInputInteractionType : int64
{
	VE_None = 16896,
	VE_Interact,
	VE_AttackInteract,
	VE_ItemInteract,
	VE_Rush,
	VE_ItemUse,
	VE_ItemDrop,
	VE_InteractMash,
	VE_LeftRightMash,
	VE_ExternalRequestDrop,
	VE_ExternalRequestFlashlightStunUncloak,
	VE_ExternalRequestStunUncloak,
	VE_ExternalRequestStun,
	VE_ExternalRequestDropByStunning,
	VE_ExternalRequestDropByWiggleFree,
	VE_ExternalRequestDropBySkillCheck,
	VE_ExternalRequestKillerCaughtInBearTrap,
	VE_ExternalEnterWithChainsaw,
	VE_FastInteract,
	VE_ExternalRequestSlashedOutOfTrap,
	VE_ExternalRequestSacrifice,
	VE_Gesture01,
	VE_Gesture02,
	VE_Gesture03,
	VE_Gesture04,
	VE_ExternalChainBlink,
	VE_ExternalRequestStunEvilWithin,
	VE_ExternalRequestKickStun,
	VE_ExternalRequestEscape,
	VE_Struggle,
	VE_Action,
	VE_Count,
	ZeroEntry = 0,
};

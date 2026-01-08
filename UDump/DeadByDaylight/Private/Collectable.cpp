#include "Collectable.h"
#include "UObject/NoExportTypes.h"
#include "EGameplayModifierFlag.h"
#include "EGameplayModifierType.h"
#include "Net/UnrealNetwork.h"
#include "ItemModifier.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

class UInteractor;
class ADBDPlayer;
class UItemAddon;

void ACollectable::Use()
{

}

void ACollectable::SetItemInteractor(UInteractor* Interactor)
{

}

void ACollectable::SetCount(int32 Count)
{

}

void ACollectable::OnUseReleased_Implementation()
{

}

void ACollectable::OnUse_Implementation()
{

}

void ACollectable::OnRep_BegunPlayFenceName()
{

}

void ACollectable::OnFenceInitialized()
{

}

void ACollectable::OnDropped_Implementation()
{

}

void ACollectable::OnCollected_Implementation(ADBDPlayer* collector)
{

}

void ACollectable::NotifyUpdated()
{

}

void ACollectable::Multicast_UseReleased_Implementation()
{

}

bool ACollectable::Multicast_UseReleased_Validate()
{
	return true;
}

void ACollectable::Multicast_Use_Implementation()
{

}

bool ACollectable::Multicast_Use_Validate()
{
	return true;
}

void ACollectable::Multicast_RemoveItemAddon_Implementation(UItemAddon* addon)
{

}

void ACollectable::Multicast_Dropped_Implementation(ADBDPlayer* droppedBy, const FVector& Location, const FRotator& Rotation)
{

}

bool ACollectable::Multicast_Dropped_Validate(ADBDPlayer* droppedBy, const FVector& Location, const FRotator& Rotation)
{
	return true;
}

void ACollectable::Multicast_Collected_Implementation(ADBDPlayer* collector)
{

}

bool ACollectable::Multicast_Collected_Validate(ADBDPlayer* collector)
{
	return true;
}

bool ACollectable::IsStored() const
{
	return false;
}

bool ACollectable::IsRechargeable_Implementation() const
{
	return false;
}

bool ACollectable::IsOnGround() const
{
	return false;
}

bool ACollectable::IsEquipped() const
{
	return false;
}

bool ACollectable::IsCountDisplayForced_Implementation() const
{
	return false;
}

bool ACollectable::IsCollected() const
{
	return false;
}

bool ACollectable::HasGameplayModifierFlag(EGameplayModifierFlag flag) const
{
	return false;
}

float ACollectable::GetUsePercentLeft_Implementation() const
{
	return 0.0f;
}

float ACollectable::GetModifierSum(EGameplayModifierType Type, float DefaultValue) const
{
	return 0.0f;
}

float ACollectable::GetModifierMax(EGameplayModifierType Type, float DefaultValue) const
{
	return 0.0f;
}

UInteractor* ACollectable::GetItemInteractor() const
{
	return NULL;
}

uint8 ACollectable::GetItemIconIndex_Implementation() const
{
	return 0;
}

TArray<UItemAddon*> ACollectable::GetItemAddons()
{
	return TArray<UItemAddon*>();
}

int32 ACollectable::GetCount_Implementation() const
{
	return 0;
}

ADBDPlayer* ACollectable::GetCollector() const
{
	return NULL;
}

void ACollectable::DebugPrintStats_Implementation()
{

}

bool ACollectable::CanUse_Implementation() const
{
	return false;
}

void ACollectable::Authority_RemoveItemAddon(UItemAddon* addon)
{

}

void ACollectable::Authority_AddItemAddon(UItemAddon* addon)
{

}

void ACollectable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollectable, _collector);
	DOREPLIFETIME(ACollectable, _state);
	DOREPLIFETIME(ACollectable, _itemCount);
	DOREPLIFETIME(ACollectable, _begunPlayFenceName);
}

ACollectable::ACollectable()
{
	this->StrafeOnUse = false;
	this->AimOnUse = false;
	this->VisibleWhenEquipped = true;
	this->StopRunningOnUse = false;
	this->HandPosition = EItemHandPosition::None;
	this->HasUseInteraction = false;
	this->HasDropInteraction = false;
	this->Category = ECollectableCategory::Common;
	this->Draft = true;
	this->IsInUse = false;
	this->ItemId = NAME_None;
	this->FromPlayerSpawn = false;
	this->BaseItemModifier = CreateDefaultSubobject<UItemModifier>(TEXT("ItemModifier"));
	this->ItemModifier1 = CreateDefaultSubobject<UItemModifier>(TEXT("ItemModifier1"));
	this->ItemModifier2 = CreateDefaultSubobject<UItemModifier>(TEXT("ItemModifier2"));
	this->BeingCollected = false;
	this->BeingDropped = false;
	this->_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	this->_placementOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("PlacementOrigin"));
	this->_state = ECollectableState::OnGround;
	this->_itemCount = 1;
}

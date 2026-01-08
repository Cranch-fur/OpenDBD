#include "DBDMenuPlayer.h"
#include "EItemHandPosition.h"
#include "Components/SkeletalMeshComponent.h"

class AActor;

void ADBDMenuPlayer::WrappedOnDestroyed(AActor* DestroyedActor)
{

}

EItemHandPosition ADBDMenuPlayer::GetHandPosition() const
{
	return EItemHandPosition::None;
}

void ADBDMenuPlayer::BeginDestroySequence_Implementation()
{

}

ADBDMenuPlayer::ADBDMenuPlayer()
{
	this->RoleSelected = false;
	this->IsOnline = false;
	this->_handPosition = EItemHandPosition::None;
	this->ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	this->_equippedItemName = NAME_None;
}

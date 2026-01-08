#include "HexPerk.h"
#include "EDBDScoreTypes.h"
#include "Net/UnrealNetwork.h"

class AActor;
class ATotem;

void UHexPerk::OnInitialized_Internal()
{

}

void UHexPerk::OnGameplayEvent_Implementation(EDBDScoreTypes gameplayEventType, float amount, AActor* Instigator, AActor* target)
{

}

void UHexPerk::Multicast_SetTotemActor_Implementation(ATotem* aTotemActor)
{

}

bool UHexPerk::IsCurseRevealedToPlayer(int32 playerUniqueID) const
{
	return false;
}

void UHexPerk::FireCursedStatusOnLocalPlayer()
{

}

void UHexPerk::Authority_SetCurseRevealedToPlayer(int32 playerUniqueID, bool Value)
{

}

bool UHexPerk::Authority_BindToDullTotem()
{
	return false;
}

void UHexPerk::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHexPerk, _playersWhoKnowCurse);
}

UHexPerk::UHexPerk()
{

}

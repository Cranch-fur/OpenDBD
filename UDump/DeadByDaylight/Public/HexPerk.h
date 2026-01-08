#pragma once

#include "CoreMinimal.h"
#include "Perk.h"
#include "EDBDScoreTypes.h"
#include "HexPerk.generated.h"

class ATotem;
class AActor;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UHexPerk : public UPerk
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient)
	ATotem* TotemActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EDBDScoreTypes> BindTotemConditions;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	TArray<int32> _playersWhoKnowCurse;

public:
	UFUNCTION(BlueprintCallable)
	void OnInitialized_Internal();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGameplayEvent(EDBDScoreTypes gameplayEventType, float amount, AActor* Instigator, AActor* target);

private:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SetTotemActor(ATotem* aTotemActor);

protected:
	UFUNCTION(BlueprintPure)
	bool IsCurseRevealedToPlayer(int32 playerUniqueID) const;

	UFUNCTION(BlueprintCallable)
	void FireCursedStatusOnLocalPlayer();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SetCurseRevealedToPlayer(int32 playerUniqueID, bool Value);

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool Authority_BindToDullTotem();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UHexPerk();
};

FORCEINLINE uint32 GetTypeHash(const UHexPerk) { return 0; }

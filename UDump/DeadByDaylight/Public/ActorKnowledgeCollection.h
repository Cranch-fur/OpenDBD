#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EKnowledgeSharingType.h"
#include "ActorKnowledgeCollection.generated.h"

class AActor;
class ACharacter;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UActorKnowledgeCollection : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	TArray<AActor*> _actors;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	bool _available;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	ACharacter* _possessor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Transient, meta=(AllowPrivateAccess=true))
	EKnowledgeSharingType _sharing;

public:
	UFUNCTION(BlueprintPure)
	bool IsAvailable(const ACharacter* Character) const;

	UFUNCTION(BlueprintPure)
	bool Contains(AActor* Actor) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SetSharingType(EKnowledgeSharingType Value);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SetPossessor(ACharacter* possessor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_SetAvailable(bool Value);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_Remove(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_Empty();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Authority_Append(const TArray<AActor*>& Actors);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool Authority_Add(AActor* Actor);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UActorKnowledgeCollection();
};

FORCEINLINE uint32 GetTypeHash(const UActorKnowledgeCollection) { return 0; }

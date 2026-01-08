#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EItemHandPosition.h"
#include "DBDMenuPlayer.generated.h"

class USkeletalMeshComponent;
class AActor;

UCLASS(Blueprintable)
class DEADBYDAYLIGHT_API ADBDMenuPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RoleSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsOnline;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	EItemHandPosition _handPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FName _equippedItemName;

private:
	UFUNCTION(BlueprintCallable)
	void WrappedOnDestroyed(AActor* DestroyedActor);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RestartInSequence();

	UFUNCTION(BlueprintPure)
	EItemHandPosition GetHandPosition() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginDestroySequence();

public:
	ADBDMenuPlayer();
};

FORCEINLINE uint32 GetTypeHash(const ADBDMenuPlayer) { return 0; }

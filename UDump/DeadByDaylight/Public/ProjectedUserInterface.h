#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "ProjectedUserInterface.generated.h"

class URootMovie;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class AProjectedUserInterface : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ScreenClassName;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, meta=(AllowPrivateAccess=true))
	URootMovie* _rootMovie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FVector _projectionPanelSize;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateMouseInput();

	UFUNCTION(BlueprintCallable)
	void InitializeComponents(UStaticMeshComponent* projectionPanel);

public:
	AProjectedUserInterface();
};

FORCEINLINE uint32 GetTypeHash(const AProjectedUserInterface) { return 0; }

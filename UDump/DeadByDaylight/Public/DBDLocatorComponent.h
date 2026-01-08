#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DBDLocatorComponent.generated.h"

class UStaticMesh;
class UStaticMeshComponent;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DEADBYDAYLIGHT_API UDBDLocatorComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* EditorOnlyLocatorMesh;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Transient, Instanced, meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* _meshComponent;

public:
	UDBDLocatorComponent();
};

FORCEINLINE uint32 GetTypeHash(const UDBDLocatorComponent) { return 0; }

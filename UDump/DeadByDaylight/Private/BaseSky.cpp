#include "BaseSky.h"

class AActor;
class UPostProcessComponent;

void ABaseSky::SetCullDistance(float newCullDistance)
{

}

void ABaseSky::OnFadeEnd_Implementation(AActor* Other)
{

}

void ABaseSky::OnFadeBegin_Implementation(AActor* Other)
{

}

void ABaseSky::OnAlphaChanged_Implementation(float newAlpha, AActor* Other)
{

}

UPostProcessComponent* ABaseSky::GetPostprocessComponent_Implementation()
{
	return NULL;
}

ABaseSky::ABaseSky()
{
	this->AdjustCullDistance = false;
}

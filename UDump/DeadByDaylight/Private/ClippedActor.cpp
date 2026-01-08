#include "ClippedActor.h"


FClippedActor::FClippedActor()
{
	this->DisplayComponents = TArray<TWeakObjectPtr<USceneComponent>>();
	this->CollidingPrimitives = TMap<UPrimitiveComponent*, TWeakObjectPtr<UPrimitiveComponent>>();
}

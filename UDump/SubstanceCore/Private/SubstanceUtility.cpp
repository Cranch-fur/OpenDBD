#include "SubstanceUtility.h"
#include "ESubstanceTextureSize.h"

class USubstanceGraphInstance;
class USubstanceTexture2D;
class UMaterialInterface;
class UObject;
class USubstanceInstanceFactory;

void USubstanceUtility::SyncRendering(USubstanceGraphInstance* InstancesToRender)
{

}

void USubstanceUtility::SetGraphInstanceOutputSizeInt(USubstanceGraphInstance* GraphInstance, int32 Width, int32 Height)
{

}

void USubstanceUtility::SetGraphInstanceOutputSize(USubstanceGraphInstance* GraphInstance, ESubstanceTextureSize Width, ESubstanceTextureSize Height)
{

}

void USubstanceUtility::ResetInputParameters(USubstanceGraphInstance* GraphInstance)
{

}

TArray<USubstanceTexture2D*> USubstanceUtility::GetSubstanceTextures(USubstanceGraphInstance* GraphInstance)
{
	return TArray<USubstanceTexture2D*>();
}

TArray<USubstanceGraphInstance*> USubstanceUtility::GetSubstances(UMaterialInterface* Material)
{
	return TArray<USubstanceGraphInstance*>();
}

float USubstanceUtility::GetSubstanceLoadingProgress()
{
	return 0.0f;
}

FString USubstanceUtility::GetGraphName(USubstanceGraphInstance* GraphInstance)
{
	return TEXT("");
}

FString USubstanceUtility::GetFactoryName(USubstanceGraphInstance* GraphInstance)
{
	return TEXT("");
}

USubstanceGraphInstance* USubstanceUtility::DuplicateGraphInstance(UObject* WorldContextObject, USubstanceGraphInstance* GraphInstance)
{
	return NULL;
}

void USubstanceUtility::CreateInstanceOutputs(UObject* WorldContextObject, USubstanceGraphInstance* GraphInstance, TArray<int32> OutputIndices)
{

}

USubstanceGraphInstance* USubstanceUtility::CreateGraphInstance(UObject* WorldContextObject, USubstanceInstanceFactory* Factory, int32 GraphDescIndex, const FString& InstanceName)
{
	return NULL;
}

void USubstanceUtility::CopyInputParameters(USubstanceGraphInstance* SourceGraphInstance, USubstanceGraphInstance* DestGraphInstance)
{

}

void USubstanceUtility::AsyncRendering(USubstanceGraphInstance* InstancesToRender)
{

}

USubstanceUtility::USubstanceUtility()
{

}

#include "SubstanceGraphInstance.h"
#include "SubstanceIntInputDesc.h"
#include "SubstanceInstanceDesc.h"
#include "ESubstanceInputType.h"
#include "SubstanceFloatInputDesc.h"

class UObject;

void USubstanceGraphInstance::SetInputInt(const FString& Identifier, const TArray<int32>& Value)
{

}

bool USubstanceGraphInstance::SetInputImg(const FString& InputName, UObject* Value)
{
	return false;
}

void USubstanceGraphInstance::SetInputFloat(const FString& Identifier, const TArray<float>& InputValues)
{

}

FSubstanceIntInputDesc USubstanceGraphInstance::GetIntInputDesc(const FString& Identifier)
{
	return FSubstanceIntInputDesc{};
}

FSubstanceInstanceDesc USubstanceGraphInstance::GetInstanceDesc()
{
	return FSubstanceInstanceDesc{};
}

ESubstanceInputType USubstanceGraphInstance::GetInputType(const FString& InputName)
{
	return SIT_Float;
}

TArray<FString> USubstanceGraphInstance::GetInputNames()
{
	return TArray<FString>();
}

TArray<int32> USubstanceGraphInstance::GetInputInt(const FString& Identifier)
{
	return TArray<int32>();
}

TArray<float> USubstanceGraphInstance::GetInputFloat(const FString& Identifier)
{
	return TArray<float>();
}

FSubstanceFloatInputDesc USubstanceGraphInstance::GetFloatInputDesc(const FString& Identifier)
{
	return FSubstanceFloatInputDesc{};
}

USubstanceGraphInstance::USubstanceGraphInstance()
{
	this->bFreezed = false;
}

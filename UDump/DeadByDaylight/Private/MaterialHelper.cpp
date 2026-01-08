#include "MaterialHelper.h"
#include "UObject/NoExportTypes.h"

class UTexture;
class UMeshComponent;

void UMaterialHelper::UpdateMaterials()
{

}

void UMaterialHelper::SetVectorParameter(const FName ParameterName, const FVector& NewValue)
{

}

void UMaterialHelper::SetTextureParameter(const FName ParameterName, UTexture* NewTexture)
{

}

void UMaterialHelper::SetScalarParameter(const FName ParameterName, const float NewValue)
{

}

void UMaterialHelper::SetRenderInCustomDepth(bool RenderInCustomDepth)
{

}

void UMaterialHelper::SetRenderInCustomColourNoDepth(bool RenderInCustomColour)
{

}

void UMaterialHelper::SetReceivesDecals(bool ReceivesDecals)
{

}

void UMaterialHelper::SetMaterial(const FString& GroupName)
{

}

void UMaterialHelper::SetColourParameter(const FName ParameterName, const FLinearColor NewValue)
{

}

void UMaterialHelper::SetCastCinematicShadows(bool CastCinematicShadows)
{

}

void UMaterialHelper::ResetMaterials()
{

}

void UMaterialHelper::RefreshMeshes()
{

}

void UMaterialHelper::RefreshMesh(UMeshComponent* mc)
{

}

UMaterialHelper::UMaterialHelper()
{
	this->_materialGroupName = TEXT("");
}

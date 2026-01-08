#include "MaterialHelperOriginalMeshState.h"


FMaterialHelperOriginalMeshState::FMaterialHelperOriginalMeshState()
{
	this->_targetMesh = NULL;
	this->_originalMaterials = TArray<UMaterialInterface*>();
	this->_originalDynamicMaterials = TArray<UMaterialInterface*>();
}

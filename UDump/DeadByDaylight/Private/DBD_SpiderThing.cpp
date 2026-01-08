#include "DBD_SpiderThing.h"
#include "Engine/HitResult.h"
#include "Components/InstancedStaticMeshComponent.h"

void ADBD_SpiderThing::RebuildLines()
{

}

void ADBD_SpiderThing::RandomTrace(FHitResult& out_result)
{

}

void ADBD_SpiderThing::CreateLine()
{

}

ADBD_SpiderThing::ADBD_SpiderThing()
{
	this->_instancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ColourRenderMeshes"));
	this->MaximumRadius = 100.000000;
	this->ThicknessY = 0.001000;
	this->ThicknessZ = 0.015000;
	this->MaxInstances = 100;
	this->InitialSpawnAmount = 50;
	this->AngleRangeDegrees = 70.000000;
	this->DecalInstanceScale = 5.000000;
}

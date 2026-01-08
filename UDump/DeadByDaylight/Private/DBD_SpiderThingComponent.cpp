#include "DBD_SpiderThingComponent.h"
#include "Engine/HitResult.h"

void UDBD_SpiderThingComponent::RebuildLines()
{

}

void UDBD_SpiderThingComponent::RandomTrace(FHitResult& out_result)
{

}

void UDBD_SpiderThingComponent::CreateLine()
{

}

UDBD_SpiderThingComponent::UDBD_SpiderThingComponent()
{
	this->MaximumRadius = 100.000000;
	this->MaximumStickLength = -1.000000;
	this->ThicknessY = 0.001000;
	this->ThicknessZ = 0.015000;
	this->ThicknessYRange = 0.000000;
	this->ThicknessZRange = 0.000000;
	this->MaxInstances = 100;
	this->InitialSpawnAmount = 50;
	this->AngleRangeDegrees = 70.000000;
	this->DecalMinDetailMode = DM_High;
	this->DecalInstanceScale = 5.000000;
}

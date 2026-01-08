#include "ServerJoiningData.h"


FServerJoiningData::FServerJoiningData()
{
	this->SessionId = 0;
	this->SessionIdStr = TEXT("");
	this->Step = EServerJoiningDataType::Idle;
	this->Settings = TArray<FDBDJoinParamSetting>();
}

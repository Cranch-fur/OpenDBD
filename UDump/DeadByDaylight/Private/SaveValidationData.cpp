#include "SaveValidationData.h"


FSaveValidationData::FSaveValidationData()
{
	this->PlayerUID = TEXT("");
	this->CharacterData = TMap<int32, FCharacterValidationData>();
}

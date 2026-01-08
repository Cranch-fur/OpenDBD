#include "MapActorDB.h"
#include "EMapActorCategory.h"

class AActor;

TArray<AActor*> UMapActorDB::GetUniqueRandom(const TArray<EMapActorCategory>& Categories, int32 Count) const
{
	return TArray<AActor*>();
}

AActor* UMapActorDB::GetRandom(EMapActorCategory Category) const
{
	return NULL;
}

UMapActorDB::UMapActorDB()
{

}

#include "RitualEvaluatorBase.h"
#include "EDBDScoreTypes.h"

class AActor;

bool URitualEvaluatorBase::EvaluateEvent_Implementation(EDBDScoreTypes event, AActor* Instigator, AActor* target)
{
	return false;
}

URitualEvaluatorBase::URitualEvaluatorBase()
{

}

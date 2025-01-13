// 


#include "PatrolPath.h"


APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;
}

FVector APatrolPath::GetPatrolPoint(int index)
{
	return PatrolPoints[index];
}

int APatrolPath::Num()
{
	return PatrolPoints.Num();
}


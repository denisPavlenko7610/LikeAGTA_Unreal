// 


#include "BTTask_FindPathPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "VehVarVol2_UE5/Characters/NPC/NPC.h"
#include "VehVarVol2_UE5/Characters/NPC/AI/NPC_AIController.h"
#include "VehVarVol2_UE5/Characters/NPC/AI/PatrolPath/PatrolPath.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint()
{
	NodeName = TEXT("Find Path point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPC_AIController* controller = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	if (!IsValid(controller))
		return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComponent))
		return EBTNodeResult::Failed;

	int index = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());
	ANPC* npc = Cast<ANPC>(controller->GetPawn());
	if (!IsValid(npc))
		return EBTNodeResult::Failed;

	FVector Point = npc->GetPatrolPath()->GetPatrolPoint(index);
	UE::Math::TVector<double> GlobalPoint = npc->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
	BlackboardComponent->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

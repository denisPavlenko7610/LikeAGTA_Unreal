// 


#include "BTTask_IncrementPathIndex.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "VehVarVol2_UE5/Characters/NPC/NPC.h"
#include "VehVarVol2_UE5/Characters/NPC/AI/NPC_AIController.h"
#include "VehVarVol2_UE5/Characters/NPC/AI/PatrolPath/PatrolPath.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex()
{
	NodeName = "Increment path index";
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPC_AIController* controller = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	if (!IsValid(controller))
		return EBTNodeResult::Failed;
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComponent))
		return EBTNodeResult::Failed;

	ANPC* npc = Cast<ANPC>(controller->GetPawn());
	if (!IsValid(npc))
		return EBTNodeResult::Failed;

	int numOfPoints = npc->GetPatrolPath()->Num();
	int minIndex = 0;
	int maxIndex = numOfPoints - 1;
	int index = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());

	if (biDirection)
	{
		if (index >= maxIndex && Direction == EDirectionType::Forward)
		{
			Direction = EDirectionType::Reverse;
		} else if (index == minIndex && Direction == EDirectionType::Reverse)
		{
			Direction = EDirectionType::Forward;
		}
	}

	int newIndex = Direction == EDirectionType::Forward
		? ++index
		: --index;
	
	BlackboardComponent->SetValueAsInt(GetSelectedBlackboardKey(), newIndex % numOfPoints);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

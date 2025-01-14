// 


#include "UBTTask_ChasePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "VehVarVol2_UE5/Characters/NPC/AI/NPC_AIController.h"

UUBTTask_ChasePlayer::UUBTTask_ChasePlayer()
{
	NodeName = TEXT("Chase player");
}

EBTNodeResult::Type UUBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ANPC_AIController* controller = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		FVector playerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, playerLocation);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

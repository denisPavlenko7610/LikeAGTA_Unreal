// 


#include "BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "VehVarVol2_UE5/Characters/NPC/AI/NPC_AIController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location in NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	if (ANPC_AIController* controller = Cast<ANPC_AIController>(OwnerComponent.GetAIOwner()))
	{
		if (APawn* npc = controller->GetPawn())
		{
			FVector origin = npc->GetActorLocation();
			if (UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation resultLocation;
				if (navSystem->GetRandomPointInNavigableRadius(origin, SearchRadius, resultLocation))
				{
					OwnerComponent.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), resultLocation.Location);
				}

				FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}

// 


#include "BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "VehVarVol2_UE5/Characters/NPC/AI/NPC_AIController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer) : Super(
	ObjectInitializer)
{
	NodeName = "Find Random Location in NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	ANPC_AIController* Controller = Cast<ANPC_AIController>(OwnerComponent.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	APawn* Npc = Controller->GetPawn();
	if (!Npc)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
		return EBTNodeResult::Failed;

	FNavLocation ResultLocation;
	FVector Origin = Npc->GetActorLocation();
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, _searchRadius, ResultLocation))
	{
		OwnerComponent.GetBlackboardComponent()->SetValueAsVector(
			GetSelectedBlackboardKey(), ResultLocation.Location);
	}

	FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

// 


#include "BTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation()
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!IsValid(Player))
		return EBTNodeResult::Failed;


	FVector TargetLocation = Player->GetActorLocation();
	if (SearchRandom)
	{
		FNavLocation NavLocation;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(TargetLocation, SearchRadius, NavLocation))
		{
			TargetLocation = NavLocation.Location;
		}
		else
		{
			FinishLatentTask(OwnerComponent, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
	}
	
	OwnerComponent.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), TargetLocation);
	FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

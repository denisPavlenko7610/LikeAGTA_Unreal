// 


#include "BTDecorator_CheckPlayerInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTDecorator_CheckPlayerInRange::UBTDecorator_CheckPlayerInRange()
{
	NodeName = "Check Player In Range";
}

bool UBTDecorator_CheckPlayerInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return false;
	
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
		return false;
	
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(AIPawn->GetWorld(), 0);
	if (!PlayerCharacter)
		return false;
	
	float DistanceToPlayer = FVector::Dist(AIPawn->GetActorLocation(), PlayerCharacter->GetActorLocation());
	bool bIsInRange = DistanceToPlayer <= Range;
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsBool(CanFireKey.SelectedKeyName, bIsInRange);
	}

	return bIsInRange;
}

// 


#include "BTService_ChangeSpeed.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VehVarVol2_UE5/Characters/NPC/NPC.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = "Change speed";
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return;

	auto NPC = Cast<ANPC>(AIController->GetPawn());
	if (!NPC)
		return;

	NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;
	
}

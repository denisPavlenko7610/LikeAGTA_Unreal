// 


#include "BTService_IsPlayerInFireAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "VehVarVol2_UE5/Characters/NPC/NPC.h"
#include "VehVarVol2_UE5/Characters/NPC/AI/NPC_AIController.h"

UBTService_IsPlayerInFireAttack::UBTService_IsPlayerInFireAttack()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is player in fire attack");
}

void UBTService_IsPlayerInFireAttack::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPC_AIController* NPCAIController = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	if (!NPCAIController)
		return;

	ANPC* NPC = Cast<ANPC>(NPCAIController->GetPawn());
	if (!NPC)
		return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), NPC->GetDistanceTo(Player) <= FireRange);
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

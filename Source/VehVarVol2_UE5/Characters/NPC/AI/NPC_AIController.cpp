
#include "NPC_AIController.h"

#include "../NPC.h"
#include "BehaviorTree/BehaviorTree.h"

inline ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitializer)
{
}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPC* npc = Cast<ANPC>(InPawn))
	{
		if (UBehaviorTree* tree = npc->GetBehaviourTree())
		{
			UBlackboardComponent* blackboardComponent;
			UseBlackboard(tree->BlackboardAsset, blackboardComponent);
			Blackboard = blackboardComponent;
			RunBehaviorTree(tree);
		}
	}
}

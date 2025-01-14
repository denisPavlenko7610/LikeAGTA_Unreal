#include "NPC_AIController.h"

#include "../NPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "VehVarVol2_UE5/Characters/Player/APlayerCharacter.h"

inline ANPC_AIController::ANPC_AIController()
{
	SetupPerceptionSystem();
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

void ANPC_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));

	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void ANPC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
	if (!Player)
	    return;

	GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
}

#include "NPC_AIController.h"

#include "../NPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "VehVarVol2_UE5/Characters/Player/APlayerCharacter.h"

inline ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitializer)
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
	_sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));

	_sightConfig->SightRadius = 500.f;
	_sightConfig->LoseSightRadius = _sightConfig->SightRadius + 25.f;
	_sightConfig->PeripheralVisionAngleDegrees = 90.f;
	_sightConfig->SetMaxAge(5.f);
	_sightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
	_sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	_sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	_sightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*_sightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*_sightConfig);
}

void ANPC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
	if (!Player)
	    return;

	GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
}

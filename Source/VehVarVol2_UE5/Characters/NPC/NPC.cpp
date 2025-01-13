// 


#include "NPC.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "VehVarVol2_UE5/Components/WeaponComponent.h"


ANPC::ANPC(FObjectInitializer const& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	SetWeaponComponent(CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent")));
	GetWeaponComponent()->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	GetWeaponComponent()->SetHiddenInGame(true);

	SetupStimulusSource();
}

void ANPC::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimulusSource->RegisterWithPerceptionSystem();
}

UBehaviorTree* ANPC::GetBehaviourTree()
{
	return Tree;
}

APatrolPath* ANPC::GetPatrolPath()
{
	return PatrolPath;
}



// 


#include "NPC.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "VehVarVol2_UE5/Components/WeaponComponent.h"


ANPC::ANPC(FObjectInitializer const& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	WeaponComponent->SetHiddenInGame(true);

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



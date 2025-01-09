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
	_stimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	_stimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	_stimulusSource->RegisterWithPerceptionSystem();
}

UBehaviorTree* ANPC::GetBehaviourTree()
{
	return _tree;
}



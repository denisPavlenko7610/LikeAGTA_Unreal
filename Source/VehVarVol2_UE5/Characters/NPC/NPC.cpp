// 


#include "NPC.h"

#include "InputActionValue.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "VehVarVol2_UE5/Components/WeaponComponent.h"


ANPC::ANPC()
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

void ANPC::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	GetWeaponComponent()->Init(this);
}

int ANPC::FireAttack_Implementation()
{
	FInputActionValue InputActionValue = FInputActionValue();
	WeaponComponent->StartFireMontage(InputActionValue);
	return 0;
}



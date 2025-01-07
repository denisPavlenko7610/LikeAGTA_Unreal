// 


#include "NPC.h"

#include "VehVarVol2_UE5/Components/WeaponComponent.h"


ANPC::ANPC(FObjectInitializer const& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	WeaponComponent->SetHiddenInGame(true);
}

UBehaviorTree* ANPC::GetBehaviourTree()
{
	return Tree;
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}


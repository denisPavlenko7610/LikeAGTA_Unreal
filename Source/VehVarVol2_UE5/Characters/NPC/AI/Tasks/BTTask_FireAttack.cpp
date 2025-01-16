// 


#include "BTTask_FireAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "VehVarVol2_UE5/Characters/NPC/NPC.h"
#include "VehVarVol2_UE5/Components/WeaponComponent.h"

UBTTask_FireAttack::UBTTask_FireAttack()
{
	NodeName = TEXT("Fire attack");
}

EBTNodeResult::Type UBTTask_FireAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;

	ANPC* NPC = Cast<ANPC>(AIController->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	IICombat* ICombat = Cast<IICombat>(NPC);
	if (!ICombat)
		return EBTNodeResult::Failed;

	UWeaponComponent* WeaponComponent = NPC->GetWeaponComponent();
	if (CanFire(NPC))
	{
		WeaponComponent->SetHiddenInGame(false);
		WeaponComponent->SetAimingWeapon(true);
		WeaponComponent->SetWeaponEquipped(true);
		ICombat->Execute_FireAttack(NPC);
	}
	else
	{
		WeaponComponent->SetHiddenInGame(true);
		WeaponComponent->SetAimingWeapon(false);
		WeaponComponent->SetWeaponEquipped(false);
	}

	FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UBTTask_FireAttack::CanFire(ANPC* NPC)
{
	UAnimInstance* AnimInstance = NPC->GetMesh()->GetAnimInstance();
	UAnimMontage* Montage = NPC->GetWeaponComponent()->GetFireMontage();
	return  AnimInstance && !AnimInstance->Montage_IsPlaying(Montage);
}

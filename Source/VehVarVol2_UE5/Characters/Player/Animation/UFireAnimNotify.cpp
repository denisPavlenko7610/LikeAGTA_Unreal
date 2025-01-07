#include "UFireAnimNotify.h"

#include "Components/SkeletalMeshComponent.h"
#include "VehVarVol2_UE5/Characters/Player/APlayerCharacter.h"
#include "VehVarVol2_UE5/Components/WeaponComponent.h"

void UFireAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& NotifyEventReference)
{
	Super::Notify(MeshComp, Animation, NotifyEventReference);
	
	Fire(MeshComp);
}

void UFireAnimNotify::Fire(USkeletalMeshComponent* skeletalMesh)
{
	if (skeletalMesh->GetOwner() == nullptr)
		return;
	
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(skeletalMesh->GetOwner()))
	{
		PlayerCharacter->getWeaponComponent()->Fire();
	}
}

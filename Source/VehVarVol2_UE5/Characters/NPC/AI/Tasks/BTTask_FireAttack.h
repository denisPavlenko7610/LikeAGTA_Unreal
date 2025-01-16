// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "VehVarVol2_UE5/Characters/NPC/NPC.h"
#include "BTTask_FireAttack.generated.h"

/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API UBTTask_FireAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FireAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;

private:
	bool CanFire(ANPC* NPC);
};

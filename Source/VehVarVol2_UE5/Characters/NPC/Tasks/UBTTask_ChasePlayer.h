// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UBTTask_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API UUBTTask_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

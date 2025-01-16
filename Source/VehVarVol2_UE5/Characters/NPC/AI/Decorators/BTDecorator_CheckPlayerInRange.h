// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CheckPlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API UBTDecorator_CheckPlayerInRange : public UBTDecorator_BlackboardBase
{
public:
	GENERATED_BODY()
	UBTDecorator_CheckPlayerInRange();

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float Range = 300.0f;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CanFireKey;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

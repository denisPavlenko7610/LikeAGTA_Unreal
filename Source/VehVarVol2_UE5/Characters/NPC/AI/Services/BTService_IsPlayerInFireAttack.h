// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsPlayerInFireAttack.generated.h"

/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API UBTService_IsPlayerInFireAttack : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_IsPlayerInFireAttack();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	float FireRange = 300.f;
};

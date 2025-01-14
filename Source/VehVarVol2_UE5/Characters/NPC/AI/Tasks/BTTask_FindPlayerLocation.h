// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API UBTTask_FindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FindPlayerLocation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Search", meta=(AllowPrivateAccess="true"))
	bool SearchRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Search", meta=(AllowPrivateAccess="true"))
	float SearchRadius = 200.f;
};

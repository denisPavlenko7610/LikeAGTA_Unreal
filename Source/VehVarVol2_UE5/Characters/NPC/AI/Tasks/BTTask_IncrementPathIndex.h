// 

#pragma once

#include "CoreMinimal.h"
#include "EDirectionType.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_IncrementPathIndex.generated.h"

enum class EDirectionType;
/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API UBTTask_IncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_IncrementPathIndex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	EDirectionType Direction = EDirectionType::Forward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	bool biDirection = false;
};

// 

#pragma once

#include "CoreMinimal.h"
#include "VehVarVol2_UE5/Characters/CharacterBase.h"
#include "NPC.generated.h"

class APatrolPath;
class UAIPerceptionStimuliSourceComponent;
class UBehaviorTree;

UCLASS()
class VEHVARVOL2_UE5_API ANPC : public ACharacterBase
{
	GENERATED_BODY()

public:
	ANPC(FObjectInitializer const& ObjectInitializer);
	
	UBehaviorTree* GetBehaviourTree();
	APatrolPath* GetPatrolPath();

private:
	void SetupStimulusSource();

	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* StimulusSource;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	APatrolPath* PatrolPath;
};

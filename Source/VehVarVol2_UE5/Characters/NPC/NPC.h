// 

#pragma once

#include "CoreMinimal.h"
#include "VehVarVol2_UE5/Characters/CharacterBase.h"
#include "NPC.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UBehaviorTree;

UCLASS()
class VEHVARVOL2_UE5_API ANPC : public ACharacterBase
{
	GENERATED_BODY()

public:
	ANPC(FObjectInitializer const& ObjectInitializer);
	
	UBehaviorTree* GetBehaviourTree();

private:
	void SetupStimulusSource();

	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* _stimulusSource;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* _tree;
};

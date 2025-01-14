// 

#pragma once

#include "CoreMinimal.h"
#include "ICombat.h"
#include "VehVarVol2_UE5/Characters/CharacterBase.h"
#include "NPC.generated.h"

class APatrolPath;
class UAIPerceptionStimuliSourceComponent;
class UBehaviorTree;

UCLASS()
class VEHVARVOL2_UE5_API ANPC : public ACharacterBase, public IICombat
{
	GENERATED_BODY()

public:
	ANPC();
	
	UBehaviorTree* GetBehaviourTree();
	APatrolPath* GetPatrolPath();
	float GetLastFireTime() const { return _lastFireTime; }
	void SetLastFireTime(float value) { _lastFireTime = value; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual int FireAttack_Implementation() override;

private:
	void SetupStimulusSource();

	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* StimulusSource;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	APatrolPath* PatrolPath;
	
	float _lastFireTime;
};

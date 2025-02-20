

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPC_AIController.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()
public:
	ANPC_AIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};



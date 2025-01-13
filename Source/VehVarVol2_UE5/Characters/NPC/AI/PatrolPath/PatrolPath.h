// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class VEHVARVOL2_UE5_API APatrolPath : public AActor
{
	GENERATED_BODY()

public:
	APatrolPath();

	FVector GetPatrolPoint(int index);
	int Num();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TArray<FVector> PatrolPoints;
};

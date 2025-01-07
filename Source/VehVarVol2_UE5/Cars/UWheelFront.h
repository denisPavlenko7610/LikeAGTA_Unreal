#pragma once
#include "CoreMinimal.h"
#include "ChaosVehicleWheel.h"
#include "UWheelFront.generated.h"


UCLASS()
class UWheelFront : public UChaosVehicleWheel
{
	GENERATED_BODY()
public:
	UWheelFront(FObjectInitializer const& ObjectInitializer);
};

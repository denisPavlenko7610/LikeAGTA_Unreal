// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VehicleInteraction.generated.h"

class APlayerCharacter;
class ACar;
/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API UVehicleInteraction : public UObject {
	GENERATED_BODY()

public:
	UVehicleInteraction(FObjectInitializer const& ObjectInitializer);
	
	void EnterVehicle(ACar* Vehicle);
	void ExitVehicle();
	void Init(APlayerCharacter* playerCharacter);

private:
	UPROPERTY()
	ACar* _currentVehicle;

	UPROPERTY()
	APlayerCharacter* _playerCharacter;

	FVector _exitOffset;
};

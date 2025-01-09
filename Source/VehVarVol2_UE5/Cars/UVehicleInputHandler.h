#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UVehicleInputHandler.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UEnhancedInputComponent;
class ACar;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VEHVARVOL2_UE5_API UVehicleInputHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	void SetupInput(UInputComponent* PlayerInputComponent, ACar* Car);

	UInputMappingContext* GetCarMappingContext() { return _carMappingContext; }

private:
	void Throttle(const FInputActionValue& Value);
	void Steering(const FInputActionValue& Value);
	void Brake(const FInputActionValue& Value);
	void StartHandbrake(const FInputActionValue& Value);
	void StopHandbrake(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* _carMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _enterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _throttleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _brakeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _handbrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _lookAroundAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _lookUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _steeringAction;
	
	UPROPERTY()
	ACar* _car;
};
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
	void SetupInput(UInputComponent* playerInputComponent, ACar* car);

	UInputMappingContext* GetCarMappingContext() { return CarMappingContext; }

private:
	void Throttle(const FInputActionValue& Value);
	void Steering(const FInputActionValue& Value);
	void Brake(const FInputActionValue& Value);
	void StartHandbrake(const FInputActionValue& Value);
	void StopHandbrake(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* CarMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EnterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BrakeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HandbrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAroundAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SteeringAction;
	
	UPROPERTY()
	ACar* Car;
};
#include "UVehicleInputHandler.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "ACar.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void UVehicleInputHandler::SetupInput(UInputComponent* PlayerInputComponent, ACar* Car)
{
    _car = Car;
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Started, Car, &ACar::UnpossessVehicle);

        EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::Steering);
        EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &UVehicleInputHandler::Steering);

        EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::Throttle);
        EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &UVehicleInputHandler::Throttle);

        EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::Brake);

        EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &UVehicleInputHandler::StartHandbrake);
        EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &UVehicleInputHandler::StopHandbrake);

        EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::LookAround);
        EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::LookUp);
    }
}

void UVehicleInputHandler::Throttle(const FInputActionValue& Value)
{
    float ThrottleValue = Value.Get<float>();
    _car->VehicleMovement->SetThrottleInput(ThrottleValue);
}

void UVehicleInputHandler::Steering(const FInputActionValue& Value)
{
    float SteeringValue = Value.Get<float>();
    _car->VehicleMovement->SetSteeringInput(SteeringValue);
}

void UVehicleInputHandler::Brake(const FInputActionValue& Value)
{
    float BrakeValue = Value.Get<float>();
    _car->VehicleMovement->SetBrakeInput(BrakeValue);
}

void UVehicleInputHandler::StartHandbrake(const FInputActionValue& Value)
{
    _car->VehicleMovement->SetHandbrakeInput(true);
}

void UVehicleInputHandler::StopHandbrake(const FInputActionValue& Value)
{
    _car->VehicleMovement->SetHandbrakeInput(false);
}

void UVehicleInputHandler::LookAround(const FInputActionValue& Value)
{
    // Implement look around functionality
}

void UVehicleInputHandler::LookUp(const FInputActionValue& Value)
{
    // Implement look up functionality
}
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
        EnhancedInputComponent->BindAction(_enterAction, ETriggerEvent::Started, Car, &ACar::UnpossessVehicle);

        EnhancedInputComponent->BindAction(_steeringAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::Steering);
        EnhancedInputComponent->BindAction(_steeringAction, ETriggerEvent::Completed, this, &UVehicleInputHandler::Steering);

        EnhancedInputComponent->BindAction(_throttleAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::Throttle);
        EnhancedInputComponent->BindAction(_throttleAction, ETriggerEvent::Completed, this, &UVehicleInputHandler::Throttle);

        EnhancedInputComponent->BindAction(_brakeAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::Brake);

        EnhancedInputComponent->BindAction(_handbrakeAction, ETriggerEvent::Started, this, &UVehicleInputHandler::StartHandbrake);
        EnhancedInputComponent->BindAction(_handbrakeAction, ETriggerEvent::Completed, this, &UVehicleInputHandler::StopHandbrake);

        EnhancedInputComponent->BindAction(_lookAroundAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::LookAround);
        EnhancedInputComponent->BindAction(_lookUpAction, ETriggerEvent::Triggered, this, &UVehicleInputHandler::LookUp);
    }
}

void UVehicleInputHandler::Throttle(const FInputActionValue& Value)
{
    float ThrottleValue = Value.Get<float>();
    _car->GetVehicleMovementComponent()->SetThrottleInput(ThrottleValue);
}

void UVehicleInputHandler::Steering(const FInputActionValue& Value)
{
    float SteeringValue = Value.Get<float>();
    _car->GetVehicleMovementComponent()->SetSteeringInput(SteeringValue);
}

void UVehicleInputHandler::Brake(const FInputActionValue& Value)
{
    float BrakeValue = Value.Get<float>();
    _car->GetVehicleMovementComponent()->SetBrakeInput(BrakeValue);
}

void UVehicleInputHandler::StartHandbrake(const FInputActionValue& Value)
{
    _car->GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void UVehicleInputHandler::StopHandbrake(const FInputActionValue& Value)
{
    _car->GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void UVehicleInputHandler::LookAround(const FInputActionValue& Value)
{
    // Implement look around functionality
}

void UVehicleInputHandler::LookUp(const FInputActionValue& Value)
{
    // Implement look up functionality
}
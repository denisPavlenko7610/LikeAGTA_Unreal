// 


#include "VehicleInteraction.h"

#include "GameFramework/PawnMovementComponent.h"
#include "VehVarVol2_UE5/Cars/ACar.h"
#include "VehVarVol2_UE5/Characters/Player/APlayerCharacter.h"

UVehicleInteraction::UVehicleInteraction(FObjectInitializer const& ObjectInitializer): _playerCharacter(nullptr)
{
	_currentVehicle = nullptr;
	_exitOffset = FVector(0.f, -200.f, 0.f);
}

void UVehicleInteraction::Init(APlayerCharacter* playerCharacter)
{
	_playerCharacter = playerCharacter;
}

void UVehicleInteraction::EnterVehicle(ACar* Vehicle)
{
	if (!Vehicle)
		return;

	_currentVehicle = Vehicle;
	_playerCharacter->GetMovementComponent()->StopMovementImmediately();
	_playerCharacter->SetActorHiddenInGame(true);
	_playerCharacter->SetActorEnableCollision(false);
	Vehicle->PossessVehicle(_playerCharacter);
}

void UVehicleInteraction::ExitVehicle()
{
	if (!_currentVehicle)
		return;

	_playerCharacter->SetActorHiddenInGame(false);
	_playerCharacter->SetActorEnableCollision(true);

	FVector ExitLocation = _currentVehicle->GetActorLocation() + _currentVehicle->GetActorRotation().RotateVector(_exitOffset);
	_playerCharacter->SetActorLocation(ExitLocation);
	_playerCharacter->SetActorRotation(_currentVehicle->GetActorRotation());
	_currentVehicle = nullptr;
}

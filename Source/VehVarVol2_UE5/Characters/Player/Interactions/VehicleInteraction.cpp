// 


#include "VehicleInteraction.h"

#include "GameFramework/PawnMovementComponent.h"
#include "VehVarVol2_UE5/Cars/ACar.h"
#include "VehVarVol2_UE5/Characters/Player/APlayerCharacter.h"

UVehicleInteraction::UVehicleInteraction(FObjectInitializer const& ObjectInitializer): PlayerCharacter(nullptr)
{
	CurrentVehicle = nullptr;
	_exitOffset = FVector(0.f, -200.f, 0.f);
}

void UVehicleInteraction::Init(APlayerCharacter* playerCharacter)
{
	PlayerCharacter = playerCharacter;
}

void UVehicleInteraction::EnterVehicle(ACar* Vehicle)
{
	if (!Vehicle)
		return;

	CurrentVehicle = Vehicle;
	PlayerCharacter->GetMovementComponent()->StopMovementImmediately();
	PlayerCharacter->SetActorHiddenInGame(true);
	PlayerCharacter->SetActorEnableCollision(false);
	Vehicle->PossessVehicle(PlayerCharacter);
}

void UVehicleInteraction::ExitVehicle()
{
	if (!CurrentVehicle)
		return;

	PlayerCharacter->SetActorHiddenInGame(false);
	PlayerCharacter->SetActorEnableCollision(true);

	FVector ExitLocation = CurrentVehicle->GetActorLocation() + CurrentVehicle->GetActorRotation().RotateVector(_exitOffset);
	PlayerCharacter->SetActorLocation(ExitLocation);
	PlayerCharacter->SetActorRotation(CurrentVehicle->GetActorRotation());
	CurrentVehicle = nullptr;
}

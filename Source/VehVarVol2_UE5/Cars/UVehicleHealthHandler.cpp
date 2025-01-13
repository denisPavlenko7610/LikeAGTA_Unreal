#include "UVehicleHealthHandler.h"
#include "Engine/Engine.h"
#include "ACar.h"
#include "UVehicleVFX.h"
#include "VehVarVol2_UE5/Components/HealthComponent.h"

void UVehicleHealthHandler::Initialize(ACar* car, UHealthComponent* healthComponent)
{
	Car = car;
	if (healthComponent)
	{
		healthComponent->OnHealthChanged.AddDynamic(this, &UVehicleHealthHandler::OnHealthChanged);
	}
}

void UVehicleHealthHandler::OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	FString HealthMessage = FString::Printf(TEXT("Current health is: %f"), Health);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, HealthMessage);

	if (Health <= 0)
	{
		//Car->Destroy();
		return;
	}

	if (Health <= _healthLevelToSpawnFire)
		Car->GetVehicleVFX()->SpawnFireEffect();
	else if (Health <= _healthLevelToSpawnSmoke)
		Car->GetVehicleVFX()->SpawnSmokeEffect();
}
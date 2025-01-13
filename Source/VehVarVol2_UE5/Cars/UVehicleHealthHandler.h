#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UVehicleHealthHandler.generated.h"

class UHealthComponent;
class ACar;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VEHVARVOL2_UE5_API UVehicleHealthHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	void Initialize(ACar* Car, UHealthComponent* healthComponent);

private:
	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY()
	ACar* Car;
	
	float _healthLevelToSpawnSmoke = 50.0f;
	float _healthLevelToSpawnFire = 20.0f;
};
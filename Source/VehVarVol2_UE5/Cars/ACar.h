#pragma once

#include "CoreMinimal.h"
#include "EMappingContextType.h"
#include "WheeledVehiclePawn.h"
#include "ACar.generated.h"

class UVehicleVfx;
class UHealthComponent;
class APlayerCharacter;
class UInputAction;
class UInputMappingContext;
class USphereComponent;
class UChaosWheeledVehicleMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class UVehicleInputHandler;
class UVehicleHealthHandler;

UCLASS(Abstract)
class VEHVARVOL2_UE5_API ACar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ACar(FObjectInitializer const& ObjectInitializer);
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* CarMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionRadius = 250.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* VehicleCamera;

	UPROPERTY()
	UChaosWheeledVehicleMovementComponent* VehicleMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UVehicleVfx* VehicleVfx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UVehicleInputHandler* InputHandler;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UVehicleHealthHandler* HealthHandler;

	UPROPERTY(EditAnywhere)
	USceneComponent* FireEffectPosition;

	void PossessVehicle(APlayerCharacter* PlayerCharacter);
	void UnpossessVehicle();
	void ChangeMappingContext(EMappingContextType ContextType);
	void SetVehicleCameraActive(bool bActive);

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void SetupVehicleComponent();
	bool CanAllowExit();

	UPROPERTY()
	APlayerCharacter* _playerCharacter;
	
	float _speedExitLimit = 120.f;
};
#pragma once

#include "CoreMinimal.h"
#include "EMappingContextType.h"
#include "WheeledVehiclePawn.h"
#include "ACar.generated.h"

class UVehicleVfx;
class UHealthComponent;
class APlayerCharacter;
class UInputAction;
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

	void PossessVehicle(APlayerCharacter* PlayerCharacter);
	void UnpossessVehicle();
	void ChangeMappingContext(EMappingContextType ContextType);
	void SetVehicleCameraActive(bool bActive);

	USceneComponent* GetFireParticlePosition() { return _fireEffectPosition; }
	UChaosWheeledVehicleMovementComponent* GetVehicleMovementComponent() { return _vehicleMovement; }
	UVehicleVfx* GetVehicleVFX() { return _vehicleVfx; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void SetupVehicleComponent();
	bool CanAllowExit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta=(AllowPrivateAccess="true"))
	USphereComponent* _collisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta=(AllowPrivateAccess="true"))
	float _collisionRadius = 250.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	UHealthComponent* _healthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* _springArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess="true"))
	UCameraComponent* _vehicleCamera;

	UPROPERTY()
	UChaosWheeledVehicleMovementComponent* _vehicleMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UVehicleVfx* _vehicleVfx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UVehicleInputHandler* _inputHandler;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UVehicleHealthHandler* _healthHandler;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	USceneComponent* _fireEffectPosition;

	UPROPERTY()
	APlayerCharacter* _playerCharacter;
	
	float _speedExitLimit = 120.f;
};
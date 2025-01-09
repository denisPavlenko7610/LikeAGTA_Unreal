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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta=(AllowPrivateAccess="true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta=(AllowPrivateAccess="true"))
	float CollisionRadius = 250.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess="true"))
	UCameraComponent* VehicleCamera;

	UPROPERTY()
	UChaosWheeledVehicleMovementComponent* VehicleMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UVehicleVfx* VehicleVfx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UVehicleInputHandler* InputHandler;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UVehicleHealthHandler* HealthHandler;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
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
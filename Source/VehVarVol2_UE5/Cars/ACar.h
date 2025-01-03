#pragma once

#include "CoreMinimal.h"
#include "EMappingContextType.h"
#include "WheeledVehiclePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ACar.generated.h"

struct FChaosWheelSetup;
struct FInputActionValue;
class UHealthComponent;
class APlayerCharacter;
class UInputAction;
class UInputMappingContext;
class USphereComponent;
class UChaosWheeledVehicleMovementComponent;

UCLASS(Abstract)
class VEHVARVOL2_UE5_API ACar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ACar();
	virtual void BeginPlay() override;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* _collisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionRadius = 250.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* _healthComponent;

	UPROPERTY(EditAnywhere, Category = "FireSettings")
	UParticleSystem* SmokeParticleTemplate;

	UPROPERTY(EditAnywhere, Category = "FireSettings")
	UParticleSystem* FireParticleTemplate;

	UPROPERTY(EditAnywhere, Category = "FireSettings")
	USceneComponent* _fireEffectPosition;

	UPROPERTY(EditAnywhere, Category = "FireSettings")
	FVector FireScale{0.2f, 0.2f, 0.2f};

	UPROPERTY(EditAnywhere, Category = "FireSettings")
	FVector SmokeScale{1.8f, 1.8f, 1.8f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* VehicleCamera;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UFUNCTION()
	void onHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	                     const UDamageType* DamageType, AController* InstigatedBy,
	                     AActor* DamageCauser);

	void destroy();
	void possessVehicle(APlayerCharacter* PlayerCharacter);
	void unpossessVehicle();
	void changeMappingContext(EMappingContextType ContextType);
	void setVehicleCameraActive(bool bActive);

	void spawnEffect(UParticleSystem*& ParticleTemplate, UParticleSystemComponent*& ParticleComponent, FVector& scale);
	void spawnFireEffect();
	void steering(const FInputActionValue& Value);
	void spawnSmokeFireEffect();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void setupVehicleComponent();
	void throttle(const FInputActionValue& Value);
	void brake(const FInputActionValue& Value);
	bool canAllowExit();
	void startHandbrake(const FInputActionValue& Value);
	void StopHandbrake(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);

	UPROPERTY()
	APlayerCharacter* _playerCharacter;

	UPROPERTY()
	UParticleSystemComponent* _smokeParticle;

	UPROPERTY()
	UParticleSystemComponent* _fireParticle;

	UPROPERTY()
	UChaosWheeledVehicleMovementComponent* _vehicleMovement;

	float _healthLevelToSpawnSmoke = 50.f;
	float _healthLevelToSpawnFire = 20.f;

	float _speedExitLimit = 120.f;
};

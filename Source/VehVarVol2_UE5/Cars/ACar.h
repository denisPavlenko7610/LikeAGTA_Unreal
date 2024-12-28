// 

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ACar.generated.h"

class UHealthComponent;
class APlayerCharacter;
class UInputAction;
class UInputMappingContext;
class USphereComponent;

UCLASS(Abstract)
class VEHVARVOL2_UE5_API ACar : public AWheeledVehiclePawn {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EnterAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* CollisionComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionRadius = 250.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, Category = "FireSettings")
	UParticleSystem* SmokeParticleTemplate;

	UPROPERTY(EditAnywhere, Category = "FireSettings")
	UParticleSystem* FireParticleTemplate;

	UPROPERTY(EditAnywhere, Category = "FireSettings")
	USceneComponent* FireEffectPosition;

	UPROPERTY(EditAnywhere, Category = "FireSettings")
	FVector FireScale {0.2f,0.2f,0.2f};

	UPROPERTY(EditAnywhere, Category = "FireSettings")
	FVector SmokeScale {1.8f,1.8f,1.8f};
	
	ACar();
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void onHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy,
	                     AActor* DamageCauser);
	void destroy();
	void possessVehicle(APlayerCharacter* PlayerCharacter);
	void unpossessVehicle();
	
	void spawnEffect(UParticleSystem*& ParticleTemplate, UParticleSystemComponent*& ParticleComponent, FVector scale);
	void spawnFireEffect();
	void spawnSmokeFireEffect();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	bool canAllowExit();
	void stopVehicle();

	UPROPERTY()
	APlayerCharacter* _playerCharacter;

	UPROPERTY()
	UParticleSystemComponent* _smokeParticle;

	UPROPERTY()
	UParticleSystemComponent* _fireParticle;

	float _healthLevelToSpawnSmoke = 50.f;
	float _healthLevelToSpawnFire = 20.f;
	
	float _speedExitLimit = 120.f;
};

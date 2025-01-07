// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UWeaponComponent;
class UHealthComponent;

UCLASS()
class VEHVARVOL2_UE5_API ACharacterBase : public ACharacter {
	GENERATED_BODY()

public:
	ACharacterBase(FObjectInitializer const& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UParticleSystem* BloodParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UWeaponComponent* WeaponComponent;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy,
	                             AActor* DamageCauser);
protected:
	virtual void BeginPlay() override;

private:
	void Die();

	void SpawnImpactParticles(FDamageEvent const& DamageEvent);

};

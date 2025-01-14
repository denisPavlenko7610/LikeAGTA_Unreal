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
	ACharacterBase();

	UWeaponComponent* GetWeaponComponent() { return WeaponComponent; }
	void SetWeaponComponent (UWeaponComponent* Component) { WeaponComponent = Component; }

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy,
	                             AActor* DamageCauser);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta=(AllowPrivateAccess="true"))
	UParticleSystem* BloodParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UWeaponComponent* WeaponComponent;

private:
	void Die();

	void SpawnImpactParticles(FDamageEvent const& DamageEvent);
};

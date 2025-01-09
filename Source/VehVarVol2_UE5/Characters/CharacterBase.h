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

	UWeaponComponent* GetWeaponComponent() { return _weaponComponent; }
	void SetWeaponComponent (UWeaponComponent* Component) { _weaponComponent = Component; }

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy,
	                             AActor* DamageCauser);

	UWeaponComponent* getWeaponComponent() { return _weaponComponent; }
	
protected:
	virtual void BeginPlay() override;

private:
	void Die();

	void SpawnImpactParticles(FDamageEvent const& DamageEvent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta=(AllowPrivateAccess="true"))
	UParticleSystem* _bloodParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UHealthComponent* _healthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UWeaponComponent* _weaponComponent;

};

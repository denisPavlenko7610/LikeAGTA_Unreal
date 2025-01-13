#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, HealthDelta,
	const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent(FObjectInitializer const& ObjectInitializer);
	
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	float GetHealth() const { return CurrentHealth; }
	float GetMaxHealth() { return MaxHealth; }
	void Heal(float HealAmount);
	
	UPROPERTY()
	FOnHealthChangedSignature OnHealthChanged;

protected:
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CurrentHealth", meta=(AllowPrivateAccess="true"))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "CurrentHealth", meta=(AllowPrivateAccess="true"))
	float CurrentHealth;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", meta=(AllowPrivateAccess="true"))
	bool IsDead;
};

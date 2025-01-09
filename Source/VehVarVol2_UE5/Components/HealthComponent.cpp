#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent(FObjectInitializer const& ObjectInitializer)
{
	_maxHealth = 100.0f;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	_currentHealth = _maxHealth;

	AActor* owner = GetOwner();
	if (owner)
	{
		owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (_isDead)
		return;
	
	if (Damage <= 0.0f || _currentHealth <= 0.0f)
		return;
	
	_currentHealth = FMath::Clamp(_currentHealth - Damage, 0.0f, _maxHealth);
	
	OnHealthChanged.Broadcast(this, _currentHealth, Damage, DamageType, InstigatedBy, DamageCauser);

	if (_currentHealth <= 0.0f)
		_isDead = true;
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || _currentHealth <= 0.0f)
		return;

	_currentHealth = FMath::Clamp(_currentHealth + HealAmount, 0.0f, _maxHealth);

	OnHealthChanged.Broadcast(this, _currentHealth, -HealAmount, nullptr,
		nullptr, nullptr);
}

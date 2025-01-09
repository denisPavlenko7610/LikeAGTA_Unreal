// 


#include "UVehicleVFX.h"

#include "ACar.h"
#include "Particles/ParticleSystemComponent.h"

void UVehicleVfx::Initialize(ACar* Car)
{
	_car = Car;
	_fireParticleTemplate = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/VFX/Realistic_Starter_VFX_Pack_Vol2/Particles/Fire/P_Fire_Big.P_Fire_Big"));
	_smokeParticleTemplate = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/VFX/Realistic_Starter_VFX_Pack_Vol2/Particles/Smoke/P_Smoke_A.P_Smoke_A"));
}

void UVehicleVfx::SpawnSmokeEffect()
{
	SpawnEffect(_smokeParticleTemplate, _smokeParticle, _smokeScale);
}

void UVehicleVfx::SpawnFireEffect()
{
	_smokeParticle = nullptr;
	SpawnEffect(_fireParticleTemplate, _fireParticle, _fireScale);
}

void UVehicleVfx::SpawnEffect(UParticleSystem*& ParticleTemplate, UParticleSystemComponent*& particleEffect, FVector& scale)
{
	if (IsValid(particleEffect))
		return;

	if (!IsValid(ParticleTemplate))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid particle template or fire effect position"));
		return;
	}

	particleEffect = NewObject<UParticleSystemComponent>(this);
	particleEffect->RegisterComponent();
	particleEffect->AttachToComponent(_car->GetFireParticlePosition(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	particleEffect->SetWorldLocation(_car->GetFireParticlePosition()->GetComponentLocation());
	particleEffect->SetRelativeScale3D(scale);
	particleEffect->SetTemplate(ParticleTemplate);
	particleEffect->ActivateSystem();
}


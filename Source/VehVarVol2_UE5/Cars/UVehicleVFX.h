// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UVehicleVFX.generated.h"

class ACar;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VEHVARVOL2_UE5_API UVehicleVfx : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "FireSettings", meta=(AllowPrivateAccess="true"))
	UParticleSystem* SmokeParticleTemplate;

	UPROPERTY(EditAnywhere, Category = "FireSettings", meta=(AllowPrivateAccess="true"))
	UParticleSystem* FireParticleTemplate;

	UPROPERTY(EditAnywhere, Category = "FireSettings", meta=(AllowPrivateAccess="true"))
	FVector FireScale{0.2f, 0.2f, 0.2f};

	UPROPERTY(EditAnywhere, Category = "FireSettings", meta=(AllowPrivateAccess="true"))
	FVector SmokeScale{1.8f, 1.8f, 1.8f};

	UPROPERTY()
	UParticleSystemComponent* _smokeParticle;

	UPROPERTY()
	UParticleSystemComponent* _fireParticle;

	UPROPERTY()
	ACar* _car;

	void Initialize(ACar* Car);
	void SpawnSmokeEffect();
	void SpawnFireEffect();
	void SpawnEffect(UParticleSystem*& ParticleTemplate, UParticleSystemComponent*& particleEffect, FVector& scale);
};

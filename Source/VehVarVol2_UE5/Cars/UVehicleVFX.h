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
	void Initialize(ACar* Car);
	void SpawnSmokeEffect();
	void SpawnFireEffect();
	void SpawnEffect(UParticleSystem*& ParticleTemplate, UParticleSystemComponent*& particleEffect, FVector& scale);

private:
	UPROPERTY(EditAnywhere, Category = "FireSettings", meta=(AllowPrivateAccess="true"))
	UParticleSystem* _smokeParticleTemplate;

	UPROPERTY(EditAnywhere, Category = "FireSettings", meta=(AllowPrivateAccess="true"))
	UParticleSystem* _fireParticleTemplate;

	UPROPERTY(EditAnywhere, Category = "FireSettings", meta=(AllowPrivateAccess="true"))
	FVector _fireScale{0.2f, 0.2f, 0.2f};

	UPROPERTY(EditAnywhere, Category = "FireSettings", meta=(AllowPrivateAccess="true"))
	FVector _smokeScale{1.8f, 1.8f, 1.8f};

	UPROPERTY()
	UParticleSystemComponent* _smokeParticle;

	UPROPERTY()
	UParticleSystemComponent* _fireParticle;

	UPROPERTY()
	ACar* _car;
};

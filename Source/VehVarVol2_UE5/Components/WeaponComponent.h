// 

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/Object.h"
#include "WeaponComponent.generated.h"

class UCameraComponent;
class APlayerCharacter;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API UWeaponComponent : public USkeletalMeshComponent {
	GENERATED_BODY()

public:
	void Init(APlayerCharacter* playerCharacter);
	void Fire();
	
	void ShakeCamera();

	void StopFire(const FInputActionValue& inputActionValue);
	void PlayFireMontage(UAnimMontage* montage);
	void PlayFireSound() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
	bool rifleEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
	bool IsAiming;

	void Hit(const FHitResult& hitResult);
	void BrakeFractureObject(const FHitResult& HitResult);

	UFUNCTION()
	void FireAnimation(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void SpawnFireEffect(FName socketName, FVector& location, FVector& direction);
	void ToggleWeapon(const FInputActionValue& Value);

private:
	bool CheckWeaponTrace(const FVector& start, const FVector& end, FHitResult& outHit) const;

	bool _canFire;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta=(AllowPrivateAccess="true"))
	float DamageAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta=(AllowPrivateAccess="true"))
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta=(AllowPrivateAccess="true"))
	UParticleSystem* FireParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta=(AllowPrivateAccess="true"))
	UParticleSystem* ImpactParticle;
};

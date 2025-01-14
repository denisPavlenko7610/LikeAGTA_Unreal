// 

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/Object.h"
#include "WeaponComponent.generated.h"

class UCameraComponent;
class ACharacterBase;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class VEHVARVOL2_UE5_API UWeaponComponent : public USkeletalMeshComponent {
	GENERATED_BODY()

public:
	void Init(ACharacterBase* Character);
	void Fire();
	
	void ShakeCamera();

	void StopFire(const FInputActionValue& inputActionValue);
	void PlayFireMontage(UAnimMontage* montage);
	void PlayFireSound() const;
	UAnimMontage* GetFireMontage() const { return FireMontage; }

	void Hit(const FHitResult& hitResult);
	void BrakeFractureObject(const FHitResult& HitResult);

	UFUNCTION()
	void StartFireMontage(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void SpawnFireEffect(FName socketName, FVector& location, FVector& direction);
	void ToggleWeapon(const FInputActionValue& Value);

	bool CanFire() const { return _canFire; }
	bool IsWeaponEquipped() const { return WeaponEquipped; }
	void SetWeaponEquipped(bool value) { WeaponEquipped = value; }
	bool IsAimingWeapon() const { return IsAiming; }
	void SetAimingWeapon(bool value) { IsAiming = value; }

private:
	bool CheckWeaponTrace(const FVector& start, const FVector& end, FHitResult& outHit) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta=(AllowPrivateAccess="true"))
	bool WeaponEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta=(AllowPrivateAccess="true"))
	bool IsAiming;
	
	UPROPERTY()
	ACharacterBase* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta=(AllowPrivateAccess="true"))
	float DamageAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta=(AllowPrivateAccess="true"))
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta=(AllowPrivateAccess="true"))
	UParticleSystem* FireParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta=(AllowPrivateAccess="true"))
	UParticleSystem* ImpactParticle;

	bool _canFire;
};

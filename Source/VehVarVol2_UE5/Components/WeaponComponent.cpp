#include "WeaponComponent.h"

#include "HealthComponent.h"
#include "InputActionValue.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "VehVarVol2_UE5/Effects/Audio/AudioPlayer.h"
#include "VehVarVol2_UE5/Effects/Camera/UCameraShake.h"
#include "VehVarVol2_UE5/Characters/Player/APlayerCharacter.h"
#include "Engine/DamageEvents.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "VehVarVol2_UE5/Characters/CharacterBase.h"
#include "VehVarVol2_UE5/Effects/Audio/AudioList.h"

void UWeaponComponent::Init(APlayerCharacter* playerCharacter)
{
	_playerCharacter = playerCharacter;
}

void UWeaponComponent::Fire()
{
	constexpr float length = 1000.f;
	const FName socketName = "Muzzle";

	FVector cameraWorldLocation = _playerCharacter->GetFollowCamera()->GetComponentLocation();
	FVector forwardVector = _playerCharacter->GetFollowCamera()->GetForwardVector();

	FVector startMuzzlePosition;
	FVector socketForwardVector;
	_playerCharacter->GetSocketTransformAndVectors(socketName, startMuzzlePosition, socketForwardVector);

	FVector endPosition = forwardVector * length + cameraWorldLocation;

	SpawnFireEffect(socketName, startMuzzlePosition, forwardVector);
	PlayFireSound();

	FHitResult hitResult;
	if (CheckWeaponTrace(cameraWorldLocation, endPosition, hitResult))
	{
		Hit(hitResult);
	}

	ShakeCamera();
}

void UWeaponComponent::FireAnimation(const FInputActionValue& InputActionValue)
{
	if (!rifleEquipped)
		return;

	_canFire = false;

	PlayFireMontage(_fireMontage);
	if (UAnimInstance* AnimInstance = _playerCharacter->GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &UWeaponComponent::OnMontageEnded);
	}
}

bool UWeaponComponent::CheckWeaponTrace(const FVector& start, const FVector& end, FHitResult& outHit) const
{
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(_playerCharacter);

	return GetWorld()->LineTraceSingleByChannel(
		outHit,
		start,
		end,
		ECC_Visibility,
		collisionParams
	);
}

void UWeaponComponent::Hit(const FHitResult& hitResult)
{
	if (!_impactParticle)
		return;

	if (!hitResult.GetActor())
		return;

	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitResult.GetActor()->GetName());

	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = hitResult;

	AActor* hitActor = hitResult.GetActor();

	if (!hitActor)
		return;

	if (hitActor->FindComponentByClass<UHealthComponent>())
	{
		hitResult.GetActor()->TakeDamage(_damageAmount, PointDamageEvent, GetOwner()->GetInstigatorController(),
		                                 _playerCharacter);
	}
	else
	{
		if (UParticleSystemComponent* pointEffect = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			_impactParticle,
			hitResult.ImpactPoint,
			hitResult.Normal.ToOrientationRotator(),
			true,
			EPSCPoolMethod::AutoRelease))
		{
			float scale = 0.5f;
			pointEffect->SetWorldScale3D(FVector(scale));
		}
	}

	BrakeFractureObject(hitResult);
}

void UWeaponComponent::BrakeFractureObject(const FHitResult& HitResult)
{
	UPrimitiveComponent* hitComponent = HitResult.GetComponent();
	if (hitComponent->IsSimulatingPhysics())
	{
		FVector HitLocation = HitResult.ImpactPoint;
		FVector ImpulseDirection = HitResult.Normal * -1;
		float ImpulseStrength = 10000.f;

		if (UGeometryCollectionComponent* GeometryCollection = Cast<UGeometryCollectionComponent>(hitComponent))
		{
			UFieldSystemComponent* FieldSystem = NewObject<UFieldSystemComponent>(GeometryCollection->GetOwner());
			FieldSystem->AttachToComponent(GeometryCollection, FAttachmentTransformRules::KeepWorldTransform);
			FieldSystem->RegisterComponent();

			URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
			RadialFalloff->SetRadialFalloff(
				ImpulseStrength, // Magnitude
				0.0f, // MinRange
				1.0f, // MaxRange
				0.0f, // Default
				500, // Radius
				HitLocation, // Position
				EFieldFalloffType::Field_FallOff_None
			);
			
			FieldSystem->ApplyPhysicsField(
				true, // Enabled
				EFieldPhysicsType::Field_ExternalClusterStrain,
				nullptr, // No metadata
				RadialFalloff
			);
		}
		else
		{
			hitComponent->AddImpulseAtLocation(ImpulseDirection * ImpulseStrength, HitLocation);
		}
	}
}

void UWeaponComponent::PlayFireSound() const
{
	UAudioPlayer::PlayMetaSoundAtLocation(GetWorld(), _playerCharacter->GetActorLocation(), AudioList::fireSound);
}

void UWeaponComponent::SpawnFireEffect(FName socketName, FVector& location, FVector& direction)
{
	if (!_fireParticle)
		return;

	float vectorLength = 10.f;
	FVector adjustedLocation = location + direction * vectorLength;
	UParticleSystemComponent* fireEffect = UGameplayStatics::SpawnEmitterAttached(
		_fireParticle,
		this,
		socketName,
		adjustedLocation,
		direction.Rotation(),
		EAttachLocation::KeepWorldPosition,
		true,
		EPSCPoolMethod::AutoRelease
	);

	if (!fireEffect)
		return;

	float scale = 0.15f;
	fireEffect->SetWorldScale3D(FVector(scale));
}

void UWeaponComponent::ShakeCamera()
{
	if (APlayerController* playerController = Cast<APlayerController>(_playerCharacter->GetController()))
	{
		playerController->PlayerCameraManager->StartCameraShake(UCameraShake::StaticClass(), 1.0f);
	}
}

void UWeaponComponent::StopFire(const FInputActionValue& inputActionValue)
{
	if (!rifleEquipped)
		return;

	_canFire = true;
}

void UWeaponComponent::PlayFireMontage(UAnimMontage* montage)
{
	if (!montage)
		return;

	if (UAnimInstance* AnimInstance = _playerCharacter->GetMesh()->GetAnimInstance())
	{
		float PlayRate = 1.0f;
		AnimInstance->Montage_Play(montage, PlayRate);
	}
}

void UWeaponComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (_canFire)
		return;

	FireAnimation({});
}

void UWeaponComponent::ToggleWeapon(const FInputActionValue& Value)
{
	rifleEquipped = !rifleEquipped;
	SetHiddenInGame(!rifleEquipped);
}

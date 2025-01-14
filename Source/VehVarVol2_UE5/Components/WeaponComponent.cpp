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
#include "Engine/DamageEvents.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "VehVarVol2_UE5/Characters/CharacterBase.h"
#include "VehVarVol2_UE5/Characters/NPC/NPC.h"
#include "VehVarVol2_UE5/Characters/Player/APlayerCharacter.h"
#include "VehVarVol2_UE5/Effects/Audio/AudioList.h"

void UWeaponComponent::Init(ACharacterBase* character)
{
	Character = character;
}

//Called by notify from fire animation montage
void UWeaponComponent::Fire()
{
	constexpr float Length = 1000.f;
	const FName SocketName = "Muzzle";
	FVector StartMuzzlePosition;
	FVector ForwardVector;
	FVector EndPosition;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character))
	{
		FVector cameraWorldLocation = PlayerCharacter->GetFollowCamera()->GetComponentLocation();
		ForwardVector = PlayerCharacter->GetFollowCamera()->GetForwardVector();
		PlayerCharacter->GetSocketTransformAndVectors(SocketName, StartMuzzlePosition, ForwardVector);
		EndPosition = ForwardVector * Length + cameraWorldLocation;
	}
	else if (ANPC* NPC = Cast<ANPC>(Character))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		StartMuzzlePosition = NPC->GetWeaponComponent()->GetSocketTransform(SocketName).GetLocation();
		FVector PlayerLocation = Player->GetActorLocation();
		ForwardVector = (PlayerLocation - StartMuzzlePosition).GetSafeNormal();
		EndPosition = StartMuzzlePosition + ForwardVector * Length;

		DrawDebugLine(GetWorld(),
		              StartMuzzlePosition,
		              EndPosition,
		              FColor::Red,
		              false,
		              1.0f,
		              0,
		              1.0f
		);
	}
	else
	{
		return;
	}

	SpawnFireEffect(SocketName, StartMuzzlePosition, ForwardVector);
	PlayFireSound();

	FHitResult HitResult;
	if (CheckWeaponTrace(StartMuzzlePosition, EndPosition, HitResult))
	{
		Hit(HitResult);
	}

	ShakeCamera();
}

void UWeaponComponent::StartFireMontage(const FInputActionValue& InputActionValue)
{
	if (!WeaponEquipped)
		return;

	_canFire = false;

	PlayFireMontage(FireMontage);
	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &UWeaponComponent::OnMontageEnded);
	}
}

bool UWeaponComponent::CheckWeaponTrace(const FVector& start, const FVector& end, FHitResult& outHit) const
{
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(Character);

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
	if (!ImpactParticle)
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
		hitResult.GetActor()->TakeDamage(DamageAmount, PointDamageEvent, GetOwner()->GetInstigatorController(),
		                                 Character);
	}
	else
	{
		if (UParticleSystemComponent* pointEffect = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ImpactParticle,
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
	UAudioPlayer::PlayMetaSoundAtLocation(GetWorld(), Character->GetActorLocation(), AudioList::fireSound);
}

void UWeaponComponent::SpawnFireEffect(FName socketName, FVector& location, FVector& direction)
{
	if (!FireParticle)
		return;

	float vectorLength = 10.f;
	FVector adjustedLocation = location + direction * vectorLength;
	UParticleSystemComponent* fireEffect = UGameplayStatics::SpawnEmitterAttached(
		FireParticle,
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
	if (APlayerController* playerController = Cast<APlayerController>(Character->GetController()))
	{
		playerController->PlayerCameraManager->StartCameraShake(UCameraShake::StaticClass(), 1.0f);
	}
}

void UWeaponComponent::StopFire(const FInputActionValue& inputActionValue)
{
	if (!WeaponEquipped)
		return;

	_canFire = true;
}

void UWeaponComponent::PlayFireMontage(UAnimMontage* montage)
{
	if (!montage)
		return;

	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
	{
		float PlayRate = 1.0f;
		AnimInstance->Montage_Play(montage, PlayRate);
	}
}

void UWeaponComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (_canFire)
		return;

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Character))
	{
		StartFireMontage({});
	}
}

void UWeaponComponent::ToggleWeapon(const FInputActionValue& Value)
{
	WeaponEquipped = !WeaponEquipped;
	SetHiddenInGame(!WeaponEquipped);
}

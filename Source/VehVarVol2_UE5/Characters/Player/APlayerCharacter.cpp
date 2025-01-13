// Copyright Epic Games, Inc. All Rights Reserved.

#include "APlayerCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../Cars/ACar.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/WeaponComponent.h"
#include "Interactions/VehicleInteraction.h"
#include "VehVarVol2_UE5/UI/UHudWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

APlayerCharacter::APlayerCharacter(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	MovementComponent->JumpZVelocity = 700.f;
	MovementComponent->AirControl = 0.35f;
	MovementComponent->MaxWalkSpeed = WalkSpeed;
	MovementComponent->MinAnalogWalkSpeed = 20.f;
	MovementComponent->BrakingDecelerationWalking = 2000.f;
	MovementComponent->BrakingDecelerationFalling = 1500.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	SetWeaponComponent(CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent")));
	GetWeaponComponent()->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	GetWeaponComponent()->SetHiddenInGame(true);

	_initialArmLength = 300.0f;
	_targetArmLength = 150.0f;
	_aimLerpDurationS = 0.25f;
	_elapsedTimeS = 0.0f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateHUD();

	VehicleInteraction = NewObject<UVehicleInteraction>(this);
	VehicleInteraction->Init(this);

	GetWeaponComponent()->Init(this);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::StopSprinting);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Started, this, &ThisClass::Interact);
		EnhancedInputComponent->BindAction(TakeWeaponAction, ETriggerEvent::Started, GetWeaponComponent(),
		                                   &UWeaponComponent::ToggleWeapon);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, GetWeaponComponent(),
		                                   &UWeaponComponent::FireAnimation);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, GetWeaponComponent(),
		                                   &UWeaponComponent::StopFire);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::StopAim);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT("'%s' Failed to find an Enhanced Input component! "),
		       *GetNameSafe(this));
	}
}

void APlayerCharacter::CreateHUD()
{
	if (HudWidget == nullptr)
		return;

	HUD = CreateWidget<UHudWidget>(GetWorld(), HudWidget);
	HUD->AddToViewport();
	HUD->HideCrosshair();
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller)
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Sprint(const FInputActionValue& Value)
{
	if (GetWeaponComponent()->rifleEquipped)
		return;

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::StopSprinting(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
		return;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::Aim(const FInputActionValue& Value)
{
	if (!GetWeaponComponent()->rifleEquipped)
		return;

	HUD->ShowCrosshair();
	_initialArmLength = CameraBoom->TargetArmLength;
	_rightOffset = FVector2d(15, 50);
	_targetArmLength = 150.0f;
	_elapsedTimeS = 0.0f;
	GetWeaponComponent()->IsAiming = true;

	GetWorld()->GetTimerManager().SetTimer(LerpTimerHandle, this, &APlayerCharacter::UpdateAimLerp,
	                                       GetWorld()->GetDeltaSeconds(), true);
}

void APlayerCharacter::StopAim(const FInputActionValue& Value)
{
	if (!GetWeaponComponent()->rifleEquipped)
		return;

	HUD->HideCrosshair();
	_initialArmLength = CameraBoom->TargetArmLength;
	_rightOffset = FVector2d(0, 0);
	_targetArmLength = 300.0f;
	_elapsedTimeS = 0.0f;
	GetWeaponComponent()->IsAiming = false;

	GetWorld()->GetTimerManager().SetTimer(LerpTimerHandle, this, &APlayerCharacter::UpdateAimLerp,
	                                       GetWorld()->GetDeltaSeconds(), true);
}

void APlayerCharacter::UpdateAimLerp()
{
	_elapsedTimeS += GetWorld()->GetDeltaSeconds();
	float alpha = FMath::Clamp(_elapsedTimeS / _aimLerpDurationS, 0.0f, 1.0f);
	float newArmLength = FMath::Lerp(_initialArmLength, _targetArmLength, alpha);
	float newRightOffset = FMath::Lerp(_rightOffset.X, _rightOffset.Y, alpha);

	CameraBoom->TargetArmLength = newArmLength;
	FVector cameraBoomLocation = CameraBoom->GetRelativeLocation();
	cameraBoomLocation.Y = newRightOffset;
	CameraBoom->SetRelativeLocation(cameraBoomLocation);

	if (alpha >= 1.f)
		GetWorld()->GetTimerManager().ClearTimer(LerpTimerHandle);
	
}

void APlayerCharacter::Interact()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Vehicle),
		FCollisionShape::MakeSphere(300.0f),
		QueryParams
	);

	if (!bHit)
		return;

	for (const FOverlapResult& Overlap : OverlapResults)
	{
		if (ACar* Car = Cast<ACar>(Overlap.GetActor()))
		{
			VehicleInteraction->EnterVehicle(Car);
			return;
		}
	}
}

void APlayerCharacter::GetSocketTransformAndVectors(const FName& socketName, FVector& outStart,
                                                    FVector& outForwardVector)
{
	FTransform socketTransform = GetWeaponComponent()->GetSocketTransform(socketName);
	outStart = socketTransform.GetLocation();
	outForwardVector = socketTransform.Rotator().Vector();
}

FRotator APlayerCharacter::GetAimRotation()
{
	FRotator controlRotation = GetControlRotation();
	FRotator deltaRotation = controlRotation - GetActorRotation();
	deltaRotation.Pitch *= -1;

	return FRotator(0, deltaRotation.Yaw, deltaRotation.Pitch);
}

void APlayerCharacter::OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta,
                                       const UDamageType* DamageType,
                                       AController* InstigatedBy, AActor* DamageCauser)
{
	Super::OnHealthChanged(OwningHealthComp, Health, HealthDelta, DamageType, InstigatedBy, DamageCauser);

	HUD->UpdateHealthBar(Health, OwningHealthComp->GetMaxHealth());
}

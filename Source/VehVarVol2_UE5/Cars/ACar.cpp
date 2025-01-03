#include "ACar.h"
#include "EnhancedInputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"
#include "VehVarVol2_UE5/Player/APlayerCharacter.h"
#include "VehVarVol2_UE5/Components/HealthComponent.h"
#include "VehVarVol2_UE5/Player/Interactions/VehicleInteraction.h"
#include "Components/InputComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EMappingContextType.h"
#include "EnhancedInputSubsystems.h"
#include "UWheelFront.h"
#include "WheelRear.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

class UEnhancedInputLocalPlayerSubsystem;

ACar::ACar()
{
	PrimaryActorTick.bCanEverTick = false;
	_vehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));

	_collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	_collisionComponent->SetSphereRadius(CollisionRadius);
	_collisionComponent->SetupAttachment(GetRootComponent());

	_fireEffectPosition = CreateDefaultSubobject<USceneComponent>(TEXT("EffectPosition"));
	_fireEffectPosition->SetupAttachment(GetRootComponent());

	_healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 650.0f;
	SpringArm->SocketOffset.Z = 150.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 2.0f;
	SpringArm->CameraLagMaxDistance = 50.0f;
	SpringArm->SetRelativeRotation(FRotator(-10, 0, 0));
	SpringArm->SetupAttachment(GetRootComponent());

	VehicleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCamera"));
	VehicleCamera->SetupAttachment(SpringArm);
	setupVehicleComponent();
}

void ACar::BeginPlay()
{
	Super::BeginPlay();

	if (_healthComponent)
	{
		_healthComponent->OnHealthChanged.AddDynamic(this, &ACar::onHealthChanged);
	}
}

float ACar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                       AActor* DamageCauser)
{
	float actualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (_healthComponent)
	{
		_healthComponent->takeDamage(this, actualDamage, DamageEvent.DamageTypeClass
			                                                 ? DamageEvent.DamageTypeClass.GetDefaultObject()
			                                                 : nullptr, EventInstigator, DamageCauser);
	}

	return actualDamage;
}

void ACar::onHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta,
                           const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	FString HealthMessage = FString::Printf(TEXT("Current health is: %f"), Health);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, HealthMessage);

	if (Health <= 0)
	{
		destroy();
		return;
	}

	if (Health <= _healthLevelToSpawnFire)
		spawnFireEffect();
	else if (Health <= _healthLevelToSpawnSmoke)
		spawnSmokeFireEffect();
}

void ACar::destroy()
{
	UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *GetName());

	if (AController* controller = GetController())
	{
		controller->UnPossess();
	}

	_smokeParticle = nullptr;
	_fireParticle = nullptr;

	Destroy();
}

void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Started, this, &ThisClass::unpossessVehicle);
		// steering 
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &ThisClass::steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &ThisClass::steering);

		// throttle 
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ThisClass::throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ThisClass::throttle);

		// break 
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ThisClass::brake);

		// handbrake 
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &ThisClass::startHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ThisClass::StopHandbrake);

		// look 
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ThisClass::LookAround);
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ThisClass::LookUp);
	}
}

void ACar::possessVehicle(APlayerCharacter* PlayerCharacter)
{
	_playerCharacter = PlayerCharacter;
	if (AController* controller = PlayerCharacter->GetController())
	{
		changeMappingContext(EMappingContextType::VehicleContext);
		controller->Possess(this);
		setVehicleCameraActive(true);
	}
}

void ACar::unpossessVehicle()
{
	if (!canAllowExit())
		return;

	if (!_playerCharacter)
		return;

	_playerCharacter->getVehicleInteraction()->exitVehicle();
	changeMappingContext(EMappingContextType::PlayerContext);
	GetController()->Possess(_playerCharacter);
	setVehicleCameraActive(false);
	_playerCharacter = nullptr;
}

void ACar::changeMappingContext(EMappingContextType ContextType)
{
	if (!_playerCharacter)
		return;
	
	if (APlayerController* PlayerController = Cast<APlayerController>(_playerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			switch (ContextType)
			{
			case EMappingContextType::VehicleContext:
					if (_playerCharacter->PlayerMappingContext)
					{
						InputSubsystem->RemoveMappingContext(_playerCharacter->PlayerMappingContext);
					}
				if (CarMappingContext)
				{
					InputSubsystem->AddMappingContext(CarMappingContext, 1);
				}
				break;

			case EMappingContextType::PlayerContext:
					if (CarMappingContext)
					{
						InputSubsystem->RemoveMappingContext(CarMappingContext);
					}
				if (_playerCharacter->PlayerMappingContext)
				{
					InputSubsystem->AddMappingContext(_playerCharacter->PlayerMappingContext, 1);
				}
				break;
			}
		}
	}
}

void ACar::setVehicleCameraActive(bool bActive)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (bActive)
		{
			PlayerController->SetViewTargetWithBlend(this, 0.5f);
		}
		else if (_playerCharacter)
		{
			PlayerController->SetViewTargetWithBlend(_playerCharacter, 0.5f);
		}
	}
}

void ACar::spawnEffect(UParticleSystem*& ParticleTemplate, UParticleSystemComponent*& ParticleComponent, FVector& scale)
{
	if (IsValid(ParticleComponent))
		return;

	if (!IsValid(ParticleTemplate) || !IsValid(_fireEffectPosition))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid particle template or fire effect position"));
		return;
	}

	ParticleComponent = NewObject<UParticleSystemComponent>(this);
	ParticleComponent->SetupAttachment(_fireEffectPosition);
	ParticleComponent->SetWorldLocation(_fireEffectPosition->GetComponentLocation());
	ParticleComponent->SetRelativeScale3D(scale);
	ParticleComponent->RegisterComponent();
	ParticleComponent->SetTemplate(ParticleTemplate);
	ParticleComponent->ActivateSystem();
}

void ACar::spawnSmokeFireEffect()
{
	spawnEffect(SmokeParticleTemplate, _smokeParticle, SmokeScale);
}

void ACar::spawnFireEffect()
{
	spawnEffect(FireParticleTemplate, _fireParticle, FireScale);
}

bool ACar::canAllowExit()
{
	double speed = GetVelocity().Size();
	return speed < _speedExitLimit;
}

void ACar::setupVehicleComponent()
{
	_vehicleMovement->bLegacyWheelFrictionPosition = true;
	_vehicleMovement->WheelSetups.SetNum(4);

	_vehicleMovement->WheelSetups[0].WheelClass = UWheelFront::StaticClass();
	_vehicleMovement->WheelSetups[0].BoneName = FName("WheelFL");
	_vehicleMovement->WheelSetups[0].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);
	
	_vehicleMovement->WheelSetups[1].WheelClass = UWheelFront::StaticClass();
	_vehicleMovement->WheelSetups[1].BoneName = FName("WheelFR");
	_vehicleMovement->WheelSetups[1].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);
	
	_vehicleMovement->WheelSetups[2].WheelClass = UWheelRear::StaticClass();
	_vehicleMovement->WheelSetups[2].BoneName = FName("WheelRL");
	_vehicleMovement->WheelSetups[2].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);
	
	_vehicleMovement->WheelSetups[3].WheelClass = UWheelRear::StaticClass();
	_vehicleMovement->WheelSetups[3].BoneName = FName("WheelRR");
	_vehicleMovement->WheelSetups[3].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	// Set up engine
	_vehicleMovement->EngineSetup.MaxRPM = 6000.0f;
	_vehicleMovement->EngineSetup.MaxTorque = 500.0f;

	// Set up differential
	_vehicleMovement->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;

	// Set up transmission
	_vehicleMovement->TransmissionSetup.bUseAutomaticGears = true;

	// Set up steering
	_vehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	_vehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	_vehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);
}

void ACar::throttle(const FInputActionValue& Value)
{
	float ThrottleValue = Value.Get<float>();
	_vehicleMovement->SetThrottleInput(ThrottleValue);
}

void ACar::steering(const FInputActionValue& Value)
{
	float SteeringValue = Value.Get<float>();
	_vehicleMovement->SetSteeringInput(SteeringValue);
}

void ACar::brake(const FInputActionValue& Value)
{
	float BreakValue = Value.Get<float>();
	_vehicleMovement->SetBrakeInput(BreakValue);
}

void ACar::startHandbrake(const FInputActionValue& Value)
{
	_vehicleMovement->SetHandbrakeInput(true);
}

void ACar::StopHandbrake(const FInputActionValue& Value)
{
	_vehicleMovement->SetHandbrakeInput(false);
}

void ACar::LookAround(const FInputActionValue& Value)
{
	float LookValue = Value.Get<float>();
	AddControllerPitchInput(LookValue);
}

void ACar::LookUp(const FInputActionValue& Value)
{
	float LookValue = Value.Get<float>();
	AddControllerYawInput(LookValue);
}

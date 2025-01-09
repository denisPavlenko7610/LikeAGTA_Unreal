#include "ACar.h"
#include "UVehicleInputHandler.h"
#include "UVehicleHealthHandler.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UVehicleVFX.h"
#include "UWheelFront.h"
#include "UWheelRear.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "VehVarVol2_UE5/Components/HealthComponent.h"
#include "VehVarVol2_UE5/Characters/Player/APlayerCharacter.h"
#include "VehVarVol2_UE5/Characters/Player/Interactions/VehicleInteraction.h"

ACar::ACar(FObjectInitializer const& ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = false;
    _vehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(FName("Vehicle"));

    _collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    _collisionComponent->SetSphereRadius(_collisionRadius);
    _collisionComponent->SetupAttachment(GetRootComponent());

    _healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

    _springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    _springArm->TargetArmLength = 650.0f;
    _springArm->SocketOffset.Z = 150.0f;
    _springArm->bDoCollisionTest = false;
    _springArm->bInheritPitch = false;
    _springArm->bInheritRoll = false;
    _springArm->bEnableCameraRotationLag = true;
    _springArm->CameraRotationLagSpeed = 2.0f;
    _springArm->CameraLagMaxDistance = 50.0f;
    _springArm->SetRelativeRotation(FRotator(-10, 0, 0));
    _springArm->SetupAttachment(GetRootComponent());

    _vehicleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCamera"));
    _vehicleCamera->SetupAttachment(_springArm);

    _fireEffectPosition = CreateDefaultSubobject<USceneComponent>(TEXT("EffectPosition"));
    _fireEffectPosition->SetupAttachment(GetRootComponent());

    SetupVehicleComponent();

    _inputHandler = CreateDefaultSubobject<UVehicleInputHandler>(TEXT("InputHandler"));
    _healthHandler = CreateDefaultSubobject<UVehicleHealthHandler>(TEXT("HealthHandler"));
    
    _vehicleVfx = CreateDefaultSubobject<UVehicleVfx>(TEXT("VehicleVFX"));
}

void ACar::BeginPlay()
{
    Super::BeginPlay();

    if (_healthComponent)
    {
        _healthHandler->Initialize(this, _healthComponent);
    }

    _vehicleVfx->Initialize(this);
}

void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    _inputHandler->SetupInput(PlayerInputComponent, this);
}

void ACar::PossessVehicle(APlayerCharacter* PlayerCharacter)
{
    _playerCharacter = PlayerCharacter;
    if (AController* controller = PlayerCharacter->GetController())
    {
        ChangeMappingContext(EMappingContextType::VehicleContext);
        controller->Possess(this);
        SetVehicleCameraActive(true);
    }
}

void ACar::UnpossessVehicle()
{
    if (!CanAllowExit())
        return;

    if (!_playerCharacter)
        return;

    _playerCharacter->GetVehicleInteraction()->ExitVehicle();
    ChangeMappingContext(EMappingContextType::PlayerContext);
    GetController()->Possess(_playerCharacter);
    SetVehicleCameraActive(false);
    _playerCharacter = nullptr;
}

void ACar::ChangeMappingContext(EMappingContextType ContextType)
{
    if (!_playerCharacter)
        return;

    APlayerController* PlayerController = Cast<APlayerController>(_playerCharacter->GetController());
    if (!PlayerController)
        return;

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (!InputSubsystem)
        return;

    auto RemoveMappingContext = [&](UInputMappingContext* Context)
    {
        if (Context)
            InputSubsystem->RemoveMappingContext(Context);
    };

    auto AddMappingContext = [&](UInputMappingContext* Context)
    {
        if (Context)
            InputSubsystem->AddMappingContext(Context, 1);
    };

    switch (ContextType)
    {
    case EMappingContextType::VehicleContext:
        RemoveMappingContext(_playerCharacter->GetPlayerMappingContext());
        AddMappingContext(_inputHandler->GetCarMappingContext());
        break;

    case EMappingContextType::PlayerContext:
        RemoveMappingContext(_inputHandler->GetCarMappingContext());
        AddMappingContext(_playerCharacter->GetPlayerMappingContext());
        break;
    }
}

void ACar::SetVehicleCameraActive(bool bActive)
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

void ACar::SetupVehicleComponent()
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

    _vehicleMovement->EngineSetup.MaxRPM = 6000.0f;
    _vehicleMovement->EngineSetup.MaxTorque = 500.0f;
    _vehicleMovement->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
    _vehicleMovement->TransmissionSetup.bUseAutomaticGears = true;
    _vehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
    _vehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
    _vehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);
}

bool ACar::CanAllowExit()
{
    double speed = GetVelocity().Size();
    return speed < _speedExitLimit;
}
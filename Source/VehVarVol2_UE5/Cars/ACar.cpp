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
    VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(FName("Vehicle"));

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetSphereRadius(CollisionRadius);
    CollisionComponent->SetupAttachment(GetRootComponent());

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

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

    FireEffectPosition = CreateDefaultSubobject<USceneComponent>(TEXT("EffectPosition"));
    FireEffectPosition->SetupAttachment(GetRootComponent());

    SetupVehicleComponent();

    InputHandler = CreateDefaultSubobject<UVehicleInputHandler>(TEXT("InputHandler"));
    HealthHandler = CreateDefaultSubobject<UVehicleHealthHandler>(TEXT("HealthHandler"));
    
    VehicleVfx = CreateDefaultSubobject<UVehicleVfx>(TEXT("VehicleVFX"));
}

void ACar::BeginPlay()
{
    Super::BeginPlay();

    if (HealthComponent)
    {
        HealthHandler->Initialize(this, HealthComponent);
    }

    VehicleVfx->Initialize(this);
}

void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    InputHandler->SetupInput(PlayerInputComponent, this);
}

void ACar::PossessVehicle(APlayerCharacter* playerCharacter)
{
    PlayerCharacter = playerCharacter;
    if (AController* controller = playerCharacter->GetController())
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

    if (!PlayerCharacter)
        return;

    PlayerCharacter->GetVehicleInteraction()->ExitVehicle();
    ChangeMappingContext(EMappingContextType::PlayerContext);
    GetController()->Possess(PlayerCharacter);
    SetVehicleCameraActive(false);
    PlayerCharacter = nullptr;
}

void ACar::ChangeMappingContext(EMappingContextType ContextType)
{
    if (!PlayerCharacter)
        return;

    APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
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
        RemoveMappingContext(PlayerCharacter->GetPlayerMappingContext());
        AddMappingContext(InputHandler->GetCarMappingContext());
        break;

    case EMappingContextType::PlayerContext:
        RemoveMappingContext(InputHandler->GetCarMappingContext());
        AddMappingContext(PlayerCharacter->GetPlayerMappingContext());
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
        else if (PlayerCharacter)
        {
            PlayerController->SetViewTargetWithBlend(PlayerCharacter, 0.5f);
        }
    }
}

void ACar::SetupVehicleComponent()
{
    VehicleMovement->bLegacyWheelFrictionPosition = true;
    VehicleMovement->WheelSetups.SetNum(4);

    VehicleMovement->WheelSetups[0].WheelClass = UWheelFront::StaticClass();
    VehicleMovement->WheelSetups[0].BoneName = FName("WheelFL");
    VehicleMovement->WheelSetups[0].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

    VehicleMovement->WheelSetups[1].WheelClass = UWheelFront::StaticClass();
    VehicleMovement->WheelSetups[1].BoneName = FName("WheelFR");
    VehicleMovement->WheelSetups[1].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

    VehicleMovement->WheelSetups[2].WheelClass = UWheelRear::StaticClass();
    VehicleMovement->WheelSetups[2].BoneName = FName("WheelRL");
    VehicleMovement->WheelSetups[2].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

    VehicleMovement->WheelSetups[3].WheelClass = UWheelRear::StaticClass();
    VehicleMovement->WheelSetups[3].BoneName = FName("WheelRR");
    VehicleMovement->WheelSetups[3].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

    VehicleMovement->EngineSetup.MaxRPM = 6000.0f;
    VehicleMovement->EngineSetup.MaxTorque = 500.0f;
    VehicleMovement->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
    VehicleMovement->TransmissionSetup.bUseAutomaticGears = true;
    VehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
    VehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
    VehicleMovement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);
}

bool ACar::CanAllowExit()
{
    double speed = GetVelocity().Size();
    return speed < _speedExitLimit;
}
#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "VehVarVol2_UE5/Characters/CharacterBase.h"
#include "APlayerCharacter.generated.h"

class UHudWidget;
class UVehicleInteraction;
class UHealthComponent;
class UWeaponComponent;
class ACar;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USkeletalMeshComponent;
class UAnimMontage;
class UUserWidget;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	USpringArmComponent* getCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	APlayerCharacter();
	virtual void BeginPlay() override;

	void GetSocketTransformAndVectors(const FName& socketName, FVector& outStart, FVector& outForwardVector);
	FRotator GetAimRotation();

	UVehicleInteraction* GetVehicleInteraction() { return VehicleInteraction; }
	UInputMappingContext* GetPlayerMappingContext() { return PlayerMappingContext; }
	
protected:
	void Move(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void StopSprinting(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	void StopAim(const FInputActionValue& Value);
	void UpdateAimLerp();
	void Interact();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void CreateHUD();

	virtual void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy,
	                             AActor* DamageCauser) override;

private:
	UPROPERTY()
	UVehicleInteraction* VehicleInteraction;

	UPROPERTY()
	UHudWidget* HUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EnterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TakeWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UHudWidget> HudWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta=(AllowPrivateAccess="true"))
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta=(AllowPrivateAccess="true"))
	float SprintSpeed = 600.0f;

	float _initialArmLength;
	float _targetArmLength;
	float _aimLerpDurationS;
	float _elapsedTimeS;
	FVector2d _rightOffset;
	FTimerHandle LerpTimerHandle;

	float pitchLimit = 100.0f;
	float yawLimit = 80.0f;
	float zLimit = 90.0f;
};


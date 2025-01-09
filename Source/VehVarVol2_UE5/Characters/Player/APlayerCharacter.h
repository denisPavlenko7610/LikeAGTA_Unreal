#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	USpringArmComponent* getCameraBoom() const { return _cameraBoom; }
	UCameraComponent* GetFollowCamera() const { return _followCamera; }
	
	APlayerCharacter(FObjectInitializer const& ObjectInitializer);
	virtual void BeginPlay() override;

	void GetSocketTransformAndVectors(const FName& socketName, FVector& outStart, FVector& outForwardVector);
	FRotator GetAimRotation();

	UVehicleInteraction* GetVehicleInteraction() { return _vehicleInteraction; }
	UInputMappingContext* GetPlayerMappingContext() { return _playerMappingContext; }
	
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
	float _initialArmLength;
	float _targetArmLength;
	float _aimLerpDurationS;
	float _elapsedTimeS;
	FVector2d _rightOffset;
	FTimerHandle LerpTimerHandle;

	float pitchLimit = 100.0f;
	float yawLimit = 80.0f;
	float zLimit = 90.0f;

	UPROPERTY()
	UVehicleInteraction* _vehicleInteraction;

	UPROPERTY()
	UHudWidget* _hud;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* _cameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _followCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* _playerMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _jumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _moveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _enterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _fireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _aimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _getWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _sprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UHudWidget> _hudWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta=(AllowPrivateAccess="true"))
	float _walkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta=(AllowPrivateAccess="true"))
	float _sprintSpeed = 600.0f;
};


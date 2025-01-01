#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "VehVarVol2_UE5/Characters/Citizen.h"
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
class APlayerCharacter : public ACitizen
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
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
	UInputAction* GetWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UHudWidget> hudWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 600.0f;

	FORCEINLINE USpringArmComponent* getCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* getFollowCamera() const { return FollowCamera; }
	
	APlayerCharacter();
	virtual void BeginPlay() override;

	void getSocketTransformAndVectors(const FName& socketName, FVector& outStart, FVector& outForwardVector) const;

	UVehicleInteraction* getVehicleInteraction() { return _vehicleInteraction; }
	UWeaponComponent* getWeaponComponent() { return WeaponComponent; }
	
protected:
	void move(const FInputActionValue& Value);
	void sprint(const FInputActionValue& Value);
	void stopSprinting(const FInputActionValue& Value);
	void look(const FInputActionValue& Value);
	void aim(const FInputActionValue& Value);
	void stopAim(const FInputActionValue& Value);
	void updateAimLerp();
	void interact();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void createHUD();

	UFUNCTION(BlueprintCallable)
	FRotator getAimRotation();

	virtual void onHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy,
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
};


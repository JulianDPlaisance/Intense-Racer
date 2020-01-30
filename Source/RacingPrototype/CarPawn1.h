// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RacingPrototypeWheelFront.h"
#include "RacingPrototypeWheelRear.h"
#include "RacingPrototypeHud.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Materials/Material.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"

#include "WheeledVehicle.h"
#include "CarPawn1.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;

UCLASS(config=Game)
class RACINGPROTOTYPE_API ACarPawn1 : public AWheeledVehicle
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = Camera, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = Camera, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* ReverseSpringArm;
	UPROPERTY(EditAnywhere, Category = Camera, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = Camera, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* ReverseCamera;
	UPROPERTY(EditAnywhere, Category = Camera, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraHolder;

public:
	ACarPawn1();

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText SpeedDisplayString;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText GearDisplayString;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FColor GearDisplayColor;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FColor GearDisplayReverseColor;
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool bInReverseGear;
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool bLookBack;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void Tick(float Delta) override;

protected:
	virtual void BeginPlay() override;

public:
	void MoveForward(float Val);

	void UpgradePhysicsMaterial();

	void MoveRight(float Val);

	void OnHandbrakePressed();

	void OnHandbrakeReleased();
	UFUNCTION(BlueprintCallable, Category = "Camera", meta = (ToolTip = "This will switch which camera is looking at the car"))
	void SwitchCamera();

	void LookAtCar(AActor* LookingActor, FVector TargetPosition, FVector WorldUp = FVector::UpVector);

	static const FName LookUpBinding;
	static const FName LookRightBinding;

private:
	//void UpdateHUDStrings();

	bool bIsLowFriction;

public:

	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
};

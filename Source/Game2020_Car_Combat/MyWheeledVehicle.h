// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "MyWheeledVehicle.generated.h"

/**
 * 
 */
UCLASS()
class GAME2020_CAR_COMBAT_API AMyWheeledVehicle : public AWheeledVehicle
{
	GENERATED_BODY()
public:
	AMyWheeledVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void ApplyThrottle(float val);
	void ApplySteering(float val);

	void OnHandbrakePressed();
	void OnHandbrakeReleased();

	void ResetVehicle();

protected:
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

};

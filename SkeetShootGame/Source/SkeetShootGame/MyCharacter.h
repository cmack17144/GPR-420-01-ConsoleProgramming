// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "MyCharacter.generated.h"


UCLASS()
class SKEETSHOOTGAME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;

public:
	AMyCharacter();

protected:
	virtual void BeginPlay();

protected:

	void MoveForward(float Val);
	void MoveRight(float Val);


	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }
};

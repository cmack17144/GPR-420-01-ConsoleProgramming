// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set up capsule collider
	GetCapsuleComponent()->InitCapsuleSize(50.0f, 100.0f);

	// create camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));
	CameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// bind look events
	PlayerInputComponent->BindAxis("LookRight", this, &AMyCharacter::LookRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);
}

void AMyCharacter::MoveForward(float val)
{
	if (bControlsEnabled && val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), val * fMoveSpeed);
	}
}
void AMyCharacter::MoveRight(float val)
{
	if (bControlsEnabled && val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), val * fMoveSpeed);
	}
}
void AMyCharacter::LookUp(float val)
{
	if (bControlsEnabled && val != 0.0f)
	{
		AddControllerPitchInput(val);
	}
}
void AMyCharacter::LookRight(float val)
{
	if (bControlsEnabled && val != 0.0f)
	{
		AddControllerYawInput(val);
	}
}
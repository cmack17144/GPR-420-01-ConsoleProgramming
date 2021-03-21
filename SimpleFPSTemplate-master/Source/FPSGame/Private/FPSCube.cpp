// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCube.h"
#include "FPSGameMode.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSCube::AFPSCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSCube::BeginPlay()
{
	Super::BeginPlay();
	
	// get the world
	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr)
	{
		// get our game mode
		AFPSGameMode* MyGameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(TheWorld));
		if (MyGameMode != nullptr)
		{
			// add handle to switch color
			MyDelegateHandle = MyGameMode->FiredDelegate.AddUObject(this, &AFPSCube::SwitchColor);
			UE_LOG(LogTemp, Warning, TEXT("Delegate added"));
		}
	}
}

// Called every frame
void AFPSCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSCube::SwitchColor()
{
	UE_LOG(LogTemp, Warning, TEXT("switch color called"));

	UMaterialInstanceDynamic* MatInst = Cast<UPrimitiveComponent>(this->GetRootComponent())->CreateAndSetMaterialInstanceDynamic(0);
	if (MatInst)
	{
		MatInst->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
	}
}

void AFPSCube::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// get world
	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr)
	{
		// get the game mode
		AFPSGameMode* myGameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(TheWorld));
		if (myGameMode != nullptr)
		{
			// remove our delegate
			myGameMode->FiredDelegate.Remove(MyDelegateHandle);
		}
	}
}
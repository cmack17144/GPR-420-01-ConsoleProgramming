// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Math/RandomStream.h"
#include "Kismet/GameplayStatics.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}


void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		FVector Scale = OtherComp->GetComponentScale();

		// check if big enough
		if (Scale.GetMin() > 0.75f)
		{
			FRandomStream RngStream;
			// make 4 children
			for (int i = 0; i < 4; i++)
			{
				// get random offset
				FVector offset = RngStream.VRand();
				// spawn the projectile at location randomly offset from og actor location
				AActor* Temp = GetWorld()->SpawnActor<AActor>((UClass*)CubeClass, OtherActor->GetActorLocation() + offset, OtherActor->GetActorRotation());

				// get the primitive component
				UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Temp->GetRootComponent());

				// set to smaller scale
				PrimComp->SetWorldScale3D(Scale * 0.5f);
				// set to a random color
				PrimComp->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
			}
		}
		else {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionTemplate, GetActorLocation());

			FCollisionObjectQueryParams QueryParams;
			QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

			FCollisionShape CollShape;
			CollShape.SetSphere(500.0f);

			TArray<FOverlapResult> OutOverlaps;
			GetWorld()->OverlapMultiByObjectType(OutOverlaps, GetActorLocation(), FQuat::Identity, QueryParams, CollShape);

			for (FOverlapResult Result : OutOverlaps)
			{
				UPrimitiveComponent* Overlap = Result.GetComponent();
				if (Overlap && Overlap->IsSimulatingPhysics())
				{
					UMaterialInstanceDynamic* MatInst = Overlap->CreateAndSetMaterialInstanceDynamic(0);
					if (MatInst)
					{
						MatInst->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
					}
				}
			}
		}

		// destroy old one regardless
		OtherActor->Destroy();

		Destroy();
	}
}
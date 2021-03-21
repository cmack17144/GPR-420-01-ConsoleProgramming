#include "FPSSuperProjectile.h"
#include "FPSCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSWeatherInformation.h"
#include "FPSGameMode.h"


AFPSSuperProjectile::AFPSSuperProjectile() {
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSSuperProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->MaxSpeed = 6000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bAutoActivate = false;


	// Die after 3 seconds by default
	InitialLifeSpan = 5.0f;

	//float ChargeValue = 0.0f;
	//ChargeValue = AFPSCharacter::CTime;
}

void AFPSSuperProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Super proj OnHit"));

	// Only add respond to something w physics enabled
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		if (ExplosionTemplate)
		{
			// spawn the emitter
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionTemplate, GetActorLocation());
		}

		/* - Get all the neighbors - */
		// set up overlap query params
		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		// init the overlap shape
		FCollisionShape CollShape;
		CollShape.SetSphere(ChargeValue);
		//CollShape.SetSphere(500.0f); // TODO: Make scale w time

		// do the overlap
		TArray<FOverlapResult> OutOverlaps;
		GetWorld()->OverlapMultiByObjectType(OutOverlaps, GetActorLocation(), FQuat::Identity, QueryParams, CollShape);

		// destroy overlap results
		for (FOverlapResult Result : OutOverlaps)
		{
			Result.Actor.Get()->Destroy();
		}

		// destroy self
		Destroy();
	}
}

void AFPSSuperProjectile::LaunchBullet()
{
	float iSpeed = 3000.0f;
	// get the wind speed from the weather information object in the world
	AFPSWeatherInformation* weatherHolder = Cast<AFPSWeatherInformation>(UGameplayStatics::GetActorOfClass(GetWorld(), AFPSWeatherInformation::StaticClass()));
	if (weatherHolder)
	{
		float wSpeed = weatherHolder->GetWindSpeed();
		iSpeed = wSpeed / 9.0f * 3000.0f;
		//UE_LOG(LogTemp, Warning, TEXT("%f => %f"), wSpeed, iSpeed);
	}
	//else
		//UE_LOG(LogTemp, Warning, TEXT("weatherHolder not found"));

	//ProjectileMovement->InitialSpeed = iSpeed;
	ProjectileMovement->SetVelocityInLocalSpace(FVector(iSpeed, 0.0f, 0.0f));

	// get our game mode
	AFPSGameMode* MyGameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (MyGameMode != nullptr)
	{
		// broadcast delegate
		MyGameMode->FiredDelegate.Broadcast();
	}

	// activate it
	ProjectileMovement->Activate();
}

void AFPSSuperProjectile::SetCharge(float Charge)
{
	ChargeValue = Charge * Charge * 100.0f;
}
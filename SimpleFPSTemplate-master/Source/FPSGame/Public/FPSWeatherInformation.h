/* *
 * Reference for setting up HTTP calls:
 * https://www.davidykay.com/UE4-Hello-Http/
 * */


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Engine/DirectionalLight.h"
#include "FPSWeatherInformation.generated.h"

UCLASS()
class FPSGAME_API AFPSWeatherInformation : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSWeatherInformation();

	// http module object
	FHttpModule* Http;

	UFUNCTION()
		float GetWindVector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector windVector = FVector::ZeroVector;		// wind vector

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// act on the response 
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	float temperature;		// temperature (C)

	float windAngle = 0.f;	// wind angle (deg)


	ADirectionalLight* sun;	// reference to sun

	// sends api request for weather
	UFUNCTION()
		void GetWeatherData();
};

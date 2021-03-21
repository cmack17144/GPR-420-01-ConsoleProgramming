// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSWeatherInformation.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LightComponent.h"

// Sets default values
AFPSWeatherInformation::AFPSWeatherInformation()
{
	Http = &FHttpModule::Get();
	//windVector = FVector(0, 0, 0);
}

// Called when the game starts or when spawned
void AFPSWeatherInformation::BeginPlay()
{
	Super::BeginPlay();
	
	// find the sun
	sun = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (sun)
		sun->GetLightComponent()->bUseTemperature = true;
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Sun not found"));
	
	// call api for weather data
	GetWeatherData();
}

void AFPSWeatherInformation::GetWeatherData()
{
	// create request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	// bind the response function to when the request is completed
	Request->OnProcessRequestComplete().BindUObject(this, &AFPSWeatherInformation::OnResponseReceived);

	// set up the request
	Request->SetURL("https://api.weather.gov/stations/KBTV/observations/latest"); // add url target
	Request->SetVerb("GET");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	// fire warning if process fails
	if (!Request->ProcessRequest())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HTTP Request not processed"));
	}
}

void AFPSWeatherInformation::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, TEXT("HTTP response recieved"));

	// json object to hold the json data
	TSharedPtr<FJsonObject> JsonObject;

	// json reader
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	// deserialize json data from Reader into the object
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		// get the array of properties passed back in api call
		TSharedPtr<FJsonObject> props = JsonObject->GetObjectField("properties");
	
		// get the temperature from the json object
		temperature = props->GetObjectField("temperature")->GetNumberField("value");

		// set light temperature based so:
		//   -15C -> 0K and 40C -> 12,000K
		if (sun)
		{
			//float sunTemp = (0.1873f * temperature + 4.5091f) * 1000.f;
			//sun->GetLightComponent()->Temperature = sunTemp;
			sun->SetLightColor(FLinearColor::LerpUsingHSV(FLinearColor::Red, FLinearColor::Blue, 0.018f * temperature + 0.273f));
			//UE_LOG(LogTemp, Warning, TEXT("sun temp: %f"), sunTemp);
		}

		// get the wind angle from json object
		windAngle = props->GetObjectField("windDirection")->GetNumberField("value");

		//windSpeed = props->GetObjectField("windSpeed")->GetNumberField("value");
		windSpeed = 5.0f;

		// update components of windVector based on angle
		windVector.X = FMath::Cos(windAngle);
		windVector.Y = FMath::Sin(windAngle);

		// output findings to screen
		FString strOut = FString::SanitizeFloat(temperature) + "; " + FString::SanitizeFloat(windAngle) + " at " + FString::SanitizeFloat(windSpeed);
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, strOut);
	}
	else 
	{
		// fire error
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Could not deserialize json response"));
	}
}

float AFPSWeatherInformation::GetWindVector()
{
	return windAngle;
}

float AFPSWeatherInformation::GetWindSpeed()
{
	return windSpeed;
}
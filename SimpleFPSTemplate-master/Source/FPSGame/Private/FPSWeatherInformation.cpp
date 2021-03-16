// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSWeatherInformation.h"

// Sets default values
AFPSWeatherInformation::AFPSWeatherInformation()
{
	Http = &FHttpModule::Get();
}

// Called when the game starts or when spawned
void AFPSWeatherInformation::BeginPlay()
{
	Super::BeginPlay();
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HTTP Request NOT prcessed"));
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
	
		// get the temperature
		temperature = props->GetObjectField("temperature")->GetNumberField("value");

		// get the wind direction
		windDirection = props->GetObjectField("windDirection")->GetNumberField("value");

		// output findings to screen
		FString strOut = FString::SanitizeFloat(temperature) + "; " + FString::SanitizeFloat(windDirection);
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, strOut);
	}
	else 
	{
		// fire error
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("Could not deserialize json response"));
	}
}

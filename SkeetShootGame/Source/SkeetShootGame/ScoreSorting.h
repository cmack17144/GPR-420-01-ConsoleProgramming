// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScoreSorting.generated.h"


/**
 * 
 */
UCLASS()
class SKEETSHOOTGAME_API UScoreSorting : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "MySort")
		static void SortStringArrays(UPARAM(ref) TArray<FString> &playerNames, int32 index, TArray<FString> &SortedArray);
	
	UFUNCTION(BlueprintCallable, Category = "MySort")
		static void SortIntArray(UPARAM(ref) TArray<int32> &playerScores, bool descending, TArray<int32> &SortedArray);

	UFUNCTION(BlueprintCallable, Category = "MySort")
		static void SortPlayerScores(UPARAM(ref) TArray<int32>& playerScores, TArray<int32>& SortedScores, UPARAM(ref) TArray<FString>& playerNames, TArray<FString>& SortedNames);
};

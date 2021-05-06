// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSorting.h"

void UScoreSorting::SortStringArrays(UPARAM(ref) TArray<FString>& playerNames, int32 index, TArray<FString>& SortedArray) 
{

}

void UScoreSorting::SortIntArray(UPARAM(ref) TArray<int32>& playerScores, bool descending, TArray<int32>& SortedArray)
{

}

void UScoreSorting::SortPlayerScores(UPARAM(ref) TArray<int32>& playerScores, TArray<int32>& SortedScores, UPARAM(ref) TArray<FString>& playerNames, TArray<FString>& SortedNames)
{

	for (int32 j = 0; j < (playerScores.Num() - 1); j++)
	{
		if (playerScores[j] < playerScores[j + 1])
		{
			int32 val = playerScores[j];
			FString curName = playerNames[j];

			playerScores[j] = playerScores[j + 1];
			playerScores[j + 1] = val;

			playerNames[j] = playerNames[j + 1];
			playerNames[j + 1] = curName;
		}
	}

	SortedScores = playerScores;
	SortedNames = playerNames;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "ShmupFunctionLibrary.h"

void UShmupFunctionLibrary::PrintString(const FString& Text, const float DisplayTime, const FColor Color, const int32 Slot)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Slot, DisplayTime, Color, Text);
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *Text);
}

void UShmupFunctionLibrary::PrintWarning(const FString& Text, float DisplayTime, FColor Color, int32 Slot)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Slot, DisplayTime, Color, Text);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Text);
}

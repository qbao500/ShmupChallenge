// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShmupFunctionLibrary.generated.h"

#define SFL UShmupFunctionLibrary

UCLASS()
class SHMUPCHALLENGE_API UShmupFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void PrintString(const FString& Text, float DisplayTime = 2, FColor Color = FColor::Cyan, int32 Slot = -1);
	static void PrintWarning(const FString& Text, float DisplayTime = 5, FColor Color = FColor::Yellow, int32 Slot = -1);
};

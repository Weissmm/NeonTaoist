// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NeoTaoistGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEONTAOIST_API UNeoTaoistGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UNeoTaoistGameInstance();

	float volumeFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UserData")
	FString UserName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UserData")
	int Id;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UserData")
	int MaxStage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UserData")
	float CurrentTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UserData")
	FText TimeText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UserData")
	TArray<bool> EatEgg;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UserData")
	int CurrentStage;
};

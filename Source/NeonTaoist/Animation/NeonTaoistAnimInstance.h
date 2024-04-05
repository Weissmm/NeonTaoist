// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NeonTaoistAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEONTAOIST_API UNeonTaoistAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		bool bIsClimbing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsLookingRight = false;
};

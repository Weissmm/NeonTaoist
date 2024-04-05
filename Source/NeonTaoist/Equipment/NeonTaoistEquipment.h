// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "UObject/Object.h"
#include "NeonTaoistEquipment.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class NEONTAOIST_API UNeonTaoistEquipment : public UObject
{
	GENERATED_BODY()
public:
	// 名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	// 描述
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Describe;

	// 图片
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture* Picture;

	// 效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGameplayEffect* Effect;

	// 是否装备
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEquipping = false;
};

UCLASS(BlueprintType)
class NEONTAOIST_API UNeonTaoistBag : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<UNeonTaoistEquipment*, uint32> Equipments;
};
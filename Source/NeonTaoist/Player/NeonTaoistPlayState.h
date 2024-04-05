// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NeonTaoist/Equipment/NeonTaoistEquipment.h"
#include "UObject/Object.h"
#include "NeonTaoistPlayState.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ANeonTaoistPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UNeonTaoistEquipment*> Equipments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UNeonTaoistEquipment*> Market;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> MarketPrice;
	
	UFUNCTION(BlueprintCallable)
	void SellEquipment(UNeonTaoistEquipment* CurEquipment);

	UFUNCTION(BlueprintCallable)
	void BuyEquipment(UNeonTaoistEquipment* CurEquipment);

	UFUNCTION(BlueprintCallable)
	void EquipEquipment(UNeonTaoistEquipment* CurEquipment);

	UFUNCTION(BlueprintCallable)
	void UnEquipEquipment(UNeonTaoistEquipment* CurEquipment);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "BPC_CardList.generated.h"


UCLASS(Blueprintable)
class NEONTAOIST_API UBPC_CardList : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBPC_CardList();

	// CardNum
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	int jumpCardNum;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	int boomCardNum;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	int sprintCardNum;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	int handGunAmmo;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	int rifleGunAmmo;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	int maxHandGunAmmo;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	int maxRifleGunAmmo;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	int handGunAmmoPerCard;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	int rifleGunAmmoPerCard;
	// Ability
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	TSubclassOf<UGameplayAbility> ClickAbility1;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	TSubclassOf<UGameplayAbility> ClickAbility2;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	TSubclassOf<UGameplayAbility> SlideAbility1;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	TSubclassOf<UGameplayAbility> SlideAbility2;

	// SetAbility
	UFUNCTION(BlueprintCallable,Category="Abilities")
	void SetAbility1(TSubclassOf<UGameplayAbility>ClickAbility,TSubclassOf<UGameplayAbility>SlideAbility);

	UFUNCTION(BlueprintCallable,Category="Abilities")
	void SetAbility2(TSubclassOf<UGameplayAbility>ClickAbility,TSubclassOf<UGameplayAbility>SlideAbility);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "BPC_CardList.h"

// Sets default values for this component's properties
UBPC_CardList::UBPC_CardList()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBPC_CardList::BeginPlay()
{
	Super::BeginPlay();
	maxRifleGunAmmo = 9;
	maxHandGunAmmo = 18;
	rifleGunAmmo = 0 ;
	handGunAmmo =0;
	handGunAmmoPerCard = 6;
	rifleGunAmmoPerCard = 3;
	// ...
	
}


// Called every frame
void UBPC_CardList::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBPC_CardList::SetAbility1(TSubclassOf<UGameplayAbility> ClickAbility, TSubclassOf<UGameplayAbility> SlideAbility)
{
	ClickAbility1 = ClickAbility;
	SlideAbility1 = SlideAbility;
}

void UBPC_CardList::SetAbility2(TSubclassOf<UGameplayAbility> ClickAbility, TSubclassOf<UGameplayAbility> SlideAbility)
{
	ClickAbility2 = ClickAbility;
	SlideAbility2 = SlideAbility;
}
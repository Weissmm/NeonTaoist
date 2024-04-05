#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "NeonTaoistASCActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FASCActorOnHealthChangeEvent, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FASCActorOnDying);

UCLASS(config=Game)
class ANeonTaoistASCActor : public AActor , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ANeonTaoistASCActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;
	
public:	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	UAbilitySystemComponent* AbilitySystem;

	/** Attribute update function */
	UPROPERTY(BlueprintAssignable, Category="Ability")
	FASCActorOnHealthChangeEvent HealthChangeEvent;
	
	UPROPERTY(BlueprintAssignable)
	FASCActorOnDying OnDying;

	UFUNCTION(BlueprintCallable)
	virtual void Die();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
	
protected:
	virtual void BeginPlay() override;
	
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "NeonTaoist/Ability/AttributeSet/NTAttributeSetBase.h"
#include "Components/BoxComponent.h"
#include "NeonTaoistMonsterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMonsterOnHealthChangeEvent, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMonsterOnDying);

UCLASS(config=Game)
class ANeonTaoistMonsterBase : public APawn , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ANeonTaoistMonsterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
public:	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Abilities")
	UAbilitySystemComponent* AbilitySystem;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	virtual void Die();
		
	/** Attribute update function */
	UPROPERTY(BlueprintAssignable, Category="Ability")
	FMonsterOnHealthChangeEvent HealthChangeEvent;
	
	UPROPERTY(BlueprintAssignable)
    FMonsterOnDying OnDying;
	
	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
	
protected:
	virtual void BeginPlay() override;
	
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Ability/AttributeSet/NTAttributeSetBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/NeonTaoistAnimInstance.h"
#include "NeonTaoistCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeEvent, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChangeEvent, float, NewEnergy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDying);

UCLASS(config=Game)
class ANeonTaoistCharacter : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="NeonTaoistCharacter|Abilities")
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NeonTaoistCharacter|AnimInstance")
	UNeonTaoistAnimInstance* AnimInstance; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="NeonTaoistCharacter|Movement")
	bool bMovingOnX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="NeonTaoistCharacter|Movement")
	bool bMovingOnXR;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="NeonTaoistCharacter|Movement")
	bool bMovingOnY;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="NeonTaoistCharacter|Movement")
	bool bMovingOnYR;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="NeonTaoistCharacter|Movement")
	bool bTurnning;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BombVector")
	FRotator BombRotation;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	/** Character State */
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "NeonTaoistCharacter|CharacterState")
	virtual void FinishDying();
	
	UPROPERTY(BlueprintAssignable)
	FPlayerDying OnPlayerDying;
	
	/** Add Ability to Character */
	UFUNCTION(BlueprintCallable, Category="NeonTaoistCharacter|Ability")
	void GiveAbility(TSubclassOf<UGameplayAbility> Ability);

	/** Add Ability to Character */
	UFUNCTION(BlueprintCallable, Category="NeonTaoistCharacter|Ability")
	void SetBombDirection(FVector2D Direction);

	/** Attribute update function */
	UPROPERTY(BlueprintAssignable, Category="NeonTaoistCharacter|Ability")
	FOnHealthChangeEvent HealthChangeEvent;
	UPROPERTY(BlueprintAssignable, Category="NeonTaoistCharacter|Ability")
	FOnEnergyChangeEvent EnergyChangeEvent;
	
	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
	void OnEnergyAttributeChanged(const FOnAttributeChangeData& Data);
	
protected:
	virtual void BeginPlay() override;
	
	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	void Jump();

	void StopJumping();

public:
	ANeonTaoistCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

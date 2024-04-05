#include "NeonTaoistCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ANeonTaoistCharacter::ANeonTaoistCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 900.f;
	GetCharacterMovement()->MaxFlySpeed = 900.f;

	//Configure turner
	bMovingOnX=true;
	bMovingOnXR=false;
	bMovingOnY=false;
	bMovingOnYR=false;
	//Ability相关
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
	// PlayerAttributeSet = CreateDefaultSubobject<UNTAttributeSetBase>("PlayerAttributeSet");
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

void ANeonTaoistCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(AbilitySystem)
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UNTAttributeSetBase::GetHealthAttribute())
		.AddUObject(this, &ANeonTaoistCharacter::OnHealthAttributeChanged);
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UNTAttributeSetBase::GetEnergyAttribute())
		.AddUObject(this, &ANeonTaoistCharacter::OnEnergyAttributeChanged);
	}
	AnimInstance = (UNeonTaoistAnimInstance*)(GetMesh()->GetAnimInstance());
	bMovingOnX = true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANeonTaoistCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	/*PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);*/
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ANeonTaoistCharacter::MoveRight);

	/*PlayerInputComponent->BindTouch(IE_Pressed, this, &ANeonTaoistCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ANeonTaoistCharacter::TouchStopped);*/
}

void ANeonTaoistCharacter::Jump()
{
	/*if (AnimInstance->bIsClimbing) {
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		AnimInstance->bIsClimbing = false;
		MoveRight(GetActorRotation().Yaw / abs(GetActorRotation().Yaw) * -1);
		Super::Jump();
		GetCharacterMovement()->AddImpulse(FVector(0, 500, 0) * GetActorForwardVector(), true);
		return;
	}*/
	Super::Jump();
}

void ANeonTaoistCharacter::StopJumping()
{
	Super::StopJumping();
}

void ANeonTaoistCharacter::MoveRight(float Value)
{
	if(bMovingOnX)
	{
		if(bMovingOnXR)
		{
			if (Value > 0 && GetActorRotation().Yaw != -90)
			{
				FRotator NewRotator(0, -90, 0);
				AnimInstance->bIsLookingRight = true;
				SetActorRotation(NewRotator);
			}
			if (Value < 0 && GetActorRotation().Yaw != 90)
			{
				FRotator NewRotator(0, 90, 0);
				SetActorRotation(NewRotator);
				AnimInstance->bIsLookingRight = false;
			}
		}
		else {
			if (Value > 0 && GetActorRotation().Yaw != 90)
			{
				FRotator NewRotator(0, 90, 0);
				AnimInstance->bIsLookingRight = true;
				SetActorRotation(NewRotator);
			}
			if (Value < 0 && GetActorRotation().Yaw != -90)
			{
				FRotator NewRotator(0, -90, 0);
				SetActorRotation(NewRotator);
				AnimInstance->bIsLookingRight = false;
			}
		}
	}
	else
	{
		if(bMovingOnYR)
		{
			if (Value > 0 && GetActorRotation().Yaw != 180)
			{
				FRotator NewRotator(0, 180, 0);
				SetActorRotation(NewRotator);
				AnimInstance->bIsLookingRight = true;
			}
			if (Value < 0 && GetActorRotation().Yaw != 0)
			{
				FRotator NewRotator(0, 0, 0);
				SetActorRotation(NewRotator);
				AnimInstance->bIsLookingRight = false;
			}
		}
		else{
			if (Value > 0 && GetActorRotation().Yaw != 0)
			{
				FRotator NewRotator(0, 0, 0);
				SetActorRotation(NewRotator);
				AnimInstance->bIsLookingRight = true;
			}
			if (Value < 0 && GetActorRotation().Yaw != 180)
			{
				FRotator NewRotator(0, 180, 0);
				SetActorRotation(NewRotator);
				AnimInstance->bIsLookingRight = false;
			}
		}
	}
	if (AnimInstance->bIsClimbing == true) {
		return;
	}
	// add movement in that direction
	if(bMovingOnX)
	{
		if(bMovingOnXR)
		{
			AddMovementInput(FVector(0.f,-1.f,0.f), Value);
		}else
		{
			AddMovementInput(FVector(0.f,1.f,0.f), Value);
		}
	}
	else
	{
		if(bMovingOnYR)
		{
			AddMovementInput(FVector(-1.f,0.f,0.f), Value);
		}
		else
		{
			AddMovementInput(FVector(1.f,0.f,0.f), Value);

		}
	}
}

void ANeonTaoistCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ANeonTaoistCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ANeonTaoistCharacter::Die()
{
	OnPlayerDying.Broadcast();
	//FinishDying();
}

void ANeonTaoistCharacter::FinishDying()
{
	Destroy();
}


UAbilitySystemComponent* ANeonTaoistCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void ANeonTaoistCharacter::GiveAbility(TSubclassOf<UGameplayAbility> Ability)
{
	if(AbilitySystem)
	{
		if(HasAuthority() && Ability)
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability,1));
		}
		AbilitySystem->InitAbilityActorInfo(this,this);
	}
}

void ANeonTaoistCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	HealthChangeEvent.Broadcast(Data.NewValue);
	if(Data.NewValue <= 0)
	{
		Die();
	}
}

void ANeonTaoistCharacter::OnEnergyAttributeChanged(const FOnAttributeChangeData& Data)
{
	EnergyChangeEvent.Broadcast(Data.NewValue);
}

void ANeonTaoistCharacter::SetBombDirection(FVector2d Direction)
{
	Direction = Direction.GetSafeNormal(0.001);
	float X = Direction.X;
	float Y = Direction.Y;

	BombRotation.Pitch = FMath::Atan(Y / FMath::Abs(X)) * 180 / DOUBLE_PI;
	if(bMovingOnX)
	{
		if(bMovingOnXR)
		{
			BombRotation.Yaw = X > 0 ? -90 : 90;	
		}
		else {
			BombRotation.Yaw = X > 0 ? 90 : -90;	
		}
	}
	else
	{
		if(bMovingOnYR)
		{
			BombRotation.Yaw = X > 0 ? 180 : 0;	
		}
		else{
			BombRotation.Yaw = X > 0 ? 0 : 180;
		}
	}
}




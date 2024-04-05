#include "NeonTaoistASCActor.h"

#include "GameFramework/GameStateBase.h"
#include "NeonTaoist/NeonTaoistGameState.h"

ANeonTaoistASCActor::ANeonTaoistASCActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 碰撞相关
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("MonsterBoxCollision");
	
	BoxComponent->SetCollisionProfileName(FName("OverlapOnlyPawn"));

	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetShouldUpdatePhysicsVolume(true);
	BoxComponent->SetCanEverAffectNavigation(false);
	BoxComponent->bDynamicObstacle = true;
	RootComponent = BoxComponent;
		
	//Ability相关
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
}

void ANeonTaoistASCActor::BeginPlay()
{
	Super::BeginPlay();
	if(AbilitySystem && AbilitySystem->HasAttributeSetForAttribute(UNTAttributeSetBase::GetHealthAttribute()))
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UNTAttributeSetBase::GetHealthAttribute())
		.AddUObject(this, &ANeonTaoistASCActor::OnHealthAttributeChanged);
	}
}

void ANeonTaoistASCActor::Die()
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	OnDying.Broadcast();
	SetLifeSpan(0.03);
}

UAbilitySystemComponent* ANeonTaoistASCActor::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void ANeonTaoistASCActor::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	HealthChangeEvent.Broadcast(Data.NewValue);
	if(Data.NewValue <= 0)
	{
		Die();
	}
}

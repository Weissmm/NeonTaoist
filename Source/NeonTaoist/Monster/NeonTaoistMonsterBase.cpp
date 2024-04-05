#include "NeonTaoistMonsterBase.h"

#include "GameFramework/GameStateBase.h"
#include "NeonTaoist/NeonTaoistGameState.h"

ANeonTaoistMonsterBase::ANeonTaoistMonsterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 碰撞相关
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("MonsterBoxCollision");
	BoxComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetShouldUpdatePhysicsVolume(true);
	BoxComponent->SetCanEverAffectNavigation(false);
	BoxComponent->bDynamicObstacle = true;
	RootComponent = BoxComponent;

	// Mesh相关
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>("MonsterMesh");
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(BoxComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
	}
	
	//Ability相关
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
}

void ANeonTaoistMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	if(AbilitySystem)
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UNTAttributeSetBase::GetHealthAttribute())
		.AddUObject(this, &ANeonTaoistMonsterBase::OnHealthAttributeChanged);
	}

	AGameStateBase* GameState = GetWorld()->GetGameState();
	if(ANeonTaoistGameState* NTGameState = Cast<ANeonTaoistGameState>(GameState))
	{
		NTGameState->AddMonster(this);
	}
}

void ANeonTaoistMonsterBase::Die()
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	if(ANeonTaoistGameState* NTGameState = Cast<ANeonTaoistGameState>(GameState))
	{
		NTGameState->DeleteMonster(this);
	}
	OnDying.Broadcast();
	Destroy();
}

UAbilitySystemComponent* ANeonTaoistMonsterBase::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void ANeonTaoistMonsterBase::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	HealthChangeEvent.Broadcast(Data.NewValue);
	if(Data.NewValue <= 0)
	{
		Die();
	}
}

#include "NeonTaoistAbilityCommon.h"

#include "NeonTaoistASCActor.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
// #include "Engine/Private/KismetTraceUtils.h"
#include "NeonTaoist/NeonTaoistCharacter.h"
#include "NeonTaoist/NeonTaoistGameState.h"

void UNeonTaoistAbilityCommon::TryTeleportTo(AActor* TargetActor, float AxisDistance, const FRotator& DestRotation)
{
	if(!TargetActor)
	{
		UE_LOG(LogActor, Error, TEXT("TryTeleportTo get invalid actor"));
		return;
	}

	const FVector CurLocation = TargetActor->GetActorLocation();
	const FRotator CurRotation = TargetActor->GetActorRotation();

	// 获得理想的传送位置
	FVector DeltaLocation(0, 0, AxisDistance);
	if(FMath::Abs(FMath::Abs(CurRotation.Yaw) - 90) < 1)
	{
		DeltaLocation.Y	= CurRotation.Yaw >= 0 ? AxisDistance : -AxisDistance;
	}
	else
	{
		DeltaLocation.X	= CurRotation.Yaw < 90 ? AxisDistance : -AxisDistance;
	}
	FVector DestLocation = CurLocation + DeltaLocation;

	// 判断是否会穿过墙体
	UWorld* World = TargetActor->GetWorld();
	static const FName FlashTraceName(TEXT("GetFlashDestLocation"));
	FCollisionQueryParams Params(FlashTraceName, true, TargetActor);
	if(World)
	{
		FHitResult OutHit;
		if(World->LineTraceSingleByProfile(OutHit, CurLocation, DestLocation, "FlashUse", Params))
		{
			DestLocation = OutHit.Location;
		}
	}
	// 判断是否挤到外侧
	FVector CurDestLocation = DestLocation;
	while(!TryToGetTeleportLocation(TargetActor, DestLocation, DestRotation, CurDestLocation) || FMath::Abs(CurDestLocation.Y - CurLocation.Y) > AxisDistance
		|| FMath::Abs(CurDestLocation.Z - CurLocation.Z) > AxisDistance || FMath::Abs(CurDestLocation.X - CurLocation.X) > AxisDistance)
	{
		if(FMath::Abs(FMath::Abs(CurRotation.Yaw) - 90) < 1)
		{
			DestLocation.Y -= DeltaLocation.Y > 0 ? 30 : -30;
		}
		else
		{
			DestLocation.X -= DeltaLocation.X > 0 ? 30 : -30;
		}
		DestLocation.Z -= 30;
		CurDestLocation = DestLocation;
	}

	if(FMath::Abs(FMath::Abs(CurRotation.Yaw) - 90) < 1)
	{
		CurDestLocation.X = CurLocation.X;
	}
	else
	{
		CurDestLocation.Y = CurLocation.Y;
	}
	
	TargetActor->TeleportTo(CurDestLocation, DestRotation);
}

// UE原生Teleport无法获得目标地点，Copy过来进行修改
bool UNeonTaoistAbilityCommon::TryToGetTeleportLocation(const AActor* TargetActor, const FVector& DestLocation, const FRotator& DestRotation, FVector& NewLocation)
{
	auto RootComponent = TargetActor->GetRootComponent();
	if(RootComponent == nullptr)
	{
		return false;
	}

	UWorld* MyWorld = TargetActor->GetWorld();

	// Can't move non-movable actors during play
	if( (RootComponent->Mobility == EComponentMobility::Static) && MyWorld->AreActorsInitialized() )
	{
		return false;
	}

	if (TargetActor->GetActorLocation() == DestLocation) 
	{
		return true;
	}

	FVector const PrevLocation = TargetActor->GetActorLocation();
	NewLocation = DestLocation;
	bool bTeleportSucceeded = true;
	UPrimitiveComponent* ActorPrimComp = Cast<UPrimitiveComponent>(RootComponent);
	if ( ActorPrimComp )
	{
		if ((ActorPrimComp->IsQueryCollisionEnabled() || (TargetActor->GetNetMode() != NM_Client)) )
		{
			// Apply the pivot offset to the desired location
			FVector Offset = RootComponent->Bounds.Origin - PrevLocation;
			NewLocation = NewLocation + Offset;

			// check if able to find an acceptable destination for this actor that doesn't embed it in world geometry
			bTeleportSucceeded = MyWorld->FindTeleportSpot(TargetActor, NewLocation, DestRotation);
			NewLocation = NewLocation - Offset;
		}

		if (NewLocation.ContainsNaN() || PrevLocation.ContainsNaN())
		{
			bTeleportSucceeded = false;
			UE_LOG(LogActor, Log,  TEXT("Attempted to teleport to NaN"));
		}
		
	}
	return bTeleportSucceeded; 
}

void UNeonTaoistAbilityCommon::GetNearByMonsters(AActor* TargetActor, FVector BoxExtent, TArray<ANeonTaoistMonsterBase*>& Monsters, bool DrawBox)
{
	Monsters.Empty();
	FVector CurLocation = TargetActor->GetActorLocation();
	
	TSet<ANeonTaoistMonsterBase*> AllMonsters;
	AGameStateBase* GameState = TargetActor->GetWorld()->GetGameState();
	if(ANeonTaoistGameState* NTGameState = Cast<ANeonTaoistGameState>(GameState))
	{
		AllMonsters = NTGameState->GetAllMonsters();
	}

	FRotator BoxRotator(TargetActor->GetActorRotation());
	BoxRotator.Yaw += 90;
	FTransform BoxTransform(BoxRotator, CurLocation);
	for(auto& Monster : AllMonsters)
	{
		if(UKismetMathLibrary::IsPointInBoxWithTransform(Monster->GetActorLocation(), BoxTransform, BoxExtent))
		{
			Monsters.Add(Monster);
		}
	}
	Monsters.Sort([](const ANeonTaoistMonsterBase& A, const ANeonTaoistMonsterBase& B)
	{
		return A.GetActorLocation().Y < B.GetActorLocation().Y;
	});

	if(DrawBox)
	{
		DrawDebugBox(TargetActor->GetWorld(), CurLocation, BoxExtent, BoxRotator.Quaternion(), FColor::White, false, 5);
	}
}

void UNeonTaoistAbilityCommon::ApplyGEToTargetWithCallerData(AActor* SourceActor, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, FGameplayTag DataTag, float Magnitude, float Level)
{
	if(!SourceActor || !TargetActor)
	{
		UE_LOG(LogActor, Error, TEXT("ApplyGE Use Valid Actors"));
		return;
	}
	
	UAbilitySystemComponent* SourceASC = GetAbilitySystemCompOfActor(SourceActor);
	UAbilitySystemComponent* TargetASC = GetAbilitySystemCompOfActor(TargetActor);

	if(!SourceASC || !TargetASC)
	{
		UE_LOG(LogActor, Error, TEXT("ApplyGE Use Valid Actors"));
		return;
	}

	UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	auto Context = SourceASC->MakeEffectContext();
	FGameplayEffectSpec	Spec(GameplayEffect, Context, Level);
	FPredictionKey PredictionKey = FPredictionKey();
	Spec.SetSetByCallerMagnitude(DataTag, -Magnitude);
	
	SourceASC->ApplyGameplayEffectSpecToTarget(Spec, TargetASC, PredictionKey);
}

void UNeonTaoistAbilityCommon::SetVelocityAfterFlash(ANeonTaoistCharacter* Player, float BaseSpeed, float PlayerSpeed, float Ratio)
{

	if(!Player)
	{
		UE_LOG(LogActor, Error, TEXT("Invalid Player"));
		return;
	}

	// 根据朝向计算新速度
	FRotator PlayerRotator = Player->GetActorRotation();
	float NewSpeed = BaseSpeed + PlayerSpeed * Ratio;
	FVector NewVelocity(0.f);
	NewVelocity.Z = NewSpeed;
	if(FMath::Abs(FMath::Abs(PlayerRotator.Yaw) - 90) < 1)
	{
		NewVelocity.Y = PlayerRotator.Yaw >= 0 ? NewSpeed : -NewSpeed;
	}
	else
	{
		NewVelocity.X = PlayerRotator.Yaw < 90 ? NewSpeed : -NewSpeed;
	}

	// 设置速度
	UPawnMovementComponent* Movement = Player->GetMovementComponent();
	Movement->Velocity = NewVelocity;
}

float UNeonTaoistAbilityCommon::GetGroundAngle(AActor* TargetActor)
{
	if(!TargetActor)
	{
		UE_LOG(LogActor, Error, TEXT("Invalid Actor"));
		return 0.f;
	}
	UWorld* World = TargetActor->GetWorld();
	static const FName GroundTraceSingleName(TEXT("GetGroundAngle"));
	FCollisionQueryParams Params(GroundTraceSingleName, true, TargetActor);
	if(World)
	{
		FHitResult OutHit;
		FVector Start = TargetActor->GetActorLocation();
		FVector End = Start - FVector(0, 0, 10000);
		if(World->LineTraceSingleByProfile(OutHit, Start, End, "BlockOnlyStatic", Params))
		{
			if(OutHit.ImpactNormal.Z == 1)
			{
				return 0.f;
			}
			FRotator ImpactRotator = OutHit.ImpactNormal.Rotation();
			float Angle = OutHit.ImpactNormal.Y > 0 ? ImpactRotator.Pitch -90 : 90 - ImpactRotator.Pitch;
			return Angle;
		}
	}
	return 0.f;
}

FVector UNeonTaoistAbilityCommon::GetForwardDirection(AActor* TargetActor)
{
	FVector NewVelocity = FVector::Zero();
	if(!TargetActor)
	{
		UE_LOG(LogActor, Error, TEXT("Invalid Actor"));
		return NewVelocity;
	}
	UWorld* World = TargetActor->GetWorld();
	static const FName GroundTraceSingleName(TEXT("GetGroundAngle"));
	FCollisionQueryParams Params(GroundTraceSingleName, true, TargetActor);
	if(World)
	{
		FHitResult OutHit;
		FVector Start = TargetActor->GetActorLocation();
		FVector End = Start - FVector(0, 0, 10000);
		if(World->LineTraceSingleByProfile(OutHit, Start, End, "BlockOnlyStatic", Params))
		{
			FRotator PlayerRotation = TargetActor->GetActorRotation();
			FRotator ImpactRotator = OutHit.ImpactNormal.Rotation();
			double Angle = FMath::Abs(ImpactRotator.Pitch - 90)  / 180 * DOUBLE_PI;
			if(IsDirectionY(TargetActor))
			{
				NewVelocity.Y = PlayerRotation.Yaw > 0 ? FMath::Cos(Angle) : -FMath::Cos(Angle);
			}
			else
			{
				NewVelocity.X = PlayerRotation.Yaw < 90 ? FMath::Cos(Angle) : -FMath::Cos(Angle);		
			}
			NewVelocity.Z = FMath::IsNearlyEqual(PlayerRotation.Yaw, ImpactRotator.Yaw, 1) ? -FMath::Sin(Angle) : FMath::Sin(Angle);
		}
		else
		{
			FRotator PlayerRotation = TargetActor->GetActorRotation();
			NewVelocity = PlayerRotation.Vector();
		}
	}
	return NewVelocity;
}

bool UNeonTaoistAbilityCommon::IsDirectionY(AActor* TargetActor)
{
	if(!TargetActor)
	{
		UE_LOG(LogActor, Error, TEXT("Invalid Actor"));
		return true;
	}
	if(FMath::Abs(FMath::Abs(TargetActor->GetActorRotation().Yaw) - 90) < 1)
	{
		return true;
	}
	return false;
}

void UNeonTaoistAbilityCommon::OverlapRangeQuery(AActor* TargetActor, float Radius, FName ProfileName, TArray<AActor*>& OverlapActors)
{
	FVector CurLocation = TargetActor->GetActorLocation();
	static const FName OverlapRangeQuery(TEXT("OverlapRangeQuery"));
	FCollisionQueryParams Params(OverlapRangeQuery, true, TargetActor);
	TArray<struct FOverlapResult> OutOverlaps;
	TargetActor->GetWorld()->OverlapMultiByProfile(OutOverlaps, CurLocation, FQuat::Identity, ProfileName, FCollisionShape::MakeSphere(Radius), Params);
	for(auto& Overlap : OutOverlaps)
	{
		OverlapActors.Add(Overlap.GetActor());
	}
}

FRotator UNeonTaoistAbilityCommon::GetMonsterNewRoll(AActor* Player, AActor* Monster)
{
	if(!Player || !Monster)
	{	
		return FRotator(0.f);
	}
	FVector PlayerLoc = Player->GetActorLocation();
	FVector MonsterLoc = Monster->GetActorLocation();
	FVector ForwardVector = PlayerLoc - MonsterLoc;
	
	FRotator Result(0.f);
	Result.Pitch = Monster->GetActorRotation().Pitch;
	if(IsDirectionY(Player))
	{
		if(ForwardVector.Y <= 0)
		{
			Result.Yaw = 0;
			Result.Roll = FMath::Atan(ForwardVector.Z / -ForwardVector.Y) * (180.0)/DOUBLE_PI;
		}
		else
		{
			Result.Yaw = 180;
			Result.Roll = FMath::Atan(ForwardVector.Z / ForwardVector.Y) * (180.0)/DOUBLE_PI;
		}
	}
	else
	{
		if(ForwardVector.X <= 0)
		{
			Result.Yaw = -90;
			Result.Roll = FMath::Atan(ForwardVector.Z / -ForwardVector.X) * (180.0)/DOUBLE_PI;
		}
		else
		{
			Result.Yaw = 90;
			Result.Roll = FMath::Atan(ForwardVector.Z / ForwardVector.X) * (180.0)/DOUBLE_PI;
		}
	}
	return Result;
}

UAbilitySystemComponent* UNeonTaoistAbilityCommon::GetAbilitySystemCompOfActor(AActor* TargetActor)
{
	if(ANeonTaoistMonsterBase* Monster = Cast<ANeonTaoistMonsterBase>(TargetActor))
	{
		return Monster->GetAbilitySystemComponent();
	}
	if(ANeonTaoistCharacter* Player = Cast<ANeonTaoistCharacter>(TargetActor))
	{
		return Player->GetAbilitySystemComponent();
	}
	if(ANeonTaoistASCActor* ASCActor = Cast<ANeonTaoistASCActor>(TargetActor))
	{
		return ASCActor->GetAbilitySystemComponent();
	}
	return nullptr;
}



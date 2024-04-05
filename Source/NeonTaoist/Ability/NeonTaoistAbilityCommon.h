#pragma once
#include "NeonTaoist/NeonTaoistCharacter.h"
#include "NeonTaoist/Monster/NeonTaoistMonsterBase.h"
#include "NeonTaoistAbilityCommon.generated.h"

UCLASS(Blueprintable)
class UNeonTaoistAbilityCommon: public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void TryTeleportTo(AActor* TargetActor, float AxisDistance, const FRotator& DestRotation);
	
	static bool TryToGetTeleportLocation(const AActor* TargetActor, const FVector& DestLocation, const FRotator& DestRotation, FVector& NewLocation);

	UFUNCTION(BlueprintCallable)
	static void GetNearByMonsters(AActor* TargetActor, FVector BoxExtent, TArray<ANeonTaoistMonsterBase*>& Monsters, bool DrawBox = false);

	UFUNCTION(BlueprintCallable)
	static void ApplyGEToTargetWithCallerData(AActor* SourceActor, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, FGameplayTag DataTag, float Magnitude, float Level = 1.0f);

	UFUNCTION(BlueprintCallable)
	static void SetVelocityAfterFlash(ANeonTaoistCharacter* Player, float BaseSpeed, float PlayerSpeed, float Ratio = 1.0f);

	UFUNCTION(BlueprintCallable)
	static float GetGroundAngle(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	static FVector GetForwardDirection(AActor* TargetActor);

	// 判断当前地图为沿Y轴还是X轴,true表示Y轴
	UFUNCTION(BlueprintCallable)
	static bool IsDirectionY(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	static void OverlapRangeQuery(AActor* TargetActor, float Radius, FName ProfileName, TArray<AActor*>& OverlapActors);

	UFUNCTION(BlueprintCallable)
	static FRotator GetMonsterNewRoll(AActor* Player, AActor* Monster);
	
	static UAbilitySystemComponent* GetAbilitySystemCompOfActor(AActor* TargetActor);
};

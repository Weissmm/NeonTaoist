// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Monster/NeonTaoistMonsterBase.h"
#include "NeonTaoistGameState.generated.h"

UCLASS()
class ANeonTaoistGameState : public AGameStateBase
{
	GENERATED_UCLASS_BODY()

public:
	// 地图怪物相关
	void AddMonster(ANeonTaoistMonsterBase* Monster);
	
	void DeleteMonster(const ANeonTaoistMonsterBase* Monster);
	
	UFUNCTION(BlueprintCallable)
	int GetMonsterCount() const {return MonsterContainer.Num();}
	
	TSet<ANeonTaoistMonsterBase*> GetAllMonsters() const {return MonsterContainer;}

	UFUNCTION(BlueprintCallable)
	bool GetEasterEgg() {return bPlayerGetEasterEgg;}

	UFUNCTION(BlueprintCallable)
	void SetEasterEgg(bool State) {bPlayerGetEasterEgg = State;}
	
private:
	UPROPERTY()
	TSet<ANeonTaoistMonsterBase*> MonsterContainer;

	UPROPERTY()
	bool bPlayerGetEasterEgg = false;
};

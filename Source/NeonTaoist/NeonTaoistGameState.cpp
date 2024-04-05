#include "NeonTaoistGameState.h"

ANeonTaoistGameState::ANeonTaoistGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void ANeonTaoistGameState::AddMonster(ANeonTaoistMonsterBase* Monster)
{
	if(Monster)
	{
		MonsterContainer.Add(Monster);
	}
}

void ANeonTaoistGameState::DeleteMonster(const ANeonTaoistMonsterBase* Monster)
{
	if(Monster)
	{
		MonsterContainer.Remove(Monster);
	}
}
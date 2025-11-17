// Copyright Kpable Games


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Description - Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum ", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</>\n<Default>Causes much more damage. </>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell locked until level: </><Level>%d</>"), Level);
}

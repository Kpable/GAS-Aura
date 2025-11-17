// Copyright Kpable Games


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

FString UAuraProjectileSpell::GetDescription(int32 Level)
{
	int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches a bolt of fire, exploding on impact and dealing </><Damage>%d</><Default> fire damage with a chance to burn.</>\n\n<Small>Level: </><Level>%d</>"), Damage, Level );
	}
	else
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches %d bolts of fire, exploding on impact and dealing </><Damage>%d</><Default> fire damage with a chance to burn.</>\n\n<Small>Level: </><Level>%d>/>"), FMath::Min(Level, MaxNumFireBolts), Damage, Level);
	}
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level)
{
	int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches %d bolts of fire, exploding on impact and dealing </><Damage>%d</><Default> fire damage with a chance to burn.</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, MaxNumFireBolts), Damage, Level);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//UKismetSystemLibrary::PrintString(this, FString("Activate Ability (C++)"), true, true, FLinearColor::Yellow, 3.f);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	
	const FVector SocketLocation =
		ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// Give the projectile a Gameplay Effect Spec for causing Damage.
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, Pair.Key, ScaledDamage);
	}
	
	Projectile->DamageEffectSpecHandle = EffectSpecHandle;
		
	Projectile->FinishSpawning(SpawnTransform);
}

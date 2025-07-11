// Copyright Kpable Games


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread * 0.5f, FVector::UpVector);
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread * 0.5f, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
		
		// DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 20.f, 12, FColor::Cyan, false, 3.f);
		// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 5.f, FLinearColor::Green, 3.f, 2.f);
		// DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 10.f, 12, FColor::Red, false, 3.f);
		// DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 10.f, 12, FColor::Red, false, 3.f);
	}

	return SpawnLocations;
}

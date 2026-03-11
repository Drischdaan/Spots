// Copyright Drischdaan All Rights Reserved Copyright

#include "Actors/SpotsVolume.h"

#include "Components/BoxComponent.h"

ASpotsVolume::ASpotsVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	VolumeComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("VolumeComponent"));
}

FBox ASpotsVolume::GetVolumeBounds() const
{
	return VolumeComponent->Bounds.GetBox();
}

float ASpotsVolume::GetGridCellSize() const
{
	return GridCellSize;
}

float ASpotsVolume::GetMinSpacing() const
{
	return MinSpacing;
}

const FGameplayTagContainer& ASpotsVolume::GetDefaultTags() const
{
	return DefaultTags;
}

const TArray<TSoftObjectPtr<USpotsEvaluator>>& ASpotsVolume::GetEvaluators() const
{
	return Evaluators;
}

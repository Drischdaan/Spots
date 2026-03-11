// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "CoreMinimal.h"

#include "Data/SpotsData.h"

#include "UObject/Object.h"
#include "SpotsEvaluator.generated.h"

class USpotsGrid;

UCLASS(Abstract, Blueprintable, EditInlineNew)
class SPOTS_API USpotsEvaluator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Spots")
	void EvaluateSpot(UWorld* World, UPARAM(ref) FSpotsData& Spot, const USpotsGrid* SpotsGrid) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Spots")
	void PostProcessSpots(UWorld* World, UPARAM(ref) TArray<FSpotsData>& Spots, USpotsGrid* SpotsGrid) const;

public:
	virtual void EvaluateSpot_Implementation(UWorld* World, FSpotsData& Spot, const USpotsGrid* SpotsGrid) const;
	virtual void PostProcessSpots_Implementation(UWorld* World, TArray<FSpotsData>& Spots, USpotsGrid* SpotsGrid) const;
};

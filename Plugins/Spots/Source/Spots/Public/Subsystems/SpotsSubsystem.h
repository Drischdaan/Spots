// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpotsSubsystem.generated.h"

struct FSpotsData;
class USpotsGrid;

UCLASS()
class SPOTS_API USpotsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Spots")
	TArray<FSpotsData> QuerySphere(FVector Center, float Radius) const;

	UFUNCTION(BlueprintCallable, Category = "Spots")
	TArray<FSpotsData> QuerySphereWithTags(FVector Center, float Radius, FGameplayTagContainer RequiredTags) const;

	UFUNCTION(BlueprintCallable, Category = "Spots")
	TArray<FSpotsData> QueryBox(FVector Center, FVector Extent) const;

	UFUNCTION(BlueprintCallable, Category = "Spots")
	TArray<FSpotsData> QueryBoxWithTags(FVector Center, FVector Extent, FGameplayTagContainer RequiredTags) const;

	UFUNCTION(BlueprintCallable, Category = "Spots")
	USpotsGrid* GetGrid(FName VolumeId) const;

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;

protected:
	void LoadSpotsData();

protected:
	UPROPERTY()
	TMap<FName, TObjectPtr<USpotsGrid>> Grids;
};

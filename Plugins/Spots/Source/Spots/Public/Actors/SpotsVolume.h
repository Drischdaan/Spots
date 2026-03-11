// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "GameFramework/Actor.h"
#include "SpotsVolume.generated.h"

class USpotsEvaluator;
class UBoxComponent;

UCLASS()
class SPOTS_API ASpotsVolume : public AActor
{
	GENERATED_BODY()

public:
	ASpotsVolume();

public:
	FBox GetVolumeBounds() const;

	float GetGridCellSize() const;
	float GetMinSpacing() const;
	const FGameplayTagContainer& GetDefaultTags() const;
	const TArray<TSoftObjectPtr<USpotsEvaluator>>& GetEvaluators() const;

protected: // Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spots", meta=(AllowPrivateAccess=true))
	TObjectPtr<UBoxComponent> VolumeComponent;

protected: // Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spots|Grid", meta=(AllowPrivateAccess=true, ClampMin = "10.0", UIMin = "10.0"))
	float GridCellSize = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spots|Generation", meta=(AllowPrivateAccess=true, ClampMin = "10.0", UIMin = "10.0"))
	float MinSpacing = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spots|Generation", meta = (AllowPrivateAccess = true))
	FGameplayTagContainer DefaultTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spots|Generation", meta=(AllowPrivateAccess=true))
	TArray<TSoftObjectPtr<USpotsEvaluator>> Evaluators;
};

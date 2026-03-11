// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "StructUtils/InstancedStruct.h"

#include "SpotsData.generated.h"

USTRUCT(BlueprintType)
struct SPOTS_API FSpotsData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector3f Position = FVector3f::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FInstancedStruct> CustomData;
};

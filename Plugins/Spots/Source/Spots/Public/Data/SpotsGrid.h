// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "CoreMinimal.h"
#include "SpotsData.h"
#include "UObject/Object.h"
#include "SpotsGrid.generated.h"

USTRUCT(BlueprintType)
struct SPOTS_API FSpotsHandle
{
	GENERATED_BODY()

public:
	bool IsValid() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Index = INDEX_NONE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Generation = 0;
};

UCLASS()
class SPOTS_API USpotsGrid : public UObject
{
	GENERATED_BODY()

private:
	struct FCell
	{
		TArray<int32> Indices;
	};

	struct FSlot
	{
		FSpotsData Data;
		FIntVector Cell = FIntVector::ZeroValue;
		int32 Generation = 0;
		bool bIsOccupied = false;
	};

public:
	UFUNCTION(BlueprintCallable, Category="Spots|Grid")
	void Initialize(float InCellSize);

	UFUNCTION(BlueprintCallable, Category="Spots|Grid")
	FSpotsHandle Insert(const FSpotsData& Data);

	UFUNCTION(BlueprintCallable, Category="Spots|Grid")
	bool Remove(FSpotsHandle Handle);

	UFUNCTION(BlueprintCallable, Category="Spots|Grid")
	bool IsHandleValid(FSpotsHandle Handle) const;

	UFUNCTION(BlueprintCallable, Category="Spots|Grid")
	bool GetDataFromHandle(FSpotsHandle Handle, FSpotsData& OutData) const;

	UFUNCTION(BlueprintCallable, Category="Spots|Grid")
	TArray<FSpotsHandle> QueryBox(FVector Center, FVector Extent) const;

	UFUNCTION(BlueprintCallable, Category="Spots|Grid")
	TArray<FSpotsHandle> QuerySphere(FVector Center, float Radius) const;

protected:
	FIntVector PositionToCell(const FVector3f& Position) const;

protected:
	float CellSize = 100.0f;

	TArray<FSlot> Slots;
	TArray<int32> FreeList;
	TMap<FIntVector, FCell> Cells;
};

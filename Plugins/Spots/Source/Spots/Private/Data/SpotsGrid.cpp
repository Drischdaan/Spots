// Copyright Drischdaan All Rights Reserved Copyright

#include "Data/SpotsGrid.h"

bool FSpotsHandle::IsValid() const
{
	return Index != INDEX_NONE;
}

void USpotsGrid::Initialize(const float InCellSize)
{
	CellSize = FMath::Max(InCellSize, 1.0f);
	Slots.Empty();
	FreeList.Empty();
	Cells.Empty();
}

FSpotsHandle USpotsGrid::Insert(const FSpotsData& Data)
{
	int32 SlotIndex;
	if (!FreeList.IsEmpty())
	{
		SlotIndex = FreeList.Pop();
	}
	else
	{
		SlotIndex = Slots.AddDefaulted();
	}

	FSlot& Slot = Slots[SlotIndex];
	Slot.Data = Data;
	Slot.Cell = PositionToCell(Data.Position);
	Slot.bIsOccupied = true;
	Cells.FindOrAdd(Slot.Cell).Indices.Add(SlotIndex);

	return FSpotsHandle{
		.Index = SlotIndex,
		.Generation = Slot.Generation
	};
}

bool USpotsGrid::Remove(const FSpotsHandle Handle)
{
	if (!IsHandleValid(Handle))
	{
		return false;
	}

	FSlot& Slot = Slots[Handle.Index];
	Slot.bIsOccupied = false;
	Slot.Generation++;

	if (FCell* Cell = Cells.Find(Slot.Cell))
	{
		Cell->Indices.RemoveSingle(Handle.Index);
		if (Cell->Indices.IsEmpty())
		{
			Cells.Remove(Slot.Cell);
		}
	}
	FreeList.Add(Handle.Index);
	return true;
}

bool USpotsGrid::IsHandleValid(const FSpotsHandle Handle) const
{
	return Handle.IsValid() && Slots.IsValidIndex(Handle.Index) && Slots[Handle.Index].Generation == Handle.Generation && Slots[Handle.Index].bIsOccupied;
}

bool USpotsGrid::GetDataFromHandle(const FSpotsHandle Handle, FSpotsData& OutData) const
{
	if (!IsHandleValid(Handle))
	{
		return false;
	}
	OutData = Slots[Handle.Index].Data;
	return true;
}

TArray<FSpotsHandle> USpotsGrid::QueryBox(const FVector Center, const FVector Extent) const
{
	TArray<FSpotsHandle> Results;

	const FVector3f Min = FVector3f(Center - Extent);
	const FVector3f Max = FVector3f(Center + Extent);
	const FIntVector MinCell = PositionToCell(Min);
	const FIntVector MaxCell = PositionToCell(Max);

	for (int32 X = MinCell.X; X <= MaxCell.X; ++X)
	{
		for (int32 Y = MinCell.Y; Y <= MaxCell.Y; ++Y)
		{
			for (int32 Z = MinCell.Z; Z <= MaxCell.Z; ++Z)
			{
				if (const FCell* Cell = Cells.Find(FIntVector(X, Y, Z)))
				{
					for (const int32 Index : Cell->Indices)
					{
						const FSlot& Slot = Slots[Index];
						const FVector3f& Pos = Slot.Data.Position;
						if (Pos.X >= Min.X && Pos.X <= Max.X &&
							Pos.Y >= Min.Y && Pos.Y <= Max.Y &&
							Pos.Z >= Min.Z && Pos.Z <= Max.Z)
						{
							FSpotsHandle Handle;
							Handle.Index = Index;
							Handle.Generation = Slot.Generation;
							Results.Add(Handle);
						}
					}
				}
			}
		}
	}

	return Results;
}

TArray<FSpotsHandle> USpotsGrid::QuerySphere(const FVector Center, const float Radius) const
{
	TArray<FSpotsHandle> Results;

	const FVector3f Center3f = FVector3f(Center);
	const float RadiusSq = Radius * Radius;
	const FVector Extent(Radius);
	const FIntVector MinCell = PositionToCell(FVector3f(Center - Extent));
	const FIntVector MaxCell = PositionToCell(FVector3f(Center + Extent));

	for (int32 X = MinCell.X; X <= MaxCell.X; ++X)
	{
		for (int32 Y = MinCell.Y; Y <= MaxCell.Y; ++Y)
		{
			for (int32 Z = MinCell.Z; Z <= MaxCell.Z; ++Z)
			{
				if (const FCell* Cell = Cells.Find(FIntVector(X, Y, Z)))
				{
					for (const int32 Index : Cell->Indices)
					{
						const FSlot& Slot = Slots[Index];
						if (FVector3f::DistSquared(Slot.Data.Position, Center3f) <= RadiusSq)
						{
							FSpotsHandle Handle;
							Handle.Index = Index;
							Handle.Generation = Slot.Generation;
							Results.Add(Handle);
						}
					}
				}
			}
		}
	}

	return Results;
}

FIntVector USpotsGrid::PositionToCell(const FVector3f& Position) const
{
	return FIntVector(
		FMath::FloorToInt(Position.X / CellSize),
		FMath::FloorToInt(Position.Y / CellSize),
		FMath::FloorToInt(Position.Z / CellSize)
	);
}

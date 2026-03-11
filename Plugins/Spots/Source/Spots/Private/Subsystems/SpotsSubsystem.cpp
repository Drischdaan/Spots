// Copyright Drischdaan All Rights Reserved Copyright

#include "Subsystems/SpotsSubsystem.h"

#include "Spots.h"

#include "AssetRegistry/AssetRegistryModule.h"

#include "Data/SpotsGrid.h"

TArray<FSpotsData> USpotsSubsystem::QuerySphere(const FVector Center, const float Radius) const
{
	TArray<FSpotsData> Results;
	for (const auto& [Id, Grid] : Grids)
	{
		TArray<FSpotsHandle> Handles = Grid->QuerySphere(Center, Radius);
		for (const FSpotsHandle& Handle : Handles)
		{
			FSpotsData Data;
			if (Grid->GetDataFromHandle(Handle, Data))
			{
				Results.Add(MoveTemp(Data));
			}
		}
	}
	return Results;
}

TArray<FSpotsData> USpotsSubsystem::QuerySphereWithTags(const FVector Center, const float Radius, const FGameplayTagContainer RequiredTags) const
{
	TArray<FSpotsData> Results;
	for (const auto& [Id, Grid] : Grids)
	{
		TArray<FSpotsHandle> Handles = Grid->QuerySphere(Center, Radius);
		for (const FSpotsHandle& Handle : Handles)
		{
			FSpotsData Data;
			if (Grid->GetDataFromHandle(Handle, Data))
			{
				if (Data.Tags.HasAll(RequiredTags))
				{
					Results.Add(MoveTemp(Data));
				}
			}
		}
	}
	return Results;
}

TArray<FSpotsData> USpotsSubsystem::QueryBox(const FVector Center, const FVector Extent) const
{
	TArray<FSpotsData> Results;
	for (const auto& [Id, Grid] : Grids)
	{
		TArray<FSpotsHandle> Handles = Grid->QueryBox(Center, Extent);
		for (const FSpotsHandle& Handle : Handles)
		{
			FSpotsData Data;
			if (Grid->GetDataFromHandle(Handle, Data))
			{
				Results.Add(MoveTemp(Data));
			}
		}
	}
	return Results;
}

TArray<FSpotsData> USpotsSubsystem::QueryBoxWithTags(const FVector Center, const FVector Extent, const FGameplayTagContainer RequiredTags) const
{
	TArray<FSpotsData> Results;
	for (const auto& [Id, Grid] : Grids)
	{
		TArray<FSpotsHandle> Handles = Grid->QueryBox(Center, Extent);
		for (const FSpotsHandle& Handle : Handles)
		{
			FSpotsData Data;
			if (Grid->GetDataFromHandle(Handle, Data))
			{
				if (Data.Tags.HasAll(RequiredTags))
				{
					Results.Add(MoveTemp(Data));
				}
			}
		}
	}
	return Results;
}

USpotsGrid* USpotsSubsystem::GetGrid(const FName VolumeId) const
{
	if (const TObjectPtr<USpotsGrid>* Found = Grids.Find(VolumeId))
	{
		return *Found;
	}
	return nullptr;
}

void USpotsSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	LoadSpotsData();
}

void USpotsSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	Grids.Empty();

	Super::OnWorldEndPlay(InWorld);
}

void USpotsSubsystem::LoadSpotsData()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FString MapName = World->GetMapName();
	const FString CleanMapName = UWorld::RemovePIEPrefix(FPackageName::GetShortName(MapName));
	const FString BasePath = FString::Printf(TEXT("/Game/Spots/%s/"), *CleanMapName);
	const FString Prefix = FString::Printf(TEXT("DT_Spots_%s_"), *CleanMapName);

	TArray<FAssetData> AssetList;
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistryModule.Get().GetAssetsByPath(FName(*BasePath), AssetList, true);

	UE_LOG(LogSpots, Log, TEXT("Trying to load spots for map %s"), *CleanMapName);
	for (const FAssetData& Asset : AssetList)
	{
		if (Asset.AssetClassPath != UDataTable::StaticClass()->GetClassPathName())
		{
			continue;
		}

		const FString AssetName = Asset.AssetName.ToString();
		if (!AssetName.StartsWith(Prefix))
		{
			continue;
		}

		const FName VolumeId = FName(*AssetName.RightChop(Prefix.Len()));
		const UDataTable* DataTable = Cast<UDataTable>(Asset.GetAsset());
		if (!DataTable)
		{
			continue;
		}

		USpotsGrid* Grid = NewObject<USpotsGrid>(this);
		Grid->Initialize(150.0f);

		TArray<FSpotsData*> Rows;
		DataTable->GetAllRows(TEXT("SpotsSubsystem"), Rows);
		for (const FSpotsData* Spot : Rows)
		{
			if (Spot)
			{
				Grid->Insert(*Spot);
			}
		}
		Grids.Add(VolumeId, Grid);
		UE_LOG(LogSpots, Log, TEXT("Loaded %d spots for volume '%s'"), Rows.Num(), *VolumeId.ToString());
	}
}

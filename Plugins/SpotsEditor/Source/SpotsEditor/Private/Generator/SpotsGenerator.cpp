// Copyright Drischdaan All Rights Reserved Copyright

#include "Generator/SpotsGenerator.h"

#include "Actors/SpotsVolume.h"
#include "Data/SpotsData.h"
#include "Data/SpotsGrid.h"
#include "Evaluator/SpotsEvaluator.h"

#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Spots.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Async/ParallelFor.h"
#include "Engine/DataTable.h"
#include "Misc/PackageName.h"
#include "Misc/ScopedSlowTask.h"
#include "UObject/SavePackage.h"

#include "NavMesh/RecastNavMesh.h"

TMap<FName, TArray<FSpotsData>> FSpotsGenerator::GenerateSpots(UWorld* World)
{
	TArray<ASpotsVolume*> Volumes;
	for (TActorIterator<ASpotsVolume> It(World); It; ++It)
	{
		Volumes.Add(*It);
	}

	FScopedSlowTask RootTask(Volumes.Num(), NSLOCTEXT("Spots", "Generating", "Generating Spots..."));
	RootTask.MakeDialog(true);

	TMap<FName, TArray<FSpotsData>> Results;
	for (ASpotsVolume* Volume : Volumes)
	{
		if (RootTask.ShouldCancel())
		{
			break;
		}
		RootTask.EnterProgressFrame(1, FText::Format(
			                            NSLOCTEXT("Spots", "GenVolume", "Processing {0}"),
			                            FText::FromString(Volume->GetName())));
		Results.Add(Volume->GetFName(), GenerateSpotsForVolume(World, Volume));
	}
	return Results;
}

void FSpotsGenerator::SaveToDataTables(const UWorld* World, const TMap<FName, TArray<FSpotsData>>& VolumeData)
{
	if (!World || VolumeData.IsEmpty())
	{
		return;
	}

	const FString MapName = FPackageName::GetShortName(World->GetMapName());

	FScopedSlowTask SaveTask(VolumeData.Num(), NSLOCTEXT("Spots", "SavingData", "Saving spots data tables..."));
	SaveTask.MakeDialog(true);

	for (const auto& [VolumeId, Spots] : VolumeData)
	{
		SaveTask.EnterProgressFrame(1, FText::Format(
			                            NSLOCTEXT("Spots", "SaveVolume", "Saving {0}..."),
			                            FText::FromName(VolumeId)));

		const FString AssetName = FString::Printf(TEXT("DT_Spots_%s_%s"), *MapName, *VolumeId.ToString());
		const FString PackagePath = FString::Printf(TEXT("/Game/Spots/%s/%s"), *MapName, *AssetName);

		UPackage* Package = CreatePackage(*PackagePath);
		Package->FullyLoad();

		UDataTable* DataTable = FindObject<UDataTable>(Package, *AssetName);
		if (DataTable)
		{
			DataTable->EmptyTable();
		}
		else
		{
			DataTable = NewObject<UDataTable>(Package, *AssetName, RF_Public | RF_Standalone);
			DataTable->RowStruct = FSpotsData::StaticStruct();
		}

		for (int32 Index = 0; Index < Spots.Num(); ++Index)
		{
			const FName RowName = FName(*FString::Printf(TEXT("Spot_%d"), Index));
			const FSpotsData* NewRow = new FSpotsData(Spots[Index]);
			DataTable->AddRow(RowName, *NewRow);
			delete NewRow;
		}

		DataTable->MarkPackageDirty();
		FAssetRegistryModule::AssetCreated(DataTable);

		const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		UPackage::SavePackage(Package, DataTable, *PackageFileName, SaveArgs);

		UE_LOG(LogSpots, Log, TEXT("Saved %d spots to %s"), Spots.Num(), *PackagePath);
	}
}

TArray<FSpotsData> FSpotsGenerator::GenerateSpotsForVolume(UWorld* World, ASpotsVolume* Volume)
{
	TArray<FSpotsData> Results;
	if (!Volume)
	{
		return Results;
	}

	const FBox VolumeBounds = Volume->GetVolumeBounds();
	const float GridSpacing = Volume->GetMinSpacing();
	const FGameplayTagContainer& DefaultTags = Volume->GetDefaultTags();

	FScopedSlowTask VolumeTask(5, NSLOCTEXT("Spots", "VolumeSteps", "Processing volume..."));

	VolumeTask.EnterProgressFrame(1, NSLOCTEXT("Spots", "ExtractNav", "Extracting navmesh triangles..."));

	TArray<FNavTriangle> Triangles = ExtractNavMeshTriangles(World, VolumeBounds);
	if (Triangles.IsEmpty())
	{
		UE_LOG(LogSpots, Warning, TEXT("No NavMesh triangles found in volume %s"), *Volume->GetName());
		return Results;
	}

	VolumeTask.EnterProgressFrame(1, FText::Format(
		                              NSLOCTEXT("Spots", "BuildTriGrid", "Building triangle grid from {0} triangles..."),
		                              FText::AsNumber(Triangles.Num())));

	const FVector2D GridOrigin(VolumeBounds.Min.X, VolumeBounds.Min.Y);
	TMap<FIntPoint, TArray<int32>> TriangleGrid;

	for (int32 TriIdx = 0; TriIdx < Triangles.Num(); ++TriIdx)
	{
		const FNavTriangle& Tri = Triangles[TriIdx];

		const float MinX = FMath::Min3(Tri.Vertices[0].X, Tri.Vertices[1].X, Tri.Vertices[2].X);
		const float MaxX = FMath::Max3(Tri.Vertices[0].X, Tri.Vertices[1].X, Tri.Vertices[2].X);
		const float MinY = FMath::Min3(Tri.Vertices[0].Y, Tri.Vertices[1].Y, Tri.Vertices[2].Y);
		const float MaxY = FMath::Max3(Tri.Vertices[0].Y, Tri.Vertices[1].Y, Tri.Vertices[2].Y);

		const int32 CellMinX = FMath::FloorToInt32((MinX - GridOrigin.X) / GridSpacing);
		const int32 CellMaxX = FMath::FloorToInt32((MaxX - GridOrigin.X) / GridSpacing);
		const int32 CellMinY = FMath::FloorToInt32((MinY - GridOrigin.Y) / GridSpacing);
		const int32 CellMaxY = FMath::FloorToInt32((MaxY - GridOrigin.Y) / GridSpacing);

		for (int32 CX = CellMinX; CX <= CellMaxX; ++CX)
		{
			for (int32 CY = CellMinY; CY <= CellMaxY; ++CY)
			{
				TriangleGrid.FindOrAdd(FIntPoint(CX, CY)).Add(TriIdx);
			}
		}
	}

	VolumeTask.EnterProgressFrame(1, NSLOCTEXT("Spots", "SampleGrid", "Sampling grid onto navmesh..."));

	const int32 CountX = FMath::FloorToInt32((VolumeBounds.Max.X - VolumeBounds.Min.X) / GridSpacing);
	const int32 CountY = FMath::FloorToInt32((VolumeBounds.Max.Y - VolumeBounds.Min.Y) / GridSpacing);
	const float MinVerticalSeparation = GridSpacing * 0.5f;
	const int32 NumRows = CountX + 1;

	TArray<TArray<FSpotsData>> PerRowResults;
	PerRowResults.SetNum(NumRows);

	ParallelFor(NumRows, [&](const int32 IX)
	{
		TArray<FSpotsData>& LocalResults = PerRowResults[IX];

		for (int32 IY = 0; IY <= CountY; ++IY)
		{
			const FVector2D SamplePoint(
				VolumeBounds.Min.X + IX * GridSpacing + GridSpacing * 0.5f,
				VolumeBounds.Min.Y + IY * GridSpacing + GridSpacing * 0.5f);

			const FIntPoint Cell(IX, IY);
			const TArray<int32>* CellTriangles = TriangleGrid.Find(Cell);
			if (!CellTriangles)
			{
				continue;
			}

			TArray<TPair<float, FVector>, TInlineAllocator<4>> Candidates;

			for (const int32 TriIdx : *CellTriangles)
			{
				const FNavTriangle& Tri = Triangles[TriIdx];

				const FVector2D A(Tri.Vertices[0].X, Tri.Vertices[0].Y);
				const FVector2D B(Tri.Vertices[1].X, Tri.Vertices[1].Y);
				const FVector2D C(Tri.Vertices[2].X, Tri.Vertices[2].Y);

				if (!PointInTriangle2D(SamplePoint, A, B, C))
				{
					continue;
				}

				const float Z = InterpolateZ(Tri.Vertices[0], Tri.Vertices[1], Tri.Vertices[2], SamplePoint);

				bool bDuplicate = false;
				for (const auto& Existing : Candidates)
				{
					if (FMath::Abs(Z - Existing.Key) < MinVerticalSeparation)
					{
						bDuplicate = true;
						break;
					}
				}

				if (!bDuplicate)
				{
					Candidates.Emplace(Z, Tri.Normal);
				}
			}

			for (const auto& Candidate : Candidates)
			{
				FSpotsData Spot;
				Spot.Position = FVector3f(SamplePoint.X, SamplePoint.Y, Candidate.Key);
				Spot.Tags = DefaultTags;
				LocalResults.Add(MoveTemp(Spot));
			}
		}
	});

	VolumeTask.EnterProgressFrame(1, NSLOCTEXT("Spots", "MergeResults", "Merging results..."));

	int32 TotalSpots = 0;
	for (const TArray<FSpotsData>& RowResult : PerRowResults)
	{
		TotalSpots += RowResult.Num();
	}

	TArray<FSpotsData> AllSpots;
	AllSpots.Reserve(TotalSpots);
	for (TArray<FSpotsData>& RowResult : PerRowResults)
	{
		AllSpots.Append(MoveTemp(RowResult));
	}

	VolumeTask.EnterProgressFrame(1, NSLOCTEXT("Spots", "RunEval", "Running evaluators..."));

	const auto& Evaluators = Volume->GetEvaluators();
	if (!Evaluators.IsEmpty())
	{
		USpotsGrid* TempGrid = NewObject<USpotsGrid>();
		TempGrid->Initialize(Volume->GetGridCellSize());
		for (const FSpotsData& Spot : AllSpots)
		{
			TempGrid->Insert(Spot);
		}

		FScopedSlowTask EvalTask(Evaluators.Num() * 2, NSLOCTEXT("Spots", "Evaluators", "Running evaluators..."));

		for (const TSoftObjectPtr<USpotsEvaluator>& EvalPtr : Evaluators)
		{
			USpotsEvaluator* Evaluator = EvalPtr.LoadSynchronous();
			EvalTask.EnterProgressFrame(1, FText::Format(
				                            NSLOCTEXT("Spots", "EvalSpot", "Evaluating: {0}"),
				                            FText::FromString(Evaluator ? Evaluator->GetName() : TEXT("null"))));

			if (!Evaluator)
			{
				continue;
			}

			ParallelFor(AllSpots.Num(), [&](const int32 Index)
			{
				Evaluator->EvaluateSpot(World, AllSpots[Index], TempGrid);
			});
		}

		for (const TSoftObjectPtr<USpotsEvaluator>& EvalPtr : Evaluators)
		{
			USpotsEvaluator* Evaluator = EvalPtr.Get();
			EvalTask.EnterProgressFrame(1, FText::Format(
				                            NSLOCTEXT("Spots", "PostProcess", "Post-processing: {0}"),
				                            FText::FromString(Evaluator ? Evaluator->GetName() : TEXT("null"))));

			if (!Evaluator)
			{
				continue;
			}

			Evaluator->PostProcessSpots(World, AllSpots, TempGrid);
		}
	}

	UE_LOG(LogSpots, Log, TEXT("Volume %s: generated %d spots (grid %dx%d, spacing %.0f)"),
	       *Volume->GetName(), AllSpots.Num(), CountX + 1, CountY + 1, GridSpacing);

	return AllSpots;
}

TArray<FNavTriangle> FSpotsGenerator::ExtractNavMeshTriangles(UWorld* World, const FBox& VolumeBounds)
{
	TArray<FNavTriangle> Triangles;

	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		return Triangles;
	}

	ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
	ARecastNavMesh* RecastNavMesh = Cast<ARecastNavMesh>(NavData);
	if (!RecastNavMesh)
	{
		return Triangles;
	}

	const int32 TileCount = RecastNavMesh->GetNavMeshTilesCount();
	RecastNavMesh->BeginBatchQuery();

	FScopedSlowTask TileTask(TileCount, NSLOCTEXT("Spots", "ExtractTiles", "Extracting NavMesh tiles..."));

	for (int32 TileIndex = 0; TileIndex < TileCount; ++TileIndex)
	{
		TileTask.EnterProgressFrame(1.0f, FText::Format(
			                            NSLOCTEXT("Spots", "ExtractTile", "Extracting tile {0} / {1}"),
			                            FText::AsNumber(TileIndex + 1), FText::AsNumber(TileCount)));

		FRecastDebugGeometry DebugGeometry;
		DebugGeometry.bGatherPolyEdges = false;
		DebugGeometry.bGatherNavMeshEdges = false;
		RecastNavMesh->GetDebugGeometryForTile(DebugGeometry, TileIndex);

		const TArray<FVector>& Vertices = DebugGeometry.MeshVerts;

		for (int32 AreaIndex = 0; AreaIndex < RECAST_MAX_AREAS; ++AreaIndex)
		{
			const TArray<int32>& Indices = DebugGeometry.AreaIndices[AreaIndex];

			for (int32 Index = 0; Index + 2 < Indices.Num(); Index += 3)
			{
				const FVector& V0 = Vertices[Indices[Index]];
				const FVector& V1 = Vertices[Indices[Index + 1]];
				const FVector& V2 = Vertices[Indices[Index + 2]];

				FBox TriBounds(ForceInit);
				TriBounds += V0;
				TriBounds += V1;
				TriBounds += V2;
				if (!VolumeBounds.Intersect(TriBounds))
				{
					continue;
				}

				const FVector Edge1 = V1 - V0;
				const FVector Edge2 = V2 - V0;
				const FVector CrossProduct = FVector::CrossProduct(Edge1, Edge2);
				const float Area = 0.5f * CrossProduct.Size();

				if (Area > KINDA_SMALL_NUMBER)
				{
					FNavTriangle Tri;
					Tri.Vertices[0] = V0;
					Tri.Vertices[1] = V1;
					Tri.Vertices[2] = V2;
					Tri.Normal = CrossProduct.GetSafeNormal();
					Triangles.Add(Tri);
				}
			}
		}
	}

	RecastNavMesh->FinishBatchQuery();
	return Triangles;
}

bool FSpotsGenerator::PointInTriangle2D(const FVector2D& P, const FVector2D& A, const FVector2D& B, const FVector2D& C)
{
	const float D1 = (P.X - B.X) * (A.Y - B.Y) - (A.X - B.X) * (P.Y - B.Y);
	const float D2 = (P.X - C.X) * (B.Y - C.Y) - (B.X - C.X) * (P.Y - C.Y);
	const float D3 = (P.X - A.X) * (C.Y - A.Y) - (C.X - A.X) * (P.Y - A.Y);

	const bool bHasNeg = (D1 < 0) || (D2 < 0) || (D3 < 0);
	const bool bHasPos = (D1 > 0) || (D2 > 0) || (D3 > 0);

	return !(bHasNeg && bHasPos);
}

float FSpotsGenerator::InterpolateZ(const FVector& V0, const FVector& V1, const FVector& V2, const FVector2D& P)
{
	const FVector2D A(V0.X, V0.Y);
	const FVector2D B(V1.X, V1.Y);
	const FVector2D C(V2.X, V2.Y);

	const float AreaABC = FMath::Abs((B.X - A.X) * (C.Y - A.Y) - (C.X - A.X) * (B.Y - A.Y));
	if (AreaABC < KINDA_SMALL_NUMBER)
	{
		return V0.Z;
	}

	const float InvArea = 1.0f / AreaABC;
	const float U = FMath::Abs((B.X - P.X) * (C.Y - P.Y) - (C.X - P.X) * (B.Y - P.Y)) * InvArea;
	const float V = FMath::Abs((C.X - P.X) * (A.Y - P.Y) - (A.X - P.X) * (C.Y - P.Y)) * InvArea;
	const float W = 1.0f - U - V;

	return V0.Z * U + V1.Z * V + V2.Z * W;
}

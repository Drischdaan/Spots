// Copyright Drischdaan All Rights Reserved Copyright

#pragma once

#include "CoreMinimal.h"

class ASpotsVolume;
struct FSpotsData;

struct FNavTriangle
{
	FVector Vertices[3];
	FVector Normal;
};

class SPOTSEDITOR_API FSpotsGenerator
{
public:
	static TMap<FName, TArray<FSpotsData>> GenerateSpots(UWorld* World);
	static void SaveToDataTables(const UWorld* World, const TMap<FName, TArray<FSpotsData>>& VolumeData);

	static TArray<FSpotsData> GenerateSpotsForVolume(UWorld* World, ASpotsVolume* Volume);

protected:
	static TArray<FNavTriangle> ExtractNavMeshTriangles(UWorld* World, const FBox& VolumeBounds);
	static bool PointInTriangle2D(const FVector2D& P, const FVector2D& A, const FVector2D& B, const FVector2D& C);
	static float InterpolateZ(const FVector& V0, const FVector& V1, const FVector& V2, const FVector2D& P);
};

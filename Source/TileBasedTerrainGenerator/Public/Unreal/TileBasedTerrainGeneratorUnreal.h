

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generator/TBGenerator.h"
#include "Unreal/Data/TileAsset.h"
#include "Unreal/Data/TileConstraintsAsset.h"
#include "Unreal/Data/SelectorEnums.h"

#include "TileBasedTerrainGeneratorUnreal.generated.h"

UCLASS()
class TILEBASEDTERRAINGENERATOR_API ATileBasedTerrainGenerator : public AActor
{
	GENERATED_BODY()
public:

	ATileBasedTerrainGenerator();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator")
	ETileMapType tileMapType = ETileMapType::Square;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator", meta = (EditCondition = "tileMapType==ETileMapType::Square"))
	bool squareMap8Connectivity = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator")
	int tileMapWidth = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator")
	int tileMapHeight = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator")
	TArray<UTileAsset*> tileSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator")
	UTileConstraintsAsset* tileConstraints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator")
	FVector tileSize = FVector(100.f, 100.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator\|Seeding")
	int numSeeds = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator\|Seeding")
	bool forceDistinctSeedTiles = true;

	// How we choose the tile from all possibilities in the selected cell
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator\|Advanced")
	ETileStrategy tileStrategy = ETileStrategy::SimilarToNeighborTile;

	// How we choose which cell to collapse next
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileBasedTerrainGenerator\|Advanced")
	ECellStrategy cellStrategy = ECellStrategy::LowestEntropy;

	UFUNCTION(BlueprintCallable, Category = "TileBasedTerrainGenerator")
	void GenerateTiles();

public: // Internal
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	TArray<AActor*> spawnedTileActors;


private:

	void SpawnTileActors(std::shared_ptr<TileMapBase> tileMap);
};

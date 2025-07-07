

#include "Unreal/TileBasedTerrainGeneratorUnreal.h"

ATileBasedTerrainGenerator::ATileBasedTerrainGenerator()
{
}

void ATileBasedTerrainGenerator::GenerateTiles()
{
	TBGenerator generator;
	std::shared_ptr<TileMapBase> tileMap = CreateTileMap(tileMapType, tileMapWidth, tileMapHeight);
	std::vector<TileBase*> tileSetVector(tileSet.GetData(), tileSet.GetData() + tileSet.Num());
	std::vector<TileConstraintBase*> tileConstraintsVector = tileConstraints->CreateTileConstraints(tileSetVector);

	if (tileMapType == ETileMapType::Square)
	{
		auto squareTileMap = std::static_pointer_cast<TileMapSquare>(tileMap);
		if (squareTileMap)
		{
			squareTileMap->Set8Connectivity(squareMap8Connectivity);
		}
	}

	// Set tilemap, tile set, and strategies
	generator.SetTileMap(tileMap);
	generator.SetTileSet(tileSetVector);
	generator.SetConstraints(tileConstraintsVector);
	generator.SetTileStrategy(GetTileStrategy(tileStrategy));
	generator.SetCellStrategy(GetCellStrategy(cellStrategy));

	// Set seeding parameters
	generator.SetNumSeeds(numSeeds);
	generator.SetForceDistinctSeedTiles(forceDistinctSeedTiles);
	
	generator.GenerateTiles();

	SpawnTileActors(tileMap);

	// Free memory
	for (auto* constraint : tileConstraintsVector)
	{
		delete constraint;
	}
}

void ATileBasedTerrainGenerator::SpawnTileActors(std::shared_ptr<TileMapBase> tileMap)
{
	FVector origin = GetActorLocation();
	std::vector<double> stdOrigin = {origin.X, origin.Y};
	std::vector<double> stdTileSize = { tileSize.X, tileSize.Y };
	CellIdx cell;

	spawnedTileActors.Reserve(tileMap->GetWidth() * tileMap->GetHeight());

	for (int i = 0 ; i < tileMap->GetHeight() ; i++)
		for (int j = 0; j < tileMap->GetWidth() ; j++)
		{
			cell.y = i;
			cell.x = j;

			UTileAsset* tile = (UTileAsset*) tileMap->GetTileAt(cell);
			std::vector<double> stdPosition = tileMap->GetCellTransformPosition(cell, stdTileSize, stdOrigin);
			FVector position = FVector(stdPosition[0], stdPosition[1], 0.0f);

			FActorSpawnParameters params;
			params.Name = MakeUniqueObjectName(GetWorld(), AActor::StaticClass(), FName(FString::Printf(TEXT("Tile%s_(%d,%d)"), *tile->tileName.ToString(), j, i)));
			auto actor = GetWorld()->SpawnActor<AActor>(tile->tileActor, position, FRotator::ZeroRotator, params);
			actor->Rename(*params.Name.ToString());
			spawnedTileActors.Emplace(actor);
		}
}




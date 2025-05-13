


#include "Unreal/TileBasedTerrainGeneratorUnreal.h"

ATileBasedTerrainGenerator::ATileBasedTerrainGenerator()
{
}

void ATileBasedTerrainGenerator::GenerateTiles()
{
	TBGenerator generator;
	std::vector<TileBase*> tileSetVector(tileSet.GetData(), tileSet.GetData() + tileSet.Num());
	std::vector<TileConstraintBase*> tileConstraintsVector = tileConstraints->CreateTileConstraints(tileSetVector);

	// Set tilemap, tile set, and strategies
	generator.SetTileMap(CreateTileMap(tileMapType, tileMapWidth, tileMapHeight));
	generator.SetTileSet(tileSetVector);
	generator.SetConstraints(tileConstraintsVector);
	generator.SetTileStrategy(GetTileStrategy(tileStrategy));
	generator.SetCellStrategy(GetCellStrategy(cellStrategy));

	// Set seeding parameters
	generator.SetNumSeeds(numSeeds);
	generator.SetForceDistinctSeedTiles(forceDistinctSeedTiles);
	
	//generator.GenerateTiles();

	// Free memory
	for (auto* constraint : tileConstraintsVector)
	{
		delete constraint;
	}
}




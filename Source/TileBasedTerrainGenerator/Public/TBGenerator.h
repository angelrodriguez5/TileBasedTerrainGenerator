#pragma once
#include <functional>
#include "Interfaces/TileBase.h"
#include "Interfaces/TileMapBase.h"
#include "Interfaces/TileConstraintBase.h"


class TBGenerator 
{
public:
	TBGenerator();

	void GenerateTiles();
	void SetTileMap(std::shared_ptr<TileMapBase> tileMap);

	void SetTileStrategy(const std::function<std::shared_ptr<TileBase>(TileMapBase&, const CellIdx&)> function);
	void SetCellStrategy(const std::function<CellIdx (TileMapBase&)> function);
	void SetConstraints(const std::vector<std::shared_ptr<TileConstraintBase>>& constraints);
	void SetNumSeeds(int numSeeds);
	void SetForceDistinctSeedTiles(bool value);

private:
	std::shared_ptr<TileMapBase> m_map;
	std::vector<std::shared_ptr<TileConstraintBase>> m_constraints;

	// Strategies
	std::function<std::shared_ptr<TileBase> (TileMapBase&, const CellIdx&)> m_tileStrategy;
	std::function<CellIdx (TileMapBase&)> m_cellStrategy;

	// Terrain seeding
	int m_numSeeds = 1;
	bool m_forceDistinctSeedtiles = false;
	
	std::vector<std::shared_ptr<TileBase>> GetUpdatedSuperposition(const CellIdx& cell);

	void CollapseAndUpdate(const CellIdx& chosenCell, const std::shared_ptr<TileBase> chosenTile);

	void PlantTerrainSeeds();
};
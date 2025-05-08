#pragma once
#include <functional>
#include "Generator/Interfaces/TileBase.h"
#include "Generator/Interfaces/TileMapBase.h"
#include "Generator/Interfaces/TileConstraintBase.h"


class TBGenerator 
{
public:
	TBGenerator();

	void GenerateTiles();
	void SetTileMap(std::shared_ptr<TileMapBase> tileMap);
	void SetTileSet(std::vector<TileBase*> tileSet);
	void SetTileStrategy(const std::function<TileBase* (TileMapBase&, const CellIdx&)> function);
	void SetCellStrategy(const std::function<CellIdx (TileMapBase&)> function);
	void SetConstraints(const std::vector<TileConstraintBase*>& constraints);
	void SetNumSeeds(int numSeeds);
	void SetForceDistinctSeedTiles(bool value);

private:
	std::vector<TileBase*> m_tileSet;
	std::shared_ptr<TileMapBase> m_map;
	std::vector<TileConstraintBase*> m_constraints;

	// Strategies
	std::function<TileBase* (TileMapBase&, const CellIdx&)> m_tileStrategy;
	std::function<CellIdx (TileMapBase&)> m_cellStrategy;

	// Terrain seeding
	int m_numSeeds = 1;
	bool m_forceDistinctSeedtiles = false;
	
	std::vector<TileBase*> GetUpdatedSuperposition(const CellIdx& cell);

	void CollapseAndUpdate(const CellIdx& chosenCell, const TileBase* chosenTile);

	void PlantTerrainSeeds();
};
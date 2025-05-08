#include "Generator/TBGenerator.h"
#include "Generator/Implementations/TileMapSquare.h"
#include "Generator/VectorUtils.h"
#include <iostream>
#include <queue>
#include <set>
#include <format>
#include <algorithm>
#include <random>

TBGenerator::TBGenerator()
{}

void TBGenerator::GenerateTiles()
{
	int resets = 0;
	bool resetFlag = false;
	while (resets < 100 && !m_map->CheckComplete())
	{
		resets++;
		if (resetFlag)
		{
			resetFlag = false;

			m_map->Reset();
		}

		// Step 1: seed tiles
		std::cout << "Planting seed tiles...\n";
		PlantTerrainSeeds();

		// Step 2: generate first tile map
		std::cout << "Generating tiles...\n";
		while (!m_map->CheckComplete())
		{
			CellIdx chosenCell = m_cellStrategy(*m_map);
			auto chosenTile = m_tileStrategy(*m_map, chosenCell);

			// If no tiles are valid for this cell, reset the process
			if (!chosenTile)
			{
				resetFlag = true;
				break;
			}

			CollapseAndUpdate(chosenCell, chosenTile);
		}
		if (resetFlag)
			continue;

		// Step 3: refine tile map

		// Step 4: validate 

		// Step 5: deliver
	}
}

void TBGenerator::PlantTerrainSeeds()
{
	std::random_device random_device;
	std::mt19937 engine{ random_device() };

	// Select seed tiles
	auto tiles = m_tileSet;
	std::vector<TileBase*> seeds;
	for (int i = 0; i < m_numSeeds; i++)
	{
		size_t idx = std::uniform_int_distribution<size_t>{ 0, tiles.size() - 1 }(engine);
		seeds.push_back(tiles[idx]);

		if (m_forceDistinctSeedtiles)
			RemoveElement(tiles, tiles[idx]);
	}

	for (size_t i = 0; i < m_numSeeds; i++)
	{
		// Random seed cells
		CellIdx cell;
		bool found = false;
		while (!found)
		{
			cell.x = std::uniform_int_distribution<int>{ 0, m_map->GetWidth() - 1 }(engine);
			cell.y = std::uniform_int_distribution<int>{ 0, m_map->GetHeight() - 1 }(engine);

			if (   !m_map->IsCellCollapsed(cell) 
				&& IsInVector(m_map->GetSuperpositionAt(cell), seeds[i]))
				found = true;
		}

		CollapseAndUpdate(cell, seeds[i]);
	}
}

void TBGenerator::CollapseAndUpdate(const CellIdx& chosenCell, const TileBase* chosenTile)
{
	m_map->CollapseCell(chosenCell, chosenTile);

	// Update neighboring cells recursively
	std::queue<CellIdx> cellsToUpdate;
	for (CellIdx neighbor : m_map->GetNeighbors(chosenCell))
		cellsToUpdate.push(neighbor);

	std::vector<CellIdx> visitedCells;
	int cellsUpdated = 0;
	while (!cellsToUpdate.empty())
	{
		CellIdx cell = cellsToUpdate.front();
		cellsToUpdate.pop();

		if (m_map->IsCellCollapsed(cell) || IsInVector(visitedCells, cell))
			continue;
		else
			visitedCells.push_back(cell);
		cellsUpdated++;

		// Calculate new superposition for this cell following the constraints,
		// only check its neighbors if the superposition of the cell actually changed
		auto newSuperposition = GetUpdatedSuperposition(cell);
		if (m_map->UpdateCellSuperposition(cell, newSuperposition))
		{
			// If a cell superposition has changed then update its neighbors
			for (CellIdx neighbor : m_map->GetNeighbors(cell))
				cellsToUpdate.push(neighbor);
		}
	}

}

std::vector<TileBase*> TBGenerator::GetUpdatedSuperposition(const CellIdx& cell)
{
	// Check every permutation of current possible tile against every possible neighbor tile
	TileConstraintArgs args;
	auto currentSuperposition = m_map->GetSuperpositionAt(cell);
	auto neighbors = m_map->GetNeighbors(cell);
	std::vector<TileBase*> newSuperposition;

	for (TileBase* cellTile : currentSuperposition)
	{
		args.tile = cellTile;
		bool tileAllowed = true;

		for (CellIdx neighbor : m_map->GetNeighbors(cell))
		{
			// A cellTile is not allowed if its forbidden by all tiles of a neighbor's superposition
			bool allowedAny = false;

			for (TileBase* neighborTile : m_map->GetSuperpositionAt(neighbor))
			{
				args.other = neighborTile;
				bool tileCombinationAllowed = true;

				// Check all constraints for this tile combination
				for (auto constraint : m_constraints)
				{
					if (!constraint->IsConstraintFollowed(args))
					{
						tileCombinationAllowed = false;
						break;
					}
				}

				if (tileCombinationAllowed)
				{
					allowedAny = true;
					break;
				}
			}

			// If a tile in the superposition is not allowed by any tile in one of the neighbors superposition, no need to check the rest of the neighbors, the tile is not allowed
			if (!allowedAny)
			{
				tileAllowed = false;
				break;
			}
		}

		if (tileAllowed)
			newSuperposition.push_back(cellTile);
	}

	if (newSuperposition.empty())
		std::cout << std::format("Cell ({},{}) has no valid tiles \n", cell.x, cell.y);

	return newSuperposition;
}

void TBGenerator::SetTileMap(std::shared_ptr<TileMapBase> tileMap)
{
	m_map = tileMap;

	if (!m_tileSet.empty())
		m_map->SetTileSet(m_tileSet);
}

void TBGenerator::SetTileSet(std::vector<TileBase*> tileSet)
{
	m_tileSet = tileSet;

	if (m_map)
		m_map->SetTileSet(m_tileSet);
}

void TBGenerator::SetTileStrategy(const std::function<TileBase* (TileMapBase&, const CellIdx&)> function)
{
	m_tileStrategy = function;
}

void TBGenerator::SetCellStrategy(const std::function<CellIdx(TileMapBase&)> function)
{
	m_cellStrategy = function;
}

void TBGenerator::SetConstraints(const std::vector<TileConstraintBase*>& constraints)
{
	m_constraints = constraints;
}

void TBGenerator::SetNumSeeds(int numSeeds)
{
	if (!m_map)
		throw std::exception("TBGenerator.SetNumSeeds must be called after TBGenerator.SetTileMap");

	int numTiles = m_tileSet.size();
	if (m_forceDistinctSeedtiles && numTiles < numSeeds)
	{
		std::cout << "TBGenerator: if ForceDistinctSeedTiles is set to true, then NumSeeds must be less or equal to the size of the tileset. Setting NumSeeds = TileSet.size";
		numSeeds = numTiles;
	}

	m_numSeeds = numSeeds;
}

void TBGenerator::SetForceDistinctSeedTiles(bool value)
{
	if (!m_map)
		throw std::exception("TBGenerator.SetForceDistinctSeedTiles must be called after TBGenerator.SetTileMap");

	m_forceDistinctSeedtiles = value;

	if (!m_forceDistinctSeedtiles)
		return;

	int numTiles = m_tileSet.size();
	if (m_forceDistinctSeedtiles && numTiles < m_numSeeds)
	{
		std::cout << "TBGenerator: trying to activate ForceDistinctSeedTiles when NumSeeds was greater than TileSet size. Setting NumSeeds = TileSet.size";
		m_numSeeds = numTiles;
	}

}

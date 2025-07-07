#include "Generator/Implementations/TileMapHex.h"
#include "Generator/VectorUtils.h"
#include <array>
#include <iostream>

TileMapHex::TileMapHex(const int width, const int height)
	: m_width(width)
	, m_height(height)
{}

std::vector<TileBase*> TileMapHex::GetSuperpositionAt(const CellIdx& cell)
{
	return m_possibleSuperpositions[GetSuperpositionIndexAt(cell)];
}

TileBase* TileMapHex::GetTileAt(const CellIdx& cell)
{
	if (IsCellCollapsed(cell))
		return m_possibleSuperpositions[GetSuperpositionIndexAt(cell)][0];
	else
		return nullptr;
}

std::vector<CellIdx> TileMapHex::GetNeighbors(const CellIdx& cell)
{
	std::vector<CellIdx> neighbors;

	std::vector<std::pair<int, int>> neighborOffsets;
	// Odd rows behave differently than even rows
	if (cell.y % 2 == 0)
	{
		neighborOffsets.reserve(6);
		neighborOffsets.push_back(std::make_pair(-1, -1));
		neighborOffsets.push_back(std::make_pair(-1,  0));
		neighborOffsets.push_back(std::make_pair( 0, -1));
		neighborOffsets.push_back(std::make_pair( 0,  0));
		neighborOffsets.push_back(std::make_pair( 1, -1));
		neighborOffsets.push_back(std::make_pair( 1,  0));
	}
	else
	{
		neighborOffsets.reserve(6);
		neighborOffsets.push_back(std::make_pair(-1,  0));
		neighborOffsets.push_back(std::make_pair(-1,  1));
		neighborOffsets.push_back(std::make_pair( 0, -1));
		neighborOffsets.push_back(std::make_pair( 0,  0));
		neighborOffsets.push_back(std::make_pair( 1,  0));
		neighborOffsets.push_back(std::make_pair( 1,  1));
	}

	for (auto offset : neighborOffsets)
	{
		CellIdx neighbor{ cell.x + offset.first, cell.y + offset.second };
		// Check index in bounds
		if (neighbor.x >= 0 && neighbor.x < m_width && neighbor.y >= 0 && neighbor.y < m_height)
			neighbors.push_back(neighbor);
	}

	return neighbors;
}

bool TileMapHex::CheckComplete()
{
	// All superpositions that point to a single tile are stored at the beginning of m_possibleSuperpositions
	int chosenCellMaxIdx = m_tileSet.size() - 1;
	// Check that all cell superpositions point to a single tile
	for (size_t i = 0; i < m_mapCells.size(); i++)
	{
		if (m_mapCells[i] > chosenCellMaxIdx)
			return false;
	}
	return true;
}

int TileMapHex::GetSuperpositionIndexAt(const CellIdx& cellIdx)
{
	return m_mapCells[m_width * cellIdx.y + cellIdx.x];
}

void TileMapHex::SetSuperpositionIndexAt(const CellIdx& cellIdx, const int superpositionIdx)
{
	m_mapCells[m_width * cellIdx.y + cellIdx.x] = superpositionIdx;
}

std::vector<TileBase*> TileMapHex::GetTileSet()
{
	return m_tileSet;
}

void TileMapHex::SetTileSet(std::vector<TileBase*> tileSet)
{
	m_tileSet = tileSet;

	// Preload every garanteed superposition
	m_possibleSuperpositions.reserve(m_tileSet.size() + 1);
	for (auto tile : m_tileSet)
	{
		// a single choice of each tile
		m_possibleSuperpositions.push_back({ tile });
	}
	// a choice of all tiles
	m_possibleSuperpositions.push_back(m_tileSet);

	// Initialize map so that every cell can have any tile
	m_mapCells = std::vector(m_width * m_height, (int)m_possibleSuperpositions.size() - 1);
}

void TileMapHex::Reset()
{
	for (size_t i = 0; i < m_mapCells.size(); i++)
	{
		m_mapCells[i] = m_tileSet.size();
	}
}

int TileMapHex::GetWidth()
{
	return m_width;
}

int TileMapHex::GetHeight()
{
	return m_height;
}

std::vector<double> TileMapHex::GetCellTransformPosition(const CellIdx& cell, const std::vector<double>& tileSize, const std::vector<double>& origin)
{
	if (tileSize.size() != 2 || origin.size() != 2)
		throw std::invalid_argument("Tile size and origin must be a vectors of 2 doubles");

	// Odd rows are offset by half a tile size in the x direction
	if (true)
	{
		return std::vector<double>{
			origin[0] + cell.x * tileSize[0],
			origin[1] + cell.y * 3/4 * tileSize[1]
		};
	}
	else 
	{
		return std::vector<double>{
			origin[0] + tileSize[0] / 2 + cell.x * tileSize[0],
			origin[1] + cell.y * 3/4 * tileSize[1]
		};
	}
}

bool TileMapHex::IsCellCollapsed(const CellIdx& cell)
{
	return GetSuperpositionIndexAt(cell) < m_tileSet.size();
}

bool TileMapHex::CollapseCell(const CellIdx& cell, const TileBase* tile)
{
	if (IsCellCollapsed(cell))
		return false;

	auto superposition = GetSuperpositionAt(cell);

	// Check that the selected tile is in the superposition of this cell
	bool found = false;
	for (size_t i = 0; i < superposition.size(); i++)
	{
		auto item = superposition[i];
		if (*item == *tile)
		{
			found = true;
			break;
		}
	}

	if (!found)
		throw std::invalid_argument("Selected tile was not allowed for this cell");

	// Collapse the cell by assigning a superposition with just the chosen tile
	int idx = -1;
	for (size_t i = 0; i < m_tileSet.size(); i++)
	{
		if (*(m_possibleSuperpositions[i].at(0)) == *tile)
		{
			idx = (int)i;
			break;
		}
	}

	if (idx == -1)
		throw std::exception("No superposition was found that represented the selected tile?!");

	m_mapCells[m_width * cell.y + cell.x] = idx;

	return true;
}

bool TileMapHex::UpdateCellSuperposition(const CellIdx& cell, const std::vector<TileBase*>& newSuperposition)
{
	int currentIdx = GetSuperpositionIndexAt(cell);
	size_t newIdx = m_possibleSuperpositions.size();  // Invalid index

	for (size_t i = 0; i < m_possibleSuperpositions.size(); i++)
	{
		if (AreSameVector(m_possibleSuperpositions[i], newSuperposition))
		{
			newIdx = i;
			break;
		}
	}

	if (newIdx == currentIdx)
		return false;

	//If the new superposition was not found, add it to the list
	if (newIdx == m_possibleSuperpositions.size())
	{
		m_possibleSuperpositions.push_back(newSuperposition);
		newIdx = m_possibleSuperpositions.size() - 1;
	}

	SetSuperpositionIndexAt(cell, newIdx);

	return true;
}

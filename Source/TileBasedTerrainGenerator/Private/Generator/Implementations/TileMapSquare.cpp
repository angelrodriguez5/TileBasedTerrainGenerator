#include "Generator/Implementations/TileMapSquare.h"
#include "Generator/VectorUtils.h"
#include <array>
#include <iostream>

TileMapSquare::TileMapSquare(const int width, const int height)
	: m_width(width)
	, m_height(height)
{}

std::vector<TileBase*> TileMapSquare::GetSuperpositionAt(const CellIdx& cell)
{
	return std::vector<TileBase*>(m_possibleSuperpositions[GetSuperpositionIndexAt(cell)]);
}

std::vector<CellIdx> TileMapSquare::GetNeighbors(const CellIdx& cell)
{
    std::vector<CellIdx> neighbors;

	std::vector<std::pair<int, int>> neighborOffsets;
	if (m_is8Connectivity)
	{
		neighborOffsets.reserve(8);
		// 8-connectivity neighbours
		neighborOffsets.push_back(std::make_pair(-1,-1));
		neighborOffsets.push_back(std::make_pair( 0,-1));
		neighborOffsets.push_back(std::make_pair( 1,-1));
		neighborOffsets.push_back(std::make_pair(-1, 0));
		neighborOffsets.push_back(std::make_pair( 1, 0));
		neighborOffsets.push_back(std::make_pair(-1, 1));
		neighborOffsets.push_back(std::make_pair( 0, 1));
		neighborOffsets.push_back(std::make_pair( 1, 1));
	}
	else
	{
		neighborOffsets.reserve(4);
		// 4-connectivity neighbours
		neighborOffsets.push_back(std::make_pair( 1, 0));
		neighborOffsets.push_back(std::make_pair(-1, 0));
		neighborOffsets.push_back(std::make_pair( 0, 1));
		neighborOffsets.push_back(std::make_pair( 0,-1));
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

bool TileMapSquare::CheckComplete()
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

void TileMapSquare::Set8Connectivity(const bool value)
{
	m_is8Connectivity = value;
}

int TileMapSquare::GetSuperpositionIndexAt(const CellIdx& cellIdx)
{
	return m_mapCells[m_width * cellIdx.y + cellIdx.x];
}

void TileMapSquare::SetSuperpositionIndexAt(const CellIdx& cellIdx, const int superpositionIdx)
{
	m_mapCells[m_width * cellIdx.y + cellIdx.x] = superpositionIdx;
}

std::vector<TileBase*> TileMapSquare::GetTileSet()
{
	return m_tileSet;
}

void TileMapSquare::SetTileSet(std::vector<TileBase*> tileSet)
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

void TileMapSquare::Reset()
{
	for (size_t i = 0; i < m_mapCells.size(); i++)
	{
		m_mapCells[i] = m_tileSet.size();
	}
}

int TileMapSquare::GetWidth()
{
	return m_width;
}

int TileMapSquare::GetHeight()
{
	return m_height;
}

bool TileMapSquare::IsCellCollapsed(const CellIdx& cell)
{
	return GetSuperpositionIndexAt(cell) < m_tileSet.size();
}

bool TileMapSquare::CollapseCell(const CellIdx& cell, const TileBase* tile)
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

bool TileMapSquare::UpdateCellSuperposition(const CellIdx& cell, const std::vector<TileBase*>& newSuperposition)
{
	int currentIdx = GetSuperpositionIndexAt(cell);
	size_t newIdx = -1;

	//Try to find superposition index, traverse vector backwards, since first stored superpositions are less likely to happen
	for (size_t i = m_possibleSuperpositions.size() - 1; i > 0; i--)
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
	if (newIdx == -1)
	{
		m_possibleSuperpositions.push_back(newSuperposition);
		newIdx = m_possibleSuperpositions.size() - 1;
	}

	SetSuperpositionIndexAt(cell, newIdx);

	return true;
}

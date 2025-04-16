#pragma once
#include "Interfaces/TileMapBase.h"

class TileMapSquare : public TileMapBase 
{
public:
	TileMapSquare(const std::vector<std::shared_ptr<TileBase>>& tileSet, const int width, const int height);;

	// Inherited via TileMapBase
	std::vector<std::shared_ptr<TileBase>> GetTileSet() override;
	std::vector<CellIdx> GetNeighbors(const CellIdx& cell) override;
	std::vector<std::shared_ptr<TileBase>> GetSuperpositionAt(const CellIdx& cell) override;
	bool CheckComplete() override;
	bool IsCellCollapsed(const CellIdx& cell) override;
	bool CollapseCell(const CellIdx& cell, const std::shared_ptr<TileBase> tile) override;
	bool UpdateCellSuperposition(const CellIdx& cell, const std::vector<std::shared_ptr<TileBase>>& newSuperposition) override;
	void Reset() override;
	int GetWidth() override;
	int GetHeight() override;

	void Set8Connectivity(const bool value);
	void PrintMap();

protected:
	const int m_width, m_height;
	bool m_is8Connectivity = false;

	const std::vector<std::shared_ptr<TileBase>> &m_tileSet;
	// Index in m_possibleSuperpositions which correspond to the superposition in each cell
	std::vector<int> m_mapCells;
	// Store each possible superposition only once
	std::vector<std::vector<std::shared_ptr<TileBase>>> m_possibleSuperpositions;

	int GetSuperpositionIndexAt(const CellIdx& cellIdx);
	void SetSuperpositionIndexAt(const CellIdx& cellIdx, const int superpositionIdx);

};
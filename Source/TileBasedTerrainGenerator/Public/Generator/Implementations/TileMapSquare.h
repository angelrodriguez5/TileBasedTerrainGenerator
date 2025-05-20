#pragma once
#include "Generator/Interfaces/TileMapBase.h"

class TileMapSquare : public TileMapBase 
{
public:
	TileMapSquare(const int width, const int height);;

	// Inherited via TileMapBase
	std::vector<TileBase*> GetTileSet() override;
	void SetTileSet(std::vector<TileBase*> tileSet) override;
	std::vector<CellIdx> GetNeighbors(const CellIdx& cell) override;
	std::vector<TileBase*> GetSuperpositionAt(const CellIdx& cell) override;
	TileBase* GetTileAt(const CellIdx& cell) override;
	bool CheckComplete() override;
	bool IsCellCollapsed(const CellIdx& cell) override;
	bool CollapseCell(const CellIdx& cell, const TileBase* tile) override;
	bool UpdateCellSuperposition(const CellIdx& cell, const std::vector<TileBase*>& newSuperposition) override;
	void Reset() override;
	int GetWidth() override;
	int GetHeight() override;

	void Set8Connectivity(const bool value);

protected:
	const int m_width, m_height;
	bool m_is8Connectivity = false;

	std::vector<TileBase*> m_tileSet;
	// The content of each cell is the index in m_possibleSuperpositions which 
	// correspond to the superposition in that cell
	std::vector<int> m_mapCells;
	// Store each possible superposition only once
	std::vector<std::vector<TileBase*>> m_possibleSuperpositions;

	int GetSuperpositionIndexAt(const CellIdx& cellIdx);
	void SetSuperpositionIndexAt(const CellIdx& cellIdx, const int superpositionIdx);

};